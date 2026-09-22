#ifndef IRIS_ZZ_X4_CHAR_CHAR_HPP
#define IRIS_ZZ_X4_CHAR_CHAR_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/char_string_literal.hpp> // IWYU pragma: export
#include <iris/x4/char/any_char.hpp> // IWYU pragma: export

#include <iris/x4/traits/char_encoding_traits.hpp>

namespace iris::x4 {

namespace detail {

template<class CharT>
struct any_char_fn : any_char<traits::char_encoding_for<CharT>>
{};

} // detail

namespace parsers {

namespace standard {
[[maybe_unused]] inline constexpr x4::detail::any_char_fn<char> char_{};
} // standard

using standard::char_;

#ifndef IRIS_X4_NO_STANDARD_WIDE
namespace standard_wide {
[[maybe_unused]] inline constexpr x4::detail::any_char_fn<wchar_t> char_{};
} // standard_wide
#endif // IRIS_X4_NO_STANDARD_WIDE

#ifdef IRIS_X4_UNICODE
namespace unicode {
[[maybe_unused]] inline constexpr x4::detail::any_char_fn<char32_t> char_{};
} // unicode
#endif // IRIS_X4_UNICODE

} // parsers

using parsers::char_;


namespace standard {
using x4::parsers::standard::char_;
} // standard

#ifndef IRIS_X4_NO_STANDARD_WIDE
namespace standard_wide {
using x4::parsers::standard_wide::char_;
} // standard_wide
#endif // IRIS_X4_NO_STANDARD_WIDE

#ifdef IRIS_X4_UNICODE
namespace unicode {
using x4::parsers::unicode::char_;
} // unicode
#endif // IRIS_X4_UNICODE

} // iris::x4

#endif
