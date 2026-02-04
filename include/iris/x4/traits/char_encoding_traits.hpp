#ifndef IRIS_X4_TRAITS_CHAR_ENCODING_TRAITS_HPP
#define IRIS_X4_TRAITS_CHAR_ENCODING_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/char_encoding/standard.hpp>

#ifndef IRIS_X4_NO_STANDARD_WIDE
# include <iris/x4/char_encoding/standard_wide.hpp>
#endif

#ifdef IRIS_X4_UNICODE
# include <iris/x4/char_encoding/unicode.hpp>
#endif

namespace iris::x4::traits {

namespace detail {

template<class Encoding>
struct char_encoding_traits_impl
{
    using encoding_type = Encoding;

    template<class... Args>
    [[nodiscard]] static constexpr auto lit(Args&&... args)
        noexcept(noexcept(Encoding::lit(std::forward<Args>(args)...)))
    {
        return Encoding::lit(std::forward<Args>(args)...);
    }

    template<class... Args>
    [[nodiscard]] static constexpr auto string(Args&&... args)
        noexcept(noexcept(Encoding::string(std::forward<Args>(args)...)))
    {
        return Encoding::string(std::forward<Args>(args)...);
    }
};

} // detail

template<CharLike CharT>
struct char_encoding_traits;

template<>
struct char_encoding_traits<char> : detail::char_encoding_traits_impl<char_encoding::standard> {};

#ifndef IRIS_X4_NO_STANDARD_WIDE
template<>
struct char_encoding_traits<wchar_t> : detail::char_encoding_traits_impl<char_encoding::standard_wide> {};
#endif

#ifdef IRIS_X4_UNICODE
template<>
struct char_encoding_traits<char32_t> : detail::char_encoding_traits_impl<char_encoding::unicode> {};
#endif

} // iris::x4::traits

#endif
