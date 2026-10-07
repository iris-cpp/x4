#ifndef IRIS_ZZ_X4_CHAR_CHAR_HPP
#define IRIS_ZZ_X4_CHAR_CHAR_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/char/any_char.hpp> // IWYU pragma: export

#include <iris/x4/core/traits/char_encoding_traits.hpp>

namespace iris::x4 {

namespace detail {

template<class CharT>
struct any_char_fn : any_char<char_encoding_for<CharT>>
{};

} // detail

namespace parsers {

namespace standard {
[[maybe_unused]] inline constexpr x4::detail::any_char_fn<char> char_{};
} // standard

using standard::char_;

namespace unicode {
[[maybe_unused]] inline constexpr x4::detail::any_char_fn<char32_t> char_{};
} // unicode

} // parsers

using parsers::char_;


namespace standard {
using x4::parsers::standard::char_;
} // standard

namespace unicode {
using x4::parsers::unicode::char_;
} // unicode

} // iris::x4

#endif
