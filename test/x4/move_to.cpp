#include "iris_x4_test.hpp"

#include <iris/x4/core/move_to.hpp>

#include <iris/alloy/tuple.hpp>

TEST_CASE("move_to")
{
    // TODO: add more test

    // tuple contains reference
    {
        int n = 0;
        alloy::tuple<int&> ref_tuple{ n };
        x4::move_to(42, ref_tuple);
        CHECK(n == 42);
    }
}
