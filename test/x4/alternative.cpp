/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>

#include <iris/x4/attribute/as.hpp>
#include <iris/x4/attribute/value.hpp>
#include <iris/x4/primitive/eps.hpp>

#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/numeric/bool.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/numeric/real.hpp>

#include <iris/x4/directive/lexeme.hpp>
#include <iris/x4/directive/omit.hpp>

#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/operator/difference.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/delimited_list.hpp>
#include <iris/x4/operator/optional.hpp>
#include <iris/x4/operator/not_predicate.hpp>

#include <iris/rvariant.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/tuple.hpp>

#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <type_traits>
#include <concepts>

struct di_ignore
{
    std::string text;
};

struct di_include
{
    std::string FileName;
};

template<>
struct alloy::adaptor<di_ignore>
{
    using getters_list = iris::constant_list<&di_ignore::text>;
};

template<>
struct alloy::adaptor<di_include>
{
    using getters_list = iris::constant_list<&di_include::FileName>;
};

struct undefined
{
    bool operator==(undefined const&) const = default;
};

struct Object
{
    std::string name;
    bool operator==(Object const&) const = default;
};

namespace declared_variant {

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

struct Call
{
    Ident callee;
    std::vector<Primary> args;
};

using Scalar = iris::rvariant<long long, double>;

} // declared_variant

IRIS_ALLOY_ADAPT_STRUCT(declared_variant::NumberLit, value);
IRIS_ALLOY_ADAPT_STRUCT(declared_variant::StringLit, value);
IRIS_ALLOY_ADAPT_STRUCT(declared_variant::Ident, name);
IRIS_ALLOY_ADAPT_STRUCT(declared_variant::Call, callee, args);

namespace declared_variant {

template<class Parser>
using attribute_t = x4::parser_traits<std::remove_cvref_t<Parser>>::attribute_type;

template<class Parser>
using candidates_t = x4::detail::attribute_candidates_t<std::remove_cvref_t<Parser>>;

constexpr auto quoted = x4::lexeme['"' >> *~x4::standard::char_('"') >> '"'];
constexpr auto number_lit = x4::as<NumberLit>(x4::long_long);
constexpr auto string_lit = x4::as<StringLit>(quoted);
constexpr auto ident = x4::as<Ident>(x4::lexeme[x4::as<std::string>(x4::standard::alpha >> *x4::standard::alnum)]);

constexpr x4::rule<struct literal_rule_id, Literal> literal_rule = "literal_rule";
constexpr auto literal_rule_def = literal_rule = number_lit | string_lit;
IRIS_X4_DEFINE(literal_rule)

constexpr auto literal_as = x4::as<Literal>(number_lit | string_lit);

constexpr auto integer = x4::lexeme[x4::long_long >> !x4::lit('.')];

constexpr x4::rule<struct scalar_rule_id, Scalar> scalar_rule = "scalar_rule";
constexpr auto scalar_rule_def = scalar_rule = integer | x4::double_;
IRIS_X4_DEFINE(scalar_rule)

constexpr auto scalar_as = x4::as<Scalar>(integer | x4::double_);

} // declared_variant

TEST_CASE("alternative")
{
    using x4::standard::char_;
    using x4::lit;
    using x4::fixed_value;
    using x4::int_;
    using x4::double_;
    using x4::unused;
    using x4::omit;
    using x4::eps;
    using x4::true_;
    using iris::rvariant;

    STATIC_CHECK(std::same_as<decltype(int_ | int_), x4::alternative<x4::int_parser<int>, x4::int_parser<int>>>);
    STATIC_CHECK(std::same_as<decltype(int_ | int_ | int_), x4::alternative<x4::int_parser<int>, x4::int_parser<int>, x4::int_parser<int>>>);
    STATIC_CHECK(std::same_as<decltype((int_ | int_) | int_), decltype(int_ | (int_ | int_))>);

    STATIC_CHECK(std::same_as<x4::parser_traits<decltype(int_ | int_)>::attribute_type, int>);
    STATIC_CHECK(std::same_as<x4::parser_traits<decltype((int_ | int_) | int_)>::attribute_type, int>);
    STATIC_CHECK(std::same_as<x4::parser_traits<decltype(int_ | (int_ | int_))>::attribute_type, int>);

    STATIC_CHECK(std::same_as<x4::parser_traits<decltype(int_ | double_)>::attribute_type, rvariant<int, double>>);
    STATIC_CHECK(std::same_as<x4::parser_traits<decltype(int_ | double_ | int_)>::attribute_type, rvariant<int, double>>);
    STATIC_CHECK(std::same_as<x4::parser_traits<decltype(int_ | double_ | double_)>::attribute_type, rvariant<int, double>>);
    STATIC_CHECK(std::same_as<x4::parser_traits<decltype(int_ | double_ | (int_ | double_))>::attribute_type, rvariant<int, double>>);
    STATIC_CHECK(std::same_as<x4::parser_traits<decltype(int_ | double_ | (double_ | int_))>::attribute_type, rvariant<int, double>>);

    {
        // `T` and `recursive_wrapper<T>` are one alternative; the wrapped form is kept, at the first position
        using iris::recursive_wrapper;
        using x4::as;
        using x4::standard::alpha;
        constexpr auto plain = as<di_include>(+alpha);
        constexpr auto wrapped = as<recursive_wrapper<di_include>>(lit('#') >> +alpha);

        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(plain | wrapped)>::attribute_type, recursive_wrapper<di_include>>);
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(wrapped | plain)>::attribute_type, recursive_wrapper<di_include>>);
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(plain | int_ | wrapped)>::attribute_type, rvariant<recursive_wrapper<di_include>, int>>);
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(int_ | wrapped | plain)>::attribute_type, rvariant<int, recursive_wrapper<di_include>>>);

        recursive_wrapper<di_include> w;
        REQUIRE(parse("abc", plain | wrapped, w));
        CHECK(w->FileName == "abc");
        REQUIRE(parse("#xyz", plain | wrapped, w));
        CHECK(w->FileName == "xyz");

        rvariant<recursive_wrapper<di_include>, int> v;
        REQUIRE(parse("#xyz", plain | int_ | wrapped, v));
        CHECK(iris::get<di_include>(v).FileName == "xyz");
        REQUIRE(parse("42", plain | int_ | wrapped, v));
        CHECK(iris::get<int>(v) == 42);
        REQUIRE(parse("abc", plain | int_ | wrapped, v));
        CHECK(iris::get<di_include>(v).FileName == "abc");
    }

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(char_ | char_);

    {
        CHECK(parse("a", char_ | char_));
        CHECK(parse("x", lit('x') | lit('i')));
        CHECK(parse("i", lit('x') | lit('i')));
        CHECK(!parse("z", lit('x') | lit('o')));
        CHECK(parse("rock", lit("rock") | lit("roll")));
        CHECK(parse("roll", lit("rock") | lit("roll")));
        CHECK(parse("rock", lit("rock") | int_));
        CHECK(parse("12345", lit("rock") | int_));
    }

    {
        using attr_type = iris::rvariant<undefined, int, char>;
        {
            attr_type v;
            REQUIRE(parse("12345", int_ | char_, v));
            CHECK(iris::get<int>(v) == 12345);
        }
        {
            attr_type v;
            REQUIRE(parse("12345", lit("rock") | int_ | char_, v));
            CHECK(iris::get<int>(v) == 12345);
        }
        {
            attr_type v;
            REQUIRE(parse("x", lit("rock") | int_ | char_, v));
            CHECK(iris::get<char>(v) == 'x');
        }
    }

    {
        // Make sure that we are using the actual supplied attribute types
        // from the variant and not the expected type.
        using attr_type = iris::rvariant<int, std::string>;
        {
            attr_type v;
            REQUIRE(parse("12345", int_ | +char_, v));
            CHECK(iris::get<int>(v) == 12345);
        }
        {
            attr_type v;
            REQUIRE(parse("abc", int_ | +char_, v));
            CHECK(iris::get<std::string>(v) == "abc");
        }
        {
            attr_type v;
            REQUIRE(parse("12345", +char_ | int_, v));
            CHECK(iris::get<std::string>(v) == "12345");
        }
    }

    {
        unused_type x;
        CHECK(parse("rock", lit("rock") | lit('x'), x));
    }

    {
        // test if alternatives with all components having unused
        // attributes have an unused attribute

        alloy::tuple<char, char> v;
        REQUIRE((parse("abc", char_ >> (omit[char_] | omit[char_]) >> char_, v)));
        CHECK((alloy::get<0>(v) == 'a'));
        CHECK((alloy::get<1>(v) == 'c'));
    }

    {
        // Test that we can still pass a "compatible" attribute to
        // an alternate even if its "expected" attribute is unused type.

        std::string s;
        REQUIRE(parse("...", *(char_('.') | char_(',')), s));
        CHECK(s == "...");
    }

    {   // make sure collapsing eps works as expected
        // (compile check only)

        using x4::rule;
        using x4::_attr;
        using x4::_rule_var;

        rule<class r1, wchar_t> r1;
        rule<class r2, wchar_t> r2;
        rule<class r3, wchar_t> r3;

        constexpr auto f = [&](auto& ctx){ _rule_var(ctx) = _attr(ctx); };

        (void)(r3 = (eps >> r1).on_match(f));
        (void)(r3 = (r1 | r2).on_match(f));
        (void)(r3 = eps >> r1 | r2);
        (void)r3;
    }

    {
        // test having a variant<container, ...>
        std::string s;
        REQUIRE(parse("a,b", char_ % ',' | eps, s));
        CHECK(s == "ab");
    }

    {
        {
            // testing a sequence taking a container as attribute
            std::string s;
            REQUIRE(parse("abc,a,b,c", char_ >> char_ >> (char_ % ','), s));
            CHECK(s == "abcabc");
        }
        {
            // test having an optional<container> inside a sequence
            std::string s;
            REQUIRE(parse("ab", char_ >> char_ >> -(char_ % ','), s));
            CHECK(s == "ab");
        }
        {
            // test having a variant<container, ...> inside a sequence
            std::string s;
            CHECK(parse("ab", char_ >> char_ >> ((char_ % ',') | eps), s));
            CHECK(s == "ab");
        }
        {
            std::string s;
            CHECK(parse("abc", char_ >> char_ >> ((char_ % ',') | eps), s));
            CHECK(s == "abc");
        }
    }

    {
        //compile test only (bug_march_10_2011_8_35_am)
        using value_type = iris::rvariant<double, std::string>;

        using x4::rule;

        rule<class r1, value_type> r1;
        [[maybe_unused]] auto r1_ = r1 = r1 | eps; // left recursive!
    }

    {
        using x4::rule;
        using d_line = iris::rvariant<di_ignore, di_include>;

        rule<class ignore, di_ignore> ignore;
        rule<class include, di_include> include;
        rule<class line, d_line> line;

        [[maybe_unused]] auto start = line = include | ignore;
        (void)line;
    }

    // attribute is a variant containing container
    {
        constexpr auto parser = +true_;
        using Parser = std::remove_const_t<decltype(parser)>;
        using Attr = iris::rvariant<int, std::vector<bool>>;

        using attribute_type = x4::parser_traits<Parser>::attribute_type;
        STATIC_CHECK(std::same_as<attribute_type, std::vector<bool>>);

        STATIC_CHECK(x4::detail::variant_alternative_for_v<Attr, attribute_type> == 1);

        Attr var;
        REQUIRE(parse("truetrue", parser, var));
    }
    {
        constexpr auto parser = +char_;
        using Parser = std::remove_const_t<decltype(parser)>;
        using Attr = iris::rvariant<int, std::string>;

        using attribute_type = x4::parser_traits<Parser>::attribute_type;
        STATIC_CHECK(std::same_as<attribute_type, std::string>);

        STATIC_CHECK(x4::detail::variant_alternative_for_v<Attr, attribute_type> == 1);

        Attr var;
        REQUIRE(parse("123", parser, var));
    }

    // single-element tuple-like case
    {
        alloy::tuple<iris::rvariant<int, std::string>> fv;
        REQUIRE(parse("12345", int_ | +char_, fv));
        CHECK(iris::get<int>(alloy::get<0>(fv)) == 12345);
    }
    {
        alloy::tuple<iris::rvariant<int, std::string>> fvi;
        REQUIRE(parse("12345", int_ | int_, fvi));
        CHECK(iris::get<int>(alloy::get<0>(fvi)) == 12345);
    }

    // alternative over a single-element tuple-like as part of another tuple
    {
        constexpr auto key1 = lit("long") >> fixed_value(long{});
        constexpr auto key2 = lit("char") >> fixed_value(char{});
        constexpr auto keys = key1 | key2;
        constexpr auto pair = keys >> lit("=") >> +char_;

        alloy::tuple<iris::rvariant<long, char>, std::string> attr_;

        REQUIRE(parse("long=ABC", pair, attr_));
        CHECK(iris::get_if<long>(&alloy::get<0>(attr_)) != nullptr);
        CHECK(iris::get_if<char>(&alloy::get<0>(attr_)) == nullptr);
    }

    {
        // ensure no unneeded synthesization, copying and moving occurred
        constexpr auto p = '{' >> int_ >> '}';

        x4_test::stationary st {0};
        REQUIRE(parse("{42}", p | eps | p, st));
        CHECK(st.val == 42);
    }

    {
        // regressing test for #603
        struct X {};
        std::vector<iris::rvariant<std::string, int, X>> v;
        REQUIRE(parse("xx42x9y", *(int_ | +char_('x') | 'y' >> fixed_value(X{})), v));
        CHECK(v.size() == 5);
    }

    {
        // sequence parser in alternative into container
        std::string s;
        REQUIRE(parse("abcbbcd", *(char_('a') >> *(*char_('b') >> char_('c')) | char_('d')), s));
        CHECK(s == "abcbbcd");
    }

    {
        // conversion between alternatives
        struct X {};
        struct Y {};
        struct Z {};
        iris::rvariant<X, Y, Z> v;
        iris::rvariant<Y, X> x{X{}};
        v = x; // iris::rvariant supports that convertion
        auto const p = 'x' >> fixed_value(x) | 'z' >> fixed_value(Z{});
        REQUIRE(parse("z", p, v));
        CHECK(iris::get_if<Z>(&v) != nullptr);
        REQUIRE(parse("x", p, v));
        CHECK(iris::get_if<X>(&v) != nullptr);
    }

    {
        // regression test for #679
        using Qaz = std::vector<iris::rvariant<int>>;
        using Foo = std::vector<iris::rvariant<Qaz, int>>;
        using Bar = std::vector<iris::rvariant<Foo, int>>;
        Bar x;
        CHECK(parse("abaabb", +('a' >> fixed_value(Foo{}) | 'b' >> fixed_value(int{})), x));
    }

    {
        constexpr auto op = x4::as<std::string>(lit("==") | lit('<'));
        std::string s;
        REQUIRE(parse("==", op, s));
        CHECK(s.empty());
        REQUIRE(parse("<", op, s));
        CHECK(s.empty());
    }
    {
        constexpr auto op = x4::as<std::string>(x4::string("==") | x4::string("<"));
        std::string s;
        REQUIRE(parse("==", op, s));
        CHECK(s == "==");
        REQUIRE(parse("<", op, s));
        CHECK(s == "<");
    }
    {
        // a failed branch is undone
        std::string s;
        REQUIRE(parse("ay", (char_ >> 'x') | (char_ >> 'y'), s));
        CHECK(s == "a");
    }
}

TEST_CASE("alternative of same attributes (a | a)")
{
    using x4::int_;
    using x4::bool_;

    // The subject is `int_ >> bool_` (non-container alternative)
    {
        constexpr auto int_bool = int_ >> bool_ | int_ >> bool_;

        using Parser = std::remove_const_t<decltype(int_bool)>;
        STATIC_CHECK(std::same_as<
            x4::parser_traits<Parser>::attribute_type,
            alloy::tuple<int, bool>
        >);
        STATIC_CHECK(x4::parser_traits<Parser>::sequence_size == 2);

        alloy::tuple<int, bool> var;
        auto const res = parse("42true", int_bool, var);
        REQUIRE(res.completed());
        CHECK(var == decltype(var){42, true});
    }
    {
        constexpr auto int_bool = int_ >> bool_ | int_ >> bool_;
        constexpr auto foo_int_bool = x4::string("foo") >> int_bool;

        using Parser = std::remove_const_t<decltype(foo_int_bool)>;
        STATIC_CHECK(std::same_as<
            x4::parser_traits<Parser>::attribute_type,
            alloy::tuple<std::string, int, bool>
        >);
        STATIC_CHECK(x4::parser_traits<Parser>::sequence_size == 3);

        alloy::tuple<std::string, int, bool> var;
        auto const res = parse("foo42true", foo_int_bool, var);
        REQUIRE(res.completed());
        CHECK(var == decltype(var){"foo", 42, true});
    }

    // The subject is `+bool_` (container alternative)
    {
        constexpr auto bools = +bool_ | +bool_;

        using Parser = std::remove_const_t<decltype(bools)>;
        STATIC_CHECK(std::same_as<
            x4::parser_traits<Parser>::attribute_type,
            std::vector<bool>
        >);
        STATIC_CHECK(x4::parser_traits<Parser>::sequence_size == 1);

        std::vector<bool> var;
        auto const res = parse("truefalse", bools, var);
        REQUIRE(res.completed());
        CHECK(var == decltype(var){true, false});
    }
    {
        constexpr auto bools = +bool_ | +bool_;
        constexpr auto foo_bools = x4::string("foo") >> bools;

        using Parser = std::remove_const_t<decltype(foo_bools)>;
        STATIC_CHECK(std::same_as<
            x4::parser_traits<Parser>::attribute_type,
            alloy::tuple<std::string, std::vector<bool>>
        >);
        STATIC_CHECK(x4::parser_traits<Parser>::sequence_size == 2);

        alloy::tuple<std::string, std::vector<bool>> var;
        auto const res = parse("footruefalse", foo_bools, var);
        REQUIRE(res.completed());
        CHECK(var == decltype(var){"foo", {true, false}});
    }
}

namespace {

// The reference for `x4::alternative` without the attribute shared between the branches, for the
// branches with an attribute:
// each branch parses into its own attribute, made fresh by `x4::parse`, and the first match
// is the result. On failure, the result is the default state, as `x4::parse` leaves it.
template<class Attr>
struct separate_branches_result
{
    bool ok = false;
    std::size_t rest = 0;
    Attr attr{};
};

template<class Attr, class... Branches>
separate_branches_result<Attr> parse_separate_branches(std::string_view input, Branches const&... branches)
{
    // A branch without an attribute leaves the default of the attribute of the alternative, not
    // of its own; see "attributeless branch leaves the default"
    static_assert((x4::has_attribute_v<Branches> && ...));

    separate_branches_result<Attr> result;
    auto parse_branch = [&](auto const& branch) {
        Attr attr{};
        auto const res = x4::parse(input, branch, attr);
        if (!res.ok) return false;
        result = {.ok = true, .rest = res.remainder.size(), .attr = std::move(attr)};
        return true;
    };
    (void)(parse_branch(branches) || ...);
    return result;
}

template<class Attr, class Parser, class... Branches>
void check_separate_branches(std::string_view input, Parser const& parser, Branches const&... branches)
{
    CAPTURE(input);
    auto const expected = parse_separate_branches<Attr>(input, branches...);

    Attr attr{};
    auto const res = x4::parse(input, parser, attr);
    REQUIRE(res.ok == expected.ok);
    if (res.ok) {
        CHECK(res.remainder.size() == expected.rest);
    }
    CHECK(attr == expected.attr);
}

} // anonymous

TEST_CASE("alternative attribute reuse")
{
    using x4::standard::alpha;
    using x4::standard::digit;
    using x4::lit;
    using x4::int_;
    using x4::eps;
    using iris::rvariant;

    // The later branch writes into what the failed branch wrote
    {
        constexpr auto excl = +alpha >> '!';
        constexpr auto quest = +alpha >> '?';
        for (std::string_view input : {"ab?", "ab!", "ab."}) {
            check_separate_branches<rvariant<int, std::string>>(input, excl | quest | int_, excl, quest, int_);
            check_separate_branches<std::string>(input, excl | quest, excl, quest);
            check_separate_branches<alloy::tuple<int, rvariant<int, std::string>>>(
                std::string{"1,"} + std::string{input},
                int_ >> ',' >> (excl | quest),
                int_ >> ',' >> excl, int_ >> ',' >> quest
            );
        }
    }
    {
        constexpr auto excl = int_ >> ',' >> int_ >> '!';
        constexpr auto quest = int_ >> ',' >> int_ >> '?';
        check_separate_branches<alloy::tuple<int, int>>("1,2?", excl | quest, excl, quest);
        check_separate_branches<alloy::tuple<int, int>>("1,2.", excl | quest, excl, quest);
    }
    {
        constexpr auto excl = (int_ % ',') >> '!';
        constexpr auto quest = (int_ % ',') >> '?';
        check_separate_branches<std::vector<int>>("1,2?", excl | quest, excl, quest);
        // into the held alternative, converted from another type
        check_separate_branches<rvariant<std::vector<int>, std::string>>("1,2?", excl | -quest, excl, -quest);
    }
    {
        // a disengaged optional, converted into the attribute
        constexpr auto int_x = int_ >> 'x';
        check_separate_branches<int>("5y", int_x | -(int_ >> 'z'), int_x, -(int_ >> 'z'));
    }
    {
        // into a container which holds the preceding elements
        constexpr auto excl = +digit >> '!';
        constexpr auto quest = +digit >> '?';
        check_separate_branches<std::string>("ab12?", +alpha >> (excl | quest), +alpha >> excl, +alpha >> quest);
    }

    // The later branch writes another alternative
    {
        constexpr auto int_x = int_ >> 'x';
        constexpr auto one_alpha = lit('1') >> +alpha;
        check_separate_branches<rvariant<int, std::string>>("1ab", int_x | one_alpha, int_x, one_alpha);

        constexpr auto alpha_excl = +alpha >> '!';
        constexpr auto a_int = lit('a') >> int_;
        check_separate_branches<rvariant<int, std::string>>("a1", alpha_excl | a_int, alpha_excl, a_int);
    }

}

template<class Attr, class Parser>
void check_root_and_slot(std::string_view input, Parser const& parser, Attr const& expected)
{
    CAPTURE(input);
    {
        Attr attr{};
        REQUIRE(x4::parse(input, parser, attr).ok);
        CHECK(attr == expected);
    }
    {
        alloy::tuple<int, Attr> attr{};
        std::string const slot_input = "1," + std::string{input};
        REQUIRE(x4::parse(slot_input, x4::int_ >> ',' >> parser, attr).ok);
        CHECK(alloy::get<1>(attr) == expected);
    }
}

TEST_CASE("attributeless branch leaves the default")
{
    using x4::standard::alpha;
    using x4::standard::digit;
    using x4::standard::char_;
    using x4::lit;
    using x4::int_;
    using x4::eps;
    using x4::lexeme;
    using x4::fixed_value;
    using x4::default_value;
    using iris::rvariant;

    // The attribute is left in its default state, not in the part written by the other branches
    // (nor by the failed branch)
    constexpr auto five = (int_ >> 'x') | (lit('5') >> 'y');
    check_root_and_slot("5y", five, 0);
    check_root_and_slot("5y", five, std::optional<int>{});
    check_root_and_slot("5y", five, rvariant<std::string, int>{});
    check_root_and_slot("5y", lexeme[five], rvariant<std::string, int>{});
    check_root_and_slot("<5y>", lit('<') >> five >> '>', rvariant<std::string, int>{});
    check_root_and_slot("5y", lexeme[five] | +alpha, rvariant<std::string, int>{});
    check_root_and_slot("5y", five - lit('q'), std::optional<int>{});
    check_root_and_slot("5y", five - lit('q'), rvariant<std::string, int>{});

    // Nor in the parts or the elements written by the failed branch
    check_root_and_slot("1,2?", (int_ >> ',' >> int_ >> '!') | lit("1,2?"), alloy::tuple<int, int>{});
    check_root_and_slot("ab?", (+alpha >> '!') | eps, std::string{});

    using undefined_int_char = rvariant<undefined, int, char>;
    check_root_and_slot("rock", lit("rock") | int_ | char_, undefined_int_char{});
    check_root_and_slot("rock", lit("rock") | int_, undefined_int_char{});
    check_root_and_slot("12", lit("rock") | int_, undefined_int_char{12});

    constexpr auto timeout = (int_ >> lit("ms")) | lit("auto");
    constexpr auto timeout_fixed = (int_ >> lit("ms")) | (lit("auto") >> fixed_value(std::optional<int>{}));
    constexpr auto timeout_default = (int_ >> lit("ms")) | (lit("auto") >> default_value<std::optional<int>>);
    check_root_and_slot("auto", timeout, std::optional<int>{});
    check_root_and_slot("auto", timeout_fixed, std::optional<int>{});
    check_root_and_slot("auto", timeout_default, std::optional<int>{});
    check_root_and_slot("10ms", timeout, std::optional<int>{10});
    check_root_and_slot("10ms", timeout_fixed, std::optional<int>{10});
    check_root_and_slot("10ms", timeout_default, std::optional<int>{10});
    {
        std::optional<int> attr;
        CHECK_FALSE(parse("", timeout, attr));
    }

    // `-p` keeps the attribute of `p` when it succeeds
    check_root_and_slot("250ms", -timeout, std::optional<int>{250});
    check_root_and_slot("0ms", -timeout, std::optional<int>{0});
    check_root_and_slot("auto", -timeout, std::optional<int>{});
    check_root_and_slot("", -timeout, std::optional<int>{});
    check_root_and_slot("7", -int_, std::optional<int>{7});
    check_root_and_slot("x", -int_, std::optional<int>{});

    // An empty array is written only when the grammar says so
    using Array = std::vector<int>;
    constexpr auto array_parser = (lit('[') >> (int_ % ',') >> ']') | lit("[]");
    constexpr auto explicit_array_parser = (lit('[') >> (int_ % ',') >> ']') | (lit("[]") >> default_value<Array>);
    check_root_and_slot("[]", array_parser, Array{});
    check_root_and_slot("[]", array_parser, std::optional<Array>{});
    check_root_and_slot("[]", array_parser, rvariant<Object, Array>{});
    check_root_and_slot("[]", explicit_array_parser, Array{});
    check_root_and_slot("[]", explicit_array_parser, std::optional<Array>{Array{}});
    check_root_and_slot("[]", explicit_array_parser, rvariant<Object, Array>{Array{}});
    check_root_and_slot("[1,2]", array_parser, Array{1, 2});
    check_root_and_slot("[1,2]", array_parser, std::optional<Array>{Array{1, 2}});
    check_root_and_slot("[1,2]", array_parser, rvariant<Object, Array>{Array{1, 2}});
    check_root_and_slot("[1,2]", explicit_array_parser, Array{1, 2});
    check_root_and_slot("[1,2]", explicit_array_parser, std::optional<Array>{Array{1, 2}});
    check_root_and_slot("[1,2]", explicit_array_parser, rvariant<Object, Array>{Array{1, 2}});

    // Into a container, a branch without an attribute appends nothing
    {
        std::vector<Array> arrays;
        REQUIRE(parse("[];[1,2];[]", explicit_array_parser % ';', arrays));
        CHECK(arrays == std::vector<Array>{{}, {1, 2}, {}});
    }
    {
        std::vector<Array> arrays;
        REQUIRE(parse("[];[1,2];[]", lexeme[explicit_array_parser] % ';', arrays));
        CHECK(arrays == std::vector<Array>{{}, {1, 2}, {}});
    }
    {
        std::vector<Array> arrays;
        REQUIRE(parse("<[]>;<[1,2]>;<[]>", (lit('<') >> explicit_array_parser >> '>') % ';', arrays));
        CHECK(arrays == std::vector<Array>{{}, {1, 2}, {}});
    }
    {
        // wrapped or not
        std::vector<int> ints;
        REQUIRE(parse("5y", *five, ints));
        CHECK(ints.empty());
        REQUIRE(parse("5y", *lexeme[five], ints));
        CHECK(ints.empty());
        REQUIRE(parse("<5y>", *(lit('<') >> five >> '>'), ints));
        CHECK(ints.empty());
        REQUIRE(parse("7x5y8x", *lexeme[five], ints));
        CHECK(ints == std::vector<int>{7, 8});
    }
    {
        // the elements of the failed repetition are not kept, and the preceding ones are
        std::vector<int> ints;
        auto const res = parse("<7x><5y><8x", *(lit('<') >> five >> '>'), ints);
        REQUIRE(res.ok);
        CHECK(res.remainder.size() == 3);
        CHECK(ints == std::vector<int>{7});
    }
    {
        std::vector<int> ints;
        REQUIRE(parse("1 2 3 - 5 - - 7 -", (int_ | '-') % ' ', ints));
        CHECK(ints == std::vector<int>{1, 2, 3, 5, 7});
    }
    {
        std::string str;
        REQUIRE(parse("xab", +(lit('x') | char_), str));
        CHECK(str == "ab");
    }
    {
        // the preceding elements are kept, and those of the failed branch are not
        std::string str;
        auto const res = parse("ab12?", +alpha >> ((+digit >> '!') | eps), str);
        REQUIRE(res.ok);
        CHECK(res.remainder.size() == 3);
        CHECK(str == "ab");
    }
}

TEST_CASE("declared variant")
{
    using namespace declared_variant;
    using x4::int_;
    using x4::double_;
    using x4::lit;
    using x4::lexeme;
    using x4::standard::alpha;
    using iris::rvariant;
    using iris::type_list;

    using Value = rvariant<int, std::string>;
    constexpr auto int_or_alpha = int_ | +alpha;
    constexpr auto hashed_double = '#' >> double_;
    constexpr x4::rule<struct value_rule_id, Value> value_rule = "value_rule";

    STATIC_CHECK(std::same_as<attribute_t<decltype(int_or_alpha)>, Value>);
    STATIC_CHECK(std::same_as<candidates_t<decltype(int_or_alpha)>, type_list<int, std::string>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype(x4::as<Value>(int_or_alpha) | lit("null"))>, Value>);
    STATIC_CHECK(std::same_as<candidates_t<decltype(x4::as<Value>(int_or_alpha) | lit("null"))>, type_list<Value>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype(x4::as<Value>(int_or_alpha) | hashed_double)>, rvariant<Value, double>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype((x4::as<Value>(int_or_alpha) | lit("null")) | hashed_double)>, rvariant<Value, double>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype(value_rule | lit("null"))>, Value>);
    STATIC_CHECK(std::same_as<attribute_t<decltype(value_rule | hashed_double)>, rvariant<Value, double>>);
    STATIC_CHECK(std::same_as<candidates_t<decltype(value_rule = int_or_alpha)>, type_list<Value>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype(x4::fixed_value(rvariant<int, double>{1}) | +alpha)>, rvariant<rvariant<int, double>, std::string>>);

    STATIC_CHECK(std::same_as<candidates_t<decltype(lexeme[x4::as<Value>(int_or_alpha) | lit("null")])>, type_list<Value>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype(lexeme[x4::as<Value>(int_or_alpha) | lit("null")] | hashed_double)>, rvariant<Value, double>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype(lexeme[value_rule | lit("null")] | hashed_double)>, rvariant<Value, double>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype(lexeme[int_or_alpha] | hashed_double)>, rvariant<int, std::string, double>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype(('<' >> (x4::as<Value>(int_or_alpha) | lit("null")) >> '>') | hashed_double)>, rvariant<Value, double>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype(('<' >> int_or_alpha >> '>') | hashed_double)>, rvariant<int, std::string, double>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype((x4::as<Value>(int_or_alpha) - lit("x")) | hashed_double)>, rvariant<Value, double>>);
    STATIC_CHECK(std::same_as<attribute_t<decltype((int_or_alpha - lit("x")) | hashed_double)>, rvariant<int, std::string, double>>);

    STATIC_CHECK(std::same_as<candidates_t<decltype(int_ >> +alpha)>, type_list<alloy::tuple<int, std::string>>>);

    Primary const number_literal{Literal{NumberLit{1}}};
    Primary const string_literal{Literal{StringLit{"s"}}};
    Primary const identifier{Ident{"y"}};

    auto const check_primary = [&](auto const& literal) {
        auto const primary = literal | ident;
        STATIC_CHECK(std::same_as<attribute_t<decltype(primary)>, Primary>);

        {
            Primary attr;
            REQUIRE(parse("1", primary, attr));
            CHECK(attr == number_literal);
            REQUIRE(parse("\"s\"", primary, attr));
            CHECK(attr == string_literal);
            REQUIRE(parse("y", primary, attr));
            CHECK(attr == identifier);
        }
        {
            std::vector<Primary> attr;
            REQUIRE(parse("1,\"s\",y", primary % ',', attr));
            CHECK(attr == std::vector<Primary>{number_literal, string_literal, identifier});
        }
        {
            std::vector<Primary> attr;
            REQUIRE(parse("1,\"s\",y", -(primary % ','), attr));
            CHECK(attr == std::vector<Primary>{number_literal, string_literal, identifier});
        }
        {
            std::vector<Primary> attr;
            REQUIRE(parse("(1,\"s\",y)", '(' >> (primary % ',') >> ')', attr));
            CHECK(attr == std::vector<Primary>{number_literal, string_literal, identifier});
        }
        {
            Call attr;
            REQUIRE(parse("f(1,\"s\",y)", ident >> '(' >> -(primary % ',') >> ')', attr));
            CHECK(attr.callee == Ident{"f"});
            CHECK(attr.args == std::vector<Primary>{number_literal, string_literal, identifier});
        }
        {
            auto const parenthesized = primary | ('(' >> primary >> ')');
            Primary attr;
            REQUIRE(parse("(\"s\")", parenthesized, attr));
            CHECK(attr == string_literal);
            REQUIRE(parse("(y)", parenthesized, attr));
            CHECK(attr == identifier);
            REQUIRE(parse("\"s\"", parenthesized, attr));
            CHECK(attr == string_literal);
        }
    };
    check_primary(literal_rule);
    check_primary(literal_as);

    using Nested = rvariant<Scalar, std::string>;
    using Flat = rvariant<long long, double, std::string>;

    auto const check_setting = [](auto const& number) {
        auto const setting = number | quoted;
        STATIC_CHECK(std::same_as<attribute_t<decltype(setting)>, Nested>);

        Nested nested;
        REQUIRE(parse("42", setting, nested));
        CHECK(nested == Nested{Scalar{42LL}});
        REQUIRE(parse("4.5", setting, nested));
        CHECK(nested == Nested{Scalar{4.5}});
        REQUIRE(parse("\"on\"", setting, nested));
        CHECK(nested == Nested{std::string("on")});

        Flat flat;
        REQUIRE(parse("42", setting, flat));
        CHECK(flat == Flat{42LL});
        REQUIRE(parse("4.5", setting, flat));
        CHECK(flat == Flat{4.5});
        REQUIRE(parse("\"on\"", setting, flat));
        CHECK(flat == Flat{std::string("on")});
    };
    check_setting(scalar_rule);
    check_setting(scalar_as);
}
