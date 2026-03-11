#include "iris_x4_test.hpp"

#include <iris/x4/auxiliary/attr.hpp>
#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/directive/as.hpp>
#include <iris/x4/directive/lexeme.hpp>
#include <iris/x4/directive/matches.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/list.hpp>
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

    bool operator==(const Ident&) const = default;
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

struct NamePath
{
    bool is_rooted = false;
    std::vector<Ident> segments;
};

struct AnyID
{
    NamePath path;
};

IRIS_ALLOY_ADAPT_STRUCT(Ident, value)
IRIS_ALLOY_ADAPT_STRUCT(Var, ident)
IRIS_ALLOY_ADAPT_STRUCT(TwoInts, a, b)
IRIS_ALLOY_ADAPT_STRUCT(NamePath, is_rooted, segments)
IRIS_ALLOY_ADAPT_STRUCT(AnyID, path)

template<class T>
using SES = x4_test::single_element_struct<T>;

using IdentRule = x4::rule<struct ident_tag, Ident>;
using VarRule = x4::rule<struct var_tag, Var>;
using NamePathRule = x4::rule<struct name_path_tag, NamePath>;
using AnyIDRule = x4::rule<struct any_id_tag, AnyID>;

IRIS_X4_DECLARE_CONSTEXPR(IdentRule)
IRIS_X4_DECLARE_CONSTEXPR(VarRule)
IRIS_X4_DECLARE_CONSTEXPR(NamePathRule)
IRIS_X4_DECLARE_CONSTEXPR(AnyIDRule)

constexpr IdentRule ident;
constexpr VarRule var;
constexpr NamePathRule name_path;
constexpr AnyIDRule any_id;

constexpr auto ident_def = x4::alpha >> *x4::alnum;
constexpr auto var_def = '$' >> ident;
constexpr auto name_path_def = x4::matches["::"] >> (ident % "::");
constexpr auto any_id_def = name_path;

IRIS_X4_DEFINE_CONSTEXPR(ident)
IRIS_X4_DEFINE_CONSTEXPR(var)
IRIS_X4_DEFINE_CONSTEXPR(name_path)
IRIS_X4_DEFINE_CONSTEXPR(any_id)

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

    // name_path
    {
        constexpr auto parser = name_path;
        NamePath attr;
        REQUIRE(parse("::foo::bar", parser, attr));
        CHECK(attr.is_rooted);
        CHECK(attr.segments == std::vector<Ident>{Ident{"foo"}, Ident{"bar"}});
    }

    // any_id
    {
        constexpr auto parser = any_id;
        AnyID attr;
        REQUIRE(parse("::foo::bar", parser, attr));
        CHECK(attr.path.is_rooted);
        CHECK(attr.path.segments == std::vector<Ident>{Ident{"foo"}, Ident{"bar"}});
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


//
// AUTOGENERATED CONTENT; DO NOT EDIT UNLESS YOU KNOW WHAT YOU ARE DOING.
//
// The test cases below are AI-generated content that was explicitly approved
// by @saki7, the current maintainer of X4. They were produced during a pair-
// programming session led by @yaito3014, a major contributor and the author
// of the `alloy` tuple library. The session was screen-shared over a Discord
// voice chat, and we spent more than six hours in deep discussion to ensure
// that the generated content was semantically meaningful.
//
// The problem these test cases were designed to solve essentially requires
// covering the full set of parent/child parser attribute combinations. The
// only practical way to express this in the test suite was to generate the
// Cartesian product of those attribute combinations, resulting in well over
// one hundred cases in total. In other words, this task was inherently well
// suited to AI-assisted generation, and we considered it an ideal place to
// explore how AI could support X4 development.
//
// As a result, these tests identified a single point of failure in
// `parse_sequence` and directly contributed to fixing the bug.
//
// For that reason, the code below was produced under very careful supervision
// by the maintainer and should be treated as an exceptional case of accepting
// AI-generated content into our codebase.
//
// Any future contributor who attempts to use the existence of this code as
// justification for submitting vibe-coded slop will be considered harmful to
// the codebase and will receive a permanent ban.
//

// Tests attribute compatibility across the matrix of:
//   attribute_category: plain, container, single_element_tuple (SES), multi_element_tuple (MET)
//   parser_form: identity, single_element_tuple, variant
//
// Invalid combos: (container, SES) and (MET, SES) — no valid move_to
// Valid combos per participant: 10
//
// Notation: SES<T> = x4_test::single_element_struct<T>
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

TEST_CASE("SES standalone")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // plain x identity
    { int a{}; REQUIRE(parse("42", int_, a)); CHECK(a == 42); }
    // plain x SES
    { int a{}; REQUIRE(parse("", x4::attr(SES<int>{42}), a)); CHECK(a == 42); }
    // plain x variant
    { int a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a == 42); }
    // container x identity
    { std::string a; REQUIRE(parse("abc", string("abc"), a)); CHECK(a == "abc"); }
    // container x variant
    { std::string a; REQUIRE(parse("a", alpha | char_, a)); CHECK(a == "a"); }
    // SES x identity
    { SES<int> a{}; REQUIRE(parse("42", int_, a)); CHECK(a.value == 42); }
    // SES x SES
    { SES<int> a{}; REQUIRE(parse("", x4::attr(SES<int>{42}), a)); CHECK(a.value == 42); }
    // SES x variant
    { SES<int> a{}; REQUIRE(parse("42", int_ | char_, a)); CHECK(a.value == 42); }
    // MET x identity
    { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    // MET x variant
    { TwoInts a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
}

// ===================================================================
// 2. In sequence: parse(input, unused >> parser, dest)
//    Tests partition_attribute with one unused side
// ===================================================================

TEST_CASE("SES in sequence")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // plain x identity
    { int a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a == 42); }
    // plain x SES
    { int a{}; REQUIRE(parse("+", '+' >> x4::attr(SES<int>{42}), a)); CHECK(a == 42); }
    // plain x variant
    { int a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a == 42); }
    // container x identity
    { std::string a; REQUIRE(parse("+a", '+' >> alpha, a)); CHECK(a == "a"); }
    // container x variant
    { std::string a; REQUIRE(parse("+a", '+' >> (alpha | char_), a)); CHECK(a == "a"); }
    // SES x identity
    { SES<int> a{}; REQUIRE(parse("+42", '+' >> int_, a)); CHECK(a.value == 42); }
    // SES x SES
    { SES<int> a{}; REQUIRE(parse("+", '+' >> x4::attr(SES<int>{42}), a)); CHECK(a.value == 42); }
    // SES x variant
    { SES<int> a{}; REQUIRE(parse("+42", '+' >> (int_ | char_), a)); CHECK(a.value == 42); }
    // MET x identity
    { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> (int_ >> ',' >> int_), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    // MET x variant
    { TwoInts a{}; REQUIRE(parse("+1,2", '+' >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(a.a == 1); CHECK(a.b == 2); }
}

// ===================================================================
// 3. Composition: left_parser >> right_parser → combined dest
//    Tests partition_attribute splitting + per-slot attribute handling
//
//    Parser forms (sequence_size=1):
//      P_int:  int_               → int
//      P_set:  attr(SES<int>{V})   → SES<int>
//      P_var:  int_ | char_        → rvariant<int, char>
//      P_chr:  alpha               → char
//      P_slit: string("abc")       → std::string
//      P_str:  +alpha              → std::string
//
//    Parser forms (sequence_size=2):
//      P_met:  int_ >> ',' >> int_
//      P_metv: (int_ >> ',' >> int_) | (char_ >> ',' >> char_)
//
//    Slot types tested: int, char, std::string, SES<int>, SES<char>,
//                       SES<std::string>, TwoInts
// ===================================================================

TEST_CASE("SES composition: left int_")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // int_ x int_ → (int, int)
    { std::tuple<int, int> a; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // int_ x int_ → MET struct
    { TwoInts a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.a == 1); CHECK(a.b == 2); }
    // int_ x int_ → (SES<int>, int)
    { std::tuple<SES<int>, int> a; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a) == 2); }
    // int_ x int_ → (int, SES<int>)
    { std::tuple<int, SES<int>> a; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a).value == 2); }
    // int_ x int_ → (SES<int>, SES<int>)
    { std::tuple<SES<int>, SES<int>> a; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a).value == 2); }
    // int_ x attr(SES)
    { std::tuple<int, int> a; REQUIRE(parse("42", int_ >> x4::attr(SES<int>{99}), a)); CHECK(alloy::get<0>(a) == 42); CHECK(alloy::get<1>(a) == 99); }
    // int_ x variant
    { std::tuple<int, int> a; REQUIRE(parse("1,2", int_ >> ',' >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // int_ x alpha
    { std::tuple<int, char> a; REQUIRE(parse("1a", int_ >> alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 'a'); }
    // int_ x alpha → (int, SES<char>)
    { std::tuple<int, SES<char>> a; REQUIRE(parse("1a", int_ >> alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a).value == 'a'); }
    // int_ x alpha → (SES<int>, SES<char>)
    { std::tuple<SES<int>, SES<char>> a; REQUIRE(parse("1a", int_ >> alpha, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a).value == 'a'); }
    // int_ x string("abc")
    { std::tuple<int, std::string> a; REQUIRE(parse("1abc", int_ >> string("abc"), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
    // int_ x +alpha
    { std::tuple<int, std::string> a; REQUIRE(parse("1abc", int_ >> +alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
    // int_ x +alpha → (int, SES<string>)
    { std::tuple<int, SES<std::string>> a; REQUIRE(parse("1abc", int_ >> +alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a).value == "abc"); }
    // int_ x +alpha → (SES<int>, string)
    { std::tuple<SES<int>, std::string> a; REQUIRE(parse("1abc", int_ >> +alpha, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a) == "abc"); }
}

TEST_CASE("SES composition: left attr(SES)")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // attr(SES) x int_
    { std::tuple<int, int> a; REQUIRE(parse("42", x4::attr(SES<int>{1}) >> int_, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 42); }
    // attr(SES) x int_ → (SES<int>, int)
    { std::tuple<SES<int>, int> a; REQUIRE(parse("42", x4::attr(SES<int>{1}) >> int_, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a) == 42); }
    // attr(SES) x attr(SES)
    { std::tuple<int, int> a; REQUIRE(parse("", x4::attr(SES<int>{1}) >> x4::attr(SES<int>{2}), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // attr(SES) x variant
    { std::tuple<int, int> a; REQUIRE(parse("2", x4::attr(SES<int>{1}) >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // attr(SES) x alpha
    { std::tuple<int, char> a; REQUIRE(parse("a", x4::attr(SES<int>{1}) >> alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 'a'); }
    // attr(SES) x alpha → (SES<int>, char)
    { std::tuple<SES<int>, char> a; REQUIRE(parse("a", x4::attr(SES<int>{1}) >> alpha, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a) == 'a'); }
    // attr(SES) x string("abc")
    { std::tuple<int, std::string> a; REQUIRE(parse("abc", x4::attr(SES<int>{1}) >> string("abc"), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
    // attr(SES) x +alpha
    { std::tuple<int, std::string> a; REQUIRE(parse("abc", x4::attr(SES<int>{1}) >> +alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
}

TEST_CASE("SES composition: left variant")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // variant x int_
    { std::tuple<int, int> a; REQUIRE(parse("1,2", (int_ | char_) >> ',' >> int_, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // variant x int_ → (SES<int>, int)
    { std::tuple<SES<int>, int> a; REQUIRE(parse("1,2", (int_ | char_) >> ',' >> int_, a)); CHECK(alloy::get<0>(a).value == 1); CHECK(alloy::get<1>(a) == 2); }
    // variant x attr(SES)
    { std::tuple<int, int> a; REQUIRE(parse("1", (int_ | char_) >> x4::attr(SES<int>{2}), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // variant x variant
    { std::tuple<int, int> a; REQUIRE(parse("1,2", (int_ | char_) >> ',' >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); }
    // variant x variant → (int, SES<int>)
    { std::tuple<int, SES<int>> a; REQUIRE(parse("1,2", (int_ | char_) >> ',' >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a).value == 2); }
    // variant x alpha
    { std::tuple<int, char> a; REQUIRE(parse("1a", (int_ | char_) >> alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 'a'); }
    // variant x string("abc")
    { std::tuple<int, std::string> a; REQUIRE(parse("1abc", (int_ | char_) >> string("abc"), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
    // variant x +alpha
    { std::tuple<int, std::string> a; REQUIRE(parse("1abc", (int_ | char_) >> +alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == "abc"); }
}

TEST_CASE("SES composition: left alpha")
{
    using x4::int_;
    using x4::alpha;
    using x4::alnum;
    using x4::string;
    using x4::standard::char_;

    // alpha x int_
    { std::tuple<char, int> a; REQUIRE(parse("a42", alpha >> int_, a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == 42); }
    // alpha x int_ → (SES<char>, int)
    { std::tuple<SES<char>, int> a; REQUIRE(parse("a42", alpha >> int_, a)); CHECK(alloy::get<0>(a).value == 'a'); CHECK(alloy::get<1>(a) == 42); }
    // alpha x int_ → (char, SES<int>)
    { std::tuple<char, SES<int>> a; REQUIRE(parse("a42", alpha >> int_, a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a).value == 42); }
    // alpha x attr(SES)
    { std::tuple<char, int> a; REQUIRE(parse("a", alpha >> x4::attr(SES<int>{2}), a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == 2); }
    // alpha x variant
    { std::tuple<char, int> a; REQUIRE(parse("a1", alpha >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == 1); }
    // alpha x alpha
    { std::tuple<char, char> a; REQUIRE(parse("ab", alpha >> alpha, a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == 'b'); }
    // alpha x alpha → (SES<char>, SES<char>)
    { std::tuple<SES<char>, SES<char>> a; REQUIRE(parse("ab", alpha >> alpha, a)); CHECK(alloy::get<0>(a).value == 'a'); CHECK(alloy::get<1>(a).value == 'b'); }
    // alpha x string("abc")
    { std::tuple<char, std::string> a; REQUIRE(parse("xabc", alpha >> string("abc"), a)); CHECK(alloy::get<0>(a) == 'x'); CHECK(alloy::get<1>(a) == "abc"); }
    // alpha x string("abc") → (char, SES<string>)
    { std::tuple<char, SES<std::string>> a; REQUIRE(parse("xabc", alpha >> string("abc"), a)); CHECK(alloy::get<0>(a) == 'x'); CHECK(alloy::get<1>(a).value == "abc"); }
    // alpha x +alpha
    { std::tuple<char, std::string> a; REQUIRE(parse("ab", alpha >> +alpha, a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == "b"); }
}

TEST_CASE("SES composition: left string")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // string("abc") x int_
    { std::tuple<std::string, int> a; REQUIRE(parse("abc42", string("abc") >> int_, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 42); }
    // string("abc") x int_ → (SES<string>, int)
    { std::tuple<SES<std::string>, int> a; REQUIRE(parse("abc42", string("abc") >> int_, a)); CHECK(alloy::get<0>(a).value == "abc"); CHECK(alloy::get<1>(a) == 42); }
    // string("abc") x attr(SES)
    { std::tuple<std::string, int> a; REQUIRE(parse("abc", string("abc") >> x4::attr(SES<int>{2}), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 2); }
    // string("abc") x variant
    { std::tuple<std::string, int> a; REQUIRE(parse("abc1", string("abc") >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 1); }
    // string("abc") x alpha
    { std::tuple<std::string, char> a; REQUIRE(parse("abcd", string("abc") >> alpha, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 'd'); }
    // string("abc") x string("def")
    { std::tuple<std::string, std::string> a; REQUIRE(parse("abcdef", string("abc") >> string("def"), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == "def"); }
    // string("abc") x string("def") → (SES<string>, SES<string>)
    { std::tuple<SES<std::string>, SES<std::string>> a; REQUIRE(parse("abcdef", string("abc") >> string("def"), a)); CHECK(alloy::get<0>(a).value == "abc"); CHECK(alloy::get<1>(a).value == "def"); }
    // string("abc") x +alpha
    { std::tuple<std::string, std::string> a; REQUIRE(parse("abcdef", string("abc") >> +alpha, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == "def"); }
}

TEST_CASE("SES composition: left +alpha")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // +alpha x int_
    { std::tuple<std::string, int> a; REQUIRE(parse("abc42", +alpha >> int_, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 42); }
    // +alpha x int_ → (SES<string>, int)
    { std::tuple<SES<std::string>, int> a; REQUIRE(parse("abc42", +alpha >> int_, a)); CHECK(alloy::get<0>(a).value == "abc"); CHECK(alloy::get<1>(a) == 42); }
    // +alpha x int_ → (string, SES<int>)
    { std::tuple<std::string, SES<int>> a; REQUIRE(parse("abc42", +alpha >> int_, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a).value == 42); }
    // +alpha x attr(SES)
    { std::tuple<std::string, int> a; REQUIRE(parse("abc", +alpha >> x4::attr(SES<int>{2}), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 2); }
    // +alpha x variant
    { std::tuple<std::string, int> a; REQUIRE(parse("abc1", +alpha >> (int_ | char_), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 1); }
    // +alpha x ',' >> alpha
    { std::tuple<std::string, char> a; REQUIRE(parse("abc,d", +alpha >> ',' >> alpha, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 'd'); }
    // +alpha x ',' >> +alpha
    { std::tuple<std::string, std::string> a; REQUIRE(parse("abc,def", +alpha >> ',' >> +alpha, a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == "def"); }
    // +alpha x ',' >> +alpha → (SES<string>, SES<string>)
    { std::tuple<SES<std::string>, SES<std::string>> a; REQUIRE(parse("abc,def", +alpha >> ',' >> +alpha, a)); CHECK(alloy::get<0>(a).value == "abc"); CHECK(alloy::get<1>(a).value == "def"); }
}

TEST_CASE("SES composition: SES dest wrapping")
{
    using x4::int_;
    using x4::alpha;
    using x4::alnum;
    using x4::string;
    using x4::standard::char_;

    // --- SES dest wrapping sequence result (core fix) ---
    // Exercises the parse_sequence unwrap path:
    //   is_single_element_tuple_like<Attr> && has_attribute_v<left> && has_attribute_v<right>

    // SES<container>: alpha >> *alnum → SES<string> (= Ident pattern)
    { SES<std::string> a; REQUIRE(parse("abc", alpha >> *alnum, a)); CHECK(a.value == "abc"); }
    // SES<MET>: int_ >> ',' >> int_ → SES<TwoInts>
    { SES<TwoInts> a{}; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(a.value.a == 1); CHECK(a.value.b == 2); }
    // SES<tuple>: int_ >> ',' >> int_ → SES<tuple<int, int>>
    { SES<std::tuple<int, int>> a; REQUIRE(parse("1,2", int_ >> ',' >> int_, a)); CHECK(alloy::get<0>(a.value) == 1); CHECK(alloy::get<1>(a.value) == 2); }
    // SES<MET> with variant parser
    { SES<TwoInts> a{}; REQUIRE(parse("1,2", (int_ >> ',' >> int_) | (char_ >> ',' >> char_), a)); CHECK(a.value.a == 1); CHECK(a.value.b == 2); }
}

TEST_CASE("SES composition: MET parsers")
{
    using x4::int_;
    using x4::alpha;
    using x4::string;
    using x4::standard::char_;

    // MET x int_ → 3-slot tuple
    { std::tuple<int, int, int> a; REQUIRE(parse("1,2:3", (int_ >> ',' >> int_) >> ':' >> int_, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == 3); }
    // MET x alpha → 3-slot tuple
    { std::tuple<int, int, char> a; REQUIRE(parse("1,2a", (int_ >> ',' >> int_) >> alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == 'a'); }
    // MET x +alpha → 3-slot tuple
    { std::tuple<int, int, std::string> a; REQUIRE(parse("1,2abc", (int_ >> ',' >> int_) >> +alpha, a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == "abc"); }
    // MET x attr(SES) → 3-slot tuple
    { std::tuple<int, int, int> a; REQUIRE(parse("1,2", (int_ >> ',' >> int_) >> x4::attr(SES<int>{99}), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == 99); }
    // int_ x MET → 3-slot tuple
    { std::tuple<int, int, int> a; REQUIRE(parse("1:2,3", int_ >> ':' >> (int_ >> ',' >> int_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == 3); }
    // alpha x MET → 3-slot tuple
    { std::tuple<char, int, int> a; REQUIRE(parse("a1,2", alpha >> (int_ >> ',' >> int_), a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a) == 1); CHECK(alloy::get<2>(a) == 2); }
    // +alpha x MET → 3-slot tuple
    { std::tuple<std::string, int, int> a; REQUIRE(parse("abc1,2", +alpha >> (int_ >> ',' >> int_), a)); CHECK(alloy::get<0>(a) == "abc"); CHECK(alloy::get<1>(a) == 1); CHECK(alloy::get<2>(a) == 2); }
    // attr(SES) x MET → 3-slot tuple
    { std::tuple<int, int, int> a; REQUIRE(parse("1,2", x4::attr(SES<int>{99}) >> (int_ >> ',' >> int_), a)); CHECK(alloy::get<0>(a) == 99); CHECK(alloy::get<1>(a) == 1); CHECK(alloy::get<2>(a) == 2); }
    // MET x MET → 4-slot tuple
    { std::tuple<int, int, int, int> a; REQUIRE(parse("1,2:3,4", (int_ >> ',' >> int_) >> ':' >> (int_ >> ',' >> int_), a)); CHECK(alloy::get<0>(a) == 1); CHECK(alloy::get<1>(a) == 2); CHECK(alloy::get<2>(a) == 3); CHECK(alloy::get<3>(a) == 4); }
    // MET_variant x alpha → 2-slot tuple (alternative has sequence_size=1)
    { std::tuple<TwoInts, char> a; REQUIRE(parse("1,2a", ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)) >> alpha, a)); CHECK(alloy::get<0>(a).a == 1); CHECK(alloy::get<0>(a).b == 2); CHECK(alloy::get<1>(a) == 'a'); }
    // alpha x MET_variant → 2-slot tuple
    { std::tuple<char, TwoInts> a; REQUIRE(parse("a1,2", alpha >> ((int_ >> ',' >> int_) | (char_ >> ',' >> char_)), a)); CHECK(alloy::get<0>(a) == 'a'); CHECK(alloy::get<1>(a).a == 1); CHECK(alloy::get<1>(a).b == 2); }

    // --- Nested SES: recursive unwrap ---
    // TODO: these hit a noexcept evaluation issue in move_to (recursive
    //       single-element-tuple-like forwarding through two layers).
    //       Uncomment once that is resolved.

    // SES<SES<int>>: double unwrap through move_to
    // {
    //     SES<SES<int>> a;
    //     REQUIRE(parse("42", int_, a));
    //     CHECK(a.value.value == 42);
    // }
    // SES<SES<string>>: double unwrap through parse_sequence (= Var pattern)
    // {
    //     SES<SES<std::string>> a;
    //     REQUIRE(parse("abc", alpha >> *alnum, a));
    //     CHECK(a.value.value == "abc");
    // }
}
