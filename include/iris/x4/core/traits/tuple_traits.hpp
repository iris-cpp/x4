#ifndef IRIS_ZZ_X4_CORE_TRAITS_TUPLE_TRAITS_HPP
#define IRIS_ZZ_X4_CORE_TRAITS_TUPLE_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
================================================_==============================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/alloy/traits.hpp>

#include <type_traits>
#include <utility>

namespace iris::x4 {

// Tuple-likes of the same size
template<class A, class B>
concept SameSizeTupleLike =
    alloy::TupleLike<A> &&
    alloy::TupleLike<B> &&
    (alloy::tuple_size_v<std::remove_cvref_t<A>> == alloy::tuple_size_v<std::remove_cvref_t<B>>);

// A single-element tuple-like: `alloy::tuple<T>`, or a user-defined struct
// adapted with a single member
template<class T>
concept SingleElementTupleLike =
    alloy::TupleLike<T> &&
    (alloy::tuple_size_v<std::remove_cvref_t<T>> == 1);

// A single-element tuple-like whose element is an lvalue reference
template<class T>
concept SingleElementTupleLikeView = SingleElementTupleLike<T> && alloy::TupleLikeView<T>;

namespace detail {

template<class T>
struct unwrap_single_element_impl
{
    using type = std::remove_cvref_t<T>;
};

template<SingleElementTupleLike T>
struct unwrap_single_element_impl<T>
{
    using type = std::remove_cvref_t<alloy::tuple_element_t<0, std::remove_cvref_t<T>>>;
};

struct unwrap_single_element_fn
{
    template<class T>
    [[nodiscard]] static constexpr T&& operator()(T&& value) noexcept
    {
        return std::forward<T>(value);
    }

    template<SingleElementTupleLike T>
    [[nodiscard]] static constexpr decltype(auto) operator()(T&& value) noexcept
    {
        return alloy::get<0>(std::forward<T>(value));
    }
};

} // detail

// The element of a single-element tuple-like, or the type itself otherwise;
// cv-qualifiers and references are removed. One level only.
template<class T>
using unwrap_single_element_t = detail::unwrap_single_element_impl<T>::type;

// The element of a single-element tuple-like, exactly as `alloy::get<0>`
// yields it (a reference element keeps its own reference type), or the
// argument itself otherwise
inline constexpr detail::unwrap_single_element_fn unwrap_single_element{};

} // iris::x4

#endif
