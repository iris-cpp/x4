#ifndef IRIS_ZZ_X4_CHAR_ENCODING_UNICODE_HPP
#define IRIS_ZZ_X4_CHAR_ENCODING_UNICODE_HPP

/*=============================================================================
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <string>

namespace iris::x4::char_encoding {

struct unicode
{
    using char_type = char32_t;
    using string_type = std::u32string;
    using classify_type = char32_t;

    [[nodiscard]] static constexpr bool
    isascii_(char32_t ch) noexcept
    {
        return (ch & ~0x7f) == 0;
    }

    [[nodiscard]] static constexpr bool
    ischar(char32_t ch) noexcept
    {
        return ch <= 0x10FFFF;
    }
};

} // iris::x4::char_encoding

#endif
