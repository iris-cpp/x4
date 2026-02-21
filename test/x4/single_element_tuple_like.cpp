#include "iris_x4_test.hpp"

#include <iris/x4.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/tuple.hpp>

struct Ident {
    std::string value;
};

struct Var {
    Ident ident;
};

struct Decl {
    Var var;
};

IRIS_ALLOY_ADAPT_STRUCT(Ident, value)
IRIS_ALLOY_ADAPT_STRUCT(Var, ident)
IRIS_ALLOY_ADAPT_STRUCT(Decl, var);

using IdentRule = x4::rule<struct ident_tag, Ident>;
using VarRule = x4::rule<struct var_tag, Var>;
using DeclRule = x4::rule<struct decl_tag, Decl>;

constexpr IdentRule ident;
constexpr VarRule var;
constexpr DeclRule decl;

IRIS_X4_DECLARE(IdentRule);
IRIS_X4_DECLARE(VarRule);
IRIS_X4_DECLARE(DeclRule)

constexpr auto ident_def = (+x4::char_ >> x4::eps);
constexpr auto var_def = x4::lexeme[x4::lit('$') >> ident];
constexpr auto decl_def = x4::skip(x4::space)[x4::lit("let") >> var];

IRIS_X4_DEFINE(ident);
IRIS_X4_DEFINE(var);
IRIS_X4_DEFINE(decl);

TEST_CASE("single element tuple like")
{
    {
        Decl attr;
        CHECK(parse("let $foo", decl, attr));
        CHECK(attr.var.ident.value == "foo");
    }
}
