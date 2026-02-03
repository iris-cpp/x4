#ifndef IRIS_X4_TRAITS_SUBRANGE_TRAITS_HPP
#define IRIS_X4_TRAITS_SUBRANGE_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iterator>
#include <ranges>
#include <type_traits>

namespace boost::spirit::x4::traits {

template<class T>
struct is_subrange : std::false_type {};

template<class T>
constexpr bool is_subrange_v = is_subrange<T>::value;

template<std::input_or_output_iterator It, std::sentinel_for<It> Se, std::ranges::subrange_kind Kind>
struct is_subrange<std::ranges::subrange<It, Se, Kind>> : std::true_type {};

} // boost::spirit::x4::traits

#endif
