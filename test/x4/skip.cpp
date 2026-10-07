/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/char/unicode_char_class.hpp>
#include <iris/x4/directive/skip.hpp>
#include <iris/x4/operator/kleene.hpp>

#include <concepts>
#include <type_traits>
#include <string>

TEST_CASE("skip")
{
    using x4::standard::space;
    using x4::standard::alpha;
    using x4::char_;
    using x4::skip;
    using x4::lit;

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(skip('x')['y']);

    CHECK(parse("a b c d", skip(space)[*char_]));

    {
        std::string s;
        REQUIRE(parse("a b c d", skip(space)[*char_], s));
        CHECK(s == "abcd");
    }

    using AnyCharParser = std::remove_cvref_t<decltype(char_)>;
    STATIC_CHECK(std::same_as<
        decltype(skip(x4::standard::space)[char_]),
        x4::builtin_skip_directive<x4::builtin_skipper_kind::space, AnyCharParser>
    >);
    STATIC_CHECK(std::same_as<
        decltype(skip(x4::unicode::space)[char_]),
        x4::builtin_skip_directive<x4::builtin_skipper_kind::space, AnyCharParser>
    >);
    STATIC_CHECK(std::same_as<
        decltype(skip(x4::standard::blank)[char_]),
        x4::builtin_skip_directive<x4::builtin_skipper_kind::blank, AnyCharParser>
    >);
    STATIC_CHECK(std::same_as<
        decltype(skip(x4::unicode::blank)[char_]),
        x4::builtin_skip_directive<x4::builtin_skipper_kind::blank, AnyCharParser>
    >);

    STATIC_CHECK(std::same_as<decltype(x4::to_builtin(x4::standard::space)), x4::builtin_skipper_kind>);
    STATIC_CHECK(std::same_as<decltype(x4::to_builtin(x4::unicode::space)), x4::builtin_skipper_kind>);
    STATIC_CHECK(std::same_as<decltype(x4::to_builtin(x4::standard::blank)), x4::builtin_skipper_kind>);
    STATIC_CHECK(std::same_as<decltype(x4::to_builtin(x4::unicode::blank)), x4::builtin_skipper_kind>);
}
