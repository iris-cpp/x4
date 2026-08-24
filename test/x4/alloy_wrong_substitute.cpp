#include "iris_x4_test.hpp"

#include <iris/x4/primitive/eps.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/rule.hpp>

#include <iris/alloy/adapt.hpp>
#include <iris/alloy/tuple.hpp>

#include <iris/rvariant.hpp>

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

namespace iris::alloy {

// If enabled, `B` can be wrongly treated as substitutable to `A`
template<>
struct adaptor<A>
{
   using getters_list = iris::constant_list<
       &A::foo,
       &A::bar
   >;
};

template<>
struct adaptor<B>
{
    using getters_list = iris::constant_list<
        &B::hoge,
        &B::fuga
    >;
};

} // iris::alloy

using AorB = iris::rvariant<
    A,
    B
>;

using ARule = x4::rule<struct a_tag, A>;
using BRule = x4::rule<struct b_tag, B>;
using AorBRule = x4::rule<struct a_or_b_tag, AorB>;

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

TEST_CASE("alloy_wrong_substitute")
{
    const char* ptr = nullptr;
    AorB result;
    (void)a_or_b.parse(ptr, nullptr, x4::unused, result);
}
