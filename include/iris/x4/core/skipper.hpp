#ifndef IRIS_ZZ_X4_CORE_SKIPPER_HPP
#define IRIS_ZZ_X4_CORE_SKIPPER_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

namespace iris::x4 {

enum class skipper_state : unsigned char
{
    pre_skip,
    post_skip,
};

enum struct builtin_skipper_kind : unsigned char
{
    no_skip,
    blank,
    space,
};

} // iris::x4

#endif
