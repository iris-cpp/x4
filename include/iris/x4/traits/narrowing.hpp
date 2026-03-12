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

// is_assignable_without_lossy_conversion<Dest, Source>
//
// True when `Dest = Source` is valid AND does not involve a lossy conversion.
// Currently this only rejects arithmetic narrowing ([dcl.init.list]), but
// the intent is broader: any conversion that silently loses information
// should eventually be caught here.
template<class Dest, class Source>
struct is_assignable_without_lossy_conversion
    : std::bool_constant<
        std::is_assignable_v<Dest, Source> &&
        (!std::is_arithmetic_v<std::remove_reference_t<Dest>> ||
         iris::is_convertible_without_narrowing_v<std::remove_cvref_t<Source>, std::remove_reference_t<Dest>>)
    >
{};

template<class DestTuple, class SourceTuple, class = std::make_index_sequence<alloy::tuple_size_v<DestTuple>>>
struct is_tuple_assignable_without_lossy_conversion;

template<class DestTuple, class SourceTuple, std::size_t... Is>
struct is_tuple_assignable_without_lossy_conversion<DestTuple, SourceTuple, std::index_sequence<Is...>>
    : std::conjunction<
        is_assignable_without_lossy_conversion<
            alloy::tuple_element_t<Is, DestTuple>&,
            decltype(alloy::get<Is>(std::declval<SourceTuple&>()))
        >...
    >
{};

} // iris::x4::detail

#endif
