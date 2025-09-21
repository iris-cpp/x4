#ifndef BOOST_SPIRIT_X4_OPERATOR_NOT_PREDICATE_HPP
#define BOOST_SPIRIT_X4_OPERATOR_NOT_PREDICATE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/expectation.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

template <class Subject>
struct not_predicate : unary_parser<Subject, not_predicate<Subject>>
{
    using base_type = unary_parser<Subject, not_predicate>;
    using attribute_type = unused_type;

    static constexpr bool has_attribute = false;

    template <class SubjectT>
        requires
            (!std::is_same_v<std::remove_cvref_t<SubjectT>, not_predicate>) &&
            std::is_constructible_v<base_type, SubjectT>
    constexpr not_predicate(SubjectT&& subject)
        noexcept(std::is_nothrow_constructible_v<base_type, SubjectT>)
        : base_type(std::forward<SubjectT>(subject))
    {}

    template <std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attribute>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, Attribute& /*attr*/) const
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            is_nothrow_parsable_v<Subject, It, Se, Context, unused_type>
        )
    {
        It local_first = first;
        return !this->subject.parse(local_first, last, context, unused)
            && !x4::has_expectation_failure(context);
    }
};

template <X4Subject Subject>
[[nodiscard]] constexpr not_predicate<as_parser_plain_t<Subject>>
operator!(Subject&& subject)
    noexcept(is_parser_nothrow_constructible_v<not_predicate<as_parser_plain_t<Subject>>, Subject>)
{
    return {as_parser(std::forward<Subject>(subject))};
}

} // boost::spirit::x4

#endif
