#include "iris_x4_test.hpp"

#include <iris/x4/core/traits/can_hold.hpp>
#include <iris/x4/core/traits/variant_traits.hpp>

#include <iris/x4/core/move_to.hpp>

#include <iris/rvariant.hpp>

using x4::can_hold_v;

TEST_CASE("can_hold")
{
    // identical types
    STATIC_CHECK(can_hold_v<int, int>);
    STATIC_CHECK(can_hold_v<std::vector<int>, std::vector<int>>);
    STATIC_CHECK(can_hold_v<alloy::tuple<int>, alloy::tuple<int>>);
    STATIC_CHECK(can_hold_v<iris::rvariant<int>, iris::rvariant<int>>);

    // `iris::rvariant<int, double>` is "broader" than `int`
    STATIC_CHECK( can_hold_v<iris::rvariant<int, double>, int>);
    STATIC_CHECK(!can_hold_v<int, iris::rvariant<int, double>>);

    // container types
    STATIC_CHECK(can_hold_v<std::vector<iris::rvariant<int, double>>, std::vector<int>>);

    // tuple-like types
    STATIC_CHECK(can_hold_v<alloy::tuple<iris::rvariant<int, double>>, alloy::tuple<int>>);
}
