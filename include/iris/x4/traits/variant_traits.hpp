#ifndef IRIS_ZZ_X4_TRAITS_VARIANT_TRAITS_HPP
#define IRIS_ZZ_X4_TRAITS_VARIANT_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/unused.hpp>

#include <iris/x4/traits/can_hold.hpp>

#include <iris/rvariant/variant_helper.hpp>

#include <concepts>
#include <type_traits>

namespace iris::x4::traits {

template<class T>
struct is_variant : std::false_type {};

template<class T>
constexpr bool is_variant_v = is_variant<T>::value;

// `std::variant` is not supported, as it does can't handle recursive types

template<class... Ts>
struct is_variant<iris::rvariant<Ts...>> : std::true_type {};


namespace detail {

template<class T, class... Ts>
struct any_of_unwrapped_exactly_same;

template<class T>
struct any_of_unwrapped_exactly_same<T>
    : std::false_type
{};

template<class T, class First, class... Rest>
    requires std::same_as<T, iris::unwrap_recursive_type<First>>
struct any_of_unwrapped_exactly_same<T, First, Rest...>
    : std::true_type
{};

template<class T, class First, class... Rest>
    requires (!std::same_as<T, iris::unwrap_recursive_type<First>>)
struct any_of_unwrapped_exactly_same<T, First, Rest...>
    : any_of_unwrapped_exactly_same<T, Rest...>
{};

template<class T, class... Ts>
struct variant_find_holdable_type_impl;

template<class T>
struct variant_find_holdable_type_impl<T>
{
    using type = T;
};

template<class T, class First, class... Rest>
struct variant_find_holdable_type_impl<T, First, Rest...>
{
    using type = std::conditional_t<
        can_hold<iris::unwrap_recursive_type<First>, T>::value,

        // Given some type `T`, when both `T` and `recursive_wrapper<T>` is seen
        // during attribute resolution, X4 should ideally materialize the latter
        // because:
        //   - It means that the user has supplied at least one explicit type
        //     (i.e. exposed attribute type, possibly a rule attribute type) that
        //     is `recursive_wrapper<T>`, and
        //   - constructing `T` and then moving it to `recursive_wrapper<T>`
        //     involves copying from stack to heap.
        //
        First, // no need to unwrap due to the reason described above

        typename variant_find_holdable_type_impl<T, Rest...>::type
    >;
};

} // detail


template<class Variant, class T>
struct variant_find_holdable_type;

template<class Variant>
struct variant_find_holdable_type<Variant, Variant>
{
    static_assert(is_variant_v<Variant>);
    using type = Variant;
};

template<class... Ts, class T>
    requires (!std::same_as<iris::rvariant<Ts...>, T>) && detail::any_of_unwrapped_exactly_same<T, Ts...>::value
struct variant_find_holdable_type<iris::rvariant<Ts...>, T>
{
    using type = T;
};

template<class... Ts, class T>
    requires (!std::same_as<iris::rvariant<Ts...>, T>) && (!detail::any_of_unwrapped_exactly_same<T, Ts...>::value)
struct variant_find_holdable_type<iris::rvariant<Ts...>, T>
{
    using type = typename detail::variant_find_holdable_type_impl<T, Ts...>::type;
};

} // iris::x4::traits

#endif
