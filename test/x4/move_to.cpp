#include "iris_x4_test.hpp"

#include <iris/x4/core/move_to.hpp>

#include <iris/alloy/tuple.hpp>
#include <iris/alloy/adapt.hpp>
#include <iris/rvariant/rvariant.hpp>
#include <iris/rvariant/rvariant_io.hpp>

#include <iris/alloy/adapted/std_tuple.hpp>

#include <vector>

struct X {};
struct Y {};
struct Z {};

struct Xs
{
    std::vector<X> xs;
};
IRIS_ALLOY_ADAPT_STRUCT(Xs, xs);

using XYZ = iris::rvariant<X, Y, Z, Xs>;


TEST_CASE("move_to")
{
    // TODO: add more test

    // Currently not permitted by design, but may be changed in the future
    // if legitimate rationale is discovered
    {
        //XYZ xyz;
        //xyz = std::vector<X>{};
        static_assert(!std::is_assignable_v<XYZ&, std::vector<X>>);
        STATIC_CHECK(!x4::X4Movable<std::vector<X>, XYZ>);
    }

    // tuple contains reference
    {
        int n = 0;
        std::tuple<int&> ref_tuple{ n };
        x4::move_to(42, ref_tuple);
        CHECK(n == 42);
    }
    {
        int n = 42;
        std::tuple<int&> ref_tuple{ n };
        std::tuple<int> dest{ 0 };
        x4::move_to(std::move(ref_tuple), dest);
        CHECK(alloy::get<0>(dest) == 42);
    }
    {
        x4_test::move_only mo;
        std::tuple<x4_test::move_only&> ref_tuple{ mo };
        std::tuple<x4_test::move_only> dest;
        x4::move_to(std::move(ref_tuple), dest); // move "far" ownership
    }
}
