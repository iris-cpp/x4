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
#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/parser.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

#include <cassert>

namespace iris::x4 {

namespace detail {

template<class Context>
[[nodiscard]] constexpr decltype(auto) make_lexeme_context(Context const& ctx, builtin_skipper_kind& skipper_kind) noexcept
{
    static_assert(!has_context_of_v<Context, contexts::skipper, builtin_skipper_kind>);
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

template<class Subject>
struct lexeme_directive : proxy_parser<lexeme_directive<Subject>, Subject>
{
    using proxy_parser<lexeme_directive, Subject>::proxy_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        auto local_it = first;
        x4::skip_over(local_it, last, ctx); // pre-skip

        bool ok;
        if constexpr (has_context_of_v<Context, contexts::skipper, builtin_skipper_kind>) {
            builtin_skipper_kind& skipper_kind = x4::get<contexts::skipper>(ctx);
            auto const old_skipper_kind = skipper_kind;
            skipper_kind = builtin_skipper_kind::no_skip;
            ok = this->subject.parse(local_it, last, ctx, attr);
            skipper_kind = old_skipper_kind;

        } else {
            // This value could be reset by some nested parsers, so it can't be const
            /* constexpr */ builtin_skipper_kind skipper_kind = builtin_skipper_kind::no_skip;
            ok = this->subject.parse(local_it, last, detail::make_lexeme_context(ctx, skipper_kind), attr);
        }

        if (ok) {
            first = std::move(local_it);
            return true;
        }
        if constexpr (has_context<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) {
                // don't rollback iterator (mimicking exception-like behavior)
                first = std::move(local_it);
            }
        }
        return false;
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
