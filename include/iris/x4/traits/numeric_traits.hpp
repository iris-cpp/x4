#ifndef IRIS_ZZ_X4_TRAITS_NUMERIC_TRAITS_HPP
#define IRIS_ZZ_X4_TRAITS_NUMERIC_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <string_view>
#include <limits>
#include <type_traits>

namespace iris::x4::traits {

// Customization points for numeric operations

template<class CharT>
struct numeric_token;

#define IRIS_X4_DEF(CharT, XSTR) \
    template<> \
    struct numeric_token<CharT> \
    { \
        inline static constexpr CharT a = XSTR('a'); \
        inline static constexpr CharT A = XSTR('A'); \
        inline static constexpr CharT e = XSTR('e'); \
        inline static constexpr CharT E = XSTR('E'); \
        inline static constexpr CharT n = XSTR('n'); \
        inline static constexpr CharT N = XSTR('N'); \
        inline static constexpr CharT i = XSTR('i'); \
        inline static constexpr CharT I = XSTR('I'); \
    \
        inline static constexpr CharT _0 = XSTR('0'); \
        inline static constexpr CharT _9 = XSTR('9'); \
    \
        inline static constexpr CharT minus = XSTR('-'); \
        inline static constexpr CharT plus = XSTR('+'); \
        inline static constexpr CharT dot = XSTR('.'); \
        inline static constexpr CharT lparen = XSTR('('); \
        inline static constexpr CharT rparen = XSTR(')'); \
    \
        inline static constexpr std::basic_string_view<CharT> nan = XSTR("nan"); \
        inline static constexpr std::basic_string_view<CharT> NAN_ = XSTR("NAN"); \
        inline static constexpr std::basic_string_view<CharT> inf = XSTR("inf"); \
        inline static constexpr std::basic_string_view<CharT>    inity = XSTR("inity"); \
        inline static constexpr std::basic_string_view<CharT> INF = XSTR("INF"); \
        inline static constexpr std::basic_string_view<CharT>    INITY = XSTR("INITY"); \
    \
        inline static constexpr std::basic_string_view<CharT> true_ = XSTR("true"); \
        inline static constexpr std::basic_string_view<CharT> false_ = XSTR("false"); \
    };

#define IRIS_X4_XSTR_char(str) str
IRIS_X4_DEF(char, IRIS_X4_XSTR_char)
#undef IRIS_X4_XSTR_char

#ifdef IRIS_X4_UNICODE
# define IRIS_X4_XSTR_u32(str) U##str
IRIS_X4_DEF(char32_t, IRIS_X4_XSTR_u32)
# undef IRIS_X4_XSTR_u32
#endif

#undef IRIS_X4_DEF


template<class T>
struct pow10_helper;

template<class T>
struct check_overflow : std::bool_constant<std::numeric_limits<T>::is_bounded>
{};

} // iris::x4::traits

#endif
