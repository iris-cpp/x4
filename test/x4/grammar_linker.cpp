/*=============================================================================
    Copyright (c) 2019 Nikita Kniazev
    Copyright (c) 2019 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"
#include "grammar.hpp"

TEST_CASE("grammar")
{
    CHECK(parse("123", grammar));
}
