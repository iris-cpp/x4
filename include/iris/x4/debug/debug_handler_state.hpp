#ifndef IRIS_X4_DEBUG_DEBUG_HANDLER_STATE_HPP
#define IRIS_X4_DEBUG_DEBUG_HANDLER_STATE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

namespace iris::x4 {

enum class debug_handler_state : char
{
    pre_parse,
    successful_parse,
    failed_parse,
};

} // iris::x4

#endif
