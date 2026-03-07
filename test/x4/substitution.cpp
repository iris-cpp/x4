#include "iris_x4_test.hpp"

#include <iris/x4/core/move_to.hpp>

#include <iris/x4/traits/can_hold.hpp>
#include <iris/x4/traits/variant_traits.hpp>

#include <iris/rvariant.hpp>

#include <iris/alloy/adapted/std_tuple.hpp>

template<class T, class U>
inline constexpr bool can_hold_v = x4::traits::can_hold<T, U>::value;

TEST_CASE("can_hold")
{
    // identical types
    STATIC_CHECK(can_hold_v<int, int>);
    STATIC_CHECK(can_hold_v<std::vector<int>, std::vector<int>>);
    STATIC_CHECK(can_hold_v<std::tuple<int>, std::tuple<int>>);
    STATIC_CHECK(can_hold_v<iris::rvariant<int>, iris::rvariant<int>>);

    // `iris::rvariant<int, double>` is "broader" than `int`
    STATIC_CHECK( can_hold_v<iris::rvariant<int, double>, int>);
    STATIC_CHECK(!can_hold_v<int, iris::rvariant<int, double>>);

    // container types
    STATIC_CHECK(can_hold_v<std::vector<iris::rvariant<int, double>>, std::vector<int>>);

    // tuple-like types
    STATIC_CHECK(can_hold_v<std::tuple<iris::rvariant<int, double>>, std::tuple<int>>);
}
