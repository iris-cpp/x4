#include "iris_x4_test.hpp"

#include <iris/x4/core/move_to.hpp>

#include <iris/x4/traits/substitution.hpp>
#include <iris/x4/traits/variant_traits.hpp>

#include <iris/rvariant.hpp>

TEST_CASE("is_substitute")
{
    // iris::rvariant<int, double> is "broader" than int
    STATIC_CHECK( x4::traits::is_substitute_v<iris::rvariant<int, double>, int>);
    STATIC_CHECK(!x4::traits::is_substitute_v<int, iris::rvariant<int, double>>);
}

TEST_CASE("variant_has_substitute")
{
    STATIC_CHECK( x4::traits::variant_has_substitute_v<iris::rvariant<int, double>, int>);
    STATIC_CHECK(!x4::traits::variant_has_substitute_v<iris::rvariant<int, double>, char>);
}
