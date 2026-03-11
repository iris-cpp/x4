#ifndef IRIS_ZZ_X4_TRAITS_TUPLE_TRAITS_HPP
#define IRIS_ZZ_X4_TRAITS_TUPLE_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
================================================_==============================*/

#include <iris/alloy/traits.hpp>

#include <type_traits>

namespace iris::x4::traits {

template<class A, class B>
struct is_same_size_tuple_like
    : std::false_type
{};

template<class A, class B>
    requires
        alloy::is_tuple_like_v<A> &&
        alloy::is_tuple_like_v<B>
struct is_same_size_tuple_like<A, B>
    : std::bool_constant<alloy::tuple_size_v<A> == alloy::tuple_size_v<B>>
{};

template<class T>
struct is_single_element_tuple_like
    : std::false_type
{};

template<class T>
    requires alloy::is_tuple_like_v<T>
struct is_single_element_tuple_like<T>
    : std::bool_constant<alloy::tuple_size_v<T> == 1>
{};

template<class T>
struct unwrap_if_single_element_tuple_like
{
    using type = T;
};

template<class T>
    requires is_single_element_tuple_like<T>::value
struct unwrap_if_single_element_tuple_like<T>
{
    using type = alloy::tuple_element_t<0, T>;
};

template<class T>
struct unwrap_single_element_plain
{
    using type = std::remove_cvref_t<T>;
};

template<class T>
    requires traits::is_single_element_tuple_like<std::remove_cvref_t<T>>::value
struct unwrap_single_element_plain<T>
{
    using type = std::remove_cvref_t<alloy::tuple_element_t<0, T>>;
};

template<class T>
[[nodiscard]] constexpr auto&& do_unwrap_if_single_element_tuple_like(T&& value) noexcept
{
    return std::forward<T>(value);
}

template<class T>
    requires traits::is_single_element_tuple_like<std::remove_cvref_t<T>>::value
[[nodiscard]] constexpr auto&& do_unwrap_if_single_element_tuple_like(T&& value) noexcept(noexcept(alloy::get<0>(std::declval<T>())))
{
    return alloy::get<0>(std::forward<T>(value));
}

} // iris::x4::traits

#endif
