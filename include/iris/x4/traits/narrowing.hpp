#ifndef IRIS_ZZ_X4_TRAITS_NARROWING_HPP
#define IRIS_ZZ_X4_TRAITS_NARROWING_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/type_traits.hpp>

#include <iris/alloy/traits.hpp>

#include <cstddef>
#include <type_traits>
#include <utility>

namespace iris::x4::detail {

template<class DestTuple, class SourceTuple, class = std::make_index_sequence<alloy::tuple_size_v<DestTuple>>>
struct is_tuple_assignable_without_narrowing;

template<class DestTuple, class SourceTuple, std::size_t... Is>
struct is_tuple_assignable_without_narrowing<DestTuple, SourceTuple, std::index_sequence<Is...>>
    : std::conjunction<
        iris::is_assignable_without_narrowing<
            alloy::tuple_element_t<Is, DestTuple>&,
            decltype(alloy::get<Is>(std::declval<SourceTuple&>()))
        >...
    >
{};

} // iris::x4::detail

#endif
