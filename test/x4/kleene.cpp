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
#include <iris/x4/directive/lexeme.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/optional.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/sequence.hpp>

#include <iris/alloy/tuple.hpp>
#include <iris/rvariant.hpp>

#include <optional>
#include <string>
#include <string_view>
#include <vector>

TEST_CASE("kleene")
{
    using x4::char_;
    using x4::standard::alpha;
    using x4::standard::upper;
    using x4::standard::space;
    using x4::standard::digit;
    using x4::int_;
    using x4::lexeme;

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(*char_);

    CHECK(parse("aaaaaaaa", *char_));
    CHECK(parse("a", *char_));
    CHECK(parse("", *char_));
    CHECK(parse("aaaaaaaa", *alpha));
    CHECK(!parse("aaaaaaaa", *upper));

    CHECK(parse(" a a aaa aa", *char_, space));
    CHECK(parse("12345 678 9", *digit, space));

    {
        std::string s;
        REQUIRE(parse("bbbb", *char_, s));
        CHECK(s == "bbbb");
    }
    {
        std::string s;
        REQUIRE(parse("b b b b ", *char_, space, s));
        CHECK(s == "bbbb");
    }

    {
        std::vector<int> v;
        REQUIRE(parse("123 456 789 10", *int_, space, v));
        REQUIRE(v.size() == 4);
        CHECK(v[0] == 123);
        CHECK(v[1] == 456);
        CHECK(v[2] == 789);
        CHECK(v[3] == 10);
    }

    {
        std::vector<std::string> v;
        REQUIRE(parse("a b c d", *lexeme[+alpha], space, v));
        REQUIRE(v.size() == 4);
        CHECK(v[0] == "a");
        CHECK(v[1] == "b");
        CHECK(v[2] == "c");
        CHECK(v[3] == "d");
    }

    {
        std::vector<int> v;
        REQUIRE(parse("123 456 789", *int_, space, v));
        REQUIRE(v.size() == 3);
        CHECK(v[0] == 123);
        CHECK(v[1] == 456);
        CHECK(v[2] == 789);
    }

    {
        // actions
        using x4::_attr;

        std::string v;
        auto f = [&](auto&& ctx){ v = _attr(ctx); };

        REQUIRE(parse("bbbb", (*char_).on_match(f)));
        REQUIRE(v.size() == 4);
        CHECK(v[0] == 'b');
        CHECK(v[1] == 'b');
        CHECK(v[2] == 'b');
        CHECK(v[3] == 'b');
    }

    {
        // more actions
        using x4::_attr;

        std::vector<int> v;
        auto f = [&](auto&& ctx){ v = _attr(ctx); };

        REQUIRE(parse("123 456 789", (*int_).on_match(f), space));
        CHECK(v.size() == 3);
        CHECK(v[0] == 123);
        CHECK(v[1] == 456);
        CHECK(v[2] == 789);
    }

    {
        x4_test::custom_container<char> x;
        (void)parse("abcde", *char_, x);
    }

    {
        std::vector<x4_test::move_only> v;
        REQUIRE(parse("sss", *x4_test::synth_move_only, v));
        CHECK(v.size() == 3);
    }

    // the whole value is one element when the element type takes it, else each parse is written as a part
    {
        std::vector<std::string> v;
        REQUIRE(parse("abc1", *~char_(','), v));
        CHECK(v == std::vector<std::string>{"abc1"});
    }
    {
        std::vector<std::string> v;
        REQUIRE(parse("a1b2", *(alpha >> digit), v));
        CHECK(v == std::vector<std::string>{"a1b2"});
    }
    {
        std::vector<iris::alloy::tuple<int, char>> v;
        REQUIRE(parse("1a2b", *(int_ >> alpha), v));
        CHECK(v == std::vector<iris::alloy::tuple<int, char>>{{1, 'a'}, {2, 'b'}});
    }
    {
        std::vector<int> v;
        REQUIRE(parse("1,2", *(int_ >> ',' >> int_), v));
        CHECK(v == std::vector<int>{1, 2});
    }
    {
        constexpr auto items = *~char_(',') >> *(',' >> *~char_(','));
        std::vector<std::string> v;
        REQUIRE(parse("abc1,abc2", items, v));
        CHECK(v == std::vector<std::string>{"abc1", "abc2"});

        std::string s;
        REQUIRE(parse("abc1,abc2", items, s));
        CHECK(s == "abc1abc2");
    }
    {
        std::vector<std::string> v;
        REQUIRE(parse("ab", +~char_(','), v));
        CHECK(v == std::vector<std::string>{"ab"});
    }
    {
        // An optional from one parse is appended as is if the element type accepts it, else only its content
        std::vector<char> v;
        REQUIRE(parse("a,,b,", *(-alpha >> ','), v));
        CHECK(v == std::vector<char>{'a', 'b'});

        std::vector<std::optional<char>> o;
        REQUIRE(parse("a,,b,", *(-alpha >> ','), o));
        CHECK(o == std::vector<std::optional<char>>{'a', std::nullopt, 'b'});
    }
    {
        // The container held by a variant is appended to, not replaced
        constexpr std::string_view input = "cd";
        iris::rvariant<int, std::string> v = std::string("ab");

        auto first = input.begin();
        REQUIRE((*char_).parse(first, input.end(), x4::unused, v));
        CHECK(first == input.end());
        CHECK(v == iris::rvariant<int, std::string>{std::string("abcd")});
    }
    {
        // The container in an optional is engaged and appended to, as a value is written into an optional
        iris::alloy::tuple<char, std::optional<std::string>> t;
        REQUIRE(parse("x:ab", char_ >> ':' >> *alpha, t));
        CHECK(t == iris::alloy::tuple<char, std::optional<std::string>>{'x', std::string("ab")});

        iris::rvariant<int, std::optional<std::string>> v;
        REQUIRE(parse("ab", *alpha, v));
        CHECK(v == iris::rvariant<int, std::optional<std::string>>{std::optional<std::string>("ab")});

        constexpr std::string_view input = "cd";
        std::optional<std::string> o = std::string("ab");
        auto first = input.begin();
        REQUIRE((*alpha).parse(first, input.end(), x4::unused, o));
        CHECK(o == std::string("abcd"));
    }
}
