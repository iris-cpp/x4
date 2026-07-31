#ifndef IRIS_ZZ_X4_CORE_STRING_TRAITS_HPP
#define IRIS_ZZ_X4_CORE_STRING_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
================================================_==============================*/

#include <iris/config.hpp>

#include <iris/x4/core/char_traits.hpp>

#include <string>
#include <string_view>
#include <type_traits>
#include <concepts>

namespace iris::x4 {

// Mixing incompatible character types is semantically wrong.
// Don't do that. It may even lead to security vulnerabilities.
template<class T, class ExpectedCharT>
concept StringLikeIncompatibleWith =
    CharLike<std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>> &&
    !std::convertible_to<T, std::basic_string_view<ExpectedCharT>>;

// Main utility to guide `char_`, `lit` and `string` to be
// resolved into either `x4::literal_char` or `x4::literal_string`.
//
// This may also be used in other codes which require the same
// semantics.
template<class T, class CharT>
concept CppStringLike =
    // This avoids converting `CharT[2]` to `std::basic_string_view`.
    (!X4VagueArrayOf2Chars<T, CharT>) &&
    // All other types that are *naturally* convertible to `std::basic_string_view`.
    std::convertible_to<std::decay_t<T>, std::basic_string_view<CharT>>;

template<class T>
using maybe_owning_string = std::conditional_t<
    std::is_pointer_v<std::decay_t<T>>,
    std::basic_string_view<std::remove_const_t<std::remove_pointer_t<std::decay_t<T>>>>,
    std::remove_cvref_t<T>
>;

} // iris::x4

#endif
