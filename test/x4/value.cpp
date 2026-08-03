/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/attribute/value.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/char/char.hpp>

#include <iris/alloy/adapted/std_pair.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <type_traits>
#include <concepts>

TEST_CASE("attr")
{
    using namespace std::string_literals;
    using namespace std::string_view_literals;

    using x4::fixed_value;
    using x4::int_;

    {
        [[maybe_unused]] constexpr auto attr_p = fixed_value(1);
        STATIC_CHECK(std::same_as<std::remove_const_t<decltype(attr_p)>, x4::fixed_value_parser<int>>);
    }
    {
        [[maybe_unused]] constexpr auto attr_p = fixed_value(3.14);
        STATIC_CHECK(std::same_as<std::remove_const_t<decltype(attr_p)>, x4::fixed_value_parser<double>>);
    }

    {
        constexpr auto attr_p = fixed_value("foo");
        STATIC_REQUIRE(std::same_as<std::remove_const_t<decltype(attr_p)>, x4::fixed_value_parser<std::basic_string<char>, std::basic_string_view<char>>>);

        // Make sure `fixed_value(std::string_view)` is parsable into std::string
        {
            constexpr auto result = [&](std::string_view expected_str) consteval {
                std::string str;
                std::string_view const input;
                auto it = input.begin();
                auto const se = input.end();
                bool const ok = attr_p.parse(it, se, unused, str);
                return std::make_pair(ok, str == expected_str);
            }("foo");
            STATIC_REQUIRE(result.first == true);
            STATIC_CHECK(result.second == true);
        }
        {
            std::string str;
            std::string_view const input;
            auto it = input.begin();
            auto const se = input.end();
            REQUIRE(attr_p.parse(it, se, unused, str));
            CHECK(str == "foo");
        }
    }
    {
        [[maybe_unused]] /*constexpr*/ auto attr_p = fixed_value("foo"s);
        STATIC_CHECK(std::same_as<std::remove_const_t<decltype(attr_p)>, x4::fixed_value_parser<std::basic_string<char>>>);
    }
    {
        [[maybe_unused]] constexpr auto attr_p = fixed_value("foo"sv);
        STATIC_CHECK(std::same_as<std::remove_const_t<decltype(attr_p)>, x4::fixed_value_parser<std::basic_string_view<char>>>);
    }

    {
        [[maybe_unused]] constexpr auto attr_p = fixed_value(U"foo");
        STATIC_CHECK(std::same_as<std::remove_const_t<decltype(attr_p)>, x4::fixed_value_parser<std::basic_string<char32_t>, std::basic_string_view<char32_t>>>);
    }
    {
        [[maybe_unused]] /*constexpr*/ auto attr_p = fixed_value(U"foo"s);
        STATIC_CHECK(std::same_as<std::remove_const_t<decltype(attr_p)>, x4::fixed_value_parser<std::basic_string<char32_t>>>);
    }
    {
        [[maybe_unused]] constexpr auto attr_p = fixed_value(U"foo"sv);
        STATIC_CHECK(std::same_as<std::remove_const_t<decltype(attr_p)>, x4::fixed_value_parser<std::basic_string_view<char32_t>>>);
    }

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(fixed_value(1));
    IRIS_X4_ASSERT_CONSTEXPR_CTORS(fixed_value("asd"));

    {
        constexpr char s[] = "asd";
        IRIS_X4_ASSERT_CONSTEXPR_CTORS(fixed_value(s));
    }

    {
        int d = 0;
        REQUIRE(parse("", fixed_value(1), d));
        CHECK(d == 1);
    }
    {
        int d = 0;
        int d1 = 1;
        REQUIRE(parse("", fixed_value(d1), d));
        CHECK(d == 1);
    }
    {
        std::pair<int, int> p;
        REQUIRE(parse("1", int_ >> fixed_value(2), p));
        CHECK(p.first == 1);
        CHECK(p.second == 2);
    }
    {
        char c = '\0';
        REQUIRE(parse("", fixed_value('a'), c));
        CHECK(c == 'a');
    }
    {
        std::string str;
        REQUIRE(parse("", fixed_value("test"), str));
        CHECK(str == "test");
    }
    {
        std::string str;
        REQUIRE(parse("", fixed_value(std::string("test")), str));
        CHECK(str == "test");
    }
    {
        std::vector<int> array = {0, 1, 2};
        std::vector<int> vec;
        REQUIRE(parse("", fixed_value(array), vec));
        REQUIRE(vec.size() == 3);
        CHECK(vec[0] == 0);
        CHECK(vec[1] == 1);
        CHECK(vec[2] == 2);
    }

    {
        std::string s;
        REQUIRE(parse("s", "s" >> fixed_value(std::string("123")), s));
        CHECK(s == "123");
    }

    // container of container

    // vector<vector<int>>
    {
        std::vector<std::vector<int>> vecs;
        std::vector<int> vec{1, 2, 3};
        x4::move_to(std::move(vec), vecs);
        CHECK(vecs == std::vector<std::vector<int>>{std::vector{1, 2, 3}});
    }
    {
        std::vector<std::vector<int>> vecs;
        REQUIRE(parse("", fixed_value(std::vector<int>{1, 2, 3}) >> fixed_value(std::vector<int>{4, 5, 6}), vecs));
        CHECK(vecs == std::vector{std::vector{1, 2, 3}, std::vector{4, 5, 6}});
    }

    // vector<string>
    {
        std::vector<std::string> strs;
        std::string str = "abc";
        x4::move_to(std::move(str), strs);
        CHECK(strs == std::vector{std::string("abc")});
    }
    {
        std::vector<std::string> strs;
        REQUIRE(parse("", fixed_value(std::string("123")) >> fixed_value(std::string("456")), strs));
        CHECK(strs == std::vector<std::string>{"123", "456"});
    }

    {
        std::string s;
        REQUIRE(parse("", fixed_value(std::string("123")) >> fixed_value(std::string("456")), s));
        CHECK(s == "123456");
    }

    {
        std::vector<int> ints;
        REQUIRE(parse("", fixed_value(std::vector<int>{1, 2, 3}) >> fixed_value(std::vector<int>{4, 5, 6}), ints));
        CHECK(ints == std::vector<int>{1, 2, 3, 4, 5, 6});
    }

    {
        std::vector<int> ints;
        REQUIRE(parse("",
            (fixed_value(std::vector<int>{1, 2, 3}) >> fixed_value(std::vector<int>{4, 5, 6})) >>
            (fixed_value(std::vector<int>{7, 8, 9}) >> fixed_value(std::vector<int>{0, 1, 2})),
            ints
        ));
        CHECK(ints == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 1, 2});
    }
}

TEST_CASE("reset_value")
{
    using x4::reset_value;

    {
        int val = 42;
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(reset_value<int>)>::attribute_type, int>);
        REQUIRE(parse("", reset_value<int>, val));
        CHECK(val == 0);
    }
    {
        std::vector<int> val;
        val.reserve(100);
        val.emplace_back(42);
        auto const prev_capacity = val.capacity();

        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(reset_value<std::vector<int>>)>::attribute_type, std::vector<int>>);
        REQUIRE(parse("", reset_value<std::vector<int>>, val));
        CHECK(val.empty());
        CHECK(val.capacity() == prev_capacity); // should preserve capacity as per `.clear()`
    }
}
