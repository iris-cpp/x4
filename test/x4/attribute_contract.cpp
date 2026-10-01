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

#include <iris/x4/rule.hpp>
#include <iris/x4/attribute/as.hpp>
#include <iris/x4/attribute/smart_ptr.hpp>
#include <iris/x4/attribute/value.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/numeric/real.hpp>
#include <iris/x4/primitive/eps.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/optional.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/delimited_list.hpp>
#include <iris/x4/operator/and_predicate.hpp>

#include <iris/x4/traits/attribute_traits.hpp>

#include <iris/alloy/adapt.hpp>

#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

#include <cctype>

namespace {

struct Pair
{
    int a;
    std::string b;

    bool operator==(Pair const&) const = default;
};

struct SingleElement
{
    int n;

    bool operator==(SingleElement const&) const = default;
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

// A container of letters, passed to `std::string` by its conversion
struct letters : std::vector<char>
{
    operator std::string() const { return {begin(), end()}; }
};

// A container of letters whose conversion to `std::string` turns them into upper case
struct shouted_letters : std::vector<char>
{
    operator std::string() const
    {
        std::string shouted(begin(), end());
        for (char& c : shouted) {
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
        return shouted;
    }
};

} // anonymous

IRIS_ALLOY_ADAPT_STRUCT(Pair, a, b);
IRIS_ALLOY_ADAPT_STRUCT(SingleElement, n);

// A rule whose value is assembled by an action, and a rule which fails after writing a part of its value
constexpr iris::x4::rule<struct assembled_word_id, std::string> assembled_word = "assembled_word";
constexpr iris::x4::rule<struct banged_word_id, std::string> banged_word = "banged_word";
constexpr iris::x4::rule<struct letters_word_id, letters> letters_word = "letters_word";
constexpr iris::x4::rule<struct shouted_word_id, shouted_letters> shouted_word = "shouted_word";

constexpr auto assembled_word_def = assembled_word = (+iris::x4::standard::alpha).on_match([](auto&& ctx) {
    iris::x4::_rule_var(ctx) = iris::x4::_attr(ctx);
});
constexpr auto banged_word_def = banged_word = +iris::x4::standard::alpha >> '!';
constexpr auto letters_word_def = letters_word = +iris::x4::standard::alpha;
constexpr auto shouted_word_def = shouted_word = +iris::x4::standard::alpha;

IRIS_X4_DEFINE(assembled_word)
IRIS_X4_DEFINE(banged_word)
IRIS_X4_DEFINE(letters_word)
IRIS_X4_DEFINE(shouted_word)

using namespace std::string_literals;
using namespace std::string_view_literals;

using x4::int_;
using x4::double_;
using x4::lit;
using x4::eps;
using x4::fixed_value;
using x4::standard::alpha;
using x4::standard::alnum;
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
    X4_TEST_SUCCESS(SingleElement{999}, "42", int_, SingleElement{42});

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
    {
        // prepared to the first alternative, not the alternative held before
        var_t attr{"poison"s};
        REQUIRE(parse("", x4::rule<struct no_write_rule, var_t>{} = eps, attr));
        CHECK(attr == var_t{});
    }
    {
        // a container is emptied by `clear()`, which keeps the allocated capacity (also on a failed parse)
        std::vector<int> attr({7, 8, 9});
        attr.reserve(100);
        auto const capacity = attr.capacity();
        REQUIRE(parse("1,2", int_ % lit(','), attr));
        CHECK(attr == std::vector<int>({1, 2}));
        CHECK(attr.capacity() == capacity);
        REQUIRE(!parse("1,2", (int_ % lit(',')) >> lit('!'), attr));
        CHECK(attr.empty());
        CHECK(attr.capacity() == capacity);
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
    X4_TEST_FAILURE(SingleElement{999}, "x", int_);

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

    // optional over a container
    X4_TEST_SUCCESS("poison"s, "abc", -(+alpha >> lit('!')) >> lit("abc"), ""s);
    X4_TEST_SUCCESS(std::vector<int>({7, 8, 9}), "1!", -(int_ >> lit('?')) >> lit("1!"), std::vector<int>{});

    X4_TEST_SUCCESS("poison"s, "ab1", +alpha >> -(digit >> lit('!')) >> lit('1'), "ab"s);
    X4_TEST_SUCCESS(std::vector<int>({7, 8, 9}), "1,2,3", +(int_ >> lit(',')) >> -(int_ >> lit('!')) >> lit('3'), std::vector<int>({1, 2}));

    X4_TEST_SUCCESS("poison"s, "ab12", +alpha >> -(+digit >> lit('!')) >> lit("12"), "ab"s);

    // Alternative entered through `parse_into_container`; the appends of a
    // failed branch must not survive into the next branch
    X4_TEST_SUCCESS("poison"s, "ab", *(alpha >> digit | alpha), "ab"s);
    X4_TEST_SUCCESS("poison"s, "ab", +(alpha >> digit | alpha), "ab"s);
    X4_TEST_SUCCESS("poison"s, "a,b", (alpha >> digit | alpha) % lit(','), "ab"s);
    X4_TEST_SUCCESS("poison"s, "xab", x4::as<std::string>(alpha >> (alpha >> digit | alpha)) >> lit('b'), "xa"s);
    X4_TEST_SUCCESS("poison"s, "ab?", banged_word | (+alpha >> lit('?')), "ab"s);
    X4_TEST_SUCCESS("poison"s, "ab?", x4::as<std::string>(+alpha >> lit('!')) | (+alpha >> lit('?')), "ab"s);
    X4_TEST_SUCCESS("poison"s, "ab?", -banged_word >> lit("ab?"), ""s);

    // Same as above, where the branch attribute is a variant and the element type is a wider variant
    {
        using expr_t = iris::rvariant<int, double>;
        using stmt_t = iris::rvariant<expr_t, std::string>;
        constexpr auto expr = x4::as<expr_t>(int_ | double_);
        constexpr auto stmt = expr >> &lit('!') | +alnum;
        X4_TEST_SUCCESS(std::vector<stmt_t>{}, "12ab", stmt % lit(','), std::vector<stmt_t>({stmt_t{"12ab"s}}));
        X4_TEST_SUCCESS(std::vector<stmt_t>{}, "12ab", *stmt, std::vector<stmt_t>({stmt_t{"12ab"s}}));
        X4_TEST_SUCCESS(std::vector<stmt_t>{}, "12ab,1!", (expr >> lit('!') | +alnum) % lit(','), std::vector<stmt_t>({stmt_t{"12ab"s}, stmt_t{expr_t{1}}}));
    }

    // A variant selects the same type over a single-element tuple-like of it, whatever the order
    {
        using single_element_or_plain = iris::rvariant<SingleElement, int>;
        using plain_or_single_element = iris::rvariant<int, SingleElement>;
        X4_TEST_SUCCESS(single_element_or_plain{}, "12", int_, single_element_or_plain{12});
        X4_TEST_SUCCESS(plain_or_single_element{}, "12", int_, plain_or_single_element{12});
        X4_TEST_SUCCESS(std::vector<single_element_or_plain>{}, "1,2", int_ % lit(','), std::vector<single_element_or_plain>({single_element_or_plain{1}, single_element_or_plain{2}}));
    }

    // A rule or `as<T>` assembles its own value, which is appended to the elements which were already there
    {
        constexpr auto assembled_as = x4::as<std::string>((+alpha).on_match([](auto&& ctx) {
            x4::_as_var(ctx) = x4::_attr(ctx);
        }));
        X4_TEST_SUCCESS("poison"s, "ab cd", assembled_word >> lit(' ') >> assembled_word, "abcd"s);
        X4_TEST_SUCCESS("poison"s, "ab cd", assembled_as >> lit(' ') >> assembled_as, "abcd"s);
    }

    // An action on `as<T>` passes the value of `as<T>` on, in a sequence into a container too
    X4_TEST_SUCCESS("poison"s, "<ab>", lit('<') >> x4::as<std::string>(+alpha).on_match([] {}) >> lit('>'), "ab"s);

    // A rule or `as<T>` of another type passes its value by the ordinary conversion; the result is
    // appended to the elements which were already there, the same as it is written into an empty one
    {
        constexpr auto letters_as = x4::as<letters>(+alpha);
        constexpr auto shouted_as = x4::as<shouted_letters>(+alpha);
        X4_TEST_SUCCESS("poison"s, "x-ab", alpha >> lit('-') >> letters_word, "xab"s);
        X4_TEST_SUCCESS("poison"s, "x-ab", alpha >> lit('-') >> letters_as, "xab"s);

        X4_TEST_SUCCESS("poison"s, "ab", shouted_word, "AB"s);
        X4_TEST_SUCCESS("poison"s, "-ab", lit('-') >> shouted_word, "AB"s);
        X4_TEST_SUCCESS("poison"s, "x-ab", alpha >> lit('-') >> shouted_word, "xAB"s);
        X4_TEST_SUCCESS("poison"s, "ab", shouted_as, "AB"s);
        X4_TEST_SUCCESS("poison"s, "-ab", lit('-') >> shouted_as, "AB"s);
        X4_TEST_SUCCESS("poison"s, "x-ab", alpha >> lit('-') >> shouted_as, "xAB"s);
    }

    // The successful branch / subject appends to the elements which were already there
    X4_TEST_SUCCESS("poison"s, "xab", alpha >> (alpha >> digit | alpha) >> lit('b'), "xa"s);
    X4_TEST_SUCCESS("poison"s, "xa1", alpha >> (alpha >> digit | alpha), "xa1"s);
    X4_TEST_SUCCESS("poison"s, "xa1", alpha >> -(alpha >> digit), "xa1"s);
    X4_TEST_SUCCESS("poison"s, "x", alpha >> -(alpha >> digit), "x"s);
    X4_TEST_SUCCESS(std::vector<int>({7, 8, 9}), "1,2!", +(int_ >> lit(',')) >> -(int_ >> lit('!')), std::vector<int>({1, 2}));
    X4_TEST_SUCCESS(std::vector<int>({7, 8, 9}), "1,2!", +(int_ >> lit(',')) >> (int_ >> lit('?') | int_ >> lit('!')), std::vector<int>({1, 2}));
}
