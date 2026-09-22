#ifndef IRIS_ZZ_X4_DIRECTIVE_LEXEME_HPP
#define IRIS_ZZ_X4_DIRECTIVE_LEXEME_HPP

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

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct lexeme_directive : proxy_parser<lexeme_directive<Subject>, Subject>
{
    using proxy_parser<lexeme_directive, Subject>::proxy_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        auto it = first;
        x4::skip_over(it, last, ctx); // pre-skip

        bool const ok = this->subject.parse(
            it, last,
            x4::remove_first_context<contexts::skipper>(ctx), // no skipper
            attr
        );
        if (ok) first = it;
        return ok;
    }

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return "lexeme[" + get_info<Subject>{}(this->subject) + ']';
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
        return lexeme_directive<as_parser_plain_t<Subject>>{as_parser(std::forward<Subject>(subject))};
    }
};

} // detail

namespace parsers::directive {

[[maybe_unused]] inline constexpr detail::lexeme_gen lexeme{};

} // parsers::directive

using parsers::directive::lexeme;

} // iris::x4

#endif
