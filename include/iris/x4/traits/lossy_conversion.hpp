#ifndef IRIS_ZZ_X4_TRAITS_LOSSY_CONVERSION_HPP
#define IRIS_ZZ_X4_TRAITS_LOSSY_CONVERSION_HPP

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

// is_lossy_assignment<Dest, Source>
//
// Customization point: determines whether `Dest = Source` is a lossy assignment.
// The default implementation detects arithmetic narrowing conversions ([dcl.init.list]).
// Users can specialize this trait for their own types (e.g. big integer libraries)
// to flag additional lossy assignments.
//
// Dest and Source are unqualified types (no references, no cv).
template<class Dest, class Source>
struct is_lossy_assignment : std::false_type {};

template<class Dest, class Source>
    requires std::is_arithmetic_v<Dest>
struct is_lossy_assignment<Dest, Source>
    : std::bool_constant<
        !iris::is_convertible_without_narrowing_v<Source, Dest>
    >
{};

// is_assignable_without_lossy_conversion<Dest, Source>
//
// True when `Dest = Source` is valid AND does not involve a lossy conversion
// as determined by `is_lossy_assignment`.
template<class Dest, class Source>
struct is_assignable_without_lossy_conversion
    : std::bool_constant<
        std::is_assignable_v<Dest, Source> &&
        !is_lossy_assignment<std::remove_reference_t<Dest>, std::remove_cvref_t<Source>>::value
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
