/*=============================================================================
    Copyright (c) 2019 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "rule_separate_tu_grammar.hpp"

#include <concepts>
#include <utility>
#include <type_traits>

namespace {

namespace sem_act {

constexpr auto nop = [](auto&&) {};

IRIS_X4_DECLARE(used_attr1, int);
constexpr auto used_attr1_def = used_attr::grammar.on_match(nop);
IRIS_X4_DEFINE(used_attr1);

IRIS_X4_DECLARE(used_attr2, int);
constexpr auto used_attr2_def = unused_attr::grammar.on_match(nop);
IRIS_X4_DEFINE(used_attr2);

IRIS_X4_DECLARE(unused_attr1, unused_type);
constexpr auto unused_attr1_def = used_attr::grammar.on_match(nop);
IRIS_X4_DEFINE(unused_attr1);

IRIS_X4_DECLARE(unused_attr2, unused_type);
constexpr auto unused_attr2_def = unused_attr::grammar.on_match(nop);
IRIS_X4_DEFINE(unused_attr2);

} // sem_act

} // anonymous

TEST_CASE("rule_separate_tu")
{
    {
        CHECK(parse("*", unused_attr::skipper));
        CHECK(parse("==", unused_attr::grammar));
        CHECK(parse("*=*=", unused_attr::grammar, unused_attr::skipper));
    }

    {
        long l = 0;
        REQUIRE(parse("123", used_attr::grammar, l));
        CHECK(l == 123);
        REQUIRE(parse(" 42", used_attr::grammar, used_attr::skipper, l));
        CHECK(l == 42);
    }

    {
        long i = 0;
        CHECK(parse("123", sem_act::used_attr1, i));
    }
    {
        using Context = x4::context<
            x4::contexts::expectation_failure,
            x4::expectation_failure<std::string_view::const_iterator>,
            x4::context<
                x4::contexts::rule_var,
                int
            >
        >;
        using RuleAgnosticContext = x4::context<
            x4::contexts::expectation_failure,
            x4::expectation_failure<std::string_view::const_iterator>
        >;

        static_assert(std::same_as<
            std::remove_cvref_t<decltype(
                x4::remove_first_context<x4::contexts::rule_var>(std::declval<Context const&>())
            )>,
            RuleAgnosticContext
        >);

        long i = 0;
        CHECK(parse("===", sem_act::used_attr2, i));
    }
    {
        CHECK(parse("123", sem_act::unused_attr1));
    }
    {
        CHECK(parse("===", sem_act::unused_attr2));
    }
}
