/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_X3_PLUS_MARCH_13_2007_0127PM
#define BOOST_SPIRIT_X3_PLUS_MARCH_13_2007_0127PM

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>
#include <boost/spirit/home/x3/support/expectation.hpp>
#include <boost/spirit/home/x3/support/traits/container_traits.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/core/detail/parse_into_container.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x3
{
    template <typename Subject>
    struct plus : unary_parser<Subject, plus<Subject>>
    {
        using base_type = unary_parser<Subject, plus<Subject>>;

        static constexpr bool handles_container = true;

        template <typename SubjectT>
            requires
                (!std::is_same_v<std::remove_cvref_t<SubjectT>, plus>) &&
                std::is_constructible_v<Subject, SubjectT>
        constexpr plus(SubjectT&& subject)
            noexcept(std::is_nothrow_constructible_v<Subject, SubjectT>)
            : base_type(std::forward<SubjectT>(subject))
        {}

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename RContext, typename Attribute>
        [[nodiscard]] constexpr bool
        parse(It& first, Se const& last, Context const& context, RContext& rcontext, Attribute& attr) const
            noexcept(noexcept(detail::parse_into_container(this->subject, first, last, context, rcontext, x3::assume_container(attr))))
        {
            if (!detail::parse_into_container(this->subject, first, last, context, rcontext, x3::assume_container(attr)))
            {
                return false;
            }

            while (detail::parse_into_container(this->subject, first, last, context, rcontext, x3::assume_container(attr)))
                /* loop */;

            return !x3::has_expectation_failure(context);
        }
    };

    template <X3Subject Subject>
    [[nodiscard]] constexpr plus<as_parser_plain_t<Subject>>
    operator+(Subject&& subject)
        noexcept(is_parser_nothrow_constructible_v<plus<as_parser_plain_t<Subject>>, Subject>)
    {
        return { as_parser(std::forward<Subject>(subject)) };
    }
} // boost::spirit::x3

namespace boost::spirit::x3::traits
{
    template <typename Subject, typename Context>
    struct attribute_of<x3::plus<Subject>, Context>
        : build_container<attribute_of_t<Subject, Context>>
    {};
} // boost::spirit::x3::traits

#endif
