#ifndef IRIS_ALLOY_DETAIL_FORWARD_LIKE_T_HPP
#define IRIS_ALLOY_DETAIL_FORWARD_LIKE_T_HPP

/*=============================================================================
    Copyright (c) 2025 Yaito Kakeyama
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <type_traits>

namespace iris::alloy::detail {

// Clang workaround; as code below does not work unless -fno-builtin-std-forward_like is specified
// ```
// template<class T, class U>
// using forward_like_t = decltype(std::forward_like<T>(std::declval<U>()));
// ```
template<class T, class U>
using forward_like_t = std::conditional_t<
    std::is_rvalue_reference_v<T&&>,
    std::conditional_t<
        std::is_const_v<std::remove_reference_t<T>>,
        std::remove_cvref_t<U> const,
        std::remove_cvref_t<U>
    >&&,
    std::conditional_t<
        std::is_const_v<std::remove_reference_t<T>>,
        std::remove_cvref_t<U> const,
        std::remove_cvref_t<U>
    >&
>;

} // iris::alloy::detail

#endif
