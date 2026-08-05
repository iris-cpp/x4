#ifndef IRIS_ZZ_X4_CORE_EXPECTATION_HPP
#define IRIS_ZZ_X4_CORE_EXPECTATION_HPP

/*=============================================================================
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp>

#include <iris/x4/core/expectation_failure.hpp> // export
#include <iris/x4/core/parser.hpp> // for `x4::what`
#include <iris/x4/core/context.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace contexts {

struct expectation_failure
{
    static constexpr bool is_unique = true;
};

} // contexts

template<class Context>
using expectation_failure_t = get_context_plain_t<contexts::expectation_failure, Context>;

template<class Context>
[[nodiscard]]
constexpr bool has_expectation_failure(Context const& ctx) noexcept
{
    static_assert(
        has_context_v<Context, contexts::expectation_failure>,
        "Context type was not specified for `x4::contexts::expectation_failure`. "
        "You probably forgot: `x4::with<x4::contexts::expectation_failure>(failure)[p]`. "
        "Note that you must also bind the context to your skipper."
    );
    return x4::get<contexts::expectation_failure>(ctx).has_value();
}

//
// Creation of a brand-new expectation_failure instance.
// This is the primary overload.
//
template<std::forward_iterator It, class Subject, class Context>
constexpr void set_expectation_failure(
    It where,
    Subject const& subject,
    Context const& ctx
)
    noexcept(noexcept(x4::get<contexts::expectation_failure>(ctx).emplace(std::move(where), x4::what(subject))))
{
    static_assert(
        has_context_v<Context, contexts::expectation_failure>,
        "Context type was not specified for `x4::contexts::expectation_failure`. "
        "You probably forgot: `x4::with<x4::contexts::expectation_failure>(failure)[p]`. "
        "Note that you must also bind the context to your skipper."
    );
    x4::get<contexts::expectation_failure>(ctx).emplace(std::move(where), x4::what(subject));
}

template<class Context>
[[nodiscard]]
constexpr decltype(auto) get_expectation_failure(Context const& ctx) noexcept
{
    static_assert(
        has_context_v<Context, contexts::expectation_failure>,
        "Context type was not specified for `x4::contexts::expectation_failure`. "
        "You probably forgot: `x4::with<x4::contexts::expectation_failure>(failure)[p]`. "
        "Note that you must also bind the context to your skipper."
    );

    return x4::get<contexts::expectation_failure>(ctx);
}

template<class Context>
constexpr void clear_expectation_failure(Context const& ctx) noexcept
{
    static_assert(
        has_context_v<Context, contexts::expectation_failure>,
        "Context type was not specified for `x4::contexts::expectation_failure`. "
        "You probably forgot: `x4::with<x4::contexts::expectation_failure>(failure)[p]`. "
        "Note that you must also bind the context to your skipper."
    );
    x4::get<contexts::expectation_failure>(ctx).clear();
}

} // iris::x4

#endif
