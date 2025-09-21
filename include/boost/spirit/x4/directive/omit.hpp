#ifndef BOOST_SPIRIT_X4_DIRECTIVE_OMIT_HPP
#define BOOST_SPIRIT_X4_DIRECTIVE_OMIT_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/x4/core/unused.hpp>
#include <boost/spirit/x4/core/parser.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

// `omit_directive` forces the attribute of subject parser
// to be `unused_type`
template <typename Subject>
struct omit_directive : unary_parser<Subject, omit_directive<Subject>>
{
    using base_type = unary_parser<Subject, omit_directive<Subject>>;
    using attribute_type = unused_type;
    using subject_type = Subject;

    static constexpr bool has_attribute = false;

    template <typename SubjectT>
        requires
            (!std::is_same_v<std::remove_cvref_t<SubjectT>, omit_directive>) &&
            std::is_constructible_v<base_type, SubjectT>
    constexpr omit_directive(SubjectT&& subject)
        noexcept(std::is_nothrow_constructible_v<base_type, SubjectT>)
        : base_type(std::forward<SubjectT>(subject))
    {}

    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, Attribute const& /* omitted */) const
        noexcept(is_nothrow_parsable_v<Subject, It, Se, Context, unused_type>)
    {
        static_assert(Parsable<Subject, It, Se, Context, unused_type>);
        return this->subject.parse(first, last, context, unused);
    }
};

namespace detail {

struct omit_gen
{
    template <X4Subject Subject>
    [[nodiscard]] constexpr omit_directive<as_parser_plain_t<Subject>>
    operator[](Subject&& subject) const
        noexcept(is_parser_nothrow_constructible_v<omit_directive<as_parser_plain_t<Subject>>, Subject>)
    {
        return {as_parser(std::forward<Subject>(subject))};
    }
};

} // detail

inline namespace cpos {

[[maybe_unused]] inline constexpr detail::omit_gen omit{};

} // cpos

} // boost::spirit::x4

#endif
