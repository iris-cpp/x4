#ifndef IRIS_ZZ_X4_CORE_CHAR_TRAITS_HPP
#define IRIS_ZZ_X4_CORE_CHAR_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
================================================_==============================*/

// This header is intended for inclusion by user-facing, non-parser headers.
// Do not add includes specific to X4's parser implementation here.

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/string.hpp>

#include <concepts>
#include <type_traits>

namespace iris::x4 {

template<class T>
concept CharArray =
    std::is_array_v<std::remove_cvref_t<T>> &&
    CharLike<std::remove_extent_t<std::remove_cvref_t<T>>>;

// Mixing incompatible character types is semantically wrong.
// Don't do that. It may even lead to security vulnerabilities.
template<class T, class ExpectedCharT>
concept CharIncompatibleWith =
    CharLike<T> &&
    !std::same_as<T, ExpectedCharT>;

namespace detail {

template<class CharT>
struct char_tokens;

template<>
struct char_tokens<char>
{
    static constexpr char hyphen = '-';
};

template<>
struct char_tokens<wchar_t>
{
    static constexpr wchar_t hyphen = L'-';
};

template<>
struct char_tokens<char32_t>
{
    static constexpr char32_t hyphen = U'-';
};

} // detail

} // iris::x4

#endif
