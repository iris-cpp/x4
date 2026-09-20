#ifndef IRIS_ZZ_X4_CORE_ATTRIBUTE_HPP
#define IRIS_ZZ_X4_CORE_ATTRIBUTE_HPP

/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <concepts>
#include <type_traits>

namespace iris::x4 {

struct unused_type;
struct unused_container_type;

namespace detail {

struct parser_base;

template<class T>
concept has_parser_base = std::is_same_v<typename std::remove_cvref_t<T>::x4_parser_base_type, parser_base>;

} // detail

template<class T>
concept X4UnusedAttribute =
    std::same_as<std::remove_const_t<T>, unused_type> ||
    std::same_as<std::remove_const_t<T>, unused_container_type>;

template<class T>
concept X4NonUnusedAttribute =
    !X4UnusedAttribute<T> &&
    std::is_object_v<T> && // implies not reference
    !detail::has_parser_base<T> &&
    std::move_constructible<std::remove_const_t<T>>;
    // TODO: `fusion::iterator_range` does not satisfy these due to `fusion::vector`'s iterator being a reference type
    //std::default_initializable<std::remove_const_t<T>> &&
    //std::assignable_from<std::remove_const_t<T>&, std::remove_const_t<T>>;

template<class T>
concept X4Attribute = X4UnusedAttribute<T> || X4NonUnusedAttribute<T>;

} // iris::x4

#endif
