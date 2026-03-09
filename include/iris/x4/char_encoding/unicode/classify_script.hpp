#ifndef IRIS_ZZ_X4_CHAR_ENCODING_UNICODE_CLASSIFY_SCRIPT_HPP
#define IRIS_ZZ_X4_CHAR_ENCODING_UNICODE_CLASSIFY_SCRIPT_HPP

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/char_encoding/unicode/category.hpp>
#include <iris/x4/char_encoding/unicode/detail/script_table.hpp>

namespace iris::x4::unicode {

[[nodiscard]] constexpr properties::script get_script(classify_type ch) noexcept
{
    return static_cast<properties::script>(detail::script_lookup(ch));
}

} // iris::x4::unicode

#endif
