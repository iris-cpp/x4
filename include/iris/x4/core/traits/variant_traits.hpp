#ifndef IRIS_ZZ_X4_CORE_TRAITS_VARIANT_TRAITS_HPP
#define IRIS_ZZ_X4_CORE_TRAITS_VARIANT_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/traits/write_rank.hpp>

#include <iris/rvariant/variant_helper.hpp>
#include <iris/type_traits.hpp>

#include <type_traits>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4::detail {

// The index of the alternative of `Variant` that `write_rank` selects for writing a `Value`.
// No `::value` if the write is rejected or does not go into a single alternative (a variant value
// written alternative by alternative).
//
// Note: This is an index, rather than a type, as a type may appear twice.
template<class Variant, class Value>
struct variant_alternative_for {};

template<class Variant, class Value>
    requires
        (planner::node_write_of<planner::write_node<Variant, planner::model_value_t<Value>>>.writable) &&
        (planner::node_write_of<planner::write_node<Variant, planner::model_value_t<Value>>>.alternative != planner::no_index)
struct variant_alternative_for<Variant, Value>
{
    static constexpr std::size_t value = planner::node_write_of<
        planner::write_node<Variant, planner::model_value_t<Value>>
    >.alternative;
};

template<class Variant, class Value>
inline constexpr std::size_t variant_alternative_for_v = variant_alternative_for<Variant, Value>::value;

template<class Variant, class Value>
inline constexpr bool variant_has_alternative_for_v = requires { variant_alternative_for<Variant, Value>::value; };

} // iris::x4::detail

#endif
