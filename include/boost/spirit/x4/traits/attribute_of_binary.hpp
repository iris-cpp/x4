#ifndef BOOST_SPIRIT_X4_TRAITS_ATTRIBUTE_OF_BINARY_HPP
#define BOOST_SPIRIT_X4_TRAITS_ATTRIBUTE_OF_BINARY_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2020 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/x4/core/attribute.hpp>
#include <boost/spirit/x4/core/unused.hpp>
#include <boost/spirit/x4/core/parser_traits.hpp>

#include <type_traits>

namespace boost::spirit::x4::traits {

namespace detail {

template<class... Ts>
struct type_sequence
{
    using type = type_sequence;

    static constexpr std::size_t size = sizeof...(Ts);

    template<class... Us>
    using prepend = type_sequence<Us..., Ts...>;

    template<class U>
    using extend = U::template prepend<Ts...>;

    template<template<class...> class U>
    using transfer_to = U<Ts...>;
};

template<X4Attribute Attr>
struct types_of_binary_init : type_sequence<Attr>
{};

template<>
struct types_of_binary_init<unused_type> : type_sequence<>
{};

template<>
struct types_of_binary_init<unused_type const> : type_sequence<>
{};

template<template<class, class> class BinaryParserTT, class ParserT>
struct get_types_of_binary
    : types_of_binary_init<typename parser_traits<ParserT>::attribute_type> // TODO: unwrap
{};

template<template<class, class> class BinaryParserTT, class Left, class Right>
struct get_types_of_binary<BinaryParserTT, BinaryParserTT<Left, Right>>
    : get_types_of_binary<BinaryParserTT, Left>::template extend<get_types_of_binary<BinaryParserTT, Right>>
{};

template<template<class...> class AttrTT, class T, std::size_t = T::size>
struct type_sequence_to_attribute
{
    using type = typename T::template transfer_to<AttrTT>;
};

template<template<class...> class AttrTT, class T>
struct type_sequence_to_attribute<AttrTT, T, 1>
    : T::template transfer_to<std::type_identity>
{};

template<template<class...> class AttrTT, class T>
struct type_sequence_to_attribute<AttrTT, T, 0>
{
    using type = unused_type;
};

} // detail

template<
    template<class...> class AttrTT,
    template<class, class> class BinaryParserTT,
    class Left, class Right
>
struct attribute_of_binary
{
    using type = detail::type_sequence_to_attribute<
        AttrTT,
        typename detail::get_types_of_binary<BinaryParserTT, BinaryParserTT<Left, Right>>::type
    >::type;
};

} // boost::spirit::x4::traits

#endif
