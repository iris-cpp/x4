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

#include <iris/x4/traits/substitution.hpp>

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
        is_substitute_v<Attr, iris::unwrap_recursive_t<First>>,

        // TODO
        // Given some type `T`, when both `T` and `recursive_wrapper<T>` is seen
        // during attribute resolution, X4 should ideally materialize the latter
        // because:
        //   - It means that the user has supplied at least one explicit type
        //     (possibly a rule attribute type) that is `recursive_wrapper<T>`,
        //   - Constructing `T` and then moving it to `recursive_wrapper<T>`
        //     involves copying from stack to heap.
        //
        // This is to-do because the above optimization is currently not
        // implementable in a straightforward way. We need to add
        // `unwrap_recursive(attr)` to every places where any parser attempts
        // to modify the content.
        iris::unwrap_recursive_t<First>,

        typename variant_find_substitute_impl<Attr, Rest...>::type
    >;
};

} // detail


template<class Variant, X4Attribute Attr>
struct variant_find_substitute;

template<class Variant, X4Attribute Attr>
using variant_find_substitute_t = typename variant_find_substitute<Variant, Attr>::type;

template<X4Attribute Attr>
struct variant_find_substitute<Attr, Attr>
{
    using type = Attr;
};

// Recursively find the first type from the variant that can be a substitute for `Attr`.
// If none is found, returns `Attr`.
template<X4Attribute Attr, class... Ts>
    requires (!std::same_as<iris::rvariant<Ts...>, Attr>)
struct variant_find_substitute<iris::rvariant<Ts...>, Attr>
{
    using type = typename detail::variant_find_substitute_impl<Attr, Ts...>::type;
};


template<class Variant, X4Attribute Attr>
struct variant_has_substitute;

template<class Variant, X4Attribute Attr>
constexpr bool variant_has_substitute_v = variant_has_substitute<Variant, Attr>::value;

template<X4Attribute Attr>
struct variant_has_substitute<Attr, Attr>
    : std::true_type
{};

template<X4Attribute Attr>
struct variant_has_substitute<unused_type, Attr>
    : std::true_type
{};

template<X4Attribute Attr>
struct variant_has_substitute<unused_type const, Attr>
    : std::true_type
{};

// Recursively find the first type from the variant that can be a substitute for `T`.
// Returns boolean value whether it was found.
template<X4Attribute Attr, class... Ts>
    requires (!std::same_as<iris::rvariant<Ts...>, Attr>)
struct variant_has_substitute<iris::rvariant<Ts...>, Attr>
    : std::disjunction<is_substitute<Attr, Ts>...>
{};

} // iris::x4::traits

#endif
