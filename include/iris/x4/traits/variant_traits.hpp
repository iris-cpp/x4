#ifndef IRIS_X4_TRAITS_VARIANT_TRAITS_HPP
#define IRIS_X4_TRAITS_VARIANT_TRAITS_HPP

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

template<class Attr, class... Ts>
struct variant_find_substitute_impl;

template<class Attr>
struct variant_find_substitute_impl<Attr>
{
    using type = Attr;
};

template<class Attr, class First, class... Rest>
struct variant_find_substitute_impl<Attr, First, Rest...>
{
    using type = std::conditional_t<
        can_hold_v<iris::unwrap_recursive_type<First>, Attr>,

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

        typename variant_find_substitute_impl<Attr, Rest...>::type
    >;
};

} // detail


template<class Variant, class T>
struct variant_find_substitute;

template<class Variant, class T>
using variant_find_substitute_t = typename variant_find_substitute<Variant, T>::type;

template<class Variant>
struct variant_find_substitute<Variant, Variant>
{
    using type = Variant;
};

template<class... Ts, class U>
    requires (!std::same_as<iris::rvariant<Ts...>, U>)
struct variant_find_substitute<iris::rvariant<Ts...>, U>
{
    using type = typename detail::variant_find_substitute_impl<U, Ts...>::type;
};


template<class Variant, class U>
struct variant_has_substitute;

template<class Variant, class U>
constexpr bool variant_has_substitute_v = variant_has_substitute<Variant, U>::value;

template<class Variant>
struct variant_has_substitute<Variant, Variant>
    : std::true_type
{};

template<class T>
struct variant_has_substitute<unused_type, T>
    : std::true_type
{};

template<class T>
struct variant_has_substitute<unused_type const, T>
    : std::true_type
{};

// Recursively find the first type from the variant that can be a substitute for `T`.
// Returns boolean value whether it was found.
template<class... Ts, class U>
    requires (!std::same_as<iris::rvariant<Ts...>, U>)
struct variant_has_substitute<iris::rvariant<Ts...>, U>
    : std::disjunction<can_hold<Ts, U>...>
{};

} // iris::x4::traits

#endif
