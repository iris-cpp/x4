/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>
#include <iris/x4/core/write_attribute.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/numeric/real.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/delimited_list.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/adapted/std_tuple.hpp>
#include <iris/rvariant.hpp>

#include <set>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

namespace {

enum class strong_int : int {};

struct converted_from_int
{
    converted_from_int(int) {} // NOLINT(misc-explicit-constructor)
};

struct assigned_from_int
{
    explicit assigned_from_int(int) {}
    assigned_from_int& operator=(int) { return *this; }
};

struct Paren
{
    int inner = 0;
    bool operator==(Paren const&) const = default;
};

struct Box
{
    double value = 0;
};

} // anonymous

IRIS_ALLOY_ADAPT_STRUCT(Paren, inner);
IRIS_ALLOY_ADAPT_STRUCT(Box, value);

IRIS_X4_DECLARE(int_rule, int);
IRIS_X4_DECLARE(ints_rule, std::vector<int>);
IRIS_X4_DECLARE(double_rule, double);
IRIS_X4_DECLARE(paren_rule, Paren);

constexpr auto int_rule_def = x4::int_;
constexpr auto ints_rule_def = x4::int_ % ',';
constexpr auto double_rule_def = x4::double_;
constexpr auto paren_rule_def = '(' >> int_rule >> ')';

IRIS_X4_DEFINE(int_rule);
IRIS_X4_DEFINE(ints_rule);
IRIS_X4_DEFINE(double_rule);
IRIS_X4_DEFINE(paren_rule);

// "The Spirit X3 rule problem" in Boost.Parser's documentation
// https://www.boost.org/doc/libs/1_89_0/doc/html/boost_parser/this_library_s_relationship_to_boost_spirit.html#boost_parser.this_library_s_relationship_to_boost_spirit.the_spirit_x3_rule_problem
// https://github.com/boostorg/spirit_x4/issues/38
TEST_CASE("x3_rule_problem")
{
    using x4::X4StrictlyWritable;

    STATIC_CHECK(X4StrictlyWritable<long long&, int&&>);
    STATIC_CHECK(!X4StrictlyWritable<short&, int&&>);
    STATIC_CHECK(!X4StrictlyWritable<unsigned long long&, int&&>);
    STATIC_CHECK(!X4StrictlyWritable<double&, int&&>);
    STATIC_CHECK(!X4StrictlyWritable<strong_int&, int&&>);
    STATIC_CHECK(!X4StrictlyWritable<assigned_from_int&, int&&>); // assignable, but not implicitly convertible
    STATIC_CHECK(X4StrictlyWritable<long double&, double&&>);
    STATIC_CHECK(!X4StrictlyWritable<float&, double&&>);
    STATIC_CHECK(!X4StrictlyWritable<long long&, double&&>);

    STATIC_CHECK(X4StrictlyWritable<converted_from_int&, long long&&>);

    STATIC_CHECK(!X4StrictlyWritable<std::set<int>&, std::vector<int>&&>);
    STATIC_CHECK(!X4StrictlyWritable<std::vector<strong_int>&, std::vector<int>&&>);
    STATIC_CHECK(!X4StrictlyWritable<std::string&, char&&>);
    STATIC_CHECK(!X4StrictlyWritable<int&, Paren&&>);

    STATIC_CHECK(X4StrictlyWritable<std::string_view&, std::string&&>);
    STATIC_CHECK(x4::detail::dangles<std::string_view, std::string&&>);

    {
        long long n = 0;
        REQUIRE(parse("42", int_rule, n));
        CHECK(n == 42);
    }
    {
        std::vector<int> v;
        REQUIRE(parse("1,2", ints_rule, v));
        CHECK(v == std::vector<int>{1, 2});
    }

    {
        Paren p;
        REQUIRE(parse("7", int_rule, p));
        CHECK(p == Paren{7});
        REQUIRE(parse("(1)", paren_rule, p));
        CHECK(p == Paren{1});
    }

    {
        using variant = iris::rvariant<std::tuple<int>, Box>;
        constexpr std::string_view input = "3.5";

        variant assigned;
        auto first = input.begin();
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4244)
#endif
        REQUIRE(double_rule.parse(first, input.end(), x4::unused, assigned));
#ifdef _MSC_VER
# pragma warning(pop)
#endif
        CHECK(std::get<0>(iris::get<0>(assigned)) == 3);

        variant written;
        x4::write_attribute(written, 3.5);
        CHECK(iris::get<1>(written).value == 3.5);

        variant prepared;
        REQUIRE(parse(input, double_rule, prepared));
        CHECK(iris::get<1>(prepared).value == 3.5);
    }
}
