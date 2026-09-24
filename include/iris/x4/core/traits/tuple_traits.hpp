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

namespace iris::x4 {

template<class A, class B>
struct tuple_has_same_size
    : std::bool_constant<
        alloy::tuple_size_v<std::remove_cvref_t<A>> ==
        alloy::tuple_size_v<std::remove_cvref_t<B>>
    >
{};

template<class A, class B>
constexpr bool tuple_has_same_size_v = tuple_has_same_size<A, B>::value;

template<class T, std::size_t N>
struct tuple_has_size
    : std::bool_constant<alloy::tuple_size_v<std::remove_cvref_t<T>> == N>
{};

template<class T, std::size_t N>
constexpr bool tuple_has_size_v = tuple_has_size<T, N>::value;

template<class A, class B>
struct tuple_is_same_size_sequence
    : std::bool_constant<std::conjunction_v<
        alloy::is_tuple_like<std::remove_cvref_t<A>>,
        alloy::is_tuple_like<std::remove_cvref_t<B>>,
        tuple_has_same_size<A, B>
    >>
{};

template<class A, class B>
constexpr bool tuple_is_same_size_sequence_v = tuple_is_same_size_sequence<A, B>::value;

template<class Seq>
struct tuple_is_size_one_sequence
    : std::bool_constant<std::conjunction_v<
        alloy::is_tuple_like<std::remove_cvref_t<Seq>>,
        tuple_has_size<Seq, 1>
    >>
{};

template<class Seq>
constexpr bool tuple_is_size_one_sequence_v = tuple_is_size_one_sequence<Seq>::value;

template<class View>
struct tuple_is_size_one_view
    : std::bool_constant<std::conjunction_v<
        alloy::is_tuple_like_view<std::remove_cvref_t<View>>,
        tuple_has_size<View, 1>
    >>
{};

template<class View>
constexpr bool tuple_is_size_one_view_v = tuple_is_size_one_view<View>::value;


template<class T>
struct synthesized_value
{
    using type = T;
};

template<class T>
using synthesized_value_t = typename synthesized_value<T>::type;

template<class T>
    requires tuple_is_size_one_sequence_v<std::remove_cvref_t<T>>
struct synthesized_value<T>
{
    using type = std::remove_cvref_t<alloy::tuple_element_t<0, T>>;
};

} // iris::x4

#endif
