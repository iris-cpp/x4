#ifndef BOOST_SPIRIT_X4_OPERATOR_OPTIONAL_HPP
#define BOOST_SPIRIT_X4_OPERATOR_OPTIONAL_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/detail/parse_into_container.hpp>
#include <boost/spirit/x4/core/expectation.hpp>
#include <boost/spirit/x4/core/move_to.hpp>

#include <boost/spirit/x4/traits/attribute.hpp>
#include <boost/spirit/x4/traits/optional_traits.hpp>
#include <boost/spirit/x4/traits/attribute_category.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

template <typename Subject>
struct optional : unary_parser<Subject, optional<Subject>>
{
    using base_type = unary_parser<Subject, optional>;
    static constexpr bool is_pass_through_unary = false;
    static constexpr bool handles_container = true;

    template <typename SubjectT>
        requires
            (!std::is_same_v<std::remove_cvref_t<SubjectT>, optional>) &&
            std::is_constructible_v<base_type, SubjectT>
    constexpr optional(SubjectT&& subject)
        noexcept(std::is_nothrow_constructible_v<base_type, SubjectT>)
        : base_type(std::forward<SubjectT>(subject))
    {}

    // catch-all overload
    template <
        std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute // unconstrained
    >
    [[nodiscard]] constexpr bool
    parse(
        It& first, Se const& last, Context const& context, Attribute& attr
    ) const
        noexcept(is_nothrow_parsable_v<Subject, It, Se, Context, Attribute>)
    {
        static_assert(Parsable<Subject, It, Se, Context, Attribute>);

        // discard [[nodiscard]]
        (void)this->subject.parse(first, last, context, attr);
        return !x4::has_expectation_failure(context);
    }

    // container attribute
    template <
        std::forward_iterator It, std::sentinel_for<It> Se, typename Context, traits::CategorizedAttr<traits::container_attribute> Attribute
    >
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, Attribute& attr) const
        noexcept(noexcept(detail::parse_into_container(this->subject, first, last, context, attr)))
    {
        // discard [[nodiscard]]
        (void)detail::parse_into_container(this->subject, first, last, context, attr);
        return !x4::has_expectation_failure(context);
    }

    // optional attribute
    template <
        std::forward_iterator It, std::sentinel_for<It> Se, typename Context, traits::CategorizedAttr<traits::optional_attribute> Attribute
    >
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, Attribute& attr) const
        noexcept(
            std::is_nothrow_default_constructible_v<x4::traits::optional_value_t<Attribute>> &&
            is_nothrow_parsable_v<Subject, It, Se, Context, x4::traits::optional_value_t<Attribute>> &&
            noexcept(x4::move_to(std::declval<x4::traits::optional_value_t<Attribute>&&>(), attr))
        )
    {
        using value_type = x4::traits::optional_value_t<Attribute>;
        value_type val; // default-initialize

        static_assert(Parsable<Subject, It, Se, Context, value_type>);
        if (this->subject.parse(first, last, context, val))
        {
            // assign the parsed value into our attribute
            x4::move_to(std::move(val), attr);
            return true;
        }

        return !x4::has_expectation_failure(context);
    }
};

template <X4Subject Subject>
[[nodiscard]] constexpr optional<as_parser_plain_t<Subject>>
operator-(Subject&& subject)
    noexcept(is_parser_nothrow_constructible_v<optional<as_parser_plain_t<Subject>>, Subject>)
{
    return { as_parser(std::forward<Subject>(subject)) };
}

} // boost::spirit::x4

namespace boost::spirit::x4::traits {

template <typename Subject, typename Context>
struct attribute_of<x4::optional<Subject>, Context>
    : build_optional<attribute_of_t<Subject, Context>>
{};

} // boost::spirit::x4::traits

#endif
