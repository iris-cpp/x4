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
struct has_same_size
    : std::bool_constant<
        alloy::tuple_size_v<A> ==
        alloy::tuple_size_v<B>
    >
{};

template<class T, std::size_t N>
struct has_size
    : std::bool_constant<alloy::tuple_size_v<T> == N>
{};

template<class A, class B>
struct is_same_size_tuple_like
    : std::bool_constant<std::conjunction_v<
        alloy::is_tuple_like<A>,
        alloy::is_tuple_like<B>,
        has_same_size<A, B>
    >>
{};

template<class T>
struct is_single_element_tuple_like
    : std::bool_constant<std::conjunction_v<
        alloy::is_tuple_like<T>,
        has_size<T, 1>
    >>
{};

template<class View>
struct is_single_element_tuple_like_view
    : std::bool_constant<std::conjunction_v<
        alloy::is_tuple_like_view<View>,
        has_size<View, 1>
    >>
{};

template<class T>
struct unwrap_single_element_tuple_like
{
    using type = T;
};

template<class T>
    requires is_single_element_tuple_like<T>::value
struct unwrap_single_element_tuple_like<T>
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
[[nodiscard]] constexpr auto&& unwrap_single_element(T&& value) noexcept
{
    return std::forward<T>(value);
}

template<class T>
    requires traits::is_single_element_tuple_like<std::remove_cvref_t<T>>::value
[[nodiscard]] constexpr auto&& unwrap_single_element(T&& value) noexcept(noexcept(alloy::get<0>(std::declval<T>())))
{
    // forward_like is *required*, since when Source is `alloy::tuple<int&>` `alloy::get<0>(std::forward<Source>(src))` returns `int&` whereas we want `int&&` instead 
    return std::forward_like<T>(alloy::get<0>(std::forward<T>(value)));
}

} // iris::x4::traits

#endif
