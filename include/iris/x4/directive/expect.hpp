#ifndef IRIS_X4_DIRECTIVE_EXPECT_HPP
#define IRIS_X4_DIRECTIVE_EXPECT_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/detail/parse_into_container.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct expect_directive : proxy_parser<Subject, expect_directive<Subject>>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        // never noexcept; expectation failure requires construction of debug information
    {
        static_assert(
            has_context_v<Context, contexts::expectation_failure>,
            "Context type was not specified for `x4::contexts::expectation_failure`. "
            "You probably forgot: `x4::with<x4::contexts::expectation_failure>(failure)[p]`. "
            "Note that you must also bind the context to your skipper."
        );

        bool const r = this->subject.parse(first, last, ctx, attr);

        // only the first failure is needed
        if (!r && !x4::has_expectation_failure(ctx)) {
            x4::set_expectation_failure(first, this->subject, ctx);
        }
        return r;
    }
};

namespace detail {

struct expect_gen
{
    template<X4Subject Subject>
    [[nodiscard]] constexpr expect_directive<as_parser_plain_t<Subject>>
    operator[](Subject&& subject) const
        noexcept(is_parser_nothrow_constructible_v<expect_directive<as_parser_plain_t<Subject>>, Subject>)
    {
        return {as_parser(std::forward<Subject>(subject))};
    }
};

} // detail

namespace parsers::directive {

[[maybe_unused]] inline constexpr detail::expect_gen expect{};

} // parsers::directive

using parsers::directive::expect;

} // iris::x4

namespace iris::x4::detail {

// Special case handling for expect expressions.
template<class Subject>
struct parse_into_container_impl<expect_directive<Subject>>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    call(
        expect_directive<Subject> const& parser,
        It& first, Se const& last, Context const& ctx, Attr& attr
    ) // never noexcept; expectation failure requires construction of debug information
    {
        static_assert(
            has_context_v<Context, contexts::expectation_failure>,
            "Context type was not specified for `x4::contexts::expectation_failure`. "
            "You probably forgot: `x4::with<x4::contexts::expectation_failure>(failure)[p]`. "
            "Note that you must also bind the context to your skipper."
        );

        bool const r = detail::parse_into_container(parser.subject, first, last, ctx, attr);

        // only the first error is needed
        if (!r && !x4::has_expectation_failure(ctx)) {
            x4::set_expectation_failure(first, parser.subject, ctx);
        }
        return r;
    }
};

} // iris::x4::detail

#endif
