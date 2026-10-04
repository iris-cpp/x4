#ifndef IRIS_ZZ_X4_TEST_X4_RULE_SEPARATE_TU_GRAMMAR_HPP
#define IRIS_ZZ_X4_TEST_X4_RULE_SEPARATE_TU_GRAMMAR_HPP

/*=============================================================================
    Copyright (c) 2019 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>
#include <iris/x4/numeric/int.hpp>

// Check that `IRIS_X4_INSTANTIATE` instantiates `parse_rule` with proper
// types when the rule has no attribute.

namespace unused_attr {

// skipper must have no attribute, check `parse` and `skip_over`
IRIS_X4_DECLARE_PUBLIC(skipper, unused_type);

// grammar must have no attribute, check `parse` and `phrase_parse`
IRIS_X4_DECLARE_PUBLIC(grammar, unused_type);

} // unused_attr

// Check instantiation when the rule has an attribute.

namespace used_attr {

IRIS_X4_DECLARE_PUBLIC(skipper, unused_type);
IRIS_X4_DECLARE_PUBLIC(grammar, int);

} // used_attr

// Check that a header which more than one translation unit includes can contain
// `IRIS_X4_DEFINE_PUBLIC`.

namespace header_defined {

IRIS_X4_DECLARE_PUBLIC(grammar, int);
constexpr auto grammar_def = x4::int_;
IRIS_X4_DEFINE_PUBLIC(grammar)

} // header_defined

#endif
