#include "iris_x4_test.hpp"

#include <iris/x4/core/move_to.hpp>

#include <iris/x4/traits/substitution.hpp>
#include <iris/x4/traits/variant_traits.hpp>

#include <iris/rvariant.hpp>

TEST_CASE("is_substitute")
{
    STATIC_CHECK( x4::traits::is_substitute_v<iris::rvariant<int, double>, int>);
    STATIC_CHECK(!x4::traits::is_substitute_v<int, iris::rvariant<int, double>>);
}
