/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_X4_IS_RANGE_DEC_06_2017_1900PM
#define BOOST_SPIRIT_X4_IS_RANGE_DEC_06_2017_1900PM

#include <iterator>
#include <ranges>
#include <type_traits>

namespace boost::spirit::x4::traits
{
    template <typename T>
    struct is_subrange : std::false_type {};

    template <typename T>
    constexpr bool is_subrange_v = is_subrange<T>::value;

    template <std::input_or_output_iterator It, std::sentinel_for<It> Se, std::ranges::subrange_kind Kind>
    struct is_subrange<std::ranges::subrange<It, Se, Kind>> : std::true_type {};

} // boost::spirit::x4::traits

#endif
