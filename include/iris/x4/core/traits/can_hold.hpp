#ifndef IRIS_ZZ_X4_CORE_TRAITS_CAN_HOLD_HPP
#define IRIS_ZZ_X4_CORE_TRAITS_CAN_HOLD_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/core/traits/tuple_traits.hpp>

#include <iris/x4/core/attribute.hpp>

#include <iris/alloy/traits.hpp>

#include <optional>
#include <type_traits>

namespace iris::x4 {

template<class T, class U>
struct can_hold;

template<class T>
struct is_variant;

namespace detail {

template<class TTuple, class UTuple, class IndexSeq = std::make_index_sequence<alloy::tuple_size_v<TTuple>>>
struct is_all_substitute_for_tuple_impl {};

template<class TTuple, class UTuple, std::size_t... Is>
struct is_all_substitute_for_tuple_impl<TTuple, UTuple, std::index_sequence<Is...>>
    : std::conjunction<can_hold<alloy::tuple_element_t<Is, TTuple>, alloy::tuple_element_t<Is, UTuple>>...> {};

template<class TTuple, class UTuple>
struct is_all_substitute_for_tuple : std::false_type {};

template<class TTuple, class UTuple>
    requires SameSizeTupleLike<TTuple, UTuple>
struct is_all_substitute_for_tuple<TTuple, UTuple> : is_all_substitute_for_tuple_impl<TTuple, UTuple> {};

template<class T, class U>
struct value_type_can_hold
    : can_hold<typename traits::container_value<T>::type, typename traits::container_value<U>::type>
{};

// This "implementation" exists for short-circuiting `can_hold` for certain trivial combinations
template<class T, class U>
struct can_hold_impl : std::false_type {};

template<class T, class U>
    requires
        alloy::is_tuple_like_v<T> &&
        alloy::is_tuple_like_v<U>
struct can_hold_impl<T, U>
    : detail::is_all_substitute_for_tuple<T, U>
{};

template<class T, class U>
    requires
        traits::is_container_v<T> &&
        traits::is_container_v<U>
struct can_hold_impl<T, U>
    : detail::value_type_can_hold<T, U>
{};

template<class T, class U>
    requires is_variant<T>::value && X4UnusedAttribute<U>
struct can_hold_impl<T, U>
    : std::false_type
{};

template<class T, class U>
    requires (!is_variant<T>::value) && X4UnusedAttribute<U>
struct can_hold_impl<T, U>
    : std::false_type
{};

template<class T, class U>
    requires is_variant<T>::value && (!X4UnusedAttribute<U>)
struct can_hold_impl<T, U>
    : std::is_assignable<T&, U>
{};

template<class T, class U>
struct can_hold_impl<std::optional<T>, std::optional<U>>
    : can_hold<T, U>
{};

} // detail

template<class T, class U>
struct can_hold
    : detail::can_hold_impl<T, U>
{
    static_assert(X4Attribute<T>);
    static_assert(X4Attribute<U>);
};

template<class T>
struct can_hold<T, T>
    : std::true_type
{
    static_assert(X4Attribute<T>);
};

template<class T, class U>
inline constexpr bool can_hold_v = can_hold<T, U>::value;

} // iris::x4

#endif
