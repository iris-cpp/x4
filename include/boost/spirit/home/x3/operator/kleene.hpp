/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_X3_KLEENE_JANUARY_07_2007_0818AM
#define BOOST_SPIRIT_X3_KLEENE_JANUARY_07_2007_0818AM

#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/support/traits/container_traits.hpp>
#include <boost/spirit/home/x3/support/traits/attribute_of.hpp>
#include <boost/spirit/home/x3/support/expectation.hpp>
#include <boost/spirit/home/x3/core/detail/parse_into_container.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x3
{
    template <typename Subject>
    struct kleene : unary_parser<Subject, kleene<Subject>>
    {
        using base_type = unary_parser<Subject, kleene<Subject>>;
        static constexpr bool handles_container = true;

        template <typename SubjectT>
            requires
                (!std::is_same_v<std::remove_cvref_t<SubjectT>, kleene>) &&
                std::is_constructible_v<Subject, SubjectT>
        constexpr kleene(SubjectT&& subject)
            noexcept(std::is_nothrow_constructible_v<Subject, SubjectT>)
            : base_type(std::forward<SubjectT>(subject))
        {}

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename RContext, typename Attribute>
        [[nodiscard]] constexpr bool
        parse(It& first, Se const& last, Context const& context, RContext& rcontext, Attribute& attr) const
            // Note: this should almost always be `noexcept(false)`, as it requires container insertion.
            // The only predictable situation for this to become `noexcept(true)` would be `unused` attribute.
            noexcept(noexcept(detail::parse_into_container(this->subject, first, last, context, rcontext, attr)))
        {
            while (detail::parse_into_container(this->subject, first, last, context, rcontext, attr))
                /* loop */;

            return !x3::has_expectation_failure(context);
        }
    };

    template <X3Subject Subject>
    [[nodiscard]] constexpr kleene<as_parser_plain_t<Subject>>
    operator*(Subject&& subject)
        noexcept(is_parser_nothrow_constructible_v<kleene<as_parser_plain_t<Subject>>, Subject>)
    {
        return { as_parser(std::forward<Subject>(subject)) };
    }
} // boost::spirit::x3

namespace boost::spirit::x3::traits
{
    template <typename Subject, typename Context>
    struct attribute_of<x3::kleene<Subject>, Context>
        : build_container<attribute_of_t<Subject, Context>> {};

} // boost::spirit::x3::traits

#endif
