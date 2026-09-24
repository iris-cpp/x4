/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

// The attribute contract of `x4::parse`, checked at its public boundary.
// See also: <iris/x4/traits/attribute_traits.hpp>.
//
// 1. Whatever the root attribute held before the call has no influence on
//    the result; it is "prepared" for the parser's attribute type first.
//    Note: see the header above for the actual semantics on preparation.
//
// 2. On success, the value of the attribute is determined by the input and
//    the grammar alone.
//
// 3. On failure, the attribute is reset to its default state.
//
// 4. When an exception escapes the parse, the attribute is reset to its
//    default state if that reset cannot throw; otherwise it is left in a
//    valid but unspecified state.
//
// This rule does not apply to direct invocations of the `p.parse(...)` member
// function, which bypass `x4::parse`. After such a failed attempt, the exposed
// attribute is left in a valid but unspecified state.

#include "iris_x4_test.hpp"

#include <iris/x4/attribute/as.hpp>
#include <iris/x4/attribute/smart_ptr.hpp>
#include <iris/x4/attribute/value.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/primitive/eps.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/optional.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/delimited_list.hpp>

#include <iris/x4/traits/attribute_traits.hpp>

#include <iris/alloy/adapt.hpp>

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

struct Pair
{
    int a;
    std::string b;

    bool operator==(Pair const&) const = default;
};

struct Single
{
    int n;

    bool operator==(Single const&) const = default;
};

// A plain type whose reset can throw: assignment is not `noexcept`.
struct throwing_plain
{
    int n = 0;

    throwing_plain() = default;
    throwing_plain(int n_) : n(n_) {}
    throwing_plain(throwing_plain const&) = default;
    throwing_plain(throwing_plain&&) = default;
    throwing_plain& operator=(int n_) { n = n_; return *this; }
    throwing_plain& operator=(throwing_plain const&) = default;
    throwing_plain& operator=(throwing_plain&& other) noexcept(false) { n = other.n; return *this; }

    bool operator==(throwing_plain const&) const = default;
};

template<class T>
struct throwing_parser : x4::parser<throwing_parser<T>>
{
    using attribute_type = T;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, x4::X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    parse(It&, Se const&, Context const&, Attr& attr)
    {
        if (std::addressof(attr)) {
            throw std::runtime_error("throwing_parser");
        }
        return false;
    }
};

template<class T>
inline constexpr throwing_parser<T> always_throw{};

} // anonymous

IRIS_ALLOY_ADAPT_STRUCT(Pair, a, b);
IRIS_ALLOY_ADAPT_STRUCT(Single, n);

using namespace std::string_literals;
using namespace std::string_view_literals;

using x4::int_;
using x4::lit;
using x4::eps;
using x4::fixed_value;
using x4::standard::alpha;
using x4::standard::digit;

// Contract 1 + 2
#define X4_TEST_SUCCESS(poison, input, p, expected) \
    do { \
        auto attr = poison; \
        REQUIRE(parse(input, p, attr)); \
        CHECK(attr == expected); \
    } while (false)

// Contract 3
#define X4_TEST_FAILURE(poison, input, p) \
    do { \
        auto attr = poison; \
        using Attr = std::remove_cvref_t<decltype(attr)>; \
        REQUIRE(!parse(input, p, attr)); \
        CHECK(attr == Attr{}); \
    } while (false)

// Contract 4
#define X4_TEST_EXCEPTION(poison, p) \
    do { \
        auto attr = poison; \
        using Attr = std::remove_cvref_t<decltype(attr)>; \
        REQUIRE_THROWS_AS(parse("1", p, attr), std::runtime_error); \
        if constexpr (x4::traits::is_nothrow_resettable_v<Attr>) { \
            CHECK(attr == Attr{}); \
        } \
    } while (false)

TEST_CASE("attribute contract: prior content does not influence the result")
{
    X4_TEST_SUCCESS(999, "42", int_, 42);
    X4_TEST_SUCCESS(999, "42", int_ | fixed_value(7), 42);

    X4_TEST_SUCCESS("poison"s, "abc", +alpha, "abc"s);
    X4_TEST_SUCCESS(std::vector<int>({7, 8, 9}), "1,2,3,", *(int_ >> lit(',')), std::vector<int>({1, 2, 3}));

    using var_t = iris::rvariant<int, std::string>;
    X4_TEST_SUCCESS(var_t{"poison"s}, "42", int_ | +alpha, var_t{42});
    X4_TEST_SUCCESS(var_t{999}, "abc", int_ | +alpha, var_t{"abc"s});

    X4_TEST_SUCCESS(std::optional<int>{999}, "42", int_, std::optional<int>{42});
    X4_TEST_SUCCESS(std::optional<int>{999}, "42", -int_, std::optional<int>{42});

    X4_TEST_SUCCESS(Pair(999, "poison"s), "42abc", int_ >> +alpha, Pair(42, "abc"s));
    X4_TEST_SUCCESS(Single{999}, "42", int_, Single{42});

    {
        std::unique_ptr<int> attr = std::make_unique<int>(999);
        REQUIRE(parse("42", x4::unique_ptr<int>(int_), attr));
        REQUIRE(attr != nullptr);
        CHECK(*attr == 42);
    }
    {
        int attr = 999;
        REQUIRE(parse("x", lit("x"), attr));
        CHECK(attr == 0);
    }
    {
        using wide_t = iris::rvariant<int, std::string, double>;
        wide_t attr{3.5};
        REQUIRE(parse("", fixed_value(iris::rvariant<std::string, int>{42}), attr));
        CHECK(attr == wide_t{42});
    }
}

TEST_CASE("attribute contract: a failed parse resets the attribute")
{
    X4_TEST_FAILURE(999, "x", int_);
    X4_TEST_FAILURE(999, "x", int_ | fixed_value(7) >> eps(false));
    X4_TEST_FAILURE("poison"s, "123", +alpha);
    X4_TEST_FAILURE(std::vector<int>({7, 8, 9}), "x", +(int_ >> lit(',')));

    using var_t = iris::rvariant<int, std::string>;
    X4_TEST_FAILURE(var_t{"poison"s}, "-", int_ | +alpha);

    X4_TEST_FAILURE(std::optional<int>{999}, "x", int_);
    X4_TEST_FAILURE(Pair(999, "poison"s), "42123", int_ >> +alpha);
    X4_TEST_FAILURE(Single{999}, "x", int_);

    {
        std::unique_ptr<int> attr = std::make_unique<int>(999);
        REQUIRE(!parse("x", x4::unique_ptr<int>(int_), attr));
        CHECK(attr == nullptr);
    }

    X4_TEST_FAILURE(Pair(999, "poison"s), "42x", (int_ >> +alpha >> lit('!')) | (int_ >> +alpha >> eps(false)));
}

TEST_CASE("attribute contract: exception")
{
    STATIC_CHECK(x4::traits::is_nothrow_resettable_v<int>);
    STATIC_CHECK(x4::traits::is_nothrow_resettable_v<std::string>);
    STATIC_CHECK(x4::traits::is_nothrow_resettable_v<Pair>);
    STATIC_CHECK(!x4::traits::is_nothrow_resettable_v<throwing_plain>);

    X4_TEST_EXCEPTION(999, always_throw<int>);
    X4_TEST_EXCEPTION("poison"s, always_throw<std::string>);
    X4_TEST_EXCEPTION(std::vector<int>({7, 8, 9}), always_throw<std::vector<int>>);
    X4_TEST_EXCEPTION(std::optional<int>{999}, always_throw<int>);
    X4_TEST_EXCEPTION(Pair(999, "poison"s), always_throw<Pair>);

    using var_t = iris::rvariant<int, std::string>;
    X4_TEST_EXCEPTION(var_t{"poison"s}, always_throw<int>);

    X4_TEST_EXCEPTION(throwing_plain{999}, always_throw<throwing_plain>);
}

TEST_CASE("attribute contract: parser depending on the previous result of the subject")
{
    X4_TEST_SUCCESS("poison"s, "a1b", *(alpha >> digit) >> lit('b'), "a1"s);
    X4_TEST_SUCCESS("poison"s, "a1b", +(alpha >> digit) >> lit('b'), "a1"s);
    X4_TEST_SUCCESS("poison"s, "a1,b", ((alpha >> digit) % lit(',')) >> lit(",b"), "a1"s);

    X4_TEST_SUCCESS(Pair(999, "poison"s), "42x", -(int_ >> +alpha >> lit('!')) >> lit("42x"), Pair{});
    X4_TEST_SUCCESS(std::optional{Pair(999, "poison"s)}, "42x", -(int_ >> +alpha >> lit('!')) >> lit("42x"), std::optional<Pair>{});
}
