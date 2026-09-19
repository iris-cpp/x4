/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/directive/no_case.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/char_string_literal.hpp>

TEST_CASE("no_case")
{
    using x4::no_case;
    using x4::lit;
    using x4::char_;

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(no_case['x']);

    {
        CHECK(parse("x", no_case[char_]));
        CHECK(parse("X", no_case[char_('x')]));
        CHECK(parse("X", no_case[char_('X')]));
        CHECK(parse("x", no_case[char_('X')]));
        CHECK(parse("x", no_case[char_('x')]));
        CHECK(!parse("z", no_case[char_('X')]));
        CHECK(!parse("z", no_case[char_('x')]));
        CHECK(parse("x", no_case[char_('a', 'z')]));
        CHECK(parse("X", no_case[char_('a', 'z')]));
        CHECK(!parse("a", no_case[char_('b', 'z')]));
        CHECK(!parse("z", no_case[char_('a', 'y')]));
    }
    {
        CHECK(parse("X", no_case['x']));
        CHECK(parse("X", no_case['X']));
        CHECK(parse("x", no_case['X']));
        CHECK(parse("x", no_case['x']));
        CHECK(!parse("z", no_case['X']));
        CHECK(!parse("z", no_case['x']));
    }

    {
        CHECK(parse("X", no_case[char_("a-z")]));
        CHECK(!parse("1", no_case[char_("a-z")]));
    }

    {
        CHECK(parse("Bochi Bochi", no_case[lit("bochi bochi")]));
        CHECK(parse("BOCHI BOCHI", no_case[lit("bochi bochi")]));
        CHECK(!parse("Vavoo", no_case[lit("bochi bochi")]));
    }

    {
        CHECK(parse("x", no_case[no_case[char_]]));
        CHECK(parse("x", no_case[no_case[char_('x')]]));
        CHECK(parse("yabadabadoo", no_case[no_case[lit("Yabadabadoo")]]));
    }

    {
        using namespace x4::standard;
        CHECK(parse("X", no_case[alnum]));
        CHECK(parse("6", no_case[alnum]));
        CHECK(!parse(":", no_case[alnum]));

        CHECK(parse("X", no_case[lower]));
        CHECK(parse("x", no_case[lower]));
        CHECK(parse("X", no_case[upper]));
        CHECK(parse("x", no_case[upper]));
        CHECK(!parse(":", no_case[lower]));
        CHECK(!parse(":", no_case[upper]));
    }

    {
        // chsets

        CHECK(parse("x", no_case[x4::standard::char_("a-z")]));
        CHECK(parse("X", no_case[x4::standard::char_("a-z")]));
        CHECK(parse(L"X", no_case[x4::standard_wide::char_(L"a-z")]));
        CHECK(parse(L"X", no_case[x4::standard_wide::char_(L"X")]));
    }

    {
        std::string s("bochi bochi");
        CHECK(parse("Bochi Bochi", no_case[lit(s)]));
        CHECK(parse("Bochi Bochi", no_case[s]));
    }

    {
        using namespace x4::standard;
        CHECK(!parse("ą", no_case['a']));
    }
}
