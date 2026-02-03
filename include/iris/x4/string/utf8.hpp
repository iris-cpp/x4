#ifndef IRIS_X4_STRING_UTF8_HPP
#define IRIS_X4_STRING_UTF8_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2023 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <string>
#include <string_view>
#include <type_traits>

namespace iris::x4 {

namespace detail {

// TODO: https://github.com/microsoft/STL/issues/2207
using impl_def_u8string = std::string;

constexpr void utf8_put_encode(impl_def_u8string& out, char32_t x) noexcept
{
    // https://www.unicode.org/versions/Unicode15.0.0/ch03.pdf D90
    if (x > 0x10FFFFul || (0xD7FFul < x && x < 0xE000ul)) [[unlikely]] {
        x = 0xFFFDul;
    }

    // Table 3-6. UTF-8 Bit Distribution
    if (x < 0x80ul) {
        out.push_back(static_cast<unsigned char>(x));
    } else if (x < 0x800ul) {
        out.push_back(static_cast<unsigned char>(0xC0ul + (x >> 6)));
        out.push_back(static_cast<unsigned char>(0x80ul + (x & 0x3Ful)));
    } else if (x < 0x10000ul) {
        out.push_back(static_cast<unsigned char>(0xE0ul + (x >> 12)));
        out.push_back(static_cast<unsigned char>(0x80ul + ((x >> 6) & 0x3Ful)));
        out.push_back(static_cast<unsigned char>(0x80ul + (x & 0x3Ful)));
    } else {
        out.push_back(static_cast<unsigned char>(0xF0ul + (x >> 18)));
        out.push_back(static_cast<unsigned char>(0x80ul + ((x >> 12) & 0x3Ful)));
        out.push_back(static_cast<unsigned char>(0x80ul + ((x >> 6) & 0x3Ful)));
        out.push_back(static_cast<unsigned char>(0x80ul + (x & 0x3Ful)));
    }
}

} // detail

template<class Char>
[[nodiscard]] constexpr detail::impl_def_u8string to_utf8(Char value)
{
    detail::impl_def_u8string result;
    using UChar = std::make_unsigned_t<Char>;
    detail::utf8_put_encode(result, static_cast<UChar>(value));
    return result;
}

template<class Char>
[[nodiscard]] constexpr detail::impl_def_u8string to_utf8(Char const* str)
{
    detail::impl_def_u8string result;
    using UChar = std::make_unsigned_t<Char>;
    while (*str) {
        detail::utf8_put_encode(result, static_cast<UChar>(*str++));
    }
    return result;
}

template<class Char, class Traits>
[[nodiscard]] constexpr detail::impl_def_u8string
to_utf8(std::basic_string_view<Char, Traits> const str)
{
    detail::impl_def_u8string result;
    using UChar = std::make_unsigned_t<Char>;
    for (Char ch : str) {
        detail::utf8_put_encode(result, static_cast<UChar>(ch));
    }
    return result;
}

template<class Char, class Traits>
[[nodiscard]] constexpr detail::impl_def_u8string
to_utf8(std::basic_string<Char, Traits> const& str)
{
    detail::impl_def_u8string result;
    using UChar = std::make_unsigned_t<Char>;
    for (Char ch : str) {
        detail::utf8_put_encode(result, static_cast<UChar>(ch));
    }
    return result;
}

// Assume wchar_t content is UTF-16 on MSVC, or mingw/wineg++ with -fshort-wchar
#if defined(_MSC_VER) || defined(__SIZEOF_WCHAR_T__) && __SIZEOF_WCHAR_T__ == 2
[[nodiscard]] constexpr detail::impl_def_u8string to_utf8(wchar_t value)
{
    detail::impl_def_u8string result;
    detail::utf8_put_encode(result, static_cast<std::make_unsigned_t<wchar_t>>(value));
    return result;
}

namespace detail {

template<std::forward_iterator It>
    requires std::is_same_v<std::remove_const_t<std::iter_value_t<It>>, wchar_t>
[[nodiscard]] constexpr char32_t decode_utf16(It& s) noexcept
{
    using uwchar_t = std::make_unsigned_t<wchar_t>;

    uwchar_t x(*s);
    if (x < 0xD800ul || x > 0xDFFFul) {
        return x;
    }

    // expected high-surrogate
    if ((x >> 10) != 0b110110ul) [[unlikely]] {
        return 0xFFFDul;
    }

    uwchar_t y(*++s);
    // expected low-surrogate
    if ((y >> 10) != 0b110111ul) [[unlikely]] {
        return 0xFFFDul;
    }

    return ((x & 0x3FFul) << 10) + (y & 0x3FFul) + 0x10000ul;
}

} // detail

[[nodiscard]] constexpr detail::impl_def_u8string
to_utf8(std::basic_string_view<wchar_t> const str)
{
    detail::impl_def_u8string result;
    for (auto it = str.begin(); it != str.end(); ++it) {
        detail::utf8_put_encode(result, detail::decode_utf16(it));
    }
    return result;
}

[[nodiscard]] constexpr detail::impl_def_u8string to_utf8(wchar_t const* str)
{
    return x4::to_utf8(std::basic_string_view(str));
}

[[nodiscard]] constexpr detail::impl_def_u8string
to_utf8(std::basic_string<wchar_t> const& str)
{
    detail::impl_def_u8string result;
    for (auto it = str.begin(); it != str.end(); ++it) {
        detail::utf8_put_encode(result, detail::decode_utf16(it));
    }
    return result;
}
#endif

} // iris::x4

#endif
