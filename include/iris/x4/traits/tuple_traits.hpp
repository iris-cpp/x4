#ifndef IRIS_X4_TRAITS_TUPLE_TRAITS_HPP
#define IRIS_X4_TRAITS_TUPLE_TRAITS_HPP

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
struct is_same_size_tuple_like_impl {};

template<class A, class B>
  requires alloy::is_tuple_like_v<A> && alloy::is_tuple_like_v<B>
struct is_same_size_tuple_like_impl<A, B>
    : std::bool_constant<alloy::tuple_size_v<A> == alloy::tuple_size_v<B>>
{};

template<class A, class B>
struct is_same_size_tuple_like
    : std::bool_constant<std::conjunction_v<
        alloy::is_tuple_like<A>,
        alloy::is_tuple_like<B>,
        is_same_size_tuple_like_impl<A, B>
    >>
{};

template<class A, class B>
constexpr bool is_same_size_tuple_like_v = is_same_size_tuple_like<A, B>::value;

template<class T>
struct is_single_element_tuple_like_impl {};

template<class T>
  requires alloy::is_tuple_like_v<T>
struct is_single_element_tuple_like_impl<T>
    : std::bool_constant<alloy::tuple_size_v<T> == 1>
{};

template<class T>
struct is_single_element_tuple_like
    : std::bool_constant<std::conjunction_v<
        alloy::is_tuple_like<T>,
        is_single_element_tuple_like_impl<T>
    >>
{};

template<class T>
constexpr bool is_single_element_tuple_like_v = is_single_element_tuple_like<T>::value;

template<class T>
struct unwrap_single_element_tuple_like;

template<class T>
using unwrap_single_element_tuple_like_t = unwrap_single_element_tuple_like<T>::type;

template<class T>
struct unwrap_single_element_tuple_like
{
    using type = T;
};

template<class T>
  requires is_single_element_tuple_like_v<T>
struct unwrap_single_element_tuple_like<T>
{
    using type = alloy::tuple_element_t<0, T>;
};

} // iris::x4::traits

#endif
