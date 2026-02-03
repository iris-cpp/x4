#ifndef BOOST_SPIRIT_X4_TRAITS_OPTIONAL_TRAITS_HPP
#define BOOST_SPIRIT_X4_TRAITS_OPTIONAL_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/config.hpp>

#include <optional>
#include <type_traits>

namespace boost::spirit::x4 {

struct unused_type;
struct unused_container_type;

} // boost::spirit::x4


namespace boost::spirit::x4::traits {

template<class T>
struct is_optional : std::false_type {};

template<class T>
constexpr bool is_optional_v = is_optional<T>::value;

template<class T>
struct is_optional<std::optional<T>> : std::true_type {};

template<class T>
struct build_optional
{
    using type = std::optional<T>;
};

template<class T>
using build_optional_t = typename build_optional<T>::type;

template<class T>
struct build_optional<std::optional<T>>
{
    using type = std::optional<T>;
};

template<>
struct build_optional<unused_type>
{
    using type = unused_type;
};

template<>
struct build_optional<unused_container_type>
{
    using type = unused_container_type;
};

template<class T>
struct optional_value
{
    static_assert(is_optional_v<T>);
    using type = typename T::value_type;
};

template<class T>
using optional_value_t = typename optional_value<T>::type;

template<>
struct optional_value<unused_type>
{
    using type = unused_type;
};

template<>
struct optional_value<unused_type const>
{
    using type = unused_type;
};

} // boost::spirit::x4::traits

#endif
