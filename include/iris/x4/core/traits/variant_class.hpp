#ifndef IRIS_ZZ_X4_CORE_TRAITS_VARIANT_CLASS_HPP
#define IRIS_ZZ_X4_CORE_TRAITS_VARIANT_CLASS_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/rvariant/rvariant_fwd.hpp>
#include <iris/rvariant/variant_helper.hpp>
#include <iris/type_list.hpp>

#include <type_traits>

namespace iris::x4 {

template<class T>
struct is_variant : std::false_type {};

template<class T>
inline constexpr bool is_variant_v = is_variant<T>::value;

template<class... Ts>
struct is_variant<rvariant<Ts...>> : std::true_type {};

// `U` is an alternative of the variant `T`, `recursive_wrapper` unwrapped
template<class T, class U>
inline constexpr bool has_alternative_of = false;

template<class... Ts, class U>
inline constexpr bool has_alternative_of<rvariant<Ts...>, U> = is_in_v<U, unwrap_recursive_t<Ts>...>;

} // iris::x4

#endif
