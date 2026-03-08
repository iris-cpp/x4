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
#include <iris/alloy/adapted/std_tuple.hpp>
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

// Tests attribute compatibility across the matrix of:
//   attribute_category: plain, container, single_element_tuple (SET), multi_element_tuple (MET)
//   parser_form: identity, single_element_tuple, variant
//
// Invalid combos: (container, SET) and (MET, SET) — no valid move_to
// Valid combos per participant: 10
//
// Notation: SET<T> = x4_test::single_element_struct<T>
//           TwoInts = struct { int a, b; }
//
// Tests are organized in three groups:
//   1. Standalone: each combo parsed independently
//   2. In sequence: unused >> parser → dest (attribute flow through sequences)
//   3. Composition: two attributed parsers → combined dest (partition and interaction)

// TODO: add container appender case (parent needs to be inside the list like parser)

// ===================================================================
// 1. Standalone: parse(input, parser, dest)
// ===================================================================

TEST_CASE("SET standalone")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // plain × identity
    { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
    // plain × SET
    { int a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    // plain × variant
    { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
    // container × identity
    { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
    // container × variant
    { std::string a; REQUIRE(parse("a", alpha | char_, a)); CHECK(a == "a"); }
    // SET × identity
    { SET<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
    // SET × SET
    { SET<int> a{}; REQUIRE(parse("", x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    // SET × variant
    { SET<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
    // MET × identity
    { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    // MET × variant
    { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
}

// ===================================================================
// 2. In sequence: parse(input, unused >> parser, dest)
//    Tests partition_attribute with one unused side
// ===================================================================

TEST_CASE("SET in sequence")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // plain × identity
    { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    // plain × SET
    { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a == 42); }
    // plain × variant
    { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    // container × identity
    { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    // container × variant
    { std::string a; REQUIRE(parse("+a", '+' >> (alpha | char_), a)); CHECK(a == "a"); }
    // SET × identity
    { SET<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    // SET × SET
    { SET<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SET<int>{42}), a)); CHECK(a.value == 42); }
    // SET × variant
    { SET<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    // MET × identity
    { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    // MET × variant
    { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
}

// ===================================================================
// 3. Composition: left_parser >> right_parser → combined dest
//    Tests partition_attribute splitting + per-slot attribute handling
//
//    Parser forms (sequence_size=1):
//      P_int:  int_               → int
//      P_set:  attr(SET<int>{V})   → SET<int>
//      P_var:  int_ | char_        → rvariant<int, char>
//      P_chr:  alpha               → char
//      P_slit: string("abc")       → std::string
//      P_str:  +alpha              → std::string
//
//    Parser forms (sequence_size=2):
//      P_met:  int_ >> ',' >> int_
//      P_metv: (int_ >> ',' >> int_) | (char_ >> ',' >> char_)
//
//    Slot types tested: int, char, std::string, SET<int>, SET<char>,
//                       SET<std::string>, TwoInts
// ===================================================================

TEST_CASE("SET composition: left int_")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // int_ × int_ → (int, int)
    { std::tuple<int, int> a; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // int_ × int_ → MET struct
    { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    // int_ × int_ → (SET<int>, int)
    { std::tuple<SET<int>, int> a; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a) == 2); }
    // int_ × int_ → (int, SET<int>)
    { std::tuple<int, SET<int>> a; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a).value == 2); }
    // int_ × int_ → (SET<int>, SET<int>)
    { std::tuple<SET<int>, SET<int>> a; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a).value == 2); }
    // int_ × attr(SET)
    { std::tuple<int, int> a; REQUIRE(parse("42", int_ >> x4::attr(SET<int>{99}), a)); CHECK(alloy::get<0>(a) == 42); CHECK(alloy::get<1>(a) == 99); }
    // int_ × variant
    { std::tuple<int, int> a; REQUIRE(parse("1,2", int_ >> ',' >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // int_ × alpha
    { std::tuple<int, char> a; REQUIRE(parse("1a", int_ >> alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 'a'); }
    // int_ × alpha → (int, SET<char>)
    { std::tuple<int, SET<char>> a; REQUIRE(parse("1a", int_ >> alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a).value == 'a'); }
    // int_ × alpha → (SET<int>, SET<char>)
    { std::tuple<SET<int>, SET<char>> a; REQUIRE(parse("1a", int_ >> alpha, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a).value == 'a'); }
    // int_ × string("abc")
    { std::tuple<int, std::string> a; REQUIRE(parse("1abc", int_ >> string("abc"), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
    // int_ × +alpha
    { std::tuple<int, std::string> a; REQUIRE(parse("1abc", int_ >> +alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
    // int_ × +alpha → (int, SET<string>)
    { std::tuple<int, SET<std::string>> a; REQUIRE(parse("1abc", int_ >> +alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a).value == "abc"); }
    // int_ × +alpha → (SET<int>, string)
    { std::tuple<SET<int>, std::string> a; REQUIRE(parse("1abc", int_ >> +alpha, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a) == "abc"); }
}

TEST_CASE("SET composition: left attr(SET)")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // attr(SET) × int_
    { std::tuple<int, int> a; REQUIRE(parse("42", x4::attr(SET<int>{1}) >> int_, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 42); }
    // attr(SET) × int_ → (SET<int>, int)
    { std::tuple<SET<int>, int> a; REQUIRE(parse("42", x4::attr(SET<int>{1}) >> int_, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a) == 42); }
    // attr(SET) × attr(SET)
    { std::tuple<int, int> a; REQUIRE(parse("", x4::attr(SET<int>{1}) >> x4::attr(SET<int>{2}), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // attr(SET) × variant
    { std::tuple<int, int> a; REQUIRE(parse("2", x4::attr(SET<int>{1}) >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // attr(SET) × alpha
    { std::tuple<int, char> a; REQUIRE(parse("a", x4::attr(SET<int>{1}) >> alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 'a'); }
    // attr(SET) × alpha → (SET<int>, char)
    { std::tuple<SET<int>, char> a; REQUIRE(parse("a", x4::attr(SET<int>{1}) >> alpha, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a) == 'a'); }
    // attr(SET) × string("abc")
    { std::tuple<int, std::string> a; REQUIRE(parse("abc", x4::attr(SET<int>{1}) >> string("abc"), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
    // attr(SET) × +alpha
    { std::tuple<int, std::string> a; REQUIRE(parse("abc", x4::attr(SET<int>{1}) >> +alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
}

TEST_CASE("SET composition: left variant")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // variant × int_
    { std::tuple<int, int> a; REQUIRE(parse("1,2", (int_ | char_) >> ',' >> int_, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // variant × int_ → (SET<int>, int)
    { std::tuple<SET<int>, int> a; REQUIRE(parse("1,2", (int_ | char_) >> ',' >> int_, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a) == 2); }
    // variant × attr(SET)
    { std::tuple<int, int> a; REQUIRE(parse("1", (int_ | char_) >> x4::attr(SET<int>{2}), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // variant × variant
    { std::tuple<int, int> a; REQUIRE(parse("1,2", (int_ | char_) >> ',' >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // variant × variant → (int, SET<int>)
    { std::tuple<int, SET<int>> a; REQUIRE(parse("1,2", (int_ | char_) >> ',' >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a).value == 2); }
    // variant × alpha
    { std::tuple<int, char> a; REQUIRE(parse("1a", (int_ | char_) >> alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 'a'); }
    // variant × string("abc")
    { std::tuple<int, std::string> a; REQUIRE(parse("1abc", (int_ | char_) >> string("abc"), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
    // variant × +alpha
    { std::tuple<int, std::string> a; REQUIRE(parse("1abc", (int_ | char_) >> +alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
}

TEST_CASE("SET composition: left alpha")
{
    using x4::int_;
    using x4::alpha;
    using x4::alnum;
    using x4::string;
    using x4::standard::char_;

    // alpha × int_
    { std::tuple<char, int> a; REQUIRE(parse("a42", alpha >> int_, a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == 42); }
    // alpha × int_ → (SET<char>, int)
    { std::tuple<SET<char>, int> a; REQUIRE(parse("a42", alpha >> int_, a)); CHECK(alloy::get<0>(a).value == 'a'); CHECK(alloy::get<1>(a) == 42); }
    // alpha × int_ → (char, SET<int>)
    { std::tuple<char, SET<int>> a; REQUIRE(parse("a42", alpha >> int_, a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a).value == 42); }
    // alpha × attr(SET)
    { std::tuple<char, int> a; REQUIRE(parse("a", alpha >> x4::attr(SET<int>{2}), a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == 2); }
    // alpha × variant
    { std::tuple<char, int> a; REQUIRE(parse("a1", alpha >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == 1); }
    // alpha × alpha
    { std::tuple<char, char> a; REQUIRE(parse("ab", alpha >> alpha, a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == 'b'); }
    // alpha × alpha → (SET<char>, SET<char>)
    { std::tuple<SET<char>, SET<char>> a; REQUIRE(parse("ab", alpha >> alpha, a)); CHECK(alloy::get<0>(a).value == 'a'); CHECK(alloy::get<1>(a).value == 'b'); }
    // alpha × string("abc")
    { std::tuple<char, std::string> a; REQUIRE(parse("xabc", alpha >> string("abc"), a)); CHECK(alloy::get<0>(a) == 'x'); CHECK(alloy::get<1>(a) == "abc"); }
    // alpha × string("abc") → (char, SET<string>)
    { std::tuple<char, SET<std::string>> a; REQUIRE(parse("xabc", alpha >> string("abc"), a)); CHECK(alloy::get<0>(a) == 'x'); CHECK(alloy::get<1>(a).value == "abc"); }
    // alpha × +alpha
    { std::tuple<char, std::string> a; REQUIRE(parse("ab", alpha >> +alpha, a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == "b"); }
}

TEST_CASE("SET composition: left string")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // string("abc") × int_
    { std::tuple<std::string, int> a; REQUIRE(parse("abc42", string("abc") >> int_, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 42); }
    // string("abc") × int_ → (SET<string>, int)
    { std::tuple<SET<std::string>, int> a; REQUIRE(parse("abc42", string("abc") >> int_, a)); CHECK(alloy::get<0>(a).value == "abc"); CHECK(alloy::get<1>(a) == 42); }
    // string("abc") × attr(SET)
    { std::tuple<std::string, int> a; REQUIRE(parse("abc", string("abc") >> x4::attr(SET<int>{2}), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 2); }
    // string("abc") × variant
    { std::tuple<std::string, int> a; REQUIRE(parse("abc1", string("abc") >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 1); }
    // string("abc") × alpha
    { std::tuple<std::string, char> a; REQUIRE(parse("abcd", string("abc") >> alpha, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 'd'); }
    // string("abc") × string("def")
    { std::tuple<std::string, std::string> a; REQUIRE(parse("abcdef", string("abc") >> string("def"), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == "def"); }
    // string("abc") × string("def") → (SET<string>, SET<string>)
    { std::tuple<SET<std::string>, SET<std::string>> a; REQUIRE(parse("abcdef", string("abc") >> string("def"), a)); CHECK(alloy::get<0>(a).value == "abc"); CHECK(alloy::get<1>(a).value == "def"); }
    // string("abc") × +alpha
    { std::tuple<std::string, std::string> a; REQUIRE(parse("abcdef", string("abc") >> +alpha, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == "def"); }
}

TEST_CASE("SET composition: left +alpha")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // +alpha × int_
    { std::tuple<std::string, int> a; REQUIRE(parse("abc42", +alpha >> int_, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 42); }
    // +alpha × int_ → (SET<string>, int)
    { std::tuple<SET<std::string>, int> a; REQUIRE(parse("abc42", +alpha >> int_, a)); CHECK(alloy::get<0>(a).value == "abc"); CHECK(alloy::get<1>(a) == 42); }
    // +alpha × int_ → (string, SET<int>)
    { std::tuple<std::string, SET<int>> a; REQUIRE(parse("abc42", +alpha >> int_, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a).value == 42); }
    // +alpha × attr(SET)
    { std::tuple<std::string, int> a; REQUIRE(parse("abc", +alpha >> x4::attr(SET<int>{2}), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 2); }
    // +alpha × variant
    { std::tuple<std::string, int> a; REQUIRE(parse("abc1", +alpha >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 1); }
    // +alpha × ',' >> alpha
    { std::tuple<std::string, char> a; REQUIRE(parse("abc,d", +alpha >> ',' >> alpha, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 'd'); }
    // +alpha × ',' >> +alpha
    { std::tuple<std::string, std::string> a; REQUIRE(parse("abc,def", +alpha >> ',' >> +alpha, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == "def"); }
    // +alpha × ',' >> +alpha → (SET<string>, SET<string>)
    { std::tuple<SET<std::string>, SET<std::string>> a; REQUIRE(parse("abc,def", +alpha >> ',' >> +alpha, a)); CHECK(alloy::get<0>(a).value == "abc"); CHECK(alloy::get<1>(a).value == "def"); }
}

TEST_CASE("SET composition: SET dest wrapping")
{
    using x4::int_;
    using x4::alpha;
    using x4::alnum;
    using x4::string;
    using x4::standard::char_;

    // --- SET dest wrapping sequence result (core fix) ---
    // Exercises the parse_sequence unwrap path:
    //   is_single_element_tuple_like<Attr> && has_attribute_v<left> && has_attribute_v<right>

    // SET<container>: alpha >> *alnum → SET<string> (= Ident pattern)
    { SET<std::string> a; REQUIRE(parse("abc", alpha >> *alnum, a)); CHECK(a.value == "abc"); }
    // SET<MET>: int_ >> ',' >> int_ → SET<TwoInts>
    { SET<TwoInts> a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.value.a == 1); CHECK(a.value.b == 2); }
    // SET<tuple>: int_ >> ',' >> int_ → SET<tuple<int, int>>
    { SET<std::tuple<int, int>> a; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(alloy::get<0>(a.value) == 1); CHECK(alloy::get<1>(a.value) == 2); }
    // SET<MET> with variant parser
    { SET<TwoInts> a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.value.a == 1); CHECK(a.value.b == 2); }
}

TEST_CASE("SET composition: MET parsers")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // MET × int_ → 3-slot tuple
    { std::tuple<int, int, int> a; REQUIRE(parse("1,2:3", (int_ >> ',' >> int_) >> ':' >> int_, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == 3); }
    // MET × alpha → 3-slot tuple
    { std::tuple<int, int, char> a; REQUIRE(parse("1,2a", (int_ >> ',' >> int_) >> alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == 'a'); }
    // MET × +alpha → 3-slot tuple
    { std::tuple<int, int, std::string> a; REQUIRE(parse("1,2abc", (int_ >> ',' >> int_) >> +alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == "abc"); }
    // MET × attr(SET) → 3-slot tuple
    { std::tuple<int, int, int> a; REQUIRE(parse("1,2", (int_ >> ',' >> int_) >> x4::attr(SET<int>{99}), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == 99); }
    // int_ × MET → 3-slot tuple
    { std::tuple<int, int, int> a; REQUIRE(parse("1:2,3", int_ >> ':' >> (int_ >> ',' >> int_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == 3); }
    // alpha × MET → 3-slot tuple
    { std::tuple<char, int, int> a; REQUIRE(parse("a1,2", alpha >> (int_ >> ',' >> int_), a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == 1); CHECK(alloy::get<2>(a) == 2); }
    // +alpha × MET → 3-slot tuple
    { std::tuple<std::string, int, int> a; REQUIRE(parse("abc1,2", +alpha >> (int_ >> ',' >> int_), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 1); CHECK(alloy::get<2>(a) == 2); }
    // attr(SET) × MET → 3-slot tuple
    { std::tuple<int, int, int> a; REQUIRE(parse("1,2", x4::attr(SET<int>{99}) >> (int_ >> ',' >> int_), a)); CHECK(alloy::get<0>(a) == 99); CHECK(alloy::get<1>(a) == 1); CHECK(alloy::get<2>(a) == 2); }
    // MET × MET → 4-slot tuple
    { std::tuple<int, int, int, int> a; REQUIRE(parse("1,2:3,4", (int_ >> ',' >> int_) >> ':' >> (int_ >> ',' >> int_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == 3); CHECK(alloy::get<3>(a) == 4); }
    // MET_variant × alpha → 2-slot tuple (alternative has sequence_size=1)
    { std::tuple<TwoInts, char> a; REQUIRE(parse("1,2a", ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)) >> alpha, a)); CHECK(alloy::get<0>(a).a == 1); CHECK(alloy::get<0>(a).b == 2); CHECK(alloy::get<1>(a) == 'a'); }
    // alpha × MET_variant → 2-slot tuple
    { std::tuple<char, TwoInts> a; REQUIRE(parse("a1,2", alpha >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a).a == 1); CHECK(alloy::get<1>(a).b == 2); }

    // --- Nested SET: recursive unwrap ---
    // TODO: these hit a noexcept evaluation issue in move_to (recursive
    //       single-element-tuple-like forwarding through two layers).
    //       Uncomment once that is resolved.

    // SET<SET<int>>: double unwrap through move_to
    // {
    //     SET<SET<int>> a;
    //     REQUIRE(parse("42", int_, a));
    //     CHECK(a.value.value == 42);
    // }
    // SET<SET<string>>: double unwrap through parse_sequence (= Var pattern)
    // {
    //     SET<SET<std::string>> a;
    //     REQUIRE(parse("abc", alpha >> *alnum, a));
    //     CHECK(a.value.value == "abc");
    // }
}
