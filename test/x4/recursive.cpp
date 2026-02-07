/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/char/char.hpp>
#include <iris/x4/string/string.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/operator/list.hpp>

#include <iris/rvariant/recursive_wrapper.hpp>

#include <vector>

TEST_CASE("recursive")
{
    using x4::int_;

    {
        iris::recursive_wrapper<int> val;
        REQUIRE(parse("1", int_, val));
        CHECK(*val == 1);
    }

    {
        iris::recursive_wrapper<std::vector<int>> ints;
        REQUIRE(parse("0,1", int_ % ',', ints));
        CHECK(*ints == std::vector{0, 1});
    }

    {
        iris::recursive_wrapper<std::vector<int>> ints;
        REQUIRE(parse("0-1-2,3-4-5", (int_ % '-') % ',', ints));
        CHECK(*ints == std::vector{0, 1, 2, 3, 4, 5});
    }
}
