#include "iris_x4_test.hpp"

#include <iris/x4/core/move_to.hpp>

#include <iris/x4/traits/can_hold.hpp>
#include <iris/x4/traits/variant_traits.hpp>

#include <iris/rvariant.hpp>

TEST_CASE("can_hold")
{
    // iris::rvariant<int, double> is "broader" than int
    STATIC_CHECK( x4::traits::can_hold_v<iris::rvariant<int, double>, int>);
    STATIC_CHECK(!x4::traits::can_hold_v<int, iris::rvariant<int, double>>);
}

TEST_CASE("variant_has_substitute")
{
    STATIC_CHECK( x4::traits::variant_has_substitute_v<iris::rvariant<int, double>, int>);
    STATIC_CHECK(!x4::traits::variant_has_substitute_v<iris::rvariant<int, double>, char>);
}
