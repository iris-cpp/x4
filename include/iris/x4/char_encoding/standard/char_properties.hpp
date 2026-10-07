#ifndef IRIS_ZZ_X4_CHAR_ENCODING_STANDARD_CHAR_PROPERTIES_HPP
#define IRIS_ZZ_X4_CHAR_ENCODING_STANDARD_CHAR_PROPERTIES_HPP

/*=============================================================================
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/char_encoding/char_properties.hpp>
#include <iris/x4/char_encoding/standard.hpp>

#include <cassert>
#include <cctype>
#include <climits>

namespace iris::x4::char_encoding {

template<>
struct char_properties<standard>
{
    // *** Note on assertions: The precondition is that the calls to
    // these functions do not violate the required range of ch (int)
    // which is that strict_ischar(ch) should be true. It is the
    // responsibility of the caller to make sure this precondition is not
    // violated.

    [[nodiscard]] static constexpr bool
    strict_ischar(int ch) noexcept
    {
        // ch should be representable as an unsigned char
        return ch >= 0 && ch <= UCHAR_MAX;
    }

    [[nodiscard]] static bool // TODO: constexpr
    isalnum(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::isalnum(ch) != 0;
    }

    [[nodiscard]] static bool // TODO: constexpr
    isalpha(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::isalpha(ch) != 0;
    }

    [[nodiscard]] static bool // TODO: constexpr
    isdigit(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::isdigit(ch) != 0;
    }

    [[nodiscard]] static bool // TODO: constexpr
    isxdigit(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::isxdigit(ch) != 0;
    }

    [[nodiscard]] static bool // TODO: constexpr
    iscntrl(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::iscntrl(ch) != 0;
    }

    [[nodiscard]] static bool // TODO: constexpr
    isgraph(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::isgraph(ch) != 0;
    }

    [[nodiscard]] static bool // TODO: constexpr
    islower(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::islower(ch) != 0;
    }

    [[nodiscard]] static bool // TODO: constexpr
    isprint(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::isprint(ch) != 0;
    }

    [[nodiscard]] static bool // TODO: constexpr
    ispunct(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::ispunct(ch) != 0;
    }

    [[nodiscard]] static bool // TODO: constexpr
    isupper(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::isupper(ch) != 0;
    }

    // Simple character conversions

    [[nodiscard]] static int // TODO: constexpr
    tolower(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::tolower(ch);
    }

    [[nodiscard]] static int // TODO: constexpr
    toupper(int ch) noexcept
    {
        assert(char_properties::strict_ischar(ch));
        return std::toupper(ch);
    }
};

} // iris::x4::char_encoding

#endif
