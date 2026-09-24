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
    parse(It&, Se const&, Context const&, Attr& attr_)
    {
        if (std::addressof(attr_)) {
            throw std::runtime_error("throwing_parser");
        }
        return false;
    }
};

template<class T>
inline constexpr throwing_parser<T> always_throw{};

// Contract 1 + 2
template<class Attr, class Parser>
void check_success(Attr poison, std::string_view input, Parser const& p, Attr const& expected)
{
    Attr attr = poison;
    REQUIRE(parse(input, p, attr));
    CHECK(attr == expected);
}

// Contract 3
template<class Attr, class Parser>
void check_failure(Attr poison, std::string_view input, Parser const& p)
{
    Attr attr = poison;
    REQUIRE(!parse(input, p, attr));
    CHECK(attr == Attr{});
}

// Contract 4
template<class Attr, class Parser>
void check_exception(Attr poison, Parser const& p)
{
    Attr attr = poison;
    REQUIRE_THROWS_AS(parse("1", p, attr), std::runtime_error);
    if constexpr (x4::traits::is_nothrow_resettable_v<Attr>) {
        CHECK(attr == Attr{});
    }
}

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

TEST_CASE("attribute contract: prior content does not influence the result")
{
    check_success<int>(999, "42", int_, 42);
    check_success<int>(999, "42", int_ | fixed_value(7), 42);

    check_success<std::string>("poison"s, "abc", +alpha, "abc"s);
    check_success<std::vector<int>>({7, 8, 9}, "1,2,3,", *(int_ >> lit(',')), std::vector<int>{1, 2, 3});

    using var_t = iris::rvariant<int, std::string>;
    check_success<var_t>(var_t{"poison"s}, "42", int_ | +alpha, var_t{42});
    check_success<var_t>(var_t{999}, "abc", int_ | +alpha, var_t{"abc"s});

    check_success<std::optional<int>>(std::optional<int>{999}, "42", int_, std::optional<int>{42});
    check_success<std::optional<int>>(std::optional<int>{999}, "42", -int_, std::optional<int>{42});

    check_success<Pair>(Pair{999, "poison"s}, "42abc", int_ >> +alpha, Pair{42, "abc"s});
    check_success<Single>(Single{999}, "42", int_, Single{42});

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
    check_failure<int>(999, "x", int_);
    check_failure<int>(999, "x", int_ | fixed_value(7) >> eps(false));
    check_failure<std::string>("poison"s, "123", +alpha);
    check_failure<std::vector<int>>({7, 8, 9}, "x", +(int_ >> lit(',')));

    using var_t = iris::rvariant<int, std::string>;
    check_failure<var_t>(var_t{"poison"s}, "-", int_ | +alpha);

    check_failure<std::optional<int>>(std::optional<int>{999}, "x", int_);
    check_failure<Pair>(Pair{999, "poison"s}, "42123", int_ >> +alpha);
    check_failure<Single>(Single{999}, "x", int_);

    {
        std::unique_ptr<int> attr = std::make_unique<int>(999);
        REQUIRE(!parse("x", x4::unique_ptr<int>(int_), attr));
        CHECK(attr == nullptr);
    }

    check_failure<Pair>(Pair{999, "poison"s}, "42x", (int_ >> +alpha >> lit('!')) | (int_ >> +alpha >> eps(false)));
}

TEST_CASE("attribute contract: exception")
{
    STATIC_CHECK(x4::traits::is_nothrow_resettable_v<int>);
    STATIC_CHECK(x4::traits::is_nothrow_resettable_v<std::string>);
    STATIC_CHECK(x4::traits::is_nothrow_resettable_v<Pair>);
    STATIC_CHECK(!x4::traits::is_nothrow_resettable_v<throwing_plain>);

    check_exception<int>(999, always_throw<int>);
    check_exception<std::string>("poison"s, always_throw<std::string>);
    check_exception<std::vector<int>>({7, 8, 9}, always_throw<std::vector<int>>);
    check_exception<std::optional<int>>(std::optional<int>{999}, always_throw<int>);
    check_exception<Pair>(Pair{999, "poison"s}, always_throw<Pair>);

    using var_t = iris::rvariant<int, std::string>;
    check_exception<var_t>(var_t{"poison"s}, always_throw<int>);

    check_exception<throwing_plain>(throwing_plain{999}, always_throw<throwing_plain>);
}
