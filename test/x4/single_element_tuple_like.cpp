#include "iris_x4_test.hpp"

#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/directive/as.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/char.hpp>
#include <iris/x4/rule.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/tuple.hpp>

#include <string>


struct Ident
{
    std::string value;
};

IRIS_ALLOY_ADAPT_STRUCT(Ident, value)

using IdentRule = x4::rule<struct ident_tag, Ident>;

IRIS_X4_DECLARE_CONSTEXPR(IdentRule)

constexpr IdentRule ident;

constexpr auto ident_def = +x4::char_;

IRIS_X4_DEFINE_CONSTEXPR(ident)

TEST_CASE("single_element_tuple_like")
{
    using x4::eps;
    using x4::as;

    {
        constexpr auto parser = ident;
        Ident attr;
        STATIC_CHECK(std::same_as<x4::parser_traits<decltype(parser)>::attribute_type, Ident>);
        REQUIRE(parse("abc", parser, attr));
        CHECK(attr.value == "abc");
    }
    {
        constexpr auto parser = as<Ident>(ident);
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
}
