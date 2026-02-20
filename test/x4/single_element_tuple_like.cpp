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

IRIS_ALLOY_ADAPT_STRUCT(Ident, value)
IRIS_ALLOY_ADAPT_STRUCT(Var, ident)

using IdentRule = x4::rule<struct ident_tag, Ident>;
using VarRule = x4::rule<struct var_tag, Var>;

constexpr IdentRule ident;
constexpr VarRule var;

IRIS_X4_DECLARE(IdentRule);
IRIS_X4_DECLARE(VarRule);

constexpr auto ident_def = +x4::char_;
constexpr auto var_def = x4::lit('$') >> ident;

IRIS_X4_DEFINE(ident);
IRIS_X4_DEFINE(var);

TEST_CASE("single element tuple like")
{
  {
    constexpr auto parser = x4::lit('$') >> ident;
    Var attr;
    CHECK(parse("$foo", parser, attr));
    CHECK(attr.ident.value == "foo");
  }
}
