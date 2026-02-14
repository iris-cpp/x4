#ifndef IRIS_X4_DIRECTIVE_NO_SKIP_HPP
#define IRIS_X4_DIRECTIVE_NO_SKIP_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2013 Agustin Berge
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

// Same as `lexeme[...]`, but does not pre-skip
template<class Subject>
struct no_skip_directive : proxy_parser<Subject, no_skip_directive<Subject>>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(is_nothrow_parsable_v<
            Subject, It, Se,
            std::remove_cvref_t<decltype(x4::remove_first_context<contexts::skipper>(ctx))>,
            Attr
        >)
    {
        //
        // No pre-skip here, in contrast to `lexeme`
        //

        return this->subject.parse(
            first, last,
            x4::remove_first_context<contexts::skipper>(ctx),
            attr
        );
    }
};

namespace detail {

struct no_skip_gen
{
    template<X4Subject Subject>
    [[nodiscard]] constexpr no_skip_directive<as_parser_plain_t<Subject>>
    operator[](Subject&& subject) const // TODO: MSVC can't handle static operator[]
        noexcept(is_parser_nothrow_constructible_v<no_skip_directive<as_parser_plain_t<Subject>>, Subject>)
    {
        return {as_parser(std::forward<Subject>(subject))};
    }
};

} // detail

namespace parsers::directive {

[[maybe_unused]] inline constexpr detail::no_skip_gen no_skip{};

} // parsers::directive

using parsers::directive::no_skip;

} // iris::x4

#endif
