#ifndef IRIS_X4_TRAITS_SUBSTITUTION_HPP
#define IRIS_X4_TRAITS_SUBSTITUTION_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/attribute.hpp>
#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/traits/tuple_traits.hpp>

#include <iris/alloy/traits.hpp>

#include <optional>
#include <type_traits>

namespace iris::x4::traits {

template<class T>
struct is_variant;

// Find out if T can be a (strong) substitute for Attribute
template<class T, X4Attribute Attr>
struct is_substitute;

template<class T, X4Attribute Attr>
constexpr bool is_substitute_v = is_substitute<T, Attr>::value;

template<class Variant, X4Attribute Attr>
struct variant_has_substitute;

// TODO: reduce MPL usage

namespace detail {

template<class TTuple, class UTuple, class IndexSeq = std::make_index_sequence<alloy::tuple_size_v<TTuple>>>
struct is_all_substitute_for_tuple_impl {};

template<class TTuple, class UTuple, std::size_t... Is>
struct is_all_substitute_for_tuple_impl<TTuple, UTuple, std::index_sequence<Is...>>
    : std::conjunction<is_substitute<alloy::tuple_element_t<Is, TTuple>, alloy::tuple_element_t<Is, UTuple>>...> {};

template<class TTuple, class UTuple>
struct is_all_substitute_for_tuple : std::false_type {};

template<class TTuple, class UTuple>
    requires is_same_size_sequence_v<TTuple, UTuple>
struct is_all_substitute_for_tuple<TTuple, UTuple> : is_all_substitute_for_tuple_impl<TTuple, UTuple> {};

template<class T, X4Attribute Attr>
struct value_type_is_substitute
    : is_substitute<container_value_t<T>, container_value_t<Attr>>
{};

template<class T, X4Attribute Attr>
struct is_substitute_impl : std::false_type {};

template<class T, X4Attribute Attr>
    requires std::conjunction_v<
        alloy::is_tuple_like<T>,
        alloy::is_tuple_like<Attr>
    >
struct is_substitute_impl<T, Attr>
    : is_all_substitute_for_tuple<T, Attr>
{};

template<class T, X4Attribute Attr>
    requires
        is_container_v<std::remove_const_t<T>> &&
        is_container_v<std::remove_const_t<Attr>>
struct is_substitute_impl<T, Attr>
    : value_type_is_substitute<T, Attr>
{};

template<class T, X4Attribute Attr>
    requires is_variant<std::remove_const_t<Attr>>::value
struct is_substitute_impl<T, Attr>
    : variant_has_substitute<Attr, T>
{};

} // detail

template<class T, X4Attribute Attr>
struct is_substitute
    : std::disjunction<
          std::is_same<T, Attr>,
          detail::is_substitute_impl<T, Attr>
    >
{};

template<class T, X4UnusedAttribute Attr>
struct is_substitute<T, Attr>
    : std::false_type
{};

// for reference T
template<class T, X4Attribute Attr>
struct is_substitute<T&, Attr>
    : is_substitute<T, Attr>
{};

// for reference Attribute
template<class T, X4Attribute Attr>
struct is_substitute<T, Attr&>
    : is_substitute<T, Attr>
{};

template<class T, X4Attribute Attr>
struct is_substitute<std::optional<T>, std::optional<Attr>>
    : is_substitute<T, Attr>
{};

} // iris::x4::traits

#endif
