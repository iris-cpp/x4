#ifndef IRIS_ZZ_X4_CORE_DETAIL_PARSE_ALTERNATIVE_HPP
#define IRIS_ZZ_X4_CORE_DETAIL_PARSE_ALTERNATIVE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/traits/attribute_traits.hpp>
#include <iris/x4/traits/container_traits.hpp>

#include <iris/x4/core/detail/action_slot.hpp>
#include <iris/x4/core/detail/parse_into_container.hpp>
#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/write_attribute.hpp>
#include <iris/x4/core/nary_parser.hpp>
#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/parser.hpp>

#include <iris/alloy/tuple.hpp> // IWYU pragma: keep

#include <concepts>
#include <iterator>
#include <type_traits>

namespace iris::x4 {

template<class... Ps>
struct alternative;

} // iris::x4

namespace iris::x4::detail {

template<class Context>
[[nodiscard]] constexpr bool alternative_should_stop(Context const& ctx) noexcept
{
    if constexpr (has_context_v<Context, contexts::expectation_failure>) {
        return x4::has_expectation_failure(ctx);
    } else {
        return false;
    }
}

// This must be an independent struct to reduce compilation time.
// For details, see notes on `parse_sequence_tuple`.
template<class Alt>
struct parse_alternative_all_impl
{
    template<std::size_t I, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute ExposedAttr>
    [[nodiscard]] static constexpr bool parse_branch(Alt const& alt, It& first, Se const& last, Context const& ctx, ExposedAttr& exposed_attr)
    {
        // Don't declare alias templates for parser type or attribute type here;
        // Visual Studio often hides the real type when it is wrapped in a local alias.

        auto const& branch = nary::get<I>(alt.elems);

        if constexpr (
            may_leave_attribute_unwritten_v<nary::element_parser_t<I, Alt>> ||
            X4UnusedAttribute<typename parser_traits<nary::element_parser_t<I, Alt>>::attribute_type> ||
            traits::detail::clearable_for<ExposedAttr, typename parser_traits<nary::element_parser_t<I, Alt>>::attribute_type>
        ) {
            auto&& attr = detail::prepare_attribute_for<nary::element_parser_t<I, Alt>>(exposed_attr);
            return branch.parse(first, last, ctx, attr);

        } else {
            static_assert(
                requires(typename parser_traits<nary::element_parser_t<I, Alt>>::attribute_type&& value) {
                    x4::write_attribute(exposed_attr, std::move(value));
                },
                "The attribute of this branch cannot be converted into the attribute of the alternative."
            );
            // The branch yields a whole value of an unrelated shape (e.g. a narrower variant);
            // parse it into a temporary and convert on success.
            typename parser_traits<nary::element_parser_t<I, Alt>>::attribute_type temp{};

            if (!branch.parse(first, last, ctx, temp)) return false;

            // As in the other branches, the value is written into the default state. An earlier
            // branch may have failed after writing, and `write_attribute` writes into an existing
            // content: it appends to a container, and writes nothing for a disengaged optional.
            traits::attribute_traits<ExposedAttr>::reset(exposed_attr);
            x4::write_attribute(exposed_attr, std::move(temp));
            return true;
        }
    }

    // The slot of a semantic action records whether the branch which matched wrote the attribute.
    template<std::size_t I, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Slot>
    [[nodiscard]] static constexpr bool parse_slot_branch(Alt const& alt, It& first, Se const& last, Context const& ctx, Slot& slot)
    {
        using branch_parser = nary::element_parser_t<I, Alt>;
        using attribute_type = Slot::attribute_type;
        auto const& branch = nary::get<I>(alt.elems);

        if constexpr (!has_attribute_v<branch_parser>) {
            slot.disengage();
            return branch.parse(first, last, ctx, unused);

        } else if constexpr (may_leave_attribute_unwritten_v<branch_parser>) {
            slot.disengage();
            return branch.parse(first, last, ctx, slot);

        } else {
            attribute_type& attr = slot.engage();
            bool matched = false;
            if constexpr (traits::X4Container<attribute_type>) {
                matched = branch.parse(first, last, ctx, attr); // into the new, empty container
            } else {
                matched = parse_alternative_all_impl::parse_branch<I>(alt, first, last, ctx, attr);
            }
            if (!matched) slot.disengage();
            return matched;
        }
    }
};

// Make this independent function to reduce lambda's type name in compilation errors
template<class Alt, bool IntoContainer, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class ContainerAttr>
[[nodiscard]] constexpr auto make_branch_parser_into_empty_container(
    Alt const& alt, It& first, Se const& last, Context const& ctx, ContainerAttr& container_attr
) noexcept
{
    return [&alt, &first, &last, &ctx, &container_attr]<std::size_t I>() -> bool {
        auto const& branch = nary::get<I>(alt.elems);
        if constexpr (!has_attribute_v<nary::element_parser_t<I, Alt>>) {
            return branch.parse(first, last, ctx, unused);

        } else {
            bool matched = false;
            if constexpr (IntoContainer) {
                matched = detail::parse_into_container(branch, first, last, ctx, container_attr);
            } else {
                matched = branch.parse(first, last, ctx, container_attr);
            }
            if (matched) return true;
            iris::container::clear(container_attr);
            return false;
        }
    };
}

// Make this independent function to reduce lambda's type name in compilation errors
template<class Alt, bool IntoContainer, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class ContainerAttr>
[[nodiscard]] constexpr auto make_branch_parser_into_buffer(
    Alt const& alt, It& first, Se const& last, Context const& ctx, ContainerAttr& buffer, ContainerAttr& container_attr
) noexcept
{
    return [&alt, &first, &last, &ctx, &buffer, &container_attr]<std::size_t I>() -> bool {
        auto const& branch = nary::get<I>(alt.elems);
        if constexpr (!has_attribute_v<nary::element_parser_t<I, Alt>>) {
            return branch.parse(first, last, ctx, unused);

        } else {
            bool matched = false;
            if constexpr (IntoContainer) {
                matched = detail::parse_into_container(branch, first, last, ctx, buffer);
            } else {
                matched = branch.parse(first, last, ctx, buffer);
            }
            if (matched) {
                iris::container::append_range(container_attr, buffer | std::views::as_rvalue);
                return true;
            }
            iris::container::clear(buffer);
            return false;
        }
    };
}

template<class AltT>
[[nodiscard]] constexpr auto const& as_alternative(AltT const& alt) noexcept
{
    // Diagnostics show this name in place of the whole alternative
    using T = AltT;
    T const& named = alt;
    return named;
}

// Tries the branches in order; stops at the first match, or at an expectation
// failure raised inside a branch.
template<class Alt, std::size_t... Is, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
[[nodiscard]] constexpr bool
parse_alternative_all(Alt const& alt, std::index_sequence<Is...>, It& first, Se const& last, Context const& ctx, UnusedAttr const& unused_attr)
{
    bool matched = false;
    (void)((((matched = nary::get<Is>(alt.elems).parse(first, last, ctx, unused_attr))) || detail::alternative_should_stop(ctx)) || ...);
    return matched;
}

template<class Alt, std::size_t... Is, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute ExposedAttr>
    requires (!traits::X4Container<ExposedAttr>) && (!is_action_slot_v<ExposedAttr>)
[[nodiscard]] constexpr bool
parse_alternative_all(Alt const& alt, std::index_sequence<Is...>, It& first, Se const& last, Context const& ctx, ExposedAttr& exposed_attr)
{
    static_assert(!std::is_const_v<ExposedAttr>);
    bool matched = false;
    (void)((((matched = parse_alternative_all_impl<Alt>::template parse_branch<Is>(alt, first, last, ctx, exposed_attr))) || detail::alternative_should_stop(ctx)) || ...);
    return matched;
}

template<class Alt, std::size_t... Is, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Slot>
    requires is_action_slot_v<Slot>
[[nodiscard]] constexpr bool
parse_alternative_all(Alt const& alt, std::index_sequence<Is...>, It& first, Se const& last, Context const& ctx, Slot& slot)
{
    bool matched = false;
    (void)((((matched = parse_alternative_all_impl<Alt>::template parse_slot_branch<Is>(alt, first, last, ctx, slot))) || detail::alternative_should_stop(ctx)) || ...);
    return matched;
}

// `IntoContainer` parses each branch by `parse_into_container` instead of its `parse`
template<
    class Alt, std::size_t... Is, std::forward_iterator It, std::sentinel_for<It> Se, class Context,
    X4NonUnusedAttribute ContainerAttr, bool IntoContainer = false
>
    requires traits::X4Container<ContainerAttr>
[[nodiscard]] constexpr bool
parse_alternative_all(
    Alt const& alt, std::index_sequence<Is...>, It& first, Se const& last, Context const& ctx,
    ContainerAttr& container_attr, std::bool_constant<IntoContainer> = {}
)
{
    static_assert(!std::same_as<std::remove_const_t<ContainerAttr>, unused_type>);
    static_assert(!std::same_as<std::remove_const_t<ContainerAttr>, unused_container_type>);
    static_assert(!std::is_const_v<ContainerAttr>);

    // Same logic as in `x4::optional`

    // We can (ab)use the exposed attribute as the temporary workspace
    // if and only if the modification or rollback of the exposed attribute
    // does not change the semantic state of the exposed container instance.
    //
    // The only situation we can guarantee such condition is when the container
    // is empty; assuming that the "empty" state of any user-provided container
    // class is monostate.
    if (std::ranges::empty(container_attr)) {
        auto const parse_branch = detail::make_branch_parser_into_empty_container<Alt, IntoContainer>(alt, first, last, ctx, container_attr);
        bool matched = false;
        (void)((((matched = parse_branch.template operator()<Is>())) || detail::alternative_should_stop(ctx)) || ...);
        return matched;
    }

    // ---------------------------------------------------------------
    // The container already holds elements

    // A failed branch must not touch the existing elements, and there is no general
    // way to undo appends. So each branch parses into a buffer that is appended only
    // on success.
    ContainerAttr buffer;
    auto const parse_branch = detail::make_branch_parser_into_buffer<Alt, IntoContainer>(alt, first, last, ctx, buffer, container_attr);
    bool matched = false;
    (void)((((matched = parse_branch.template operator()<Is>())) || detail::alternative_should_stop(ctx)) || ...);
    return matched;
}

template<class... Ps>
struct parse_into_container_impl<alternative<Ps...>>
{
    using parser_type = alternative<Ps...>;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute ExposedAttr>
    [[nodiscard]] static constexpr bool
    call(
        parser_type const& parser,
        It& first, Se const& last, Context const& ctx, ExposedAttr& exposed_attr
    )
    {
        static_assert(traits::X4Container<ExposedAttr>);

        return detail::parse_alternative_all(
            detail::as_alternative(parser), std::index_sequence_for<Ps...>{},
            first, last, ctx, exposed_attr, std::true_type{}
        );
    }
};

} // iris::x4::detail

#endif
