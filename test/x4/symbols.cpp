/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/symbols.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/directive/no_case.hpp>

#include <stdexcept>
#include <string_view>

namespace {

enum class Op { lt, le, shl, shl_assign, add };

constexpr auto ops = x4::symbols<Op>("operator", {
    {"<", Op::lt}, {"<=", Op::le}, {"<<", Op::shl}, {"<<=", Op::shl_assign}, {"+", Op::add}
});

constexpr auto keywords = x4::symbols("keyword", {
    "instanceof", "in", "if"
});

} // anonymous

TEST_CASE("symbols")
{
    using x4::no_case;

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(ops);
    STATIC_CHECK(*ops.find("<<=") == Op::shl_assign);
    STATIC_CHECK(ops.find("<<<") == nullptr);
    STATIC_CHECK(ops.find("") == nullptr);
    CHECK(x4::what(ops) == "operator");
    CHECK(x4::what(keywords) == "keyword");

    Op op{};
    CHECK(parse("<<=", ops, op));
    CHECK(op == Op::shl_assign);
    CHECK(parse("<=", ops, op));
    CHECK(op == Op::le);
    CHECK(parse("<", ops, op));
    CHECK(op == Op::lt);
    CHECK(parse(" + ", ops, x4::standard::space, op));
    CHECK(op == Op::add);
    CHECK(!parse("", ops, op));
    CHECK(!parse("=", ops, op));
    {
        auto const res = parse("<<x", ops, op);
        CHECK(res.is_partial_match());
        CHECK(res.remainder_str() == "x");
        CHECK(op == Op::shl);
    }

    CHECK(parse("instanceof", keywords));
    CHECK(parse("if", keywords));
    {
        auto const res = parse("insta", keywords);
        CHECK(res.is_partial_match());
        CHECK(res.remainder_str() == "sta");
    }

    {
        constexpr std::string_view input = "<<=1";
        auto first = input.begin();
        Op const* found = ops.prefix_find(first, input.end());
        REQUIRE(found != nullptr);
        CHECK(*found == Op::shl_assign);
        CHECK(first == input.begin() + 3);

        found = ops.prefix_find(first, input.end());
        CHECK(found == nullptr);
        CHECK(first == input.begin() + 3);
    }

    {
        constexpr auto names = x4::symbols<int>("name", {{"Bob", 1}, {"alice", 2}, {"ab", 3}, {"AB", 4}, {"ABC", 5}});
        int value = 0;
        CHECK(!parse("bob", names, value));
        CHECK(parse("bob", no_case[names], value));
        CHECK(value == 1);
        CHECK(parse("BOB", no_case[names], value));
        CHECK(value == 1);
        CHECK(parse("ALICE", no_case[names], value));
        CHECK(value == 2);

        CHECK(parse("ab", no_case[names], value));
        CHECK(value == 3);
        CHECK(parse("AB", no_case[names], value));
        CHECK(value == 4);
        CHECK(parse("aB", no_case[names], value));
        CHECK(value == 3);
        CHECK(parse("Ab", no_case[names], value));
        CHECK(value == 4);

        CHECK(parse("abc", no_case[names], value));
        CHECK(value == 5);
    }

    {
        int value = 0;
        constexpr auto unicode = x4::symbols<int>("symbol", {{U"Äb", 1}});
        CHECK(parse(U"äB", no_case[unicode], value));
        CHECK(value == 1);
    }

    CHECK_THROWS_AS((void)x4::symbols<int>("symbol", {{"+", 1}, {"+", 2}}), std::invalid_argument);
    CHECK_THROWS_AS((void)x4::symbols("symbol", {"if", ""}), std::invalid_argument);
}
