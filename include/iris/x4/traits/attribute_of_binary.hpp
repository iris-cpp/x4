#ifndef IRIS_X4_TRAITS_ATTRIBUTE_OF_BINARY_HPP
#define IRIS_X4_TRAITS_ATTRIBUTE_OF_BINARY_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2020 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/attribute.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/parser_traits.hpp>

#include <iris/type_traits.hpp>  // TODO: move iris::type_list to separate header

#include <type_traits>

namespace iris::x4::traits {

namespace detail {

template<class TypeList, class... Us>
struct append_to_type_list {};

template<class TypeList, class... Us>
using append_to_type_list_t = append_to_type_list<TypeList, Us...>::type; 

template<class... Ts>
struct append_to_type_list<type_list<Ts...>>
{
    using type = type_list<Ts...>;
};

template<class... Ts, class... Us>
struct append_to_type_list<type_list<Ts...>, unused_type, Us...>
    : append_to_type_list<type_list<Ts...>, Us...>
{};

template<class... Ts, class U, class... Us>
struct append_to_type_list<type_list<Ts...>, U, Us...>
    : append_to_type_list<type_list<Ts..., U>, Us...>
{};

template<class... Ts, class... Us, class... Vs>
struct append_to_type_list<type_list<Ts...>, type_list<Us...>, Vs...>
    : append_to_type_list<append_to_type_list_t<type_list<Ts...>, Us...>, Vs...>
{};

template<template<class...> class TupleTT, class T>
struct tuple_to_type_list;

template<template<class...> class TupleTT, class T>
using tuple_to_type_list_t = tuple_to_type_list<TupleTT, T>::type;

template<template<class...> class TupleTT, class T>
struct tuple_to_type_list
{
    using type = T;
};

template<template<class...> class TupleTT, class... Ts>
struct tuple_to_type_list<TupleTT, TupleTT<Ts...>>
{
    using type = type_list<tuple_to_type_list_t<TupleTT, Ts>...>;
};

template<template<class...> class TupleTT, class T>
using tuple_to_type_list_t = tuple_to_type_list<TupleTT, T>::type;

template<template<class...> class TupleTT, class TypeList>
struct type_list_to_tuple {};

template<template<class...> class TupleTT>
struct type_list_to_tuple<TupleTT, type_list<>>
{
    using type = unused_type;
};

template<template<class...> class TupleTT, class T>
struct type_list_to_tuple<TupleTT, type_list<T>>
{
    using type = T;
};

template<template<class...> class TupleTT, class T0, class T1, class... Ts>
struct type_list_to_tuple<TupleTT, type_list<T0, T1, Ts...>>
{
    using type = TupleTT<T0, T1, Ts...>;
};

template<template<class...> class TupleTT, class TypeList>
using type_list_to_tuple_t = type_list_to_tuple<TupleTT, TypeList>::type;

} // detail

template<
    template<class...> class TupleTT,
    class LeftParser, class RightParser
>
struct attribute_of_binary
{
    using type = detail::type_list_to_tuple_t<
        TupleTT,
        detail::append_to_type_list_t<
            type_list<>,
            detail::tuple_to_type_list_t<TupleTT, typename parser_traits<LeftParser>::attribute_type>,
            detail::tuple_to_type_list_t<TupleTT, typename parser_traits<RightParser>::attribute_type>
        >
    >;
};

} // iris::x4::traits

#endif
