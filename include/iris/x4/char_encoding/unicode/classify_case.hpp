#ifndef IRIS_ZZ_X4_CHAR_ENCODING_UNICODE_CLASSIFY_CASE_HPP
#define IRIS_ZZ_X4_CHAR_ENCODING_UNICODE_CLASSIFY_CASE_HPP

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/char_encoding/unicode/category.hpp>
#include <iris/x4/char_encoding/unicode/detail/lowercase_table.hpp>
#include <iris/x4/char_encoding/unicode/detail/uppercase_table.hpp>

namespace iris::x4::unicode {

[[nodiscard]] constexpr classify_type to_lowercase(classify_type ch) noexcept
{
    // The table returns 0 to signal that this code maps to itself
    classify_type const r = detail::lowercase_lookup(ch);
    return r == 0 ? ch : r;
}

[[nodiscard]] constexpr classify_type to_uppercase(classify_type ch) noexcept
{
    // The table returns 0 to signal that this code maps to itself
    classify_type const r = detail::uppercase_lookup(ch);
    return r == 0 ? ch : r;
}

} // iris::x4::unicode

#endif
