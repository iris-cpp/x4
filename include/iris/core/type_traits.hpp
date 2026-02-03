#ifndef IRIS_CORE_TYPE_TRAITS_HPP
#define IRIS_CORE_TYPE_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp>

#include <type_traits>

namespace boost::spirit {

template<class T, template<class...> class TT>
struct is_ttp_specialization_of : std::false_type {};

template<template<class...> class TT, class... Ts>
struct is_ttp_specialization_of<TT<Ts...>, TT> : std::true_type {};

template<class T, template<class...> class TT>
inline constexpr bool is_ttp_specialization_of_v = is_ttp_specialization_of<T, TT>::value;


template<class T, template<auto...> class TT>
struct is_nttp_specialization_of : std::false_type {};

template<template<auto...> class TT, auto... Ts>
struct is_nttp_specialization_of<TT<Ts...>, TT> : std::true_type {};

template<class T, template<auto...> class TT>
inline constexpr bool is_nttp_specialization_of_v = is_nttp_specialization_of<T, TT>::value;

} // boost::spirit

#endif
