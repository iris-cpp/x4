#ifndef IRIS_ZZ_X4_CORE_TRAITS_CHAR_ENCODING_TRAITS_HPP
#define IRIS_ZZ_X4_CORE_TRAITS_CHAR_ENCODING_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

// Do NOT include `char_encoding/` related headers in this file

#ifdef IRIS_X4_NO_STANDARD_WIDE
# warning "wchar_t support is completely removed from X4 due to severe portability issue. Remove `#define IRIS_X4_NO_STANDARD_WIDE`."
#endif

#ifdef IRIS_X4_UNICODE
# warning "X4 now enables Unicode support by default. Remove `#define IRIS_X4_UNICODE`."
#endif

#include <iris/string.hpp>

namespace iris::x4 {

namespace char_encoding {
struct standard;
struct unicode;
} // char_encoding

namespace detail {

template<CharLike CharT> struct char_encoding_for_impl;
template<> struct char_encoding_for_impl<char> { using type = char_encoding::standard; };
template<> struct char_encoding_for_impl<char32_t> { using type = char_encoding::unicode; };

} // detail

template<CharLike CharT>
using char_encoding_for = detail::char_encoding_for_impl<CharT>::type;

} // iris::x4

#endif
