#ifndef BOOST_SPIRIT_X4_DIRECTIVE_RAW_HPP
#define BOOST_SPIRIT_X4_DIRECTIVE_RAW_HPP

/*=============================================================================
    Copyright (c) 2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/x4/core/skip_over.hpp>
#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/move_to.hpp>

#include <boost/spirit/x4/traits/attribute.hpp>

#include <ranges>
#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

// Pseudo attribute type indicating that the parser wants the
// iterator range pointing to the [first, last) matching characters from
// the input iterators.
struct raw_attribute_type {}; // TODO: move this to detail

template<class Subject>
struct raw_directive : unary_parser<Subject, raw_directive<Subject>>
{
    using base_type = unary_parser<Subject, raw_directive<Subject>>;
    using attribute_type = raw_attribute_type;
    using subject_type = Subject;

    static constexpr bool handles_container = true;

    template<class SubjectT>
        requires
            (!std::is_same_v<std::remove_cvref_t<SubjectT>, raw_directive>) &&
            std::is_constructible_v<base_type, SubjectT>
    constexpr raw_directive(SubjectT&& subject)
        noexcept(std::is_nothrow_constructible_v<base_type, SubjectT>)
        : base_type(std::forward<SubjectT>(subject))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, Attr& attr) const
        // never noexcept; construction of `std::ranges::subrange` is never noexcept
    {
        static_assert(Parsable<Subject, It, Se, Context, unused_type>);

        x4::skip_over(first, last, context);
        It local_it = first;
        if (!this->subject.parse(local_it, last, context, unused)) return false;

        x4::move_to(first, local_it, attr);
        first = local_it;
        return true;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, unused_type) const
        noexcept(is_nothrow_parsable_v<Subject, It, Se, Context, unused_type>)
    {
        return this->subject.parse(first, last, context, unused);
    }
};

namespace detail {

struct raw_gen
{
    template<X4Subject Subject>
    [[nodiscard]] constexpr raw_directive<as_parser_plain_t<Subject>>
    operator[](Subject&& subject) const
        noexcept(is_parser_nothrow_constructible_v<raw_directive<as_parser_plain_t<Subject>>, Subject>)
    {
        return {as_parser(std::forward<Subject>(subject))};
    }
};

} // detail

inline namespace cpos {

inline constexpr detail::raw_gen raw{};

} // cpos

} // boost::spirit::x4

namespace boost::spirit::x4::traits {

template<class Context, std::forward_iterator It>
struct pseudo_attribute<Context, raw_attribute_type, It>
{
    using attribute_type = raw_attribute_type;
    using type = std::ranges::subrange<It>;

    [[nodiscard]] static constexpr type call(It& first, std::sentinel_for<It> auto const& last, raw_attribute_type)
    {
        return {first, last};
    }
};

} // boost::spirit::x4::traits

#endif
