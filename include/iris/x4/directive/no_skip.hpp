#ifndef IRIS_ZZ_X4_DIRECTIVE_NO_SKIP_HPP
#define IRIS_ZZ_X4_DIRECTIVE_NO_SKIP_HPP

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
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/skip_over.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

#include <cassert>

namespace iris::x4 {

namespace detail {

template<class Context>
[[nodiscard]] constexpr decltype(auto) make_no_skip_context(Context const& ctx, builtin_skipper_kind& skipper_kind) noexcept
{
    static_assert(!has_context_of<Context, contexts::skipper, builtin_skipper_kind>);
    assert(skipper_kind == builtin_skipper_kind::no_skip);

    // Declare a concrete alias type; MSVC prints the alias instead of actual type,
    // which makes the compilation error significantly shorter.

    // Note: we must use "append" here since the position of the skipper should be
    // super stable in the context as the exact type is often referenced by
    // `IRIS_X4_INSTANTIATE`.
    using T = std::remove_cvref_t<decltype(x4::replace_first_or_append_context<contexts::skipper>(ctx, skipper_kind))>;
    return detail::named_context<T>(x4::replace_first_or_append_context<contexts::skipper>(ctx, skipper_kind));
}

} // detail

// Same as `lexeme[...]`, but does not pre-skip
template<class Subject>
struct no_skip_directive : proxy_parser<no_skip_directive<Subject>, Subject>
{
    using proxy_parser<no_skip_directive, Subject>::proxy_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        // No pre-skip here, in contrast to `lexeme`

        if constexpr (has_context_of<Context, contexts::skipper, builtin_skipper_kind>) {
            builtin_skipper_kind& skipper_kind = x4::get<contexts::skipper>(ctx);
            auto const old_skipper_kind = skipper_kind;
            skipper_kind = builtin_skipper_kind::no_skip;
            bool const ok = this->subject.parse(first, last, ctx, attr);
            skipper_kind = old_skipper_kind;
            return ok;

        } else {
            // This value could be reset by some nested parsers, so it can't be const
            /* constexpr */ builtin_skipper_kind skipper_kind = builtin_skipper_kind::no_skip;
            return this->subject.parse(first, last, detail::make_no_skip_context(ctx, skipper_kind), attr);
        }
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
        return no_skip_directive<as_parser_plain_t<Subject>>{as_parser(std::forward<Subject>(subject))};
    }
};

} // detail

namespace parsers::directive {

[[maybe_unused]] inline constexpr detail::no_skip_gen no_skip{};

} // parsers::directive

using parsers::directive::no_skip;

} // iris::x4

#endif
