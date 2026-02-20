#ifndef IRIS_X4_DIRECTIVE_LEXEME_HPP
#define IRIS_X4_DIRECTIVE_LEXEME_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/context.hpp>
#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/parser.hpp>

#include <format>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct lexeme_directive : proxy_parser<Subject, lexeme_directive<Subject>>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(
            noexcept(x4::skip_over(first, last, ctx)) &&
            is_nothrow_parsable_v<
                Subject, It, Se,
                std::remove_cvref_t<decltype(x4::remove_first_context<contexts::skipper>(ctx))>,
                Attr
            >
        )
    {
        x4::skip_over(first, last, ctx); // pre-skip

        return this->subject.parse(
            first, last,
            x4::remove_first_context<contexts::skipper>(ctx), // no skipper
            attr
        );
    }

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return std::format("lexeme[{}]", get_info<Subject>{}(this->subject));
    }
};

namespace detail {

struct lexeme_gen
{
    template<X4Subject Subject>
    [[nodiscard]] constexpr lexeme_directive<as_parser_plain_t<Subject>>
    operator[](Subject&& subject) const
        noexcept(is_parser_nothrow_constructible_v<lexeme_directive<as_parser_plain_t<Subject>>, Subject>)
    {
        return {as_parser(std::forward<Subject>(subject))};
    }
};

} // detail

namespace parsers::directive {

[[maybe_unused]] inline constexpr detail::lexeme_gen lexeme{};

} // parsers::directive

using parsers::directive::lexeme;

} // iris::x4

#endif
