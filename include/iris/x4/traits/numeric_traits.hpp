#ifndef IRIS_X4_TRAITS_NUMERIC_TRAITS_HPP
#define IRIS_X4_TRAITS_NUMERIC_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <limits>
#include <type_traits>

namespace boost::spirit::x4::traits {

// Customization points for numeric operations

template<class T>
struct pow10_helper;

template<class T>
struct check_overflow : std::bool_constant<std::numeric_limits<T>::is_bounded>
{};

} // boost::spirit::x4::traits

#endif
