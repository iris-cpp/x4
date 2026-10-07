/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/char/char.hpp>
#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/operator/sequence.hpp>

#include <iris/alloy/tuple.hpp>

#include <string>
#include <string_view>

TEST_CASE("lit")
{
    // standard
    {
        (void)x4::lit('f');
        (void)x4::lit("f");
        (void)x4::lit("foo");

        (void)x4::standard::char_('f');
        (void)x4::standard::char_("f");
        (void)x4::standard::char_("foo");

        (void)x4::string('f');
        (void)x4::string("f");
        (void)x4::string("foo");
    }

    // unicode
    {
        (void)x4::lit(U'f');
        (void)x4::lit(U"f");
        (void)x4::lit(U"foo");

        (void)x4::unicode::char_(U'f');
        (void)x4::unicode::char_(U"f");
        (void)x4::unicode::char_(U"foo");

        (void)x4::string(U'f');
        (void)x4::string(U"f");
        (void)x4::string(U"foo");
    }

    {
        std::string attr;
        constexpr auto p = x4::standard::char_ >> x4::lit("\n"); // TODO: MSVC 2022 bug, [[no_unique_address]] on binary_parser "overruns"
        REQUIRE(parse("A\n", p, attr));
        CHECK(attr == "A");
    }

    // -------------------------------------------------

    {
        CHECK(parse("kimpo", x4::lit("kimpo")));

        std::basic_string<char> s("kimpo");
        CHECK(parse("kimpo", x4::lit(s)));
    }

    {
        std::basic_string<char> s("kimpo");
        CHECK(parse("kimpo", x4::lit(s)));
    }

    // -------------------------------------------------

    {
        CHECK(parse("kimpo", "kimpo"));
        CHECK(parse("kimpo", x4::string("kimpo")));

        CHECK(parse("x", x4::string("x")));

        std::basic_string<char> s("kimpo");
        CHECK(parse("kimpo", s));
        CHECK(parse("kimpo", x4::string(s)));
    }

    {
        std::basic_string<char> s("kimpo");
        CHECK(parse("kimpo", x4::string(s)));
    }

    {
        // single-element tuple-like tests
        alloy::tuple<std::string> s;
        REQUIRE(parse("kimpo", x4::string("kimpo"), s));
        CHECK(alloy::get<0>(s) == "kimpo");
    }

    {
        constexpr std::string_view input = "ab";
        std::string s = "z";

        auto first = input.begin();
        REQUIRE(x4::lit('a').parse(first, input.end(), x4::unused, s));
        REQUIRE(x4::lit("b").parse(first, input.end(), x4::unused, s));
        CHECK(first == input.end());
        CHECK(s == "z");
    }
}
