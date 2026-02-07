/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"

#include <iris/x4/auxiliary/eoi.hpp>

TEST_CASE("eoi")
{
    using x4::eoi;

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(eoi);

    CHECK(parse("", eoi));
    CHECK(!(parse("x", eoi)));
    CHECK(x4::what(eoi) == "eoi");
}
