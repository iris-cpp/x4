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
    isascii_(char32_t const ch) noexcept
    {
        return (ch & ~0x7f) == 0;
    }

    [[nodiscard]] static constexpr bool
    ischar(char32_t const ch) noexcept
    {
        return ch <= 0x10FFFF;
    }

    [[nodiscard]] static constexpr bool
    isspace(char32_t const ch) noexcept
    {
        switch (ch) {
        case U'\t': case U'\n': case U'\v': case U'\f': case U'\r':
        case U'\u0020': case U'\u0085': case U'\u00A0': case U'\u1680':
        case U'\u2000': case U'\u2001': case U'\u2002': case U'\u2003': case U'\u2004': case U'\u2005':
        case U'\u2006': case U'\u2007': case U'\u2008': case U'\u2009': case U'\u200A':
        case U'\u2028': case U'\u2029': case U'\u202F': case U'\u205F': case U'\u3000':
            return true;
        default:
            return false;
        }
    }

    [[nodiscard]] static constexpr bool
    (isblank)(char32_t const ch) noexcept
    {
        switch (ch) {
        case U'\t':
        case U'\u0020': case U'\u00A0': case U'\u1680':
        case U'\u2000': case U'\u2001': case U'\u2002': case U'\u2003': case U'\u2004': case U'\u2005':
        case U'\u2006': case U'\u2007': case U'\u2008': case U'\u2009': case U'\u200A':
        case U'\u202F': case U'\u205F': case U'\u3000':
            return true;
        default:
            return false;
        }
    }

    // Mixing character encodings is semantically wrong
    static constexpr bool isascii_(char) = delete;
    static constexpr bool isascii_(wchar_t) = delete;
    static constexpr bool isascii_(char8_t) = delete;
    static constexpr bool isascii_(char16_t) = delete;
    static constexpr bool ischar(char) = delete;
    static constexpr bool ischar(wchar_t) = delete;
    static constexpr bool ischar(char8_t) = delete;
    static constexpr bool ischar(char16_t) = delete;

    static constexpr bool isspace(char) = delete;
    static constexpr bool isspace(wchar_t) = delete;
    static constexpr bool isspace(char8_t) = delete;
    static constexpr bool isspace(char16_t) = delete;
    static constexpr bool isblank(char) = delete;
    static constexpr bool isblank(wchar_t) = delete;
    static constexpr bool isblank(char8_t) = delete;
    static constexpr bool isblank(char16_t) = delete;
};

} // iris::x4::char_encoding

#endif
