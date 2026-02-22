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

template<builtin_skipper_kind Kind>
struct builtin_skipper_traits;

template<>
struct builtin_skipper_traits<builtin_skipper_kind::blank>
{
    static constexpr char const* name = "blank";
};

template<>
struct builtin_skipper_traits<builtin_skipper_kind::space>
{
    static constexpr char const* name = "space";
};

} // detail

template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    requires X4Subject<get_context_plain_t<contexts::skipper, Context>>
constexpr void skip_over(It& first, Se const& last, Context const& ctx)
    noexcept(is_nothrow_parsable_v<
        get_context_plain_t<contexts::skipper, Context>,
        It, Se,
        std::remove_cvref_t<decltype(x4::remove_first_context<contexts::skipper>(ctx))>,
        unused_type
    >)
{
    auto const& skipper = x4::get<contexts::skipper>(ctx);

    // A custom skipper may require the context object as-is. However, we should omit the
    // `contexts::skipper` because not doing so would invoke infinite instantiation when
    // the skipper itself has a recursive skipper.
    auto const local_ctx = x4::remove_first_context<contexts::skipper>(ctx);

    while (skipper.parse(first, last, local_ctx, unused))
        /* loop */;
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
