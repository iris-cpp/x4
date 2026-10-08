#ifndef IRIS_ZZ_X4_OPERATOR_PLUS_HPP
#define IRIS_ZZ_X4_OPERATOR_PLUS_HPP

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

#include <string>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct plus : unary_parser<plus<Subject>, Subject>
{
    using attribute_type = traits::default_container<typename parser_traits<Subject>::attribute_type>::type;

    template<class Container>
    static constexpr bool accepts_container = writes_into_container<Subject, Container>;

    using unary_parser<plus, Subject>::unary_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        if constexpr (list_like_parser::writes_as_one_element<attribute_type, Attr>) {
            return list_like_parser::parse_as_one_element(*this, first, last, ctx, attr);

        } else {
            list_like_parser::chunk_buffer<Subject, attribute_type, Attr> chunk_buf(detail::ref_or_init_attribute_for<attribute_type>(attr));

            if (detail::parse_into_container(this->subject, first, last, ctx, chunk_buf.container())) {
                chunk_buf.merge();
            } else {
                return false;
            }

            while (detail::parse_into_container(this->subject, first, last, ctx, chunk_buf.container())) {
                chunk_buf.merge();
            }

            if constexpr (has_context<Context, contexts::expectation_failure>) {
                return !x4::has_expectation_failure(ctx);
            } else {
                return true;
            }
        }
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr& unused_attr) const
    {
        if (!detail::parse_into_container(this->subject, first, last, ctx, x4::assume_container(unused_attr))) {
            return false;
        }

        while (detail::parse_into_container(this->subject, first, last, ctx, x4::assume_container(unused_attr)))
            /* loop */;

        if constexpr (has_context<Context, contexts::expectation_failure>) {
            return !x4::has_expectation_failure(ctx);
        } else {
            return true;
        }
    }

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return '+' + get_info<Subject>{}(this->subject);
    }
};

template<X4Subject Subject>
[[nodiscard]] constexpr plus<as_parser_plain_t<Subject>>
operator+(Subject&& subject)
    noexcept(is_parser_nothrow_constructible_v<plus<as_parser_plain_t<Subject>>, Subject>)
{
    return plus<as_parser_plain_t<Subject>>{as_parser(std::forward<Subject>(subject))};
}

} // iris::x4

#endif
