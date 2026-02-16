/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/directive/without.hpp>
#include <iris/x4/directive/with.hpp>
#include <iris/x4/core/parser.hpp>

#include <concepts>
#include <iterator>

template<class ExpectedContext>
struct context_checker : x4::parser<context_checker<ExpectedContext>>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, x4::X4Attribute Attr>
    [[nodiscard]] static constexpr bool parse(It&, Se const&, Context const&, Attr const&)
    {
        STATIC_CHECK(std::same_as<Context, ExpectedContext>);
        return true;
    }
};

static_assert(x4::X4ExplicitParser<context_checker<unused_type>, char const*, char const*>);

struct OK {};
struct Bad0 {};
struct NonExistent {};

TEST_CASE("without")
{
    using x4::without;
    using x4::with;
    using x4::context;

    constexpr int dummy = 0;
    char const* first = nullptr, *const last = nullptr;

    {
        (void)x4::with<OK>(dummy)[
            x4::without<OK>[
                context_checker<unused_type>{}
            ]
        ].parse(first, last, unused, unused);
    }
    {
        (void)x4::with<OK>(dummy)[
            x4::without<NonExistent>[
                context_checker<context<OK, int const>>{}
            ]
        ].parse(first, last, unused, unused);
    }

    {
        (void)x4::with<OK>(dummy)[
            x4::with<Bad0>(dummy)[
                x4::without<Bad0>[
                    context_checker<context<OK, int const>>{}
                ]
            ]
        ].parse(first, last, unused, unused);
    }
    {
        (void)x4::with<OK>(dummy)[
            x4::with<Bad0>(dummy)[
                x4::without<OK>[
                    context_checker<context<Bad0, int const>>{}
                ]
            ]
        ].parse(first, last, unused, unused);
    }
    {
        (void)x4::with<OK>(dummy)[
            x4::with<Bad0>(dummy)[
                x4::without<NonExistent>[
                    context_checker<context<Bad0, int const, context<OK, int const> const&>>{}
                ]
            ]
        ].parse(first, last, unused, unused);
    }
    {
        (void)x4::with<OK>(dummy)[
            x4::with<Bad0>(dummy)[
                x4::without<OK, Bad0>[
                    context_checker<unused_type>{}
                ]
            ]
        ].parse(first, last, unused, unused);
    }
}
