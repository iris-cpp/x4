#ifndef IRIS_ZZ_X4_OPERATOR_OPTIONAL_HPP
#define IRIS_ZZ_X4_OPERATOR_OPTIONAL_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/detail/parse_into_container.hpp>
#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/move_to.hpp>

#include <iris/x4/traits/optional_traits.hpp>
#include <iris/x4/traits/attribute_category.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct optional : unary_parser<Subject, optional<Subject>>
{
    using attribute_type = traits::build_optional<typename parser_traits<Subject>::attribute_type>::type;

    static constexpr bool maybe_handles_container = true;

    template<class Container>
    static constexpr bool handles_container = std::disjunction_v<
        std::bool_constant<parser_traits<Subject>::template handles_container<Container>>,
        traits::can_hold<typename parser_traits<Subject>::attribute_type, typename traits::container_value<Container>::type>
    >;

    // catch-all overload
    template<
        std::forward_iterator It, std::sentinel_for<It> Se, class Context,
        class Attr // unconstrained
    >
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(is_nothrow_parsable_v<Subject, It, Se, Context, Attr>)
    {
        static_assert(Parsable<Subject, It, Se, Context, Attr>);

        // discard [[nodiscard]]
        (void)this->subject.parse(first, last, ctx, attr);

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }

    // container attribute
    template<
        std::forward_iterator It, std::sentinel_for<It> Se, class Context,
        traits::CategorizedAttr<traits::container_attr> Attr
    >
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(noexcept(detail::parse_into_container(this->subject, first, last, ctx, attr)))
    {
        // discard [[nodiscard]]
        (void)detail::parse_into_container(this->subject, first, last, ctx, attr);

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }

    // optional attribute
    template<
        std::forward_iterator It, std::sentinel_for<It> Se, class Context,
        traits::CategorizedAttr<traits::optional_attr> Attr
    >
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(
            std::is_nothrow_default_constructible_v<typename traits::optional_value<Attr>::type> &&
            is_nothrow_parsable_v<Subject, It, Se, Context, typename traits::optional_value<Attr>::type> &&
            noexcept(x4::move_to(std::declval<typename traits::optional_value<Attr>::type&&>(), attr))
        )
    {
        typename traits::optional_value<Attr>::type val{}; // value-initialize

        if (this->subject.parse(first, last, ctx, val)) {
            // assign the parsed value into our attribute
            x4::move_to(std::move(val), attr);
            return true;
        }

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }
};

template<X4Subject Subject>
[[nodiscard]] constexpr optional<as_parser_plain_t<Subject>>
operator-(Subject&& subject)
    noexcept(is_parser_nothrow_constructible_v<optional<as_parser_plain_t<Subject>>, Subject>)
{
    return {as_parser(std::forward<Subject>(subject))};
}

} // iris::x4

#endif
