/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/core/write_attribute.hpp>

#include <iris/alloy/tuple.hpp>
#include <iris/alloy/adapt.hpp>
#include <iris/rvariant.hpp>
#include <iris/rvariant/rvariant_io.hpp>

#include <iris/x4/core/traits/write_rank.hpp>
#include <iris/x4/core/detail/parse_into_container.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/operator/delimited_list.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/optional.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/rule.hpp>

#include <filesystem>
#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using iris::rvariant;
using iris::recursive_wrapper;
using x4::write_rank;

// the types of the requirements

struct Port
{
    Port(int value) : value(value) {}
    int value;
    bool operator==(Port const&) const = default;
};

struct Shape
{
    Shape(int) {}
    Shape(std::initializer_list<long long>) {}
};

struct Id
{
    explicit Id(int) {}
    Id& operator=(int) { return *this; }
};

struct Wrap
{
    template<class T>
    Wrap(T&& t) : v(t) {}  // NOLINT(bugprone-forwarding-reference-overload, cppcoreguidelines-missing-std-forward)
    int v;
    bool operator==(Wrap const&) const = default;
};

struct NoDefault
{
    NoDefault(int value) : value(value) {}
    int value;
    bool operator==(NoDefault const&) const = default;
};

struct Ctor
{
    Ctor(int) {}
    Ctor& operator=(int) = delete;
};

struct Span
{
    Span(int first, int count) : first(first), last(first + count) {}
    int first, last;
    bool operator==(Span const&) const = default;
};

struct Paren;
using Expr = rvariant<int, recursive_wrapper<Paren>>;

struct Paren
{
    Expr inner;
    bool operator==(Paren const&) const = default;
};

struct Ident
{
    std::string name;
    bool operator==(Ident const&) const = default;
};

struct Assign { Ident lhs; Expr rhs; };
struct Compare { Ident lhs; Expr rhs; };

struct Literal
{
    long long value = 0;
    bool operator==(Literal const&) const = default;
};

struct Real { double value = 0; };
struct Count { int value = 0; };

struct Ints
{
    std::vector<int> values;
    bool operator==(Ints const&) const = default;
};

struct ReturnStmt { Expr value; };
struct ExprStmt { Expr value; };
struct Number { rvariant<long long, double> value; };

struct Point
{
    int x = 0, y = 0;
    bool operator==(Point const&) const = default;
};

// a single-element tuple-like of a single-element tuple-like of its own value
struct LongLongNumber { long long value = 0; };
struct VariantNumber { rvariant<int, long long> value; };

// recursive types

struct binop;
using operand = rvariant<int, recursive_wrapper<binop>>;

struct binop
{
    operand l;
    char op = 0;
    operand r;
    bool operator==(binop const&) const = default;
};

struct Tree
{
    int v = 0;
    std::vector<Tree> kids;
};

struct Items
{
    std::vector<Items> items;
};

struct Box;
using boxed = rvariant<std::optional<recursive_wrapper<Box>>>;
struct Box { boxed inner; };

struct Single { int n = 0; };

struct BoxOrSingle;
using box_or_single = rvariant<std::optional<recursive_wrapper<BoxOrSingle>>, Single>;
struct BoxOrSingle { box_or_single inner; };

struct BoxOrOptional;
using box_or_optional = rvariant<std::optional<recursive_wrapper<BoxOrOptional>>, std::optional<int>>;
struct BoxOrOptional { box_or_optional inner; };

// each writes an int into the other before its own single-element struct: the selections return
// to each other with the same value, a conflict
struct SingleA { int n = 0; };
struct SingleB { int n = 0; };
struct CycleBoxA;
struct CycleBoxB;
using cycle_a = rvariant<std::optional<recursive_wrapper<CycleBoxB>>, SingleA>;
using cycle_b = rvariant<std::optional<recursive_wrapper<CycleBoxA>>, SingleB>;
struct CycleBoxA { cycle_a inner; };
struct CycleBoxB { cycle_b inner; };
struct HoldsCycleA { cycle_a a; };

// agree_b converts before it writes into agree_a: no conflict
struct AgreeBoxA;
struct AgreeBoxB;
using agree_a = rvariant<std::optional<recursive_wrapper<AgreeBoxB>>, SingleA>;
using agree_b = rvariant<std::optional<recursive_wrapper<AgreeBoxA>>, long long>;
struct AgreeBoxA { agree_a inner; };
struct AgreeBoxB { agree_b inner; };

// both optionals of tie_m take an int, one of them through tie_n
struct TieBoxM;
struct TieBoxN;
using tie_n = rvariant<std::optional<recursive_wrapper<TieBoxM>>, Single>;
using tie_m = rvariant<std::optional<int>, std::optional<recursive_wrapper<TieBoxN>>>;
struct TieBoxM { tie_m inner; };
struct TieBoxN { tie_n inner; };

// X = rvariant<int, rw<vector<X>>>
struct IntArray;
using int_node = rvariant<int, recursive_wrapper<IntArray>>;

struct IntArray : std::vector<int_node>
{
    using std::vector<int_node>::vector;
};

// X = rvariant<int, string, rw<vector<X>>>
struct StringArray;
using string_node = rvariant<int, std::string, recursive_wrapper<StringArray>>;

struct StringArray : std::vector<string_node>
{
    using std::vector<string_node>::vector;
};

struct X {};
struct Y {};
struct Z {};
struct Xs { std::vector<X> xs; };

// an AST whose statements and expressions recur into each other
namespace ast {

struct Binary;
struct Unary;
struct Call;
struct Lambda;
struct Name { std::string text; };
using Expr = rvariant<long long, double, std::string, Name, recursive_wrapper<Binary>, recursive_wrapper<Unary>, recursive_wrapper<Call>, recursive_wrapper<Lambda>>;
struct Binary { Expr lhs; char op = 0; Expr rhs; };
struct Unary { char op = 0; Expr operand; };
struct Call { Expr callee; std::vector<Expr> args; };
struct Assign { Name lhs; Expr rhs; };
struct If;
struct While;
struct Block;
using Stmt = rvariant<Expr, Assign, recursive_wrapper<If>, recursive_wrapper<While>, recursive_wrapper<Block>>;
struct Block { std::vector<Stmt> stmts; };
struct If { Expr cond; Block then; std::optional<Block> otherwise; };
struct While { Expr cond; Block body; };
struct Lambda { std::vector<Name> params; Block body; };

} // ast

template<class S, class V>
constexpr write_rank rank = x4::write_rank_v<S&, V>;

// `value` written as an rvalue and as a `const&` into copies of `initial`
template<class S, class V>
void check_write(S const& initial, V const& value, S const& expected)
{
    {
        S s = initial;
        V v = value;
        x4::write_attribute(s, std::move(v));
        CHECK(s == expected);
    }
    {
        S s = initial;
        x4::write_attribute(s, value);
        CHECK(s == expected);
    }
}

IRIS_ALLOY_ADAPT_STRUCT(Span, first, last);
IRIS_ALLOY_ADAPT_STRUCT(Paren, inner);
IRIS_ALLOY_ADAPT_STRUCT(Ident, name);
IRIS_ALLOY_ADAPT_STRUCT(Assign, lhs, rhs);
IRIS_ALLOY_ADAPT_STRUCT(Compare, lhs, rhs);
IRIS_ALLOY_ADAPT_STRUCT(Literal, value);
IRIS_ALLOY_ADAPT_STRUCT(Real, value);
IRIS_ALLOY_ADAPT_STRUCT(Count, value);
IRIS_ALLOY_ADAPT_STRUCT(Ints, values);
IRIS_ALLOY_ADAPT_STRUCT(ReturnStmt, value);
IRIS_ALLOY_ADAPT_STRUCT(ExprStmt, value);
IRIS_ALLOY_ADAPT_STRUCT(Number, value);
IRIS_ALLOY_ADAPT_STRUCT(Point, x, y);
IRIS_ALLOY_ADAPT_STRUCT(LongLongNumber, value);
IRIS_ALLOY_ADAPT_STRUCT(VariantNumber, value);
IRIS_ALLOY_ADAPT_STRUCT(binop, l, op, r);
IRIS_ALLOY_ADAPT_STRUCT(Tree, v, kids);
IRIS_ALLOY_ADAPT_STRUCT(Items, items);
IRIS_ALLOY_ADAPT_STRUCT(Box, inner);
IRIS_ALLOY_ADAPT_STRUCT(Single, n);
IRIS_ALLOY_ADAPT_STRUCT(BoxOrSingle, inner);
IRIS_ALLOY_ADAPT_STRUCT(BoxOrOptional, inner);
IRIS_ALLOY_ADAPT_STRUCT(SingleA, n);
IRIS_ALLOY_ADAPT_STRUCT(SingleB, n);
IRIS_ALLOY_ADAPT_STRUCT(CycleBoxA, inner);
IRIS_ALLOY_ADAPT_STRUCT(CycleBoxB, inner);
IRIS_ALLOY_ADAPT_STRUCT(HoldsCycleA, a);
IRIS_ALLOY_ADAPT_STRUCT(AgreeBoxA, inner);
IRIS_ALLOY_ADAPT_STRUCT(AgreeBoxB, inner);
IRIS_ALLOY_ADAPT_STRUCT(TieBoxM, inner);
IRIS_ALLOY_ADAPT_STRUCT(TieBoxN, inner);
IRIS_ALLOY_ADAPT_STRUCT(Xs, xs);
IRIS_ALLOY_ADAPT_STRUCT(ast::Name, text);
IRIS_ALLOY_ADAPT_STRUCT(ast::Binary, lhs, op, rhs);
IRIS_ALLOY_ADAPT_STRUCT(ast::Unary, op, operand);
IRIS_ALLOY_ADAPT_STRUCT(ast::Call, callee, args);
IRIS_ALLOY_ADAPT_STRUCT(ast::Assign, lhs, rhs);
IRIS_ALLOY_ADAPT_STRUCT(ast::Block, stmts);
IRIS_ALLOY_ADAPT_STRUCT(ast::If, cond, then, otherwise);
IRIS_ALLOY_ADAPT_STRUCT(ast::While, cond, body);
IRIS_ALLOY_ADAPT_STRUCT(ast::Lambda, params, body);

TEST_CASE("write_rank")
{
    constexpr auto none = write_rank::none;
    constexpr auto converts = write_rank::assignable_without_narrowing;
    constexpr auto structural = write_rank::structural;
    constexpr auto exact = write_rank::exact;

    // plain: the assignment of the language without narrowing
    STATIC_CHECK(rank<int, int> == exact);
    STATIC_CHECK(rank<long long, int> == converts);
    STATIC_CHECK(rank<int, char> == converts);
    STATIC_CHECK(rank<int, long long> == none);
    STATIC_CHECK(rank<Port, int> == converts);
    // narrowed into the parameter of a user-defined conversion or assignment operator: the responsibility of the type
    STATIC_CHECK(rank<Port, long long> == converts);
    STATIC_CHECK(rank<Shape, long long> == converts);
    STATIC_CHECK(rank<Id, long long> == converts);
    STATIC_CHECK(rank<Wrap, long long> == converts);
    STATIC_CHECK(rank<char32_t, char> == none);
    STATIC_CHECK(rank<std::string_view, std::string> == none); // would dangle
    STATIC_CHECK(rank<std::string_view, std::string&> == converts);
    STATIC_CHECK(rank<std::unique_ptr<int>, std::unique_ptr<int>> == exact);
    STATIC_CHECK(rank<std::unique_ptr<int>, std::unique_ptr<int> const&> == none);
    STATIC_CHECK(rank<std::filesystem::path, std::filesystem::path> == exact); // not a range of itself

    // container: appending a range
    STATIC_CHECK(rank<std::vector<int>, std::vector<int>> == structural);
    STATIC_CHECK(rank<std::string, std::string const&> == structural);
    STATIC_CHECK(rank<std::vector<long long>, std::vector<int>> == converts);
    STATIC_CHECK(rank<std::vector<int>, int> == none);
    STATIC_CHECK(rank<std::string, char> == none);
    STATIC_CHECK(rank<std::vector<std::string>, std::vector<char>> == structural);
    STATIC_CHECK(rank<std::vector<std::string>, std::vector<std::vector<char>>> == structural);
    STATIC_CHECK(rank<std::vector<rvariant<int, std::string>>, std::string> == structural);
    STATIC_CHECK(rank<std::vector<rvariant<int, std::vector<int>>>, std::vector<int>> == structural);
    STATIC_CHECK(rank<std::vector<bool>, std::vector<bool>> == converts); // through the proxy reference
    STATIC_CHECK(rank<std::map<int, std::string>, std::vector<std::pair<int, std::string>>> == structural);
    STATIC_CHECK(rank<std::vector<Span>, std::vector<Span>> == structural);
    STATIC_CHECK(rank<std::vector<char>, std::optional<char>> == none);
    STATIC_CHECK(rank<std::vector<int>, std::vector<long long>> == none);

    // tuple-like: member by member; a single-element tuple-like holds its element
    STATIC_CHECK(rank<Point, alloy::tuple<int, int>> == structural);
    STATIC_CHECK(rank<Paren, int> == structural);
    STATIC_CHECK(rank<int, alloy::tuple<int>> == none);
    STATIC_CHECK(rank<Point, alloy::tuple<long long, int>> == none);
    STATIC_CHECK(rank<Expr, Paren> == structural); // the alternative of its own type
    STATIC_CHECK(rank<int, Paren> == none);

    // optional
    STATIC_CHECK(rank<std::optional<int>, int> == structural);
    STATIC_CHECK(rank<std::optional<NoDefault>, NoDefault> == structural);
    STATIC_CHECK(rank<std::optional<Span>, Span> == structural);
    STATIC_CHECK(rank<std::optional<std::vector<int>>, std::vector<int>> == structural);
    STATIC_CHECK(rank<std::optional<std::vector<int>>, int> == none);
    STATIC_CHECK(rank<std::optional<Ctor>, int> == none);
    STATIC_CHECK(rank<std::optional<long long>, std::optional<int>> == converts);
    STATIC_CHECK(rank<std::optional<int>, long long> == none);
    STATIC_CHECK(rank<char, std::optional<char>> == structural);

    // rvariant: the same type, a conversion, by the shape, then wrapped
    STATIC_CHECK(rank<rvariant<std::string, std::vector<char>>, std::vector<char>> == structural);
    STATIC_CHECK(rank<rvariant<char, long long>, int> == converts);
    STATIC_CHECK(rank<rvariant<char, short>, int> == none);
    STATIC_CHECK(rank<rvariant<std::vector<int>, long long>, int> == converts);
    STATIC_CHECK(rank<rvariant<std::string, int>, char> == converts);
    STATIC_CHECK(rank<rvariant<std::optional<int>, long long>, int> == converts);
    STATIC_CHECK(rank<rvariant<Assign, Compare>, alloy::tuple<Ident, Expr>> == none);
    STATIC_CHECK(rank<rvariant<int, NoDefault>, NoDefault> == structural);
    STATIC_CHECK(rank<rvariant<std::string, Span>, alloy::tuple<int, int>> == none);
    STATIC_CHECK(rank<rvariant<int, int>, int> == structural);
    STATIC_CHECK(rank<rvariant<long, long>, int> == none); // ambiguous
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4244)
#endif
    STATIC_CHECK(rank<rvariant<Port, int>, long long> == converts); // Port
#ifdef _MSC_VER
# pragma warning(pop)
#endif
    STATIC_CHECK(rank<rvariant<Port, long long>, int> == converts); // long long, by a standard conversion
    STATIC_CHECK(rank<rvariant<Port, Wrap>, int> == none); // ambiguous, both by a user-defined conversion

    // rvariant from an rvariant: into the same type, alternative by alternative, then wrapped
    STATIC_CHECK(rank<rvariant<int, rvariant<int, char>>, rvariant<int, char>> == structural);
    STATIC_CHECK(rank<rvariant<long long, std::string>, rvariant<int, char>> == converts);
    STATIC_CHECK(rank<rvariant<char, std::string>, rvariant<int, std::string>> == none);
    STATIC_CHECK(rank<rvariant<LongLongNumber, VariantNumber>, rvariant<int, long long>> == none); // each state ambiguous

    // wrapped only when the value becomes the element as it is or by a standard conversion
    STATIC_CHECK(rank<rvariant<std::string, Literal>, int> == converts);
    STATIC_CHECK(rank<rvariant<long long, Literal>, int> == converts);
    STATIC_CHECK(rank<rvariant<Literal, Real>, long long> == structural);
    STATIC_CHECK(rank<rvariant<Port, Literal>, long long> == converts); // Port, as a conversion precedes wrapping
    STATIC_CHECK(rank<rvariant<std::string, Count>, double> == none);
    STATIC_CHECK(rank<rvariant<std::string, Ints>, std::vector<int>> == structural);
    STATIC_CHECK(rank<rvariant<char, Paren>, int> == none);
    STATIC_CHECK(rank<rvariant<ReturnStmt, ExprStmt>, Expr> == none);
    STATIC_CHECK(rank<rvariant<Number, Ident>, rvariant<long long, double, Ident>> == none);
    STATIC_CHECK(rank<rvariant<X, Y, Z, Xs>, std::vector<X>> == structural);

    // recursive types
    STATIC_CHECK(rank<binop, binop> == structural);
    STATIC_CHECK(rank<std::vector<int>, std::vector<Tree>> == structural); // flattened
    STATIC_CHECK(rank<Items, std::vector<int>> == none);
    STATIC_CHECK(rank<Items, std::vector<Items>> == structural);
    STATIC_CHECK(rank<boxed, int> == none); // an optional in itself without an end
    STATIC_CHECK(rank<box_or_single, int> == structural);
    STATIC_CHECK(rank<box_or_optional, int> == structural);
    STATIC_CHECK(rank<cycle_a, int> == none); // a conflict
    STATIC_CHECK(rank<cycle_b, int> == none);
    STATIC_CHECK(rank<HoldsCycleA, int> == none); // into the conflict
    STATIC_CHECK(rank<std::optional<cycle_a>, int> == none);
    STATIC_CHECK(rank<std::vector<cycle_a>, std::vector<int>> == none);
    STATIC_CHECK(rank<cycle_a, SingleA> == structural);
    STATIC_CHECK(rank<cycle_a, CycleBoxB> == structural);
    STATIC_CHECK(rank<agree_a, int> == converts);
    STATIC_CHECK(rank<tie_m, int> == none); // ambiguous
    STATIC_CHECK(rank<tie_n, int> == none); // into tie_m
    STATIC_CHECK(rank<IntArray, std::vector<int>> == structural);
    STATIC_CHECK(rank<int_node, std::vector<int>> == structural);
    STATIC_CHECK(rank<StringArray, std::string> == structural);
    STATIC_CHECK(rank<ast::Stmt, ast::Stmt> == structural);
    STATIC_CHECK(rank<ast::Expr, ast::Binary> == structural);
    STATIC_CHECK(rank<ast::Stmt, int> == converts);

    // not an attribute
    STATIC_CHECK(rank<unused_type, int> == none);
    STATIC_CHECK(rank<int, unused_type> == none);
}

TEST_CASE("write_attribute")
{
    // plain
    check_write(0LL, 42, 42LL);
    check_write(0, 'c', 99);
    check_write(Port{0}, 3, Port{3});
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4244)
#endif
    check_write(Port{0}, 5LL, Port{5});
    check_write(Wrap{0}, 7LL, Wrap{7});
#ifdef _MSC_VER
# pragma warning(pop)
#endif

    // container: appended, never replaced
    check_write(std::vector<int>{1, 2}, std::vector<int>{3}, std::vector<int>{1, 2, 3});
    check_write(std::string("x"), std::string("ab"), std::string("xab"));
    check_write(std::set<int>{1, 5}, std::vector<int>{3, 5}, std::set<int>{1, 3, 5});
    check_write(std::vector<std::string>{"x"}, std::vector<char>{'a', 'b'}, std::vector<std::string>{"x", "ab"});
    check_write(std::vector<std::string>{}, std::vector<std::vector<char>>{{'a', 'b'}, {'c'}}, std::vector<std::string>{"ab", "c"});
    check_write(std::vector<rvariant<int, std::string>>{}, std::string("abc"), std::vector<rvariant<int, std::string>>{std::string("abc")});
    check_write(
        std::vector<rvariant<int, std::vector<int>>>{},
        std::vector<int>{1, 2},
        std::vector<rvariant<int, std::vector<int>>>{std::vector<int>{1, 2}}
    );
    check_write(
        std::vector<std::string>{},
        std::vector<alloy::tuple<char, char>>{{'a', '1'}, {'b', '2'}},
        std::vector<std::string>{"a1b2"}
    );
    check_write(std::vector<int>{}, std::vector<alloy::tuple<int, int>>{{1, 2}, {3, 4}}, std::vector<int>{1, 2, 3, 4});
    check_write(std::vector<char>{}, std::vector<std::optional<char>>{'a', std::nullopt, 'b'}, std::vector<char>{'a', 'b'});
    check_write(std::vector<Span>{}, std::vector<Span>{Span(1, 4)}, std::vector<Span>{Span(1, 4)});

    // tuple-like
    check_write(Point{}, alloy::tuple<int, int>{1, 2}, Point{1, 2});
    check_write(Paren{}, 1, Paren{Expr{1}});

    // optional: into the content if any, else a new one
    check_write(std::optional<int>{}, 1, std::optional<int>{1});
    check_write(std::optional<int>{5}, 1, std::optional<int>{1});
    check_write(std::optional<NoDefault>{}, NoDefault{3}, std::optional<NoDefault>{NoDefault{3}});
    check_write(std::optional<Span>{}, Span(1, 4), std::optional<Span>{Span(1, 4)});
    check_write(std::optional<std::vector<int>>{{1, 2}}, std::vector<int>{3}, std::optional<std::vector<int>>{{1, 2, 3}});
    check_write(std::optional<int>{5}, std::optional<int>{}, std::optional<int>{});
    check_write(std::optional<long long>{}, std::optional<int>{7}, std::optional<long long>{7});
    check_write('x', std::optional<char>{}, 'x');
    check_write('x', std::optional<char>{'c'}, 'c');

    // rvariant
    using string_or_chars = rvariant<std::string, std::vector<char>>;
    check_write(string_or_chars{}, std::vector<char>{'a'}, string_or_chars{std::vector<char>{'a'}});
    check_write(rvariant<char, long long>{}, 42, rvariant<char, long long>{42LL});
    check_write(rvariant<std::vector<int>, long long>{}, 42, rvariant<std::vector<int>, long long>{42LL});
    check_write(rvariant<std::string, int>{}, 'c', rvariant<std::string, int>{99});
    check_write(rvariant<std::optional<int>, long long>{}, 42, rvariant<std::optional<int>, long long>{42LL});
    check_write(rvariant<int, NoDefault>{}, NoDefault{3}, rvariant<int, NoDefault>{NoDefault{3}});
    check_write(rvariant<int, int>{}, 3, rvariant<int, int>{std::in_place_index<0>, 3});
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4244)
#endif
    check_write(rvariant<Port, int>{0}, 5LL, rvariant<Port, int>{Port{5}});
    check_write(rvariant<Port, long long>{0LL}, 5, rvariant<Port, long long>{5LL});
    check_write(rvariant<Port, Literal>{Literal{}}, 5LL, rvariant<Port, Literal>{Port{5}});
#ifdef _MSC_VER
# pragma warning(pop)
#endif

    using nested = rvariant<int, rvariant<int, char>>;
    check_write(nested{}, rvariant<int, char>{'a'}, nested{rvariant<int, char>{'a'}});
    check_write(rvariant<long long, std::string>{}, rvariant<int, char>{'b'}, rvariant<long long, std::string>{98LL});
    check_write(rvariant<long long, std::string>{}, rvariant<int, char>{1}, rvariant<long long, std::string>{1LL});

    check_write(rvariant<std::string, Literal>{}, 5, rvariant<std::string, Literal>{Literal{5}});
    check_write(rvariant<long long, Literal>{}, 5, rvariant<long long, Literal>{5LL});
    check_write(rvariant<std::string, Ints>{}, std::vector<int>{1, 2}, rvariant<std::string, Ints>{Ints{{1, 2}}});
    {
        rvariant<Literal, Real> s;
        x4::write_attribute(s, 7LL);
        CHECK(iris::get<Literal>(s).value == 7);
    }
    {
        rvariant<X, Y, Z, Xs> s;
        x4::write_attribute(s, std::vector<X>(2));
        CHECK(iris::get<Xs>(s).xs.size() == 2);
    }

    // the alternative held is written into
    using ints_or_int = rvariant<int, std::vector<int>>;
    check_write(ints_or_int{std::vector<int>{1}}, std::vector<int>{2}, ints_or_int{std::vector<int>{1, 2}});
    check_write(rvariant<std::string, Ints>{Ints{{1}}}, std::vector<int>{2}, rvariant<std::string, Ints>{Ints{{1, 2}}});

    // recursive types
    {
        binop const value{1, '+', recursive_wrapper<binop>(binop{2, '*', 3})};
        check_write(binop{}, value, value);
    }
    {
        std::vector<Tree> const tree{Tree{1, {Tree{2, {}}, Tree{3, {Tree{4, {}}}}}}, Tree{5, {}}};
        check_write(std::vector<int>{}, tree, std::vector<int>{1, 2, 3, 4, 5});
    }
    {
        box_or_single s;
        x4::write_attribute(s, 1);
        CHECK(iris::get<Single>(s).n == 1);
    }
    {
        box_or_optional s;
        x4::write_attribute(s, 1);
        CHECK(iris::get<std::optional<int>>(s) == 1);
    }
    {
        cycle_a s;
        x4::write_attribute(s, CycleBoxB{cycle_b{SingleB{1}}});
        CHECK(iris::get<SingleB>((*iris::get<0>(s))->inner).n == 1);
    }
    {
        agree_a s;
        x4::write_attribute(s, 1);
        CHECK(iris::get<long long>((*iris::get<0>(s))->inner) == 1);
    }
    check_write(IntArray{}, std::vector<int>{1, 2}, IntArray{int_node{1}, int_node{2}});
    check_write(int_node{}, std::vector<int>{1, 2}, int_node{IntArray{int_node{1}, int_node{2}}});
    check_write(StringArray{}, std::string("ab"), StringArray{string_node{std::string("ab")}});
    {
        ast::Stmt s;
        x4::write_attribute(s, ast::Stmt{recursive_wrapper<ast::While>(ast::While{ast::Expr{1LL}, ast::Block{{ast::Stmt{ast::Expr{2.5}}}}})});
        auto const& loop = iris::get<ast::While>(s);
        CHECK(iris::get<long long>(loop.cond) == 1);
        CHECK(iris::get<double>(iris::get<ast::Expr>(loop.body.stmts.at(0))) == 2.5);
    }
}


// - Can be default constructed
// - Can be constructed from `int`
// - Can assign `int`
struct WeakNumber
{
    WeakNumber() = default;
    WeakNumber(int) : constructed_from_int_and_never_reassigned(true) {}
    WeakNumber& operator=(int) { constructed_from_int_and_never_reassigned = false; return *this; }
    bool constructed_from_int_and_never_reassigned = false;
};

// - Can be default constructed
// - Can NOT be converted from `int` as constructor being `explicit`
// - Can assign `int`
struct StrongNumber
{
    StrongNumber() = default;
    explicit StrongNumber(int) {}
    StrongNumber& operator=(int) { return *this; }
};

struct ID_Param
{
    WeakNumber id;
    std::optional<WeakNumber> param;
};
IRIS_ALLOY_ADAPT_STRUCT(ID_Param, id, param);

TEST_CASE("new object")
{
    using x4::int_;
    using x4::is_writable_v;

    // A new plain object is constructed from the value, by `write_attribute` and while parsing alike
    {
        std::vector<WeakNumber> v;
        x4::write_attribute(v, std::vector<int>{1});
        CHECK(v[0].constructed_from_int_and_never_reassigned);
    }
    {
        std::vector<WeakNumber> v;
        REQUIRE(parse("1", int_ % ',', v));
        CHECK(v[0].constructed_from_int_and_never_reassigned);
    }
    {
        std::optional<WeakNumber> o;
        x4::write_attribute(o, 1);
        REQUIRE(o.has_value());
        CHECK(o->constructed_from_int_and_never_reassigned);  // NOLINT(bugprone-unchecked-optional-access)
    }
    {
        std::optional<WeakNumber> o;
        REQUIRE(parse("1", -int_, o));
        REQUIRE(o.has_value());
        CHECK(o->constructed_from_int_and_never_reassigned);  // NOLINT(bugprone-unchecked-optional-access)
    }
    {
        std::optional<WeakNumber> o;
        REQUIRE(parse("1", int_, o));
        REQUIRE(o.has_value());
        CHECK(o->constructed_from_int_and_never_reassigned);  // NOLINT(bugprone-unchecked-optional-access)
    }
    {
        rvariant<std::string, WeakNumber> var;
        x4::write_attribute(var, 1);
        CHECK(iris::get<1>(var).constructed_from_int_and_never_reassigned);
    }
    {
        rvariant<std::string, WeakNumber> var;
        REQUIRE(parse("1", int_, var));
        CHECK(iris::get<1>(var).constructed_from_int_and_never_reassigned);
    }

    {
        std::optional<rvariant<WeakNumber, std::string>> o;
        x4::write_attribute(o, 1);
        REQUIRE(o.has_value());
        CHECK(iris::get<0>(*o).constructed_from_int_and_never_reassigned);  // NOLINT(bugprone-unchecked-optional-access)
    }
    {
        std::vector<rvariant<WeakNumber, std::string>> v;
        REQUIRE(parse("1", int_ % ',', v));
        CHECK(iris::get<0>(v[0]).constructed_from_int_and_never_reassigned);
    }
    {
        rvariant<std::string, recursive_wrapper<rvariant<WeakNumber, std::string>>> var;
        x4::write_attribute(var, 1);
        CHECK(iris::get<0>(iris::get<1>(var)).constructed_from_int_and_never_reassigned);
    }
    {
        std::vector<rvariant<WeakNumber, std::string>> v;
        x4::write_attribute(v, std::vector<rvariant<int, std::string>>{1});
        CHECK(iris::get<0>(v[0]).constructed_from_int_and_never_reassigned);
    }

    // An existing one is assigned
    {
        std::optional<WeakNumber> o{std::in_place};
        x4::write_attribute(o, 1);
        CHECK(!o->constructed_from_int_and_never_reassigned);
    }
    {
        rvariant<std::string, WeakNumber> var{std::in_place_index<1>};
        x4::write_attribute(var, 1);
        CHECK(!iris::get<1>(var).constructed_from_int_and_never_reassigned);
    }
    {
        // The default state holds `WeakNumber`, as `std::variant<WeakNumber, std::string> v; v = 1;` assigns
        rvariant<WeakNumber, std::string> var;
        REQUIRE(parse("1", int_, var));
        CHECK(!iris::get<0>(var).constructed_from_int_and_never_reassigned);
    }
    {
        ID_Param idp;
        x4::write_attribute(idp, alloy::tuple<int, int>{1, 2});
        CHECK(!idp.id.constructed_from_int_and_never_reassigned);
        REQUIRE(idp.param.has_value());
        CHECK(idp.param->constructed_from_int_and_never_reassigned);  // NOLINT(bugprone-unchecked-optional-access)
    }
    {
        ID_Param idp;
        REQUIRE(parse("1,2", int_ >> ',' >> int_, idp));
        CHECK(!idp.id.constructed_from_int_and_never_reassigned);
        REQUIRE(idp.param.has_value());
        CHECK(idp.param->constructed_from_int_and_never_reassigned);  // NOLINT(bugprone-unchecked-optional-access)
    }

    // No default constructor is needed
    check_write(std::vector<Port>{}, std::vector<int>{1, 2}, std::vector<Port>{Port{1}, Port{2}});
    {
        std::vector<Port> v;
        REQUIRE(parse("1,2", int_ % ',', v));
        CHECK(v == std::vector<Port>{Port{1}, Port{2}});
    }
    {
        constexpr auto ports = x4::rule<struct ports_rule, std::vector<Port>>{"ports"} = *(int_ >> ';');
        std::vector<Port> v;
        REQUIRE(parse("1;2;", ports, v));
        CHECK(v == std::vector<Port>{Port{1}, Port{2}});
    }
    {
        // The failed parse adds nothing
        std::vector<Port> v;
        auto const result = parse("1;2", *(int_ >> ';'), v);
        REQUIRE(result.is_partial_match());
        CHECK(result.remainder_str() == "2");
        CHECK(v == std::vector<Port>{Port{1}});
    }
    {
        std::optional<WeakNumber> o;
        auto const result = parse("1", -(int_ >> ';'), o);
        REQUIRE(result.is_partial_match());
        CHECK(result.remainder_str() == "1");
        CHECK(!o);
    }

    // A new plain object is never default-constructed-then-assigned.
    // See also: `creatable_in_place_for`.
    STATIC_CHECK(is_writable_v<StrongNumber&, int>);
    STATIC_CHECK(!is_writable_v<std::optional<StrongNumber>&, int>);
    STATIC_CHECK(!is_writable_v<std::vector<StrongNumber>&, std::vector<int>>);
    STATIC_CHECK(!is_writable_v<rvariant<std::string, StrongNumber>&, int>);
    STATIC_CHECK(x4::detail::container_parse_strategy_for<std::remove_const_t<decltype(int_)>, std::vector<StrongNumber>> == x4::detail::container_parse_strategy::none);
}

TEST_CASE("is_writable")
{
    using x4::is_writable_v;

    // Identical types
    STATIC_CHECK(is_writable_v<int&, int>);
    STATIC_CHECK(is_writable_v<std::vector<int>&, std::vector<int>>);
    STATIC_CHECK(is_writable_v<alloy::tuple<int>&, alloy::tuple<int>>);
    STATIC_CHECK(is_writable_v<iris::rvariant<int>&, iris::rvariant<int>>);

    // `iris::rvariant<int, double>` is "broader" than `int`
    STATIC_CHECK( is_writable_v<iris::rvariant<int, double>&, int>);
    STATIC_CHECK(!is_writable_v<int&, iris::rvariant<int, double>>);

    // Container types
    STATIC_CHECK(is_writable_v<std::vector<iris::rvariant<int, double>>&, std::vector<int>>);

    // Tuple-like types
    STATIC_CHECK(is_writable_v<alloy::tuple<iris::rvariant<int, double>>&, alloy::tuple<int>>);
}
