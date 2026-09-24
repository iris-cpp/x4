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

#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/move_to.hpp>
#include <iris/x4/core/multi_parser.hpp>
#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/unused.hpp>

#include <iris/x4/traits/tuple_traits.hpp>
#include <iris/x4/traits/variant_traits.hpp>
#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/traits/attribute_traits.hpp>

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/detail/parse_into_container.hpp>

#include <iris/alloy/tuple.hpp> // IWYU pragma: keep

#include <concepts>
#include <iterator>
#include <type_traits>

namespace iris::x4 {

template<class... Ps>
struct alternative;

} // iris::x4

namespace iris::x4::detail {

struct pass_variant_unused
{
    using type = unused_type;

    template<class T>
    [[nodiscard]] static constexpr unused_type
    call(T&) noexcept
    {
        return unused_type{};
    }
};

template<X4Attribute Attr>
struct pass_variant_used
{
    using type = Attr&;

    [[nodiscard]] static constexpr Attr&
    call(Attr& v) noexcept
    {
        return v;
    }
};

template<>
struct pass_variant_used<unused_type> : pass_variant_unused {};

template<class Parser, X4Attribute Attr>
struct pass_parser_attribute
{
    using attribute_type = parser_traits<Parser>::attribute_type;
    using substitute_type = traits::variant_find_holdable_type<Attr, attribute_type>::type;

    using type = std::conditional_t<
        std::same_as<Attr, substitute_type>,
        Attr&,
        substitute_type
    >;

    template<X4Attribute Attr_>
        requires std::same_as<Attr_, std::remove_reference_t<type>>
    [[nodiscard]] static constexpr Attr_&
    call(Attr_& attr) noexcept
    {
        return attr;
    }

    template<X4Attribute Attr_>
        requires (!std::same_as<Attr_, std::remove_reference_t<type>>)
    [[nodiscard]] static type
    call(Attr_&)
        noexcept(std::is_nothrow_default_constructible_v<type>)
    {
        return type{};
    }
};

// Pass non-variant attributes as-is
template<class Parser, X4Attribute Attr>
struct pass_non_variant_attribute
{
    using type = Attr&;

    [[nodiscard]] constexpr static Attr&
    call(Attr& attribute) noexcept
    {
        return attribute;
    }
};

// Unwrap single element sequences
template<class Parser, X4Attribute Attr>
    requires traits::is_size_one_sequence_v<Attr>
struct pass_non_variant_attribute<Parser, Attr>
{
    using attr_type = std::remove_reference_t<
        alloy::tuple_element_t<0, Attr>
    >;
    using pass = pass_parser_attribute<Parser, attr_type>;
    using type = pass::type;

    template<X4Attribute Attr_>
    [[nodiscard]] static constexpr type
    call(Attr_& attr)
        noexcept(noexcept(pass::call(alloy::get<0>(attr))))
    {
        return pass::call(alloy::get<0>(attr));
    }
};

template<class Parser, X4Attribute Attr>
    requires (!traits::is_variant_v<Attr>)
struct pass_parser_attribute<Parser, Attr>
    : pass_non_variant_attribute<Parser, Attr>
{};

template<class Parser>
struct pass_parser_attribute<Parser, unused_type>
    : pass_variant_unused
{};

template<class Parser, X4Attribute Attr>
struct pass_variant_attribute
    : std::conditional_t<
        has_attribute_v<Parser>,
        pass_parser_attribute<Parser, Attr>,
        pass_variant_unused
    >
{};

template<class... Ps, X4Attribute Attr>
struct pass_variant_attribute<alternative<Ps...>, Attr>
    : std::conditional_t<
        has_attribute_v<alternative<Ps...>>,
        pass_variant_used<Attr>,
        pass_variant_unused
    >
{};

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    requires std::is_lvalue_reference_v<typename pass_variant_attribute<Parser, Attr>::type>
[[nodiscard]] constexpr bool
parse_alternative(
    Parser const& p, It& first, Se const& last,
    Context const& ctx, Attr& attr
) {
    return p.parse(first, last, ctx, pass_variant_attribute<Parser, Attr>::call(attr));
}

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    requires (!std::is_lvalue_reference_v<typename pass_variant_attribute<Parser, Attr>::type>)
[[nodiscard]] constexpr bool
parse_alternative(
    Parser const& p, It& first, Se const& last,
    Context const& ctx, Attr& attr
) {
    auto&& actual_attr = pass_variant_attribute<Parser, Attr>::call(attr);
    if (!p.parse(first, last, ctx, actual_attr)) return false;
    x4::move_to(std::move(actual_attr), attr);
    return true;
}

template<class Subject>
struct alternative_helper : proxy_parser<alternative_helper<Subject>, Subject>
{
    using proxy_parser<alternative_helper, Subject>::proxy_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        return detail::parse_alternative(this->subject, first, last, ctx, attr);
    }
};
template<class Subject>
alternative_helper(Subject const&) -> alternative_helper<Subject>;

template<class Context>
[[nodiscard]] constexpr bool alternative_should_stop(Context const& ctx) noexcept
{
    if constexpr (has_context_v<Context, contexts::expectation_failure>) {
        return x4::has_expectation_failure(ctx);
    } else {
        return false;
    }
}

// Tries the branches in order; stops at the first match, or at an expectation
// failure raised inside a branch.
template<class... Ps>
struct parse_alternative_all
{
    // Tries the branches in order without touching any attribute; used where each
    // branch writes on its own (appending into a container).
    template<std::size_t... Is, class Try, class Context>
    [[nodiscard]] static constexpr bool
    call(std::index_sequence<Is...>, Try&& try_branch, Context const& ctx)
    {
        bool matched = false;
        (void)((((matched = try_branch.template operator()<Is>())) || detail::alternative_should_stop(ctx)) || ...);
        return matched;
    }

    // -------------------------------------------------------------------------

    template<std::size_t... Is, class Try, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] static constexpr bool
    call(std::index_sequence<Is...>, Try&& try_branch, Context const& ctx, UnusedAttr const& unused_attr)
    {
        bool matched = false;
        (void)((((matched = try_branch.template operator()<Is>(unused_attr))) || detail::alternative_should_stop(ctx)) || ...);
        return matched;
    }

    template<std::size_t... Is, class Try, class Context, X4NonUnusedAttribute ExposedAttr>
        requires (!traits::X4Container<ExposedAttr>)
    [[nodiscard]] static constexpr bool
    call(std::index_sequence<Is...>, Try&& try_branch, Context const& ctx, ExposedAttr& exposed_attr)
    {
        static_assert(!std::is_const_v<ExposedAttr>);

        auto parse_branch = [&]<std::size_t I>() -> bool {
            using branch_attr = parser_traits<multi_parser_t<I, Ps...>>::attribute_type;
            if constexpr (X4UnusedAttribute<branch_attr> || traits::detail::clearable_for<ExposedAttr, branch_attr>) {
                auto&& alt_attr = detail::prepare_attribute<branch_attr>(exposed_attr);
                return try_branch.template operator()<I>(alt_attr);

            } else {
                static_assert(
                    requires(branch_attr&& value) { x4::move_to(std::move(value), exposed_attr); },
                    "The attribute of this branch cannot be converted into the attribute of the alternative."
                );
                // The branch yields a whole value of an unrelated shape (e.g. a narrower variant);
                // parse it into a temporary and convert on success.
                branch_attr temp{};
                if (!try_branch.template operator()<I>(temp)) return false;
                x4::move_to(std::move(temp), exposed_attr);
                return true;
            }
        };
        bool matched = false;
        (void)((((matched = parse_branch.template operator()<Is>())) || detail::alternative_should_stop(ctx)) || ...);
        return matched;
    }

    template<std::size_t... Is, class Try, class Context, X4NonUnusedAttribute ContainerAttr>
        requires traits::X4Container<ContainerAttr>
    [[nodiscard]] static constexpr bool
    call(std::index_sequence<Is...>, Try&& try_branch, Context const& ctx, ContainerAttr& container_attr)
    {
        static_assert(!std::same_as<std::remove_const_t<ContainerAttr>, unused_type>);
        static_assert(!std::same_as<std::remove_const_t<ContainerAttr>, unused_container_type>);
        static_assert(!std::is_const_v<ContainerAttr>);

        // We can (ab)use the exposed attribute as the temporary workspace
        // if and only if the modification or rollback of the exposed attribute
        // does not change the semantic state of the exposed container instance.
        //
        // The only situation we can guarantee such condition is when the container
        // is empty; assuming that the "empty" state of any user-provided container
        // class is monostate.
        if (traits::is_empty(container_attr)) {
            auto parse_branch = [&]<std::size_t I>() -> bool {
                if (try_branch.template operator()<I>(container_attr)) return true;
                traits::clear(container_attr);
                return false;
            };
            bool matched = false;
            (void)((((matched = parse_branch.template operator()<Is>())) || detail::alternative_should_stop(ctx)) || ...);
            return matched;
        }

        // The container already holds elements: a failed branch must not touch
        // them, and there is no general way to undo appends, so each branch parses
        // into a buffer that is appended only on success.
        unwrap_container_appender_t<ContainerAttr> buffer;
        auto parse_branch = [&]<std::size_t I>() -> bool {
            if (try_branch.template operator()<I>(buffer)) {
                x4::move_to(std::move(buffer), container_attr);
                return true;
            }
            traits::clear(buffer);
            return false;
        };
        bool matched = false;
        (void)((((matched = parse_branch.template operator()<Is>())) || detail::alternative_should_stop(ctx)) || ...);
        return matched;
    }
};

template<class... Ps>
struct parse_into_container_impl<alternative<Ps...>>
{
    using parser_type = alternative<Ps...>;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    call(
        parser_type const& parser,
        It& first, Se const& last, Context const& ctx, Attr& attr
    )
    {
        static_assert(traits::is_container_v<Attr>);

        return parse_alternative_all<Ps...>::call(
            std::index_sequence_for<Ps...>{},
            [&]<std::size_t I>() {
                if constexpr (traits::is_variant_v<typename traits::container_value<Attr>::type>) {
                    return detail::parse_into_container(alternative_helper{x4::get_parser<I>(parser.elems)}, first, last, ctx, attr);
                } else {
                    return detail::parse_into_container(x4::get_parser<I>(parser.elems), first, last, ctx, attr);
                }
            },
            ctx
        );
    }
};

} // iris::x4::detail

#endif
