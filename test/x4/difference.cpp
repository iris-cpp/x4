/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/string/string.hpp>
#include <iris/x4/operator/difference.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/directive/skip.hpp>

#include <string>

TEST_CASE("difference")
{
    using x4::standard::char_;
    using x4::standard::blank;
    using x4::standard::space;
    using x4::lit;
    using x4::skip;
    using x4::_attr;

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(char_ - 'a');

    // Basic tests
    {
        CHECK(parse("b", char_ - 'a'));
        CHECK(!parse("a", char_ - 'a'));
        CHECK(parse("/* abcdefghijk */", "/*" >> *(char_ - "*/") >> "*/"));
        CHECK(!parse("switch", lit("switch") - "switch"));
    }

    // Test attributes
    {
        char attr{};
        REQUIRE(parse("xg", (char_ - 'g') >> 'g', attr));
        CHECK(attr == 'x');
    }

    // Test handling of container attributes
    {
        std::string attr;
        REQUIRE(parse("abcdefg", *(char_ - 'g') >> 'g', attr));
        CHECK(attr == "abcdef");
    }

    {
        std::string s;
        REQUIRE(parse(
            "/*abcdefghijk*/",
            "/*" >> *(char_ - "*/")[([&](auto&& ctx){ s += _attr(ctx); })] >> "*/"
        ));
        CHECK(s == "abcdefghijk");
    }
    {
        std::string s;
        REQUIRE(parse(
            "    /*abcdefghijk*/",
            "/*" >> *(char_ - "*/")[([&](auto&& ctx){ s += _attr(ctx); })] >> "*/",
            space
        ));
        CHECK(s == "abcdefghijk");
    }

    {
        // Rollback on failed parse
        std::string s;
        REQUIRE(parse(
            "foo die ,",
            +(
                // RHS fails, so the difference parser should rollback iterator
                // Note that RHS has a skipper, so the buffer skipped by `skip_over`
                // should also be rolled back
                char_ - skip(blank)[',']
            ) >> skip(blank)[','],
            s
        ));
        CHECK(s == "foo die"); // wrong implementation yields "foodie"
    }
}
