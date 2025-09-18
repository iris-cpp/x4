/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_X3_DIFFERENCE_FEBRUARY_11_2007_1250PM
#define BOOST_SPIRIT_X3_DIFFERENCE_FEBRUARY_11_2007_1250PM

#include <boost/spirit/x4/support/traits/attribute_of.hpp>
#include <boost/spirit/x4/support/traits/has_attribute.hpp>
#include <boost/spirit/x4/support/expectation.hpp>
#include <boost/spirit/x4/core/parser.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x3
{
    template <typename Left, typename Right>
    struct difference : binary_parser<Left, Right, difference<Left, Right>>
    {
        using base_type = binary_parser<Left, Right, difference<Left, Right>>;

        static constexpr bool handles_container = Left::handles_container;

        template <typename LeftT, typename RightT>
            requires std::is_constructible_v<Left, LeftT> && std::is_constructible_v<Right, RightT>
        constexpr difference(LeftT&& left, RightT&& right)
            noexcept(std::is_nothrow_constructible_v<Left, LeftT> && std::is_nothrow_constructible_v<Right, RightT>)
            : base_type(std::forward<LeftT>(left), std::forward<RightT>(right))
        {}

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename RContext, typename Attribute>
        [[nodiscard]] constexpr bool
        parse(It& first, Se const& last, Context const& context, RContext& rcontext, Attribute& attr) const
        {
            // Try Right first
            It start = first;
            if (this->right.parse(first, last, context, rcontext, unused))
            {
                // Right succeeds, we fail.
                first = start;
                return false;
            }

            // In case of `Left - expect[r]`,
            // if Right yielded expectation error,
            // the whole difference expression (*this) should also yield error.
            // In other words, when the THROW macro was 1 (i.e. traditional behavior),
            // Right should already have thrown an exception.
            if (x3::has_expectation_failure(context))
            {
                // don't rollback iterator (mimicking exception-like behavior)
                return false;
            }

            // Right fails, now try Left
            return this->left.parse(first, last, context, rcontext, attr);
        }
    };

    template <X3Subject Left, X3Subject Right>
    [[nodiscard]] constexpr difference<as_parser_plain_t<Left>, as_parser_plain_t<Right>>
    operator-(Left&& left, Right&& right)
        noexcept(
            is_parser_nothrow_castable_v<Left> &&
            is_parser_nothrow_castable_v<Right> &&
            std::is_nothrow_constructible_v<
                difference<as_parser_plain_t<Left>, as_parser_plain_t<Right>>,
                as_parser_t<Left>,
                as_parser_t<Right>
            >
        )
    {
        return { as_parser(std::forward<Left>(left)), as_parser(std::forward<Right>(right)) };
    }

} // boost::spirit::x3

namespace boost::spirit::x3::traits
{
    template <typename Left, typename Right, typename Context>
    struct attribute_of<x3::difference<Left, Right>, Context>
        : attribute_of<Left, Context>
    {};

    template <typename Left, typename Right, typename Context>
    struct has_attribute<x3::difference<Left, Right>, Context>
        : has_attribute<Left, Context>
    {};

} // boost::spirit::x3::traits

#endif
