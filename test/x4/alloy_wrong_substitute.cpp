#include "iris_x4_test.hpp"

#include <iris/x4/primitive/eps.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/rule.hpp>

#include <iris/alloy/adapt.hpp>

#include <iris/rvariant/rvariant.hpp>

#include <iris/type_list.hpp>

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

IRIS_X4_DECLARE(a, A);
IRIS_X4_DECLARE(b, B);
IRIS_X4_DECLARE(a_or_b, AorB);

constexpr auto a_def = x4::eps;
constexpr auto b_def = x4::eps;
constexpr auto a_or_b_def = a | b;

IRIS_X4_DEFINE(a);
IRIS_X4_DEFINE(b);
IRIS_X4_DEFINE(a_or_b);

IRIS_X4_INSTANTIATE(a_or_b, const char*, x4::unused_type);

TEST_CASE("alloy_wrong_substitute")
{
    const char* ptr = nullptr;
    AorB result;
    (void)a_or_b.parse(ptr, nullptr, x4::unused, result);
}
