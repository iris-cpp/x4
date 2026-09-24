#ifndef IRIS_ZZ_X4_TRAITS_ATTRIBUTE_TRAITS_HPP
#define IRIS_ZZ_X4_TRAITS_ATTRIBUTE_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/core/traits/variant_traits.hpp>
#include <iris/x4/core/traits/attribute_category.hpp>

#include <iris/x4/core/attribute.hpp>
#include <iris/x4/core/unused.hpp>

#include <iris/rvariant/rvariant.hpp>
#include <iris/alloy/tuple.hpp> // IWYU pragma: keep

#include <memory>
#include <concepts>
#include <utility>
#include <type_traits>

#include <cstddef>
#include <cassert>

namespace iris::x4::traits {

template<class ExposedAttr>
struct attribute_traits;

namespace detail {

template<class ExposedAttr, class ParserAttr>
concept clearable_for = requires(ExposedAttr& attr) {
    attribute_traits<ExposedAttr>::template clear<ParserAttr>(attr);
};

template<class T, class U>
concept proper_attribute_for =
    X4ValueAttribute<U> &&
    weakly_assignable_from<T&, U>;

} // detail

// Note: We don't need to place conditional `noexcept` specifiers to
//       `attribute_traits<T>::clear`. It has been confirmed that it
//       does not affect the generated binary code in any ways, and
//       it does increase the compilation time by few hundred msec.
//
// We do need to place them in `reset`, as it affects the semantics of
// `attribute_reset_guard`.

template<class ExposedAttr>
struct attribute_traits
{
    static_assert(CategorizedAttr<ExposedAttr, plain_tag>);

    static constexpr void reset(ExposedAttr& attr)
        noexcept(noexcept(attr = ExposedAttr{}))
    {
        attr = ExposedAttr{};
    }

    template<class ParserAttr>
        requires
            std::default_initializable<ParserAttr> &&
            weakly_assignable_from<ExposedAttr&, ParserAttr>
    static constexpr ExposedAttr& clear(ExposedAttr& attr)
    {
        static_assert(detail::proper_attribute_for<ExposedAttr, ParserAttr>);
        attr = ParserAttr{};
        return attr;
    }

    template<class ParserAttr>
        requires
            std::default_initializable<ParserAttr> &&
            (!weakly_assignable_from<ExposedAttr&, ParserAttr>) &&
            std::constructible_from<ExposedAttr, ParserAttr>
    static constexpr ExposedAttr& clear(ExposedAttr& attr)
    {
        static_assert(std::default_initializable<ParserAttr>);
        static_assert(detail::proper_attribute_for<ExposedAttr, ExposedAttr>);
        attr = ExposedAttr{ParserAttr{}};
        return attr;
    }
};

template<class ExposedAttr>
inline constexpr bool is_nothrow_resettable_v = noexcept(attribute_traits<ExposedAttr>::reset(std::declval<ExposedAttr&>()));

template<CategorizedAttr<unused_tag> UnusedTypeT>
struct attribute_traits<UnusedTypeT>
{
    static constexpr void reset(UnusedTypeT const&) noexcept
    {}

    template<class ParserAttr>
    static constexpr UnusedTypeT const& clear(UnusedTypeT const& attr) noexcept
    {
        return attr;
    }
};

template<CategorizedAttr<optional_tag> OptionalT>
struct attribute_traits<OptionalT>
{
    using value_type = OptionalT::value_type;

    static constexpr void reset(OptionalT& opt) noexcept
    {
        opt.reset();
    }

    // The branch writes the optional as a whole (a nested optional parser, or
    // a rule with an optional attribute): hand it over disengaged.
    template<class ParserAttr>
        requires std::same_as<ParserAttr, OptionalT> || CategorizedAttr<ParserAttr, optional_tag>
    static constexpr OptionalT& clear(OptionalT& opt) noexcept
    {
        opt.reset();
        return opt;
    }

    // The branch writes the contained value: engage the optional, reusing the
    // contained object if already engaged, and prepare that value for `ParserAttr`.
    template<class ParserAttr>
        requires
            (!CategorizedAttr<ParserAttr, optional_tag>) &&
            detail::clearable_for<value_type, ParserAttr>
    static constexpr decltype(auto) clear(OptionalT& opt)
    {
        if (!opt) opt.emplace();
        return attribute_traits<value_type>::template clear<ParserAttr>(*opt);
    }
};

template<class RecursiveWrapperT>
    requires is_recursive_wrapper_v<RecursiveWrapperT>
struct attribute_traits<RecursiveWrapperT>
{
    static constexpr void reset(RecursiveWrapperT& rec_wrapper)
        noexcept(is_nothrow_resettable_v<unwrap_recursive_t<RecursiveWrapperT>>)
    {
        attribute_traits<unwrap_recursive_t<RecursiveWrapperT>>::reset(*rec_wrapper);
    }

    template<class ParserAttr>
    static constexpr decltype(auto) clear(RecursiveWrapperT& rec_wrapper)
    {
        return attribute_traits<unwrap_recursive_t<RecursiveWrapperT>>::template
            clear<unwrap_recursive_t<ParserAttr>>(*rec_wrapper);
    }
};

namespace detail {

template<class VariantT>
inline constexpr bool is_variant_nothrow_resettable = []<std::size_t... Is>(std::index_sequence<Is...>) {
    return (is_nothrow_resettable_v<variant_alternative_t<Is, VariantT>> && ...);
}(std::make_index_sequence<variant_size_v<VariantT>>{});

} // detail

template<CategorizedAttr<variant_tag> VariantT>
struct attribute_traits<VariantT>
{
    static constexpr void reset(VariantT& var)
        noexcept(
            is_nothrow_resettable_v<variant_alternative_t<0, VariantT>> &&
            noexcept(var.template emplace<0>())
        )
    {
        if (auto* first = iris::get_if<0>(&var)) {
            attribute_traits<variant_alternative_t<0, VariantT>>::reset(*first);
        } else {
            var.template emplace<0>();
        }
    }

    template<class ParserAttr>
    using tag_type = unwrap_recursive_t<typename variant_find_holdable_type<VariantT, ParserAttr>::type>;

    template<class ParserAttr>
        requires
            (!std::same_as<ParserAttr, VariantT>) &&
            requires(VariantT& var) { var.template emplace<tag_type<ParserAttr>>(); }
    static constexpr tag_type<ParserAttr>& clear(VariantT& var)
    {
        if (auto* existing_alt = iris::get_if<tag_type<ParserAttr>>(&var)) {
            attribute_traits<tag_type<ParserAttr>>::template clear<tag_type<ParserAttr>>(*existing_alt);
            return *existing_alt;

        } else {
            return var.template emplace<tag_type<ParserAttr>>();
        }
    }

    template<class ParserAttr>
        requires std::same_as<ParserAttr, VariantT>
    static constexpr VariantT& clear(VariantT& var)
    {
        assert(!var.valueless_by_exception());

        // Reuse the existing alternative instance.
        // We don't call `visit(...)` here, since it increases the compilation time by few hundred msec
        [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            (void)(
                (
                    var.index() == Is
                    ? (attribute_traits<variant_alternative_t<Is, VariantT>>::reset(*iris::get_if<Is>(&var)), true)
                    : false
                ) || ...
            );
        }(std::make_index_sequence<variant_size_v<VariantT>>{});
        return var;
    }
};

template<CategorizedAttr<container_tag> ContainerT>
struct attribute_traits<ContainerT>
{
    static constexpr void reset(ContainerT& container)
        noexcept(noexcept(traits::clear(container)))
    {
        traits::clear(container);
    }

    template<class ParserAttr>
    static constexpr ContainerT& clear(ContainerT& container)
    {
        traits::clear(container);
        return container;
    }
};

namespace detail {

template<std::size_t I, class TupleLikeT>
using tuple_slot_t = std::remove_reference_t<alloy::tuple_element_t<I, TupleLikeT>>;

template<class TupleLikeT>
inline constexpr bool is_tuple_nothrow_resettable = []<std::size_t... Is>(std::index_sequence<Is...>) {
    return (is_nothrow_resettable_v<tuple_slot_t<Is, TupleLikeT>> && ...);
}(std::make_index_sequence<alloy::tuple_size_v<TupleLikeT>>{});

} // detail

template<CategorizedAttr<tuple_tag> TupleLikeT>
struct attribute_traits<TupleLikeT>
{
    static constexpr void reset(TupleLikeT& tup)
        noexcept(detail::is_tuple_nothrow_resettable<TupleLikeT>)
    {
        [&]<std::size_t... Is>(std::index_sequence<Is...>) {(
            attribute_traits<detail::tuple_slot_t<Is, TupleLikeT>>::reset(
                alloy::get<Is>(tup)
            ), ...
        );
        }(std::make_index_sequence<alloy::tuple_size_v<TupleLikeT>>{});
    }

    template<class ParserAttr>
    static constexpr TupleLikeT& clear(TupleLikeT& tup)
    {
        attribute_traits::reset(tup);
        return tup;
    }

    template<class ParserAttr>
        requires
            (!std::same_as<ParserAttr, TupleLikeT>) &&
            (alloy::tuple_size_v<TupleLikeT> == 1) &&
            detail::clearable_for<detail::tuple_slot_t<0, TupleLikeT>, ParserAttr>
    static constexpr decltype(auto) clear(TupleLikeT& tup)
    {
        // A single-element tuple-like is transparent, as in `move_to`: the
        // branch parses into the element.
        return attribute_traits<detail::tuple_slot_t<0, TupleLikeT>>::template clear<ParserAttr>(
            alloy::get<0>(tup)
        );
    }
};

} // iris::x4::traits

namespace iris::x4::detail {

template<X4UnusedAttribute ParserAttr, class ExposedAttr>
[[nodiscard]] constexpr unused_type const& prepare_attribute(ExposedAttr& exposed_attr)
{
    traits::attribute_traits<ExposedAttr>::reset(exposed_attr);
    return unused;
}

template<X4NonUnusedAttribute ParserAttr, class ExposedAttr>
    requires traits::detail::clearable_for<ExposedAttr, ParserAttr>
[[nodiscard]] constexpr decltype(auto) prepare_attribute(ExposedAttr& exposed_attr IRIS_LIFETIMEBOUND)
{
    return traits::attribute_traits<ExposedAttr>::template clear<ParserAttr>(exposed_attr);
}

template<X4NonUnusedAttribute ParserAttr, class ExposedAttr>
    requires (!traits::detail::clearable_for<ExposedAttr, ParserAttr>)
[[nodiscard]] constexpr ExposedAttr& prepare_attribute(ExposedAttr& exposed_attr IRIS_LIFETIMEBOUND)
{
    traits::attribute_traits<ExposedAttr>::reset(exposed_attr);
    return exposed_attr;
}

template<X4Attribute ExposedAttr>
struct [[nodiscard]] attribute_reset_guard
{
    constexpr explicit attribute_reset_guard(ExposedAttr& exposed_attr) noexcept
        : exposed_attr_(std::addressof(exposed_attr))
    {}

    constexpr explicit attribute_reset_guard(ExposedAttr const&&) = delete;

    constexpr void commit() noexcept
    {
        exposed_attr_ = nullptr;
    }

    constexpr ~attribute_reset_guard() noexcept
    {
        // If `ExposedAttr` can be reset without throwing, it is reset here even during
        // unwinding. Otherwise, it is left in a valid but unspecified state.
        if constexpr (traits::is_nothrow_resettable_v<ExposedAttr>) {
            if (exposed_attr_) traits::attribute_traits<ExposedAttr>::reset(*exposed_attr_);
        }
    }

private:
    ExposedAttr* exposed_attr_;
};

} // iris::x4::detail

#endif
