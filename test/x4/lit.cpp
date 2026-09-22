/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define IRIS_X4_UNICODE

#include "iris_x4_test.hpp"

#include <iris/x4/char/char.hpp>
#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/operator/sequence.hpp>

#include <iris/alloy/adapted/std_tuple.hpp>
#include <iris/alloy/tuple.hpp>

#include <string>
#include <tuple>

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

    // standard_wide
    {
        (void)x4::lit(L'f');
        (void)x4::lit(L"f");
        (void)x4::lit(L"foo");

        (void)x4::standard_wide::char_(L'f');
        (void)x4::standard_wide::char_(L"f");
        (void)x4::standard_wide::char_(L"foo");

        (void)x4::string(L'f');
        (void)x4::string(L"f");
        (void)x4::string(L"foo");
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

    {
        std::wstring attr;
        constexpr auto p = x4::standard_wide::char_ >> x4::lit(L"\n");
        REQUIRE(parse(L"É\n", p, attr));
        CHECK(attr == L"É");
    }

    // -------------------------------------------------

    {
        CHECK(parse("kimpo", x4::lit("kimpo")));

        std::basic_string<char> s("kimpo");
        CHECK(parse("kimpo", x4::lit(s)));

        std::basic_string<wchar_t> ws(L"kimpo");
        CHECK(parse(L"kimpo", x4::lit(ws)));
    }

    {
        std::basic_string<char> s("kimpo");
        std::basic_string<wchar_t> ws(L"kimpo");
        CHECK(parse("kimpo", x4::lit(s)));
        CHECK(parse(L"kimpo", x4::lit(ws)));
    }

    // -------------------------------------------------

    {
        CHECK(parse("kimpo", "kimpo"));
        CHECK(parse("kimpo", x4::string("kimpo")));

        CHECK(parse("x", x4::string("x")));
        CHECK(parse(L"x", x4::string(L"x")));

        std::basic_string<char> s("kimpo");
        std::basic_string<wchar_t> ws(L"kimpo");
        CHECK(parse("kimpo", s));
        CHECK(parse(L"kimpo", ws));
        CHECK(parse("kimpo", x4::string(s)));
        CHECK(parse(L"kimpo", x4::string(ws)));
    }

    {
        CHECK(parse(L"kimpo", L"kimpo"));
        CHECK(parse(L"kimpo", x4::string(L"kimpo")));
        CHECK(parse(L"x", x4::string(L"x")));
    }

    {
        std::basic_string<char> s("kimpo");
        CHECK(parse("kimpo", x4::string(s)));

        std::basic_string<wchar_t> ws(L"kimpo");
        CHECK(parse(L"kimpo", x4::string(ws)));
    }

    {
        // single-element tuple tests
        std::tuple<std::string> s;
        REQUIRE(parse("kimpo", x4::string("kimpo"), s));
        CHECK(alloy::get<0>(s) == "kimpo");
    }
}
