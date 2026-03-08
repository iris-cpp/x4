#include "iris_x4_test.hpp"

#include <iris/x4/auxiliary/attr.hpp>
#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/directive/as.hpp>
#include <iris/x4/directive/lexeme.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/string/string.hpp>
#include <iris/x4/rule.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/tuple.hpp>

#include <iris/rvariant/rvariant.hpp>

#include <string>


struct Ident
{
    std::string value;
};

struct Var
{
    Ident ident;
};

struct TwoInts
{
    int a;
    int b;
};

IRIS_ALLOY_ADAPT_STRUCT(Ident, value)
IRIS_ALLOY_ADAPT_STRUCT(Var, ident)
IRIS_ALLOY_ADAPT_STRUCT(TwoInts, a, b)

template<class T>
using SET = x4_test::single_element_struct<T>;

using IdentRule = x4::rule<struct ident_tag, Ident>;
using VarRule = x4::rule<struct var_tag, Var>;

IRIS_X4_DECLARE_CONSTEXPR(IdentRule)
IRIS_X4_DECLARE_CONSTEXPR(VarRule)

constexpr IdentRule ident;
constexpr VarRule var;

constexpr auto ident_def = x4::alpha >> *x4::alnum;
constexpr auto var_def = '$' >> ident;

IRIS_X4_DEFINE_CONSTEXPR(ident)
IRIS_X4_DEFINE_CONSTEXPR(var)

TEST_CASE("single_element_tuple_like")
{
    using x4::as;
    using x4::alnum;
    using x4::alpha;
    using x4::eps;
    using x4::int_;
    using x4::lexeme;
    using x4::string;

    // ident
    {
        constexpr auto parser = string("abc");
        Ident attr;
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(parser)>::attribute_type, std::string>);
        REQUIRE(parse("abc", parser, attr));
        CHECK(attr.value == "abc");
    }
    {
        constexpr auto parser = ident;
        Ident attr;
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(parser)>::attribute_type, Ident>);
        REQUIRE(parse("abc", parser, attr));
        CHECK(attr.value == "abc");
    }
    {
        constexpr auto parser = ident >> eps;
        Ident attr;
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(parser)>::attribute_type, Ident>);
        REQUIRE(parse("abc", parser, attr));
        CHECK(attr.value == "abc");
    }

    // var
    {
        constexpr auto parser = '$' >> string("abc");
        Var attr;
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(parser)>::attribute_type, std::string>);
        REQUIRE(parse("$abc", parser, attr));
        CHECK(attr.ident.value == "abc");
    }
    {
        constexpr auto parser = '$' >> as<Ident>(string("abc"));
        Var attr;
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(parser)>::attribute_type, Ident>);
        REQUIRE(parse("$abc", parser, attr));
        CHECK(attr.ident.value == "abc");
    }
    {
        constexpr auto parser = var;
        Var attr;
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(parser)>::attribute_type, Var>);
        REQUIRE(parse("$abc", parser, attr));
        CHECK(attr.ident.value == "abc");
    }
    {
        constexpr auto parser = var >> eps;
        Var attr;
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(parser)>::attribute_type, Var>);
        REQUIRE(parse("$abc", parser, attr));
        CHECK(attr.ident.value == "abc");
    }

    // see https://github.com/boostorg/spirit_x4/issues/27
    {
        constexpr auto parser = alpha >> *alnum;
        std::string attr;
        REQUIRE(parse("abc", parser, attr));
        CHECK(attr == "abc");
    }
    {
        constexpr auto parser = lexeme[alpha >> *alnum];
        std::string attr;
        REQUIRE(parse("abc", parser, attr));
        CHECK(attr == "abc");
    }
    {
        constexpr auto identifier = alpha >> *alnum;
        constexpr auto func_call = identifier >> '(' >> int_ >> ')';
        REQUIRE(parse("abc(42)", func_call));
    }
    {
        constexpr auto identifier = lexeme[alpha >> *alnum];
        constexpr auto func_call = identifier >> '(' >> int_ >> ')';
        REQUIRE(parse("abc(42)", func_call));
    }
}

TEST_CASE("product_of_parent_child_attribute_categories")
{
    // as parent (e.g. Var)
    // as child (e.g. Ident)
    //
    // x
    //
    // plain (e.g. int)
    // container (e.g. vector<T>)
    // single element tuple (e.g. std::tuple<T>)
    // multi element tuple (e.g. std::tuple<T, U>)
    //
    // x
    //
    // identity (e.g. T)
    // single element tuple (e.g. std::tuple<T>)
    // variant (e.g. rvariant<T, U>)

    // TODO: add container appender case (parent needs to be inside the list like parser)

    // Each participant (parent/child) independently varies over:
    //   attribute_category (4) x parser_attribute_form (3) = 12 combos
    //
    // Invalid combos (2 per participant):
    //   (container, single_element_tuple) - no move_to for SET<T> -> container
    //   (multi_element_tuple, single_element_tuple) - no move_to for SET<T> -> MET
    //
    // Valid combos per participant: 10
    // Total valid entries: 10 x 10 = 100
    //
    // Notation: SET<T> = x4_test::single_element_struct<T>
    //           TwoInts = struct { int a, b; }
    //
    // For each entry:
    //   child: parsed standalone (parse(input, parser, dest))
    //   parent: parsed in sequence context (parse(input, '+' >> parser, dest))

    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // ===================================================================
    // parent: plain x identity
    // ===================================================================

    // child: plain x identity
    {
        { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    }
    // child: plain x single_element_tuple
    {
        { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    }
    // child: plain x variant
    {
        { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    }
    // child: container x identity
    {
        { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
        { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    }
    // child: container x variant
    {
        { std::string a; REQUIRE(parse("a", int_ | char_, a)); CHECK(a == "a"); }
        { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    }
    // child: single_element_tuple x identity
    {
        { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    }
    // child: single_element_tuple x single_element_tuple
    {
        { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    }
    // child: single_element_tuple x variant
    {
        { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    }
    // child: multi_element_tuple x identity
    {
        { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    }
    // child: multi_element_tuple x variant
    {
        { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    }

    // ===================================================================
    // parent: plain x single_element_tuple
    // ===================================================================

    // child: plain x identity
    {
        { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
        { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    }
    // child: plain x single_element_tuple
    {
        { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
        { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    }
    // child: plain x variant
    {
        { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
        { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    }
    // child: container x identity
    {
        { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
        { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    }
    // child: container x variant
    {
        { std::string a; REQUIRE(parse("a", int_ | char_, a)); CHECK(a == "a"); }
        { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    }
    // child: single_element_tuple x identity
    {
        { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
        { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    }
    // child: single_element_tuple x single_element_tuple
    {
        { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
        { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    }
    // child: single_element_tuple x variant
    {
        { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
        { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    }
    // child: multi_element_tuple x identity
    {
        { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    }
    // child: multi_element_tuple x variant
    {
        { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    }

    // ===================================================================
    // parent: plain x variant
    // ===================================================================

    // child: plain x identity
    {
        { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    }
    // child: plain x single_element_tuple
    {
        { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    }
    // child: plain x variant
    {
        { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    }
    // child: container x identity
    {
        { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
        { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    }
    // child: container x variant
    {
        { std::string a; REQUIRE(parse("a", int_ | char_, a)); CHECK(a == "a"); }
        { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    }
    // child: single_element_tuple x identity
    {
        { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    }
    // child: single_element_tuple x single_element_tuple
    {
        { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    }
    // child: single_element_tuple x variant
    {
        { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
        { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    }
    // child: multi_element_tuple x identity
    {
        { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    }
    // child: multi_element_tuple x variant
    {
        { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    }

    // ===================================================================
    // parent: container x identity
    // ===================================================================

    // child: plain x identity
    {
        { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    }
    // child: plain x single_element_tuple
    {
        { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    }
    // child: plain x variant
    {
        { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    }
    // child: container x identity
    {
        { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
        { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    }
    // child: container x variant
    {
        { std::string a; REQUIRE(parse("a", int_ | char_, a)); CHECK(a == "a"); }
        { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    }
    // child: single_element_tuple x identity
    {
        { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    }
    // child: single_element_tuple x single_element_tuple
    {
        { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    }
    // child: single_element_tuple x variant
    {
        { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    }
    // child: multi_element_tuple x identity
    {
        { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    }
    // child: multi_element_tuple x variant
    {
        { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    }

    // ===================================================================
    // parent: container x variant
    // ===================================================================

    // child: plain x identity
    {
        { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> (int_ | char_), a)); CHECK(a == "a"); }
    }
    // child: plain x single_element_tuple
    {
        { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> (int_ | char_), a)); CHECK(a == "a"); }
    }
    // child: plain x variant
    {
        { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> (int_ | char_), a)); CHECK(a == "a"); }
    }
    // child: container x identity
    {
        { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
        { std::string a; REQUIRE(parse("+a", '+' >> (int_ | char_), a)); CHECK(a == "a"); }
    }
    // child: container x variant
    {
        { std::string a; REQUIRE(parse("a", int_ | char_, a)); CHECK(a == "a"); }
        { std::string a; REQUIRE(parse("+a", '+' >> (int_ | char_), a)); CHECK(a == "a"); }
    }
    // child: single_element_tuple x identity
    {
        { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> (int_ | char_), a)); CHECK(a == "a"); }
    }
    // child: single_element_tuple x single_element_tuple
    {
        { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> (int_ | char_), a)); CHECK(a == "a"); }
    }
    // child: single_element_tuple x variant
    {
        { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
        { std::string a; REQUIRE(parse("+a", '+' >> (int_ | char_), a)); CHECK(a == "a"); }
    }
    // child: multi_element_tuple x identity
    {
        { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { std::string a; REQUIRE(parse("+a", '+' >> (int_ | char_), a)); CHECK(a == "a"); }
    }
    // child: multi_element_tuple x variant
    {
        { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { std::string a; REQUIRE(parse("+a", '+' >> (int_ | char_), a)); CHECK(a == "a"); }
    }

    // ===================================================================
    // parent: single_element_tuple x identity
    // ===================================================================

    // child: plain x identity
    {
        { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    }
    // child: plain x single_element_tuple
    {
        { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    }
    // child: plain x variant
    {
        { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    }
    // child: container x identity
    {
        { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    }
    // child: container x variant
    {
        { std::string a; REQUIRE(parse("a", int_ | char_, a)); CHECK(a == "a"); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    }
    // child: single_element_tuple x identity
    {
        { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    }
    // child: single_element_tuple x single_element_tuple
    {
        { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    }
    // child: single_element_tuple x variant
    {
        { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    }
    // child: multi_element_tuple x identity
    {
        { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    }
    // child: multi_element_tuple x variant
    {
        { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    }

    // ===================================================================
    // parent: single_element_tuple x single_element_tuple
    // ===================================================================

    // child: plain x identity
    {
        { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
        { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    }
    // child: plain x single_element_tuple
    {
        { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
        { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    }
    // child: plain x variant
    {
        { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
        { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    }
    // child: container x identity
    {
        { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
        { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    }
    // child: container x variant
    {
        { std::string a; REQUIRE(parse("a", int_ | char_, a)); CHECK(a == "a"); }
        { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    }
    // child: single_element_tuple x identity
    {
        { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
        { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    }
    // child: single_element_tuple x single_element_tuple
    {
        { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
        { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    }
    // child: single_element_tuple x variant
    {
        { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
        { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    }
    // child: multi_element_tuple x identity
    {
        { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    }
    // child: multi_element_tuple x variant
    {
        { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    }

    // ===================================================================
    // parent: single_element_tuple x variant
    // ===================================================================

    // child: plain x identity
    {
        { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    }
    // child: plain x single_element_tuple
    {
        { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    }
    // child: plain x variant
    {
        { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    }
    // child: container x identity
    {
        { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    }
    // child: container x variant
    {
        { std::string a; REQUIRE(parse("a", int_ | char_, a)); CHECK(a == "a"); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    }
    // child: single_element_tuple x identity
    {
        { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    }
    // child: single_element_tuple x single_element_tuple
    {
        { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    }
    // child: single_element_tuple x variant
    {
        { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    }
    // child: multi_element_tuple x identity
    {
        { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    }
    // child: multi_element_tuple x variant
    {
        { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    }

    // ===================================================================
    // parent: multi_element_tuple x identity
    // ===================================================================

    // child: plain x identity
    {
        { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: plain x single_element_tuple
    {
        { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: plain x variant
    {
        { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: container x identity
    {
        { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: container x variant
    {
        { std::string a; REQUIRE(parse("a", int_ | char_, a)); CHECK(a == "a"); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: single_element_tuple x identity
    {
        { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: single_element_tuple x single_element_tuple
    {
        { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: single_element_tuple x variant
    {
        { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: multi_element_tuple x identity
    {
        { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: multi_element_tuple x variant
    {
        { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }

    // ===================================================================
    // parent: multi_element_tuple x variant
    // ===================================================================

    // child: plain x identity
    {
        { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: plain x single_element_tuple
    {
        { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: plain x variant
    {
        { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: container x identity
    {
        { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: container x variant
    {
        { std::string a; REQUIRE(parse("a", int_ | char_, a)); CHECK(a == "a"); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: single_element_tuple x identity
    {
        { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: single_element_tuple x single_element_tuple
    {
        { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: single_element_tuple x variant
    {
        { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: multi_element_tuple x identity
    {
        { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
    // child: multi_element_tuple x variant
    {
        { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
        { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    }
}