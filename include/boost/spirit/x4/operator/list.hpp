/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_X3_LIST_MARCH_24_2007_1031AM
#define BOOST_SPIRIT_X3_LIST_MARCH_24_2007_1031AM

#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/detail/parse_into_container.hpp>
#include <boost/spirit/x4/core/expectation.hpp>
#include <boost/spirit/x4/core/unused.hpp>
#include <boost/spirit/x4/traits/container_traits.hpp>
#include <boost/spirit/x4/traits/attribute_of.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x3
{
    template <typename Left, typename Right>
    struct list : binary_parser<Left, Right, list<Left, Right>>
    {
        using base_type = binary_parser<Left, Right, list<Left, Right>>;

        static constexpr bool handles_container = true;

        template <typename LeftT, typename RightT>
            requires std::is_constructible_v<Left, LeftT> && std::is_constructible_v<Right, RightT>
        constexpr list(LeftT&& left, RightT&& right)
            noexcept(std::is_nothrow_constructible_v<Left, LeftT> && std::is_nothrow_constructible_v<Right, RightT>)
            : base_type(std::forward<LeftT>(left), std::forward<RightT>(right))
        {}

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename RContext, typename Attribute>
        [[nodiscard]] constexpr bool
        parse(It& first, Se const& last, Context const& context, RContext& rcontext, Attribute& attr) const
            noexcept(
                noexcept(detail::parse_into_container(this->left, first, last, context, rcontext, x3::assume_container(attr))) &&
                std::is_nothrow_copy_assignable_v<It> &&
                is_nothrow_parsable_v<Right, It, Se, Context, RContext, unused_type>
            )
        {
            // In order to succeed we need to match at least one element
            if (!detail::parse_into_container(this->left, first, last, context, rcontext, x3::assume_container(attr)))
            {
                return false;
            }

            It last_parse_it = first;
            while (
                this->right.parse(last_parse_it, last, context, rcontext, unused) &&
                detail::parse_into_container(this->left, last_parse_it, last, context, rcontext, x3::assume_container(attr))
            )
            {
                // TODO: can we reduce this copy assignment?
                first = last_parse_it;
            }

            return !x3::has_expectation_failure(context);
        }
    };

    template <X3Subject Left, X3Subject Right>
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
        return { as_parser(std::forward<Left>(left)), as_parser(std::forward<Right>(right)) };
    }
} // boost::spirit::x3

namespace boost::spirit::x3::traits
{
    template <typename Left, typename Right, typename Context>
    struct attribute_of<x3::list<Left, Right>, Context>
        : traits::build_container<attribute_of_t<Left, Context>>
    {};

    template <typename Left, typename Right, typename Context>
    struct has_attribute<x3::list<Left, Right>, Context>
        : has_attribute<Left, Context>
    {};
} // boost::spirit::x3::traits

#endif
