#ifndef IRIS_ZZ_X4_OPERATOR_LIST_HPP
#define IRIS_ZZ_X4_OPERATOR_LIST_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/list_like_parser.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/expectation.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Left, class Right>
struct list : binary_parser<Left, Right, list<Left, Right>>
{
    using attribute_type = traits::default_container<typename parser_traits<Left>::attribute_type>::type;

    template<class Container>
    static constexpr bool handles_container = std::disjunction_v<
        std::bool_constant<parser_traits<Left>::template handles_container<Container>>,
        traits::can_hold<typename parser_traits<Left>::attribute_type, typename traits::container_value<Container>::type>
    >;

    using binary_parser<Left, Right, list>::binary_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        // never noexcept; requires container insertion
    {
        auto& container_attr = list_like_parser::get_container<attribute_type, Attr>(attr);
        list_like_parser::chunk_buffer<attribute_type, Attr> chunk_buf;

        // In order to succeed, we need to match at least one element
        if (detail::parse_into_container(this->left, first, last, ctx, chunk_buf)) {
            list_like_parser::successful_merge_into(chunk_buf, container_attr);
        } else {
            return false;
        }

        It last_parse_it = first;
        while (
            this->right.parse(last_parse_it, last, ctx, unused) &&
            detail::parse_into_container(this->left, last_parse_it, last, ctx, chunk_buf)
        ) {
            list_like_parser::successful_merge_into(chunk_buf, container_attr);
            first = last_parse_it;
        }

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr& unused_attr) const
        noexcept(
            noexcept(detail::parse_into_container(this->left, first, last, ctx, x4::assume_container(unused_attr))) &&
            std::is_nothrow_copy_assignable_v<It> &&
            is_nothrow_parsable_v<Right, It, Se, Context, unused_type>
        )
    {
        // In order to succeed we need to match at least one element
        if (!detail::parse_into_container(this->left, first, last, ctx, x4::assume_container(unused_attr))) {
            return false;
        }

        It last_parse_it = first;
        while (
            this->right.parse(last_parse_it, last, ctx, unused) &&
            detail::parse_into_container(this->left, last_parse_it, last, ctx, x4::assume_container(unused_attr))
        ) {
            // TODO: can we reduce this copy assignment?
            first = last_parse_it;
        }

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }
};

template<X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr list<as_parser_plain_t<Left>, as_parser_plain_t<Right>>
operator%(Left&& left, Right&& right)
    noexcept(
        is_parser_nothrow_castable_v<Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<
            list<as_parser_plain_t<Left>, as_parser_plain_t<Right>>,
            as_parser_t<Left>,
            as_parser_t<Right>
        >
    )
{
    return {as_parser(std::forward<Left>(left)), as_parser(std::forward<Right>(right))};
}

} // iris::x4

#endif
