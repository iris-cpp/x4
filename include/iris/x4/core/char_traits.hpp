#ifndef IRIS_ZZ_X4_CORE_CHAR_TRAITS_HPP
#define IRIS_ZZ_X4_CORE_CHAR_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
================================================_==============================*/

#include <iris/config.hpp>

#include <concepts>
#include <type_traits>

namespace iris::x4 {

template<class T>
concept CharLike =
    std::same_as<std::remove_cvref_t<T>, char> ||
    std::same_as<std::remove_cvref_t<T>, wchar_t> ||
    std::same_as<std::remove_cvref_t<T>, char8_t> ||
    std::same_as<std::remove_cvref_t<T>, char16_t> ||
    std::same_as<std::remove_cvref_t<T>, char32_t>;

template<class T>
concept CharArray =
    std::is_array_v<std::remove_cvref_t<T>> &&
    CharLike<std::remove_extent_t<std::remove_cvref_t<T>>>;

// Spirit has historically converted "c" to 'c'.
//
// While we think it's still useful to retain the conversion,
// we need to avoid further conversion to `std::basic_string_view`,
// which leads to performance overhead. This trait enables
// detection of such arrays.
//
// Note that the status quo introduces ambiguity in determining
// {'c', '\0'} and {'c', 'd'}, but we're not aware of any practical
// usage of non-null-terminated character array in the context of
// DSL on parser combinator.
//
// However, if compelling use cases emerge, we may revise these
// semantics. Versioned as `X4` for forward compatibility.
template<class T, class CharT>
concept X4VagueArrayOf2Chars =
    std::same_as<std::remove_extent_t<std::remove_cvref_t<T>>, CharT> &&
    std::is_bounded_array_v<std::remove_cvref_t<T>> &&
    std::extent_v<std::remove_cvref_t<T>> == 2;

// Mixing incompatible character types is semantically wrong.
// Don't do that. It may even lead to security vulnerabilities.
template<class T, class ExpectedCharT>
concept CharIncompatibleWith =
    CharLike<T> &&
    !std::same_as<T, ExpectedCharT>;

} // iris::x4

#endif
