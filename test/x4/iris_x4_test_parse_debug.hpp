#ifndef IRIS_ZZ_X4_TEST_IRIS_X4_TEST_PARSE_DEBUG_HPP
#define IRIS_ZZ_X4_TEST_IRIS_X4_TEST_PARSE_DEBUG_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/parse_debug.hpp>

#include <iterator>
#include <ranges>
#include <utility>

namespace x4_test {

namespace detail {

// Provide `x4::unused` default arg fallback
struct parse_debug_overloads : x4::detail::parse_debug_fn_main
{
    using x4::detail::parse_debug_fn_main::operator();

    // It/Se + Parser
    template<std::forward_iterator It, std::sentinel_for<It> Se, x4::X4Parser<It, Se> Parser>
    static constexpr x4::parse_result<It, Se>
    operator()(It first, Se last, Parser&& p)
    {
        return x4::parse_debug(first, last, std::forward<Parser>(p), x4::unused);
    }

    // parse_result + It/Se + Parser
    template<std::forward_iterator It, std::sentinel_for<It> Se, x4::X4Parser<It, Se> Parser>
    static constexpr void
    operator()(x4::parse_result<It, Se>& res, It first, Se last, Parser&& p)
    {
        return x4::parse_debug(res, first, last, std::forward<Parser>(p), x4::unused);
    }

    // R + Parser
    template<std::ranges::forward_range R, x4::detail::X4RangeParseParser<R> Parser>
    static constexpr x4::parse_result_for<R>
    operator()(R const& r, Parser&& p)
    {
        return x4::parse_debug(r, std::forward<Parser>(p), x4::unused);
    }

    // parse_result + R + Parser
    template<std::ranges::forward_range R, x4::detail::X4RangeParseParser<R> Parser>
    static constexpr void
    operator()(x4::parse_result_for<R>& res, R const& r, Parser&& p)
    {
        return x4::parse_debug(res, r, std::forward<Parser>(p), x4::unused);
    }

    // It/Se + Parser + Skipper
    template<std::forward_iterator It, std::sentinel_for<It> Se, x4::X4Parser<It, Se> Parser, x4::X4ExplicitParser<It, Se> Skipper>
    static constexpr x4::parse_result<It, Se>
    operator()(It first, Se last, Parser&& p, Skipper&& s, x4::root_skipper_flag flag = x4::root_skipper_flag::do_post_skip)
    {
        return x4::parse_debug(first, last, std::forward<Parser>(p), std::forward<Skipper>(s), x4::unused, flag);
    }

    // parse_result + It/Se + Parser + Skipper
    template<std::forward_iterator It, std::sentinel_for<It> Se, x4::X4Parser<It, Se> Parser, x4::X4ExplicitParser<It, Se> Skipper>
    static constexpr void
    operator()(x4::parse_result<It, Se>& res, It first, Se last, Parser&& p, Skipper&& s, x4::root_skipper_flag flag = x4::root_skipper_flag::do_post_skip)
    {
        return x4::parse_debug(res, first, last, std::forward<Parser>(p), std::forward<Skipper>(s), x4::unused, flag);
    }

    // R + Parser + Skipper
    template<
        std::ranges::forward_range R,
        x4::detail::X4RangeParseParser<R> Parser,
        x4::detail::X4RangeParseSkipper<R> Skipper
    >
    static constexpr x4::parse_result_for<R>
    operator()(R const& r, Parser&& p, Skipper&& s, x4::root_skipper_flag flag = x4::root_skipper_flag::do_post_skip)
    {
        return x4::parse_debug(r, std::forward<Parser>(p), std::forward<Skipper>(s), x4::unused, flag);
    }

    // parse_result + R + Parser + Skipper
    template<
        std::ranges::forward_range R,
        x4::detail::X4RangeParseParser<R> Parser,
        x4::detail::X4RangeParseSkipper<R> Skipper
    >
    static constexpr void
    operator()(x4::parse_result_for<R>& res, R const& r, Parser&& p, Skipper&& s, x4::root_skipper_flag flag = x4::root_skipper_flag::do_post_skip)
    {
        return x4::parse_debug(res, r, std::forward<Parser>(p), std::forward<Skipper>(s), x4::unused, flag);
    }
}; // parse_overload

} // detail

inline namespace cpos {

[[maybe_unused]] inline constexpr detail::parse_debug_overloads parse_debug{};

} // cpos

} // x4_test

using x4_test::parse_debug;

#endif
