#ifndef IRIS_ZZ_X4_TRAITS_ATTRIBUTE_OF_BINARY_HPP
#define IRIS_ZZ_X4_TRAITS_ATTRIBUTE_OF_BINARY_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2020 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/parser_traits.hpp>

#include <iris/type_traits.hpp>  // TODO: move iris::type_list to separate header

namespace iris::x4::traits {

namespace detail {

template<class TTypeList, class UTypeList>
struct concat_type_list;

template<class... Ts, class... Us>
struct concat_type_list<type_list<Ts...>, type_list<Us...>>
{
    using type = type_list<Ts..., Us...>;
};

template<class TTypeList, class UTypeList>
using concat_type_list_t = concat_type_list<TTypeList, UTypeList>::type;

} // detail

#define IRIS_X4_TRAITS_DETAIL_DEFINE_TYPE_LIST_CONV(postfix, tmpl) \
    namespace detail { \
    template<class T> \
    struct to_type_list_##postfix \
    { \
        using type = type_list<T>; \
    }; \
    template<> \
    struct to_type_list_##postfix<unused_type> \
    { \
        using type = type_list<>; \
    }; \
    template<class... Ts> \
    struct to_type_list_##postfix<tmpl<Ts...>> \
    { \
        using type = type_list<Ts...>; \
    }; \
    template<class T> \
    using to_type_list_##postfix##_t = to_type_list_##postfix<T>::type; \
    template<class TypeList> \
    struct from_type_list_##postfix {}; \
    template<> \
    struct from_type_list_##postfix<type_list<>> \
    { \
        using type = unused_type; \
    }; \
    template<class T> \
    struct from_type_list_##postfix<type_list<T>> \
    { \
        using type = T; \
    }; \
    template<class T0, class T1, class... Ts> \
    struct from_type_list_##postfix<type_list<T0, T1, Ts...>> \
    { \
        using type = tmpl<T0, T1, Ts...>; \
    }; \
    template<class TypeList> \
    using from_type_list_##postfix##_t = from_type_list_##postfix<TypeList>::type; \
    } \
    template<class LeftParser, class RightParser> \
    struct attribute_of_##postfix { \
        using type = detail::from_type_list_##postfix##_t< \
            detail::concat_type_list_t< \
                detail::to_type_list_##postfix##_t<typename parser_traits<LeftParser>::attribute_type>, \
                detail::to_type_list_##postfix##_t<typename parser_traits<RightParser>::attribute_type> \
            > \
        >; \
    };

IRIS_X4_TRAITS_DETAIL_DEFINE_TYPE_LIST_CONV(sequence, alloy::tuple)
IRIS_X4_TRAITS_DETAIL_DEFINE_TYPE_LIST_CONV(alternative, rvariant)

#undef IRIS_X4_TRAITS_DETAIL_DEFINE_TYPE_LIST_CONV

} // iris::x4::traits

#endif
