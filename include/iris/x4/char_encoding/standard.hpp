#ifndef IRIS_ZZ_X4_CHAR_ENCODING_STANDARD_HPP
#define IRIS_ZZ_X4_CHAR_ENCODING_STANDARD_HPP

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

// Test characters for specified conditions (using std functions)
struct standard
{
    using char_type = char;
    using string_type = std::string;
    using classify_type = unsigned char;

    [[nodiscard]] static constexpr bool
    isascii_(int ch) noexcept
    {
        return 0 == (ch & ~0x7f);
    }

    [[nodiscard]] static constexpr bool
    ischar(int ch) noexcept
    {
        // uses all 8 bits
        // we have to watch out for sign extensions
        return (0 == (ch & ~0xff) || ~0 == (ch | 0xff)) != 0;
    }
};

} // iris::x4::char_encoding

#endif
