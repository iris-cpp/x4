#ifndef IRIS_ZZ_X4_OPERATOR_KLEENE_HPP
#define IRIS_ZZ_X4_OPERATOR_KLEENE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/list_like_parser.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/expectation.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct kleene : unary_parser<Subject, kleene<Subject>>
{
    using attribute_type = typename traits::default_container<typename parser_traits<Subject>::attribute_type>::type;

    template<class Container>
    static constexpr bool handles_container = std::disjunction_v<
        std::bool_constant<parser_traits<Subject>::template handles_container<Container>>,
        traits::can_hold<typename parser_traits<Subject>::attribute_type, typename traits::container_value<Container>::type>
    >;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        // never noexcept; requires container insertion
    {
        auto& container_attr = list_like_parser::get_container<attribute_type, Attr>(attr);
        list_like_parser::chunk_buffer<attribute_type, Attr> chunk_buf;

        while (detail::parse_into_container(this->subject, first, last, ctx, chunk_buf)) {
            list_like_parser::successful_merge_into(chunk_buf, container_attr);
        }

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr& unused_attr) const
        noexcept(noexcept(detail::parse_into_container(this->subject, first, last, ctx, x4::assume_container(unused_attr))))
    {
        while (detail::parse_into_container(this->subject, first, last, ctx, x4::assume_container(unused_attr)))
            /* loop */;

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }
};

template<X4Subject Subject>
[[nodiscard]] constexpr kleene<as_parser_plain_t<Subject>>
operator*(Subject&& subject)
    noexcept(is_parser_nothrow_constructible_v<kleene<as_parser_plain_t<Subject>>, Subject>)
{
    return {as_parser(std::forward<Subject>(subject))};
}

} // iris::x4

#endif
