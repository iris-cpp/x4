#include "iris_x4_test.hpp"

#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/numeric/real.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/string/string.hpp>
#include <iris/x4/rule.hpp>

#include <iris/x4/traits/lossy_conversion.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/adapted/std_tuple.hpp>

#include <cstdint>
#include <optional>
#include <string>
#include <tuple>

template<class T>
using SES = x4_test::single_element_struct<T>;


// =======================================================================
// Mock "small int" type for is_lossy_assignment customization point test
// =======================================================================

struct small_int {
    std::int8_t value{};

    small_int() = default;
    explicit small_int(std::int8_t v) : value(v) {}

    small_int& operator=(int v) { value = static_cast<std::int8_t>(v); return *this; } // convenient but may narrow
};

// Specialization: assigning int to small_int is lossy because
// small_int can only hold 8-bit values.
template<>
struct iris::x4::detail::is_lossy_assignment<small_int, int> : std::true_type {};

struct convertible_to_int {
    operator int() const { return 42; }
};

struct convertible_to_long_long {
    operator long long() const { return 42; }
};


// =======================================================================
// Rule definitions for lossy conversion tests
// =======================================================================

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


// =======================================================================
// Trait-level static checks
// =======================================================================

TEST_CASE("lossy: is_assignable_without_lossy_conversion trait")
{
    using x4::detail::is_assignable_without_lossy_conversion;

    // Non-lossy: same type
    STATIC_CHECK(is_assignable_without_lossy_conversion<int&, int>::value);
    STATIC_CHECK(is_assignable_without_lossy_conversion<double&, double>::value);

    // Non-lossy: widening
    STATIC_CHECK(is_assignable_without_lossy_conversion<long long&, int>::value);
    STATIC_CHECK(is_assignable_without_lossy_conversion<double&, float>::value);
    STATIC_CHECK(is_assignable_without_lossy_conversion<int&, short>::value);

    // Lossy conversions
    STATIC_CHECK(!is_assignable_without_lossy_conversion<int&, long long>::value);
    STATIC_CHECK(!is_assignable_without_lossy_conversion<short&, int>::value);
    STATIC_CHECK(!is_assignable_without_lossy_conversion<float&, double>::value);
    STATIC_CHECK(!is_assignable_without_lossy_conversion<int&, float>::value);
    STATIC_CHECK(!is_assignable_without_lossy_conversion<float&, int>::value);

    // Signed/unsigned mismatch
    STATIC_CHECK(!is_assignable_without_lossy_conversion<int&, unsigned>::value);
    STATIC_CHECK(!is_assignable_without_lossy_conversion<unsigned&, int>::value);

    // Non-arithmetic dest: lossy conversion not checked
    STATIC_CHECK(is_assignable_without_lossy_conversion<std::string&, const char*>::value);
}

TEST_CASE("lossy: is_tuple_assignable_without_lossy_conversion trait")
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

TEST_CASE("lossy: is_lossy_assignment trait")
{
    using x4::detail::is_lossy_assignment;

    // Default: arithmetic narrowing
    STATIC_CHECK(!is_lossy_assignment<int, int>::value);
    STATIC_CHECK(!is_lossy_assignment<long long, int>::value);
    STATIC_CHECK(!is_lossy_assignment<double, float>::value);
    STATIC_CHECK(is_lossy_assignment<int, long long>::value);
    STATIC_CHECK(is_lossy_assignment<float, double>::value);
    STATIC_CHECK(is_lossy_assignment<short, int>::value);

    // Non-arithmetic Dest: not checked by default
    STATIC_CHECK(!is_lossy_assignment<std::string, const char*>::value);

    // Non-arithmetic Source with conversion operator to arithmetic Dest
    STATIC_CHECK(!is_lossy_assignment<int, convertible_to_int>::value);
    STATIC_CHECK(is_lossy_assignment<int, convertible_to_long_long>::value);
    STATIC_CHECK(!is_lossy_assignment<long long, convertible_to_int>::value);

    // Customization point: small_int = int is flagged as lossy (user-specialized)
    STATIC_CHECK(is_lossy_assignment<small_int, int>::value);

    // small_int = int via is_assignable_without_lossy_conversion
    // small_int::operator=(int) exists, but the specialization rejects it
    STATIC_CHECK(!x4::detail::is_assignable_without_lossy_conversion<small_int&, int>::value);
}


// =======================================================================
// Parse-level tests (non-lossy conversions that must compile & run)
// =======================================================================

TEST_CASE("lossy: parse int to long long (widening)")
{
    long long a{};
    REQUIRE(parse("42", x4::int_, a));
    CHECK(a == 42);
}

TEST_CASE("lossy: parse short to int (widening)")
{
    int a{};
    REQUIRE(parse("7", x4::short_, a));
    CHECK(a == 7);
}

TEST_CASE("lossy: parse float to double (widening)")
{
    double a{};
    REQUIRE(parse("3.14", x4::float_, a));
    CHECK(a > 3.13);
    CHECK(a < 3.15);
}

TEST_CASE("lossy: parse int to long long via SES (single-element tuple-like)")
{
    SES<long long> a{};
    REQUIRE(parse("99", x4::int_, a));
    CHECK(a.value == 99);
}

TEST_CASE("lossy: parse int to optional<long long> (widening)")
{
    std::optional<long long> a;
    REQUIRE(parse("55", x4::int_, a));
    REQUIRE(a.has_value());
    CHECK(*a == 55);
}

TEST_CASE("lossy: parse tuple element-wise (all widening)")
{
    std::tuple<long long, double> a;
    REQUIRE(parse("1,3.0", x4::int_ >> ',' >> x4::float_, a));
    CHECK(std::get<0>(a) == 1);
    CHECK(std::get<1>(a) > 2.99);
}

TEST_CASE("lossy: parse same type (int to int)")
{
    int a{};
    REQUIRE(parse("123", x4::int_, a));
    CHECK(a == 123);
}

TEST_CASE("lossy: parse same type (char to char)")
{
    char a{};
    REQUIRE(parse("x", x4::char_, a));
    CHECK(a == 'x');
}


// =======================================================================
// Rule-level tests
//
// When the exposed attribute type differs from the rule's attribute type,
// the rule materializes its own attribute internally and then assigns it
// to the exposed attribute. The lossy conversion check applies to that
// assignment.
// =======================================================================

TEST_CASE("lossy: RuleAttrAssignableWithoutLoss concept")
{
    using x4::detail::RuleAttrAssignableWithoutLoss;

    // Same type: always OK
    STATIC_CHECK(RuleAttrAssignableWithoutLoss<int, int>);
    STATIC_CHECK(RuleAttrAssignableWithoutLoss<long long, long long>);

    // Widening: OK
    STATIC_CHECK(RuleAttrAssignableWithoutLoss<long long, int>);
    STATIC_CHECK(RuleAttrAssignableWithoutLoss<double, float>);
    STATIC_CHECK(RuleAttrAssignableWithoutLoss<int, short>);

    // Lossy: rejected
    STATIC_CHECK(!RuleAttrAssignableWithoutLoss<int, long long>);
    STATIC_CHECK(!RuleAttrAssignableWithoutLoss<short, int>);
    STATIC_CHECK(!RuleAttrAssignableWithoutLoss<float, double>);
    STATIC_CHECK(!RuleAttrAssignableWithoutLoss<char, int>);

    // Signed/unsigned mismatch
    STATIC_CHECK(!RuleAttrAssignableWithoutLoss<int, unsigned>);
    STATIC_CHECK(!RuleAttrAssignableWithoutLoss<unsigned, int>);
}

TEST_CASE("lossy: rule with same exposed type (int rule to int)")
{
    int a{};
    REQUIRE(parse("42", int_rule, a));
    CHECK(a == 42);
}

TEST_CASE("lossy: rule widening (int rule to long long)")
{
    long long a{};
    REQUIRE(parse("42", int_rule, a));
    CHECK(a == 42);
}

TEST_CASE("lossy: rule widening (short rule to int)")
{
    int a{};
    REQUIRE(parse("7", short_rule, a));
    CHECK(a == 7);
}

TEST_CASE("lossy: rule widening (float rule to double)")
{
    double a{};
    REQUIRE(parse("3.14", float_rule, a));
    CHECK(a > 3.13);
    CHECK(a < 3.15);
}

TEST_CASE("lossy: immediate rule widening (int to long long)")
{
    long long a{};
    REQUIRE(parse("42", x4::rule<struct imm_tag, int>{} = x4::int_, a));
    CHECK(a == 42);
}


// =======================================================================
// Lossy conversion static_assert tests
//
// The following test cases are commented out because they are expected to
// produce static_assert failures. Uncomment any single case to verify the
// assertion fires.
// =======================================================================

// --- move_to: source to plain (lossy) ---
// TEST_CASE("lossy: int to char") { char a{}; REQUIRE(parse("42", x4::int_, a)); }
// TEST_CASE("lossy: int to short") { short a{}; REQUIRE(parse("42", x4::int_, a)); }
// TEST_CASE("lossy: long long to int") { int a{}; REQUIRE(parse("42", x4::long_long, a)); }
// TEST_CASE("lossy: double to float") { float a{}; REQUIRE(parse("3.14", x4::double_, a)); }

// --- move_to: single-element tuple-like to plain (lossy) ---
// TEST_CASE("lossy: SES int to char") { SES<char> a{}; REQUIRE(parse("42", x4::int_, a)); }

// --- move_to: source to optional (lossy) ---
// TEST_CASE("lossy: int to optional<char>") { std::optional<char> a; REQUIRE(parse("42", x4::int_, a)); }

// --- move_to: tuple element-wise (lossy) ---
// TEST_CASE("lossy: tuple char,int") { std::tuple<char, int> a; REQUIRE(parse("1,2", x4::int_ >> ',' >> x4::int_, a)); }
// TEST_CASE("lossy: tuple int,char") { std::tuple<int, char> a; REQUIRE(parse("1,2", x4::int_ >> ',' >> x4::int_, a)); }

// --- rule: rule attribute to exposed attribute (lossy) ---
// TEST_CASE("lossy: int rule to char") { char a{}; REQUIRE(parse("42", int_rule, a)); }
// TEST_CASE("lossy: int rule to short") { short a{}; REQUIRE(parse("42", int_rule, a)); }
// TEST_CASE("lossy: long long rule to int") { int a{}; REQUIRE(parse("42", long_long_rule, a)); }
// TEST_CASE("lossy: double rule to float") { float a{}; REQUIRE(parse("3.14", double_rule, a)); }
// TEST_CASE("lossy: immediate rule lossy") { char a{}; REQUIRE(parse("42", x4::rule<struct imm_n_tag, int>{} = x4::int_, a)); }
