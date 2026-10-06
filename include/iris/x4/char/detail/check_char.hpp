#ifndef IRIS_ZZ_X4_CHAR_DETAIL_CHECK_CHAR_HPP
#define IRIS_ZZ_X4_CHAR_DETAIL_CHECK_CHAR_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/error/throwf.hpp>

#include <concepts>
#include <stdexcept>

namespace iris::x4::detail {

template<class Encoding>
constexpr void check_char(std::same_as<typename Encoding::char_type> auto const ch)
{
    if (!Encoding::ischar(ch)) {
        iris::throwf<std::invalid_argument>("The character is not valid in the encoding");
    }
}

template<class Encoding>
constexpr void check_char_range(
    std::same_as<typename Encoding::char_type> auto const from,
    std::same_as<typename Encoding::char_type> auto const to
)
{
    detail::check_char<Encoding>(from);
    detail::check_char<Encoding>(to);
    if (static_cast<Encoding::classify_type>(from) > static_cast<Encoding::classify_type>(to)) {
        iris::throwf<std::invalid_argument>("The first character of a range is greater than the last character");
    }
}

} // iris::x4::detail

#endif
