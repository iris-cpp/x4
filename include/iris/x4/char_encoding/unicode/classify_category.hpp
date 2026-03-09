#ifndef IRIS_ZZ_X4_CHAR_ENCODING_UNICODE_CLASSIFY_CATEGORY_HPP
#define IRIS_ZZ_X4_CHAR_ENCODING_UNICODE_CLASSIFY_CATEGORY_HPP

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/char_encoding/unicode/category.hpp>
#include <iris/x4/char_encoding/unicode/detail/category_table.hpp>

namespace iris::x4::unicode {

[[nodiscard]] constexpr properties::category get_category(classify_type ch) noexcept
{
    return static_cast<properties::category>(detail::category_lookup(ch) & 0x3F);
}

[[nodiscard]] constexpr properties::major_category get_major_category(classify_type ch) noexcept
{
    return static_cast<properties::major_category>(unicode::get_category(ch) >> 3);
}

[[nodiscard]] constexpr bool is_punctuation(classify_type ch) noexcept
{
    return unicode::get_major_category(ch) == properties::punctuation;
}

[[nodiscard]] constexpr bool is_decimal_number(classify_type ch) noexcept
{
    return unicode::get_category(ch) == properties::decimal_number;
}

[[nodiscard]] constexpr bool is_hex_digit(classify_type ch) noexcept
{
    return (detail::category_lookup(ch) & properties::hex_digit) != 0;
}

[[nodiscard]] constexpr bool is_control(classify_type ch) noexcept
{
    return unicode::get_category(ch) == properties::control;
}

[[nodiscard]] constexpr bool is_alphabetic(classify_type ch) noexcept
{
    return (detail::category_lookup(ch) & properties::alphabetic) != 0;
}

[[nodiscard]] constexpr bool is_alphanumeric(classify_type ch) noexcept
{
    return unicode::is_decimal_number(ch) || unicode::is_alphabetic(ch);
}

[[nodiscard]] constexpr bool is_uppercase(classify_type ch) noexcept
{
    return (detail::category_lookup(ch) & properties::uppercase) != 0;
}

[[nodiscard]] constexpr bool is_lowercase(classify_type ch) noexcept
{
    return (detail::category_lookup(ch) & properties::lowercase) != 0;
}

[[nodiscard]] constexpr bool is_white_space(classify_type ch) noexcept
{
    return (detail::category_lookup(ch) & properties::white_space) != 0;
}

[[nodiscard]] constexpr bool is_blank(classify_type ch) noexcept
{
    switch (ch)
    {
    case '\n': case '\v': case '\f': case '\r':
        return false;
    default:
        return unicode::is_white_space(ch) &&
            !(
                unicode::get_category(ch) == properties::line_separator ||
                unicode::get_category(ch) == properties::paragraph_separator
            );
    }
}

[[nodiscard]] constexpr bool is_graph(classify_type ch) noexcept
{
    return !(
        unicode::is_white_space(ch) ||
        unicode::get_category(ch) == properties::control ||
        unicode::get_category(ch) == properties::surrogate ||
        unicode::get_category(ch) == properties::unassigned
    );
}

[[nodiscard]] constexpr bool is_print(classify_type ch) noexcept
{
    return (unicode::is_graph(ch) || unicode::is_blank(ch)) && !unicode::is_control(ch);
}

[[nodiscard]] constexpr bool is_noncharacter_code_point(classify_type ch) noexcept
{
    return (detail::category_lookup(ch) & properties::noncharacter_code_point) != 0;
}

[[nodiscard]] constexpr bool is_default_ignorable_code_point(classify_type ch) noexcept
{
    return (detail::category_lookup(ch) & properties::default_ignorable_code_point) != 0;
}

} // iris::x4::unicode

#endif
