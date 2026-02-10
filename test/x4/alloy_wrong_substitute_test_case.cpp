#include "iris_x4_test.hpp"

#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/rule.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/tuple.hpp>

#include <iris/rvariant.hpp>

namespace alloy_wrong_substitute_test_case {

struct A
{
    int foo;
    int bar;
};

struct B
{
    int hoge;
    std::string fuga;
};

} // alloy_wrong_substitute_test_case

namespace iris::alloy {

// If enabled, `B` can be wrongly treated as substitutable to `A`
template<>
struct adaptor<alloy_wrong_substitute_test_case::A>
{
   using getters_list = make_getters_list<
       &alloy_wrong_substitute_test_case::A::foo,
       &alloy_wrong_substitute_test_case::A::bar
   >;
};

template<>
struct adaptor<alloy_wrong_substitute_test_case::B>
{
    using getters_list = make_getters_list<
        &alloy_wrong_substitute_test_case::B::hoge,
        &alloy_wrong_substitute_test_case::B::fuga
    >;
};

} // iris::alloy

namespace ast {

using A = alloy_wrong_substitute_test_case::A;
using B = alloy_wrong_substitute_test_case::B;
using AorB = iris::rvariant<
    alloy_wrong_substitute_test_case::A,
    alloy_wrong_substitute_test_case::B
>;

} // ast

using ARule = x4::rule<struct a_tag, ast::A>;
using BRule = x4::rule<struct b_tag, ast::B>;
using AorBRule = x4::rule<struct a_or_b_tag, ast::AorB>;

constexpr ARule a;
constexpr BRule b;
constexpr AorBRule a_or_b;

IRIS_X4_DECLARE(ARule);
IRIS_X4_DECLARE(BRule);
IRIS_X4_DECLARE(AorBRule);

constexpr auto a_def = x4::eps;

constexpr auto b_def = x4::eps;

constexpr auto a_or_b_def = a | b;

IRIS_X4_DEFINE(a);
IRIS_X4_DEFINE(b);
IRIS_X4_DEFINE(a_or_b);

IRIS_X4_INSTANTIATE(AorBRule, const char*, x4::unused_type);

TEST_CASE("alloy_wrong_substitute_test_case")
{
    const char* ptr = nullptr;
    ast::AorB result;
    (void)a_or_b.parse(ptr, nullptr, x4::unused, result);
}
