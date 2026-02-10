/*=============================================================================
    Copyright (c) 2013 Carl Barron
    Copyright (c) 2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define IRIS_X4_DEBUG
#define IRIS_X4_UNICODE

#include "iris_x4_test.hpp"

#include <iris/x4/symbols.hpp>
#include <iris/x4/char_encoding/unicode.hpp>
#include <iris/x4/operator/optional.hpp>
#include <iris/x4/operator/sequence.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/tuple.hpp>

#include <optional>
#include <numeric>

struct roman
{
    std::optional<int> a;
    std::optional<int> b;
    std::optional<int> c;
};

template<>
struct alloy::adaptor<roman> {
    using getters_list = make_getters_list<&roman::a, &roman::b, &roman::c>;
};

namespace {

int eval(roman const & c)
{
    return c.a.value_or(0) + c.b.value_or(0) + c.c.value_or(0);
}

} // anonymous

TEST_CASE("symbols3")
{
    using x4::shared_symbols;

    {
        // construction from initializer-list
        shared_symbols<int> const ones = {
            {"I", 1}, {"II", 2}, {"III", 3}, {"IV", 4},
            {"V", 5}, {"VI", 6}, {"VII", 7}, {"VIII", 8},
            {"IX", 9}
        };
        shared_symbols<int> const tens = {
            {"X", 10}, {"XX", 20}, {"XXX", 30}, {"XL", 40},
            {"L", 50}, {"LX", 60}, {"LXX", 70}, {"LXXX", 80},
            {"XC", 90}
        };
        shared_symbols<int> const hundreds {
            {"C", 100}, {"CC", 200}, {"CCC", 300}, {"CD", 400},
            {"D", 500}, {"DC", 600}, {"DCC", 700}, {"DCCC", 800},
            {"CM", 900}
        };

        auto number = -hundreds >> -tens >> -ones;

        roman r;
        REQUIRE(parse("CDXLII", number, r));
        CHECK(eval(r) == 442);
    }

    {
        // construction from initializer-list without attribute
        shared_symbols<> foo = {"a1", "a2", "a3"};
        CHECK(parse("a3", foo));
    }

    {
        // assignment from initializer-list
        shared_symbols<> foo;
        foo = {"a1", "a2", "a3"};
        CHECK(parse("a3", foo));
    }

    {
        // unicode | construction from initializer-list
        x4::shared_symbols_parser<x4::char_encoding::unicode, int> foo = {{U"a1", 1}, {U"a2", 2}, {U"a3", 3}};

        int r = 0;
        REQUIRE(parse(U"a3", foo, r));
        CHECK(r == 3);
    }
}
