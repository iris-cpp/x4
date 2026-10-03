/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>

TEST_CASE("rule: constructible with incomplete type")
{
    struct incomplete_attr_type;
    struct rule_id { using rule_attribute_type = incomplete_attr_type; };
    [[maybe_unused]] constexpr x4::rule<rule_id> incomplete_type_rule{};
}
