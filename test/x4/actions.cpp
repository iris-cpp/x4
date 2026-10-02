/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>
#include <iris/x4/attribute/as.hpp>
#include <iris/x4/attribute/value.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/numeric/real.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/directive/lexeme.hpp>
#include <iris/x4/directive/omit.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/operator/delimited_list.hpp>
#include <iris/x4/operator/difference.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/sequence.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/tuple.hpp>
#include <iris/rvariant.hpp>

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace visit_attr_test {

struct NumberLit
{
    long long value = 0;
    bool operator==(NumberLit const&) const = default;
};

struct StringLit
{
    std::string value;
    bool operator==(StringLit const&) const = default;
};

struct Ident
{
    std::string name;
    bool operator==(Ident const&) const = default;
};

using Literal = iris::rvariant<NumberLit, StringLit>;
using Primary = iris::rvariant<Literal, Ident>;

} // visit_attr_test

IRIS_ALLOY_ADAPT_STRUCT(visit_attr_test::NumberLit, value);
IRIS_ALLOY_ADAPT_STRUCT(visit_attr_test::StringLit, value);
IRIS_ALLOY_ADAPT_STRUCT(visit_attr_test::Ident, name);

namespace visit_attr_test {

// Succeeds with an `int` ("7x", "0x"), or without an attribute ("5y")
constexpr auto five = (x4::int_ >> 'x') | (x4::lit('5') >> 'y');

// The example in README
constexpr auto timeout = ((x4::int_ >> "ms") | x4::lit("auto")).on_match([](auto&& ctx) {
    x4::visit_attr(
        ctx,
        [](int& ms) { ms = std::min(ms, 1000); },
        [](x4::unused_type) {} // corresponds to the "auto" branch
    );
});

using Value = iris::rvariant<int, std::string>;

constexpr x4::rule<struct value_rule_id, Value> value_rule = "value_rule";
constexpr auto value_rule_def = value_rule = x4::int_ | +x4::standard::alpha;
IRIS_X4_DEFINE(value_rule)

constexpr auto quoted = x4::lexeme['"' >> *~x4::standard::char_('"') >> '"'];
constexpr auto number_lit = x4::as<NumberLit>(x4::long_long);
constexpr auto string_lit = x4::as<StringLit>(quoted);
constexpr auto ident = x4::as<Ident>(x4::lexeme[x4::as<std::string>(x4::standard::alpha >> *x4::standard::alnum)]);

constexpr x4::rule<struct literal_rule_id, Literal> literal_rule = "literal_rule";
constexpr auto literal_rule_def = literal_rule = number_lit | string_lit;
IRIS_X4_DEFINE(literal_rule)

constexpr auto literal_as = x4::as<Literal>(number_lit | string_lit);

// What an action saw: the `int` written, or none
using seen_ints = std::vector<std::optional<int>>;

} // visit_attr_test

TEST_CASE("action")
{
    using x4::int_;
    using x4::standard::digit;
    using x4::standard::space;

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(x4::int_.on_match(std::true_type{}));

    {
        int x = 0;
        auto const fun_action = [&](auto&& ctx) { x += x4::_attr(ctx); };
        CHECK(parse("{42}", '{' >> int_.on_match(fun_action) >> '}'));
    }
    {
        auto const fail = [](auto&&) { return false; };
        std::string input("1234 6543");
        char next = '\0';

        auto const setnext = [&](auto&& ctx) {
            next = x4::_attr(ctx);
        };

        REQUIRE(parse(input, int_.on_match(fail) | digit.on_match(setnext), space).is_partial_match());
        CHECK(next == '1');
    }

    {
        // ensure no unneeded synthesization, copying and moving occurred
        auto p = '{' >> int_ >> '}';

        x4_test::stationary st { 0 };
        static_assert(x4::X4Attribute<x4_test::stationary>);

        REQUIRE(parse("{42}", p.on_match([]{}), st));
        CHECK(st.val == 42);
    }
}

TEST_CASE("visit_attr")
{
    using namespace visit_attr_test;
    using x4::int_;
    using x4::lit;
    using x4::lexeme;
    using iris::rvariant;

    seen_ints seen;
    auto const observe = [&](auto&& ctx) {
        STATIC_CHECK(x4::detail::is_action_attribute_view_v<decltype(x4::_attr(ctx))>);
        STATIC_CHECK(std::is_const_v<std::remove_reference_t<decltype(x4::_attr(ctx))>>);
        x4::visit_attr(
            ctx,
            [&](int& value) { seen.emplace_back(value); },
            [&](x4::unused_type) { seen.emplace_back(std::nullopt); }
        );
    };
    auto const times_ten = [](auto&& ctx) {
        x4::visit_attr(ctx, [](int& value) { value *= 10; }, [](x4::unused_type) {});
    };

    // The action is called with the int written, including 0, or with no attribute
    {
        int number = 99;
        REQUIRE(parse("7x", five.on_match(observe), number));
        CHECK(number == 7);
        REQUIRE(parse("0x", five.on_match(observe), number));
        CHECK(number == 0);
        number = 99;
        REQUIRE(parse("5y", five.on_match(observe), number));
        CHECK(number == 0);
        CHECK(seen == seen_ints{7, 0, std::nullopt});
    }

    // A changed value is written, a value changed to 0 as well
    {
        int ms = 0;
        REQUIRE(parse("300ms", timeout, ms));
        CHECK(ms == 300);
        REQUIRE(parse("5000ms", timeout, ms));
        CHECK(ms == 1000);
        REQUIRE(parse("auto", timeout, ms));
        CHECK(ms == 0);

        auto const to_zero = [](auto&& ctx) {
            x4::visit_attr(ctx, [](int& value) { value = 0; }, [](x4::unused_type) {});
        };
        std::optional<int> optional_number;
        REQUIRE(parse("7x", five.on_match(to_zero), optional_number));
        CHECK(optional_number == 0);
        REQUIRE(parse("5y", five.on_match(to_zero), optional_number));
        CHECK(optional_number == std::nullopt);
    }

    // The action sees the same in a sequence and a repetition
    {
        seen.clear();
        rvariant<std::string, int> variant;
        REQUIRE(parse("<7x>", '<' >> five.on_match(observe) >> '>', variant));
        CHECK(variant == rvariant<std::string, int>{7});
        REQUIRE(parse("<5y>", '<' >> five.on_match(observe) >> '>', variant));
        CHECK(variant == rvariant<std::string, int>{std::string{}});

        std::vector<int> ints;
        REQUIRE(parse("7x5y0x", *five.on_match(observe), ints));
        CHECK(ints == std::vector<int>{7, 0});
        CHECK(seen == seen_ints{7, std::nullopt, 7, std::nullopt, 0});
    }

    // No attribute: the whole destination is in its default state, and a container gets no element
    {
        int number = 99;
        REQUIRE(parse("5y", five.on_match(observe), number));
        CHECK(number == 0);
        std::optional<int> optional_number = 99;
        REQUIRE(parse("5y", five.on_match(observe), optional_number));
        CHECK(optional_number == std::nullopt);
        rvariant<std::string, int> variant = 99;
        REQUIRE(parse("5y", five.on_match(observe), variant));
        CHECK(variant == rvariant<std::string, int>{std::string{}});

        std::vector<int> ints;
        REQUIRE(parse("5y5y", +five.on_match(observe), ints));
        CHECK(ints.empty());
        REQUIRE(parse("7x5y", +five.on_match(observe), ints));
        CHECK(ints == std::vector<int>{7});
        REQUIRE(parse("1,5y,2", ((int_ >> 'x') | (lit('5') >> 'y') | int_).on_match(observe) % ',', ints));
        CHECK(ints == std::vector<int>{1, 2});
    }

    // The same in a sequence of two attributes, whose other element is kept: the destination
    // is not prepared here, as `parse` is called directly
    {
        constexpr std::string_view input = "7,5y";
        alloy::tuple<int, int> numbers{1, 99};
        auto first = input.begin();
        REQUIRE((int_ >> ',' >> five.on_match(observe)).parse(first, input.end(), x4::unused, numbers));
        CHECK(first == input.end());
        CHECK(numbers == alloy::tuple<int, int>{7, 0});
    }

    // Through `lexeme`, a sequence of one attribute and the left of a difference
    {
        seen.clear();
        int number = 0;
        REQUIRE(parse("7x", lexeme[five].on_match(observe), number));
        CHECK(number == 7);
        REQUIRE(parse("5y", lexeme[five.on_match(observe)], number));
        CHECK(number == 0);
        REQUIRE(parse("<5y>", ('<' >> five >> '>').on_match(observe), number));
        CHECK(number == 0);
        REQUIRE(parse("<7x>", ('<' >> five >> '>').on_match(times_ten), number));
        CHECK(number == 70);
        REQUIRE(parse("7x", (five - lit("9")).on_match(observe), number));
        CHECK(number == 7);
        CHECK(seen == seen_ints{7, std::nullopt, std::nullopt, 7});
    }

    // The inner action's change and whether it wrote reach the outer action
    {
        seen.clear();
        int number = 0;
        REQUIRE(parse("7x", five.on_match(times_ten).on_match(observe), number));
        CHECK(number == 70);
        REQUIRE(parse("5y", five.on_match(times_ten).on_match(observe), number));
        CHECK(number == 0);
        REQUIRE(parse("7x", (five.on_match(times_ten) | lit('z')).on_match(observe), number));
        CHECK(number == 70);
        REQUIRE(parse("z", (five.on_match(times_ten) | lit('z')).on_match(observe), number));
        CHECK(number == 0);
        CHECK(seen == seen_ints{70, std::nullopt, 70, std::nullopt});
    }

    // An action which rejects the match after changing the value: the change is not written
    {
        auto const reject_zero = [](auto&& ctx) {
            bool accepted = true;
            x4::visit_attr(
                ctx,
                [&](int& value) {
                    accepted = value != 0;
                    value = 99;
                },
                [](x4::unused_type) {}
            );
            return accepted;
        };

        std::vector<int> ints;
        auto const res = x4::parse(std::string_view("7x0x"), *five.on_match(reject_zero), ints);
        REQUIRE(res.ok);
        CHECK(res.remainder_str() == "0x");
        CHECK(ints == std::vector<int>{99});

        int number = -1;
        REQUIRE(parse("0x", five.on_match(reject_zero) | (int_ >> 'x'), number));
        CHECK(number == 0);

        ints.clear();
        REQUIRE(parse("0x7x", *(five.on_match(reject_zero) | (int_ >> 'x')), ints));
        CHECK(ints == std::vector<int>{0, 99});
    }

    // An action on a subject which always writes keeps `int&`; with no destination, the action
    // still sees the attribute written
    {
        int number = 0;
        REQUIRE(parse("7", int_.on_match([](auto&& ctx) {
            STATIC_CHECK(std::is_same_v<decltype(x4::_attr(ctx)), int&>);
            x4::_attr(ctx) += 1;
        }), number));
        CHECK(number == 8);

        seen.clear();
        REQUIRE(parse("7x5y", x4::omit[*five.on_match(observe)], x4::unused));
        REQUIRE(parse("0x", five.on_match(observe), x4::unused));
        CHECK(seen == seen_ints{7, std::nullopt, 0});
    }
}

TEST_CASE("visit_attr candidates")
{
    using namespace visit_attr_test;
    using x4::int_;
    using x4::double_;
    using x4::lit;
    using x4::standard::alpha;
    using iris::rvariant;

    // The candidates of an alternative are passed one by one; a declared variant is one candidate
    {
        std::vector<std::string> seen;
        auto const change_declared = [&](auto&& ctx) {
            x4::visit_attr(
                ctx,
                [&](Value& value) {
                    seen.emplace_back(value.index() == 0 ? "Value int" : "Value string");
                    value = 42;
                },
                [&](x4::unused_type) { seen.emplace_back("none"); }
            );
        };
        auto const observe_declared_or_double = [&](auto&& ctx) {
            x4::visit_attr(
                ctx,
                [&](Value&) { seen.emplace_back("Value"); },
                [&](double& value) { seen.emplace_back("double " + std::to_string(static_cast<int>(value))); },
                [&](x4::unused_type) { seen.emplace_back("none"); }
            );
        };

        Value value;
        auto const flat = (lit("null") | int_ | +alpha).on_match([&](auto&& ctx) {
            x4::visit_attr(
                ctx,
                [&](int& number) { seen.emplace_back("int " + std::to_string(number)); },
                [&](std::string& string) { seen.emplace_back("string " + string); },
                [&](x4::unused_type) { seen.emplace_back("none"); }
            );
        });
        REQUIRE(parse("1", flat, value));
        REQUIRE(parse("ab", flat, value));
        REQUIRE(parse("null", flat, value));

        auto const declared_as = (lit("null") | x4::as<Value>(int_ | +alpha)).on_match(change_declared);
        auto const declared_rule = (lit("null") | value_rule).on_match(change_declared);
        REQUIRE(parse("ab", declared_as, value));
        CHECK(value == Value{42});
        REQUIRE(parse("ab", declared_rule, value));
        CHECK(value == Value{42});
        REQUIRE(parse("null", declared_rule, value));
        CHECK(value == Value{});

        using value_or_double = rvariant<Value, double>;
        value_or_double destination;
        auto const declared_as_or_double = (lit("null") | x4::as<Value>(int_ | +alpha) | ('#' >> double_)).on_match(observe_declared_or_double);
        auto const declared_rule_or_double = (lit("null") | value_rule | ('#' >> double_)).on_match(observe_declared_or_double);
        REQUIRE(parse("#2", declared_as_or_double, destination));
        CHECK(destination == value_or_double{2.0});
        REQUIRE(parse("ab", declared_as_or_double, destination));
        CHECK(destination == value_or_double{Value{std::string("ab")}});
        REQUIRE(parse("7", declared_rule_or_double, destination));
        CHECK(destination == value_or_double{Value{7}});

        CHECK(seen == std::vector<std::string>{
            "int 1", "string ab", "none",
            "Value string", "Value string", "none",
            "double 2", "Value", "Value",
        });
    }

    {
        auto const check_number = [](auto const& number) {
            long long value = -1;
            REQUIRE(parse("42", number, value));
            CHECK(value == 42);
            REQUIRE(parse("12345678901", number, value)); // `long_long`, as `int_` overflows
            CHECK(value == 12345678901);
            value = -1;
            REQUIRE(parse("null", number, value));
            CHECK(value == 0);

            std::vector<long long> values;
            REQUIRE(parse("42,12345678901,null", number % ',', values));
            CHECK(values == std::vector<long long>{42, 12345678901});
        };
        auto const number = int_ | x4::long_long | lit("null");
        check_number(number);
        check_number(number.on_match([] {}));
    }

    // `Literal&`, `Ident&` or none, written into `Primary` and `std::vector<Primary>`
    {
        std::vector<std::string> seen;
        auto const observe = [&](auto&& ctx) {
            x4::visit_attr(
                ctx,
                [&](Literal& literal) { seen.emplace_back(literal.index() == 0 ? "NumberLit" : "StringLit"); },
                [&](Ident&) { seen.emplace_back("Ident"); },
                [&](x4::unused_type) { seen.emplace_back("none"); }
            );
        };
        Primary const number_literal{Literal{NumberLit{1}}};
        Primary const string_literal{Literal{StringLit{"s"}}};
        Primary const identifier{Ident{"y"}};

        auto const check_primary = [&](auto const& literal) {
            auto const primary = (lit("null") | literal | ident).on_match(observe);
            Primary attr;
            REQUIRE(parse("\"s\"", primary, attr));
            CHECK(attr == string_literal);
            REQUIRE(parse("y", primary, attr));
            CHECK(attr == identifier);
            REQUIRE(parse("null", primary, attr));
            CHECK(attr == Primary{});

            std::vector<Primary> primaries;
            REQUIRE(parse("1,null,\"s\",y", primary % ',', primaries));
            CHECK(primaries == std::vector<Primary>{number_literal, string_literal, identifier});
        };
        check_primary(literal_rule);
        check_primary(literal_as);

        std::vector<std::string> const once{"StringLit", "Ident", "none", "NumberLit", "none", "StringLit", "Ident"};
        std::vector<std::string> twice = once;
        twice.insert(twice.end(), once.begin(), once.end());
        CHECK(seen == twice);
    }

    // An explicit empty array is one element; no attribute adds none
    {
        using Array = std::vector<int>;
        std::vector<std::string> seen;
        auto const array = ((lit('[') >> (int_ % ',') >> ']') | (lit("[]") >> x4::default_value<Array>) | lit("null")).on_match([&](auto&& ctx) {
            x4::visit_attr(
                ctx,
                [&](Array& elements) { seen.emplace_back("Array " + std::to_string(elements.size())); },
                [&](x4::unused_type) { seen.emplace_back("none"); }
            );
        });
        std::vector<Array> arrays;
        REQUIRE(parse("[];[1,2];null;[]", array % ';', arrays));
        CHECK(arrays == std::vector<Array>{{}, {1, 2}, {}});
        CHECK(seen == std::vector<std::string>{"Array 0", "Array 2", "none", "Array 0"});
    }
}
