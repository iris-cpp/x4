#include "iris_x4_test.hpp"

#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/numeric/real.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/string/string.hpp>
#include <iris/x4/rule.hpp>

#include <iris/x4/traits/narrowing.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/adapted/std_tuple.hpp>

#include <optional>
#include <string>
#include <tuple>

template<class T>
using SES = x4_test::single_element_struct<T>;


// ===================================================================
// Rule definitions for narrowing tests
// ===================================================================

using IntRule = x4::rule<struct int_rule_tag, int>;
using LongLongRule = x4::rule<struct long_long_rule_tag, long long>;
using ShortRule = x4::rule<struct short_rule_tag, short>;
using FloatRule = x4::rule<struct float_rule_tag, float>;
using DoubleRule = x4::rule<struct double_rule_tag, double>;

IRIS_X4_DECLARE_CONSTEXPR(IntRule)
IRIS_X4_DECLARE_CONSTEXPR(LongLongRule)
IRIS_X4_DECLARE_CONSTEXPR(ShortRule)
IRIS_X4_DECLARE_CONSTEXPR(FloatRule)
IRIS_X4_DECLARE_CONSTEXPR(DoubleRule)

constexpr IntRule int_rule;
constexpr LongLongRule long_long_rule;
constexpr ShortRule short_rule;
constexpr FloatRule float_rule;
constexpr DoubleRule double_rule;

constexpr auto int_rule_def = x4::int_;
constexpr auto long_long_rule_def = x4::long_long;
constexpr auto short_rule_def = x4::short_;
constexpr auto float_rule_def = x4::float_;
constexpr auto double_rule_def = x4::double_;

IRIS_X4_DEFINE_CONSTEXPR(int_rule)
IRIS_X4_DEFINE_CONSTEXPR(long_long_rule)
IRIS_X4_DEFINE_CONSTEXPR(short_rule)
IRIS_X4_DEFINE_CONSTEXPR(float_rule)
IRIS_X4_DEFINE_CONSTEXPR(double_rule)


// ===================================================================
// Trait-level static checks
// ===================================================================

TEST_CASE("narrowing: is_assignable_without_lossy_conversion trait")
{
    using x4::detail::is_assignable_without_lossy_conversion;

    // Non-narrowing: same type
    STATIC_CHECK(is_assignable_without_lossy_conversion<int&, int>::value);
    STATIC_CHECK(is_assignable_without_lossy_conversion<double&, double>::value);

    // Non-narrowing: widening
    STATIC_CHECK(is_assignable_without_lossy_conversion<long long&, int>::value);
    STATIC_CHECK(is_assignable_without_lossy_conversion<double&, float>::value);
    STATIC_CHECK(is_assignable_without_lossy_conversion<int&, short>::value);

    // Narrowing: lossy conversions
    STATIC_CHECK(!is_assignable_without_lossy_conversion<int&, long long>::value);
    STATIC_CHECK(!is_assignable_without_lossy_conversion<short&, int>::value);
    STATIC_CHECK(!is_assignable_without_lossy_conversion<float&, double>::value);
    STATIC_CHECK(!is_assignable_without_lossy_conversion<int&, float>::value);
    STATIC_CHECK(!is_assignable_without_lossy_conversion<float&, int>::value);

    // Signed/unsigned mismatch
    STATIC_CHECK(!is_assignable_without_lossy_conversion<int&, unsigned>::value);
    STATIC_CHECK(!is_assignable_without_lossy_conversion<unsigned&, int>::value);

    // Non-arithmetic dest: narrowing not checked
    STATIC_CHECK(is_assignable_without_lossy_conversion<std::string&, const char*>::value);
}

TEST_CASE("narrowing: is_tuple_assignable_without_lossy_conversion trait")
{
    using x4::detail::is_tuple_assignable_without_lossy_conversion;

    STATIC_CHECK((is_tuple_assignable_without_lossy_conversion<
        std::tuple<int, double>,
        std::tuple<int, float>
    >::value));

    STATIC_CHECK((!is_tuple_assignable_without_lossy_conversion<
        std::tuple<char, int>,
        std::tuple<int, int>
    >::value));

    STATIC_CHECK((!is_tuple_assignable_without_lossy_conversion<
        std::tuple<int, char>,
        std::tuple<int, int>
    >::value));
}


// ===================================================================
// Parse-level tests (non-narrowing conversions that must compile & run)
// ===================================================================

TEST_CASE("narrowing: parse int to long long (widening)")
{
    long long a{};
    REQUIRE(parse("42", x4::int_, a));
    CHECK(a == 42);
}

TEST_CASE("narrowing: parse short to int (widening)")
{
    int a{};
    REQUIRE(parse("7", x4::short_, a));
    CHECK(a == 7);
}

TEST_CASE("narrowing: parse float to double (widening)")
{
    double a{};
    REQUIRE(parse("3.14", x4::float_, a));
    CHECK(a > 3.13);
    CHECK(a < 3.15);
}

TEST_CASE("narrowing: parse int to long long via SES (single-element tuple-like)")
{
    SES<long long> a{};
    REQUIRE(parse("99", x4::int_, a));
    CHECK(a.value == 99);
}

TEST_CASE("narrowing: parse int to optional<long long> (widening)")
{
    std::optional<long long> a;
    REQUIRE(parse("55", x4::int_, a));
    REQUIRE(a.has_value());
    CHECK(*a == 55);
}

TEST_CASE("narrowing: parse tuple element-wise (all widening)")
{
    std::tuple<long long, double> a;
    REQUIRE(parse("1,3.0", x4::int_ >> ',' >> x4::float_, a));
    CHECK(std::get<0>(a) == 1);
    CHECK(std::get<1>(a) > 2.99);
}

TEST_CASE("narrowing: parse same type (int to int)")
{
    int a{};
    REQUIRE(parse("123", x4::int_, a));
    CHECK(a == 123);
}

TEST_CASE("narrowing: parse same type (char to char)")
{
    char a{};
    REQUIRE(parse("x", x4::char_, a));
    CHECK(a == 'x');
}


// ===================================================================
// Rule-level tests
//
// When the exposed attribute type differs from the rule's attribute
// type, the rule materializes its own attribute internally and then
// assigns it to the exposed attribute. The narrowing check applies
// to that assignment.
// ===================================================================

TEST_CASE("narrowing: RuleAttrConvertibleWithoutNarrowing concept")
{
    using x4::detail::RuleAttrConvertibleWithoutNarrowing;

    // Same type: always OK
    STATIC_CHECK(RuleAttrConvertibleWithoutNarrowing<int, int>);
    STATIC_CHECK(RuleAttrConvertibleWithoutNarrowing<long long, long long>);

    // Widening: OK
    STATIC_CHECK(RuleAttrConvertibleWithoutNarrowing<long long, int>);
    STATIC_CHECK(RuleAttrConvertibleWithoutNarrowing<double, float>);
    STATIC_CHECK(RuleAttrConvertibleWithoutNarrowing<int, short>);

    // Narrowing: rejected
    STATIC_CHECK(!RuleAttrConvertibleWithoutNarrowing<int, long long>);
    STATIC_CHECK(!RuleAttrConvertibleWithoutNarrowing<short, int>);
    STATIC_CHECK(!RuleAttrConvertibleWithoutNarrowing<float, double>);
    STATIC_CHECK(!RuleAttrConvertibleWithoutNarrowing<char, int>);

    // Signed/unsigned mismatch
    STATIC_CHECK(!RuleAttrConvertibleWithoutNarrowing<int, unsigned>);
    STATIC_CHECK(!RuleAttrConvertibleWithoutNarrowing<unsigned, int>);
}

TEST_CASE("narrowing: rule with same exposed type (int rule to int)")
{
    int a{};
    REQUIRE(parse("42", int_rule, a));
    CHECK(a == 42);
}

TEST_CASE("narrowing: rule widening (int rule to long long)")
{
    long long a{};
    REQUIRE(parse("42", int_rule, a));
    CHECK(a == 42);
}

TEST_CASE("narrowing: rule widening (short rule to int)")
{
    int a{};
    REQUIRE(parse("7", short_rule, a));
    CHECK(a == 7);
}

TEST_CASE("narrowing: rule widening (float rule to double)")
{
    double a{};
    REQUIRE(parse("3.14", float_rule, a));
    CHECK(a > 3.13);
    CHECK(a < 3.15);
}

TEST_CASE("narrowing: immediate rule widening (int to long long)")
{
    long long a{};
    REQUIRE(parse("42", x4::rule<struct imm_tag, int>{} = x4::int_, a));
    CHECK(a == 42);
}


// ===================================================================
// Narrowing conversion static_assert tests
//
// The following test cases are commented out because they are expected
// to produce static_assert failures. Uncomment any single case to
// verify the assertion fires.
// ===================================================================

// --- move_to: source to plain (narrowing) ---
// TEST_CASE("narrowing: int to char") { char a{}; REQUIRE(parse("42", x4::int_, a)); }
// TEST_CASE("narrowing: int to short") { short a{}; REQUIRE(parse("42", x4::int_, a)); }
// TEST_CASE("narrowing: long long to int") { int a{}; REQUIRE(parse("42", x4::long_long, a)); }
// TEST_CASE("narrowing: double to float") { float a{}; REQUIRE(parse("3.14", x4::double_, a)); }

// --- move_to: single-element tuple-like to plain (narrowing) ---
// TEST_CASE("narrowing: SES int to char") { SES<char> a{}; REQUIRE(parse("42", x4::int_, a)); }

// --- move_to: source to optional (narrowing) ---
// TEST_CASE("narrowing: int to optional<char>") { std::optional<char> a; REQUIRE(parse("42", x4::int_, a)); }

// --- move_to: tuple element-wise (narrowing) ---
// TEST_CASE("narrowing: tuple char,int") { std::tuple<char, int> a; REQUIRE(parse("1,2", x4::int_ >> ',' >> x4::int_, a)); }
// TEST_CASE("narrowing: tuple int,char") { std::tuple<int, char> a; REQUIRE(parse("1,2", x4::int_ >> ',' >> x4::int_, a)); }

// --- rule: rule attribute to exposed attribute (narrowing) ---
// TEST_CASE("narrowing: int rule to char") { char a{}; REQUIRE(parse("42", int_rule, a)); }
// TEST_CASE("narrowing: int rule to short") { short a{}; REQUIRE(parse("42", int_rule, a)); }
// TEST_CASE("narrowing: long long rule to int") { int a{}; REQUIRE(parse("42", long_long_rule, a)); }
// TEST_CASE("narrowing: double rule to float") { float a{}; REQUIRE(parse("3.14", double_rule, a)); }
// TEST_CASE("narrowing: immediate rule narrowing") { char a{}; REQUIRE(parse("42", x4::rule<struct imm_n_tag, int>{} = x4::int_, a)); }
