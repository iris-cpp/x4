#ifndef BOOST_SPIRIT_X4_OPERATOR_ALTERNATIVE_HPP
#define BOOST_SPIRIT_X4_OPERATOR_ALTERNATIVE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/x4/core/expectation.hpp>
#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/detail/parse_alternative.hpp>

#include <boost/spirit/x4/traits/attribute.hpp>

#include <boost/variant/variant.hpp> // TODO: remove this

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

template<class Left, class Right>
struct alternative : binary_parser<Left, Right, alternative<Left, Right>>
{
    using base_type = binary_parser<Left, Right, alternative>;

    template<class LeftT, class RightT>
        requires std::is_constructible_v<base_type, LeftT, RightT>
    constexpr alternative(LeftT&& left, RightT&& right)
        noexcept(std::is_nothrow_constructible_v<base_type, LeftT, RightT>)
        : base_type(std::forward<LeftT>(left), std::forward<RightT>(right))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, unused_type) const
        noexcept(
            is_nothrow_parsable_v<Left, It, Se, Context, unused_type> &&
            is_nothrow_parsable_v<Right, It, Se, Context, unused_type>
        )
    {
        return this->left.parse(first, last, ctx, unused)
            || (!x4::has_expectation_failure(ctx)
                && this->right.parse(first, last, ctx, unused));
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(
            noexcept(detail::parse_alternative(this->left, first, last, ctx, attr)) &&
            noexcept(detail::parse_alternative(this->right, first, last, ctx, attr))
        )
    {
        return detail::parse_alternative(this->left, first, last, ctx, attr)
            || (!x4::has_expectation_failure(ctx)
                && detail::parse_alternative(this->right, first, last, ctx, attr));
    }
};

template<X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr alternative<as_parser_plain_t<Left>, as_parser_plain_t<Right>>
operator|(Left&& left, Right&& right)
    noexcept(
        is_parser_nothrow_castable_v<Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<
            alternative<as_parser_plain_t<Left>, as_parser_plain_t<Right>>,
            as_parser_t<Left>,
            as_parser_t<Right>
        >
    )
{
    return {as_parser(std::forward<Left>(left)), as_parser(std::forward<Right>(right))};
}

} // boost::spirit::x4

namespace boost::spirit::x4::traits {

template<class Left, class Right, class Context>
struct attribute_of<alternative<Left, Right>, Context>
    : x4::detail::attribute_of_binary<boost::variant, alternative, Left, Right, Context>
{};

} // boost::spirit::x4::traits

#endif
