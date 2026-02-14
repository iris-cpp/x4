#ifndef IRIS_X4_CORE_SKIP_OVER_HPP
#define IRIS_X4_CORE_SKIP_OVER_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp>

#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/context.hpp>
#include <iris/x4/core/parser.hpp>

#include <iterator>
#include <type_traits>

namespace iris::x4 {

namespace contexts {

// Tag used to find the skipper from the context
struct skipper
{
    static constexpr bool is_unique = true;
};

} // contexts

using skipper_tag [[deprecated("Use `x4::contexts::skipper`")]] = contexts::skipper;


enum struct builtin_skipper_kind : char
{
    no_skip,
    blank,
    space,
};

namespace detail {

template<class Context>
struct skip_over_context
{
    using type = unused_type;
};

template<class Context>
    requires (!std::is_same_v<expectation_failure_t<Context>, unused_type>)
struct skip_over_context<Context>
{
    using type = std::remove_cvref_t<decltype(x4::make_context<contexts::expectation_failure>(
        x4::get<contexts::expectation_failure>(std::declval<Context const&>())
    ))>;
};

} // detail

template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    requires X4Subject<get_context_plain_t<contexts::skipper, Context>>
constexpr void skip_over(It& first, Se const& last, Context const& ctx)
    noexcept(is_nothrow_parsable_v<
        get_context_plain_t<contexts::skipper, Context>,
        It, Se,
        typename detail::skip_over_context<Context>::type, unused_type
    >)
{
    auto const& skipper = x4::get<contexts::skipper>(ctx);

    if constexpr (!has_context_v<Context, contexts::expectation_failure>) {
        // The context given by parent was truly `unused_type`.
        // There exists only one such case in core; that is
        // `x4::phrase_parse(...)` which creates a fresh context
        // for the (post)skipper.
        //
        // In that case, it is perfectly fine to pass `unused`
        // because the skipper should have been wrapped
        // like `x4::with<x4::contexts::expectation_failure>(failure)[skipper]`.
        // (Note that we have plenty of static_asserts in other
        // locations to detect the absence of the context.)
        //
        // If we encounter this branch in any other situations,
        // that should be a BUG of `expectation_failure` logic.

        while (skipper.parse(first, last, unused, unused))
            /* loop */;

    } else {
        // In order to cut the template instantiation chain,
        // we must *forget* the original context at least once
        // during the (recursive) invocation of skippers.
        //
        // Traditionally, implementation detail of `skip_over`
        // was disposing the context because we can clearly assume
        // that any 'context,' including those provided by users,
        // is semantically meaningless as long as we're just
        // *skipping* iterators. As you can see in the other branch,
        // `unused` was passed for that purpose.
        //
        // However, we need to do a quite different thing when the
        // non-throwing expectation_failure mode is enabled.
        //
        // Since the reference bound to `x4::contexts::expectation_failure` is
        // provided by the user in the first place, if we do forget it
        // then it will be impossible to resurrect the value afterward.
        // It will also be problematic for `skip_over` itself because the
        // underlying skipper may (or may not) raise an expectation failure.
        // In traditional mode, the error was thrown by a C++ exception.
        // But how can we propagate that error without throwing?
        //
        // For this reason we're going to cherry-pick the reference
        // and repack it into a brand-new context.

        auto const local_ctx = x4::make_context<contexts::expectation_failure>(
            x4::get<contexts::expectation_failure>(ctx));

        while (skipper.parse(first, last, local_ctx, unused))
            /* loop */;
    }
}

// Implemented in `char_class.hpp`
template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    requires std::same_as<get_context_plain_t<contexts::skipper, Context>, builtin_skipper_kind>
constexpr void skip_over(It& first, Se const& last, Context const& ctx) noexcept;


template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    requires (!has_context_v<Context, contexts::skipper>)
constexpr void skip_over(It&, Se const&, Context const&) noexcept
{
}

} // iris::x4

#endif
