#ifndef BOOST_SPIRIT_X4_OPERATOR_SEQUENCE_HPP
#define BOOST_SPIRIT_X4_OPERATOR_SEQUENCE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/x4/core/expectation.hpp>
#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/detail/parse_sequence.hpp>

#include <boost/spirit/x4/traits/attribute.hpp>

#include <boost/spirit/x4/directive/expect.hpp>

#include <boost/fusion/include/deque_fwd.hpp> // TODO: remove this

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

template <typename Left, typename Right>
struct sequence : binary_parser<Left, Right, sequence<Left, Right>>
{
    using base_type = binary_parser<Left, Right, sequence<Left, Right>>;

    template <typename LeftT, typename RightT>
        requires std::is_constructible_v<base_type, LeftT, RightT>
    constexpr sequence(LeftT&& left, RightT&& right)
        noexcept(std::is_nothrow_constructible_v<base_type, LeftT, RightT>)
        : base_type(std::forward<LeftT>(left), std::forward<RightT>(right))
    {}

    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, unused_type) const
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            is_nothrow_parsable_v<Left, It, Se, Context, unused_type> &&
            is_nothrow_parsable_v<Right, It, Se, Context, unused_type>
        )
    {
        It const first_saved = first;

        if (this->left.parse(first, last, context, unused)
            && this->right.parse(first, last, context, unused))
        {
            return true;
        }

        if (x4::has_expectation_failure(context))
        {
            // don't rollback iterator (mimicking exception-like behavior)
            return false;
        }

        first = first_saved;
        return false;
    }

    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
        requires (!std::is_same_v<std::remove_const_t<Attribute>, unused_type>)
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, Attribute& attr) const
        noexcept(noexcept(detail::parse_sequence(*this, first, last, context, attr)))
    {
        return detail::parse_sequence(*this, first, last, context, attr);
    }
};

template <X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr sequence<as_parser_plain_t<Left>, as_parser_plain_t<Right>>
operator>>(Left&& left, Right&& right)
    noexcept(
        is_parser_nothrow_castable_v<Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<
            sequence<as_parser_plain_t<Left>, as_parser_plain_t<Right>>,
            as_parser_t<Left>,
            as_parser_t<Right>
        >
    )
{
    return { as_parser(std::forward<Left>(left)), as_parser(std::forward<Right>(right)) };
}

template <X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr sequence<as_parser_plain_t<Left>, expect_directive<as_parser_plain_t<Right>>>
operator>(Left&& left, Right&& right)
    noexcept(
        is_parser_nothrow_castable_v<Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<
            expect_directive<as_parser_plain_t<Right>>,
            as_parser_t<Right>
        > &&
        std::is_nothrow_constructible_v<
            sequence<as_parser_plain_t<Left>, expect_directive<as_parser_plain_t<Right>>>,
            as_parser_t<Left>,
            expect_directive<as_parser_plain_t<Right>>
        >
    )
{
    return {
        as_parser(std::forward<Left>(left)),
        expect_directive<as_parser_plain_t<Right>>(as_parser(std::forward<Right>(right)))
    };
}

} // boost::spirit::x4

namespace boost::spirit::x4::traits {

template <typename Left, typename Right, typename Context>
struct attribute_of<x4::sequence<Left, Right>, Context>
    : x4::detail::attribute_of_binary<fusion::deque, x4::sequence, Left, Right, Context> {};

} // boost::spirit::x4::traits

#endif
