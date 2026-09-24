#ifndef IRIS_ZZ_X4_OPERATOR_OPTIONAL_HPP
#define IRIS_ZZ_X4_OPERATOR_OPTIONAL_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/traits/attribute_traits.hpp>
#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/core/traits/optional_traits.hpp>
#include <iris/x4/core/traits/attribute_category.hpp>

#include <iris/x4/core/detail/parse_into_container.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/expectation.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct optional : unary_parser<optional<Subject>, Subject>
{
    using attribute_type = build_optional<typename parser_traits<Subject>::attribute_type>::type;

    template<class Container>
    static constexpr bool handles_container = std::disjunction_v<
        std::bool_constant<parser_traits<Subject>::template handles_container<Container>>,
        can_hold<typename parser_traits<Subject>::attribute_type, typename traits::container_value<Container>::type>
    >;

    using unary_parser<optional, Subject>::unary_parser;

    // catch-all overload
    template<
        std::forward_iterator It, std::sentinel_for<It> Se, class Context,
        class Attr // unconstrained
    >
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        if (this->subject.parse(first, last, ctx, attr)) {
            return true;
        }
        traits::attribute_traits<Attr>::reset(attr);

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }

    // optional attribute
    template<
        std::forward_iterator It, std::sentinel_for<It> Se, class Context,
        CategorizedAttr<optional_tag> Attr
    >
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        if (this->subject.parse(
            first, last, ctx,
            detail::prepare_attribute<typename parser_traits<Subject>::attribute_type>(attr)
        )) {
            return true;
        }
        traits::attribute_traits<Attr>::reset(attr);

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }

    // container attribute
    template<
        std::forward_iterator It, std::sentinel_for<It> Se, class Context,
        CategorizedAttr<container_tag> Attr
    >
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        // Same logic as in `x4::alternative`

        if (traits::is_empty(attr)) {
            if (detail::parse_into_container(this->subject, first, last, ctx, attr)) {
                return true;
            }
            traits::clear(attr);

        } else {
            unwrap_container_appender_t<Attr> buffer;
            if (detail::parse_into_container(this->subject, first, last, ctx, buffer)) {
                traits::append(
                    attr,
                    std::make_move_iterator(traits::begin(buffer)),
                    std::make_move_iterator(traits::end(buffer))
                );
                return true;
            }
        }

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }
};

template<X4Subject Subject>
[[nodiscard]] constexpr optional<as_parser_plain_t<Subject>>
operator-(Subject&& subject)
    noexcept(is_parser_nothrow_constructible_v<optional<as_parser_plain_t<Subject>>, Subject>)
{
    return optional<as_parser_plain_t<Subject>>{as_parser(std::forward<Subject>(subject))};
}

} // iris::x4

#endif
