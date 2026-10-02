#ifndef IRIS_ZZ_X4_TRAITS_CONTAINER_TRAITS_HPP
#define IRIS_ZZ_X4_TRAITS_CONTAINER_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/unused.hpp>

#include <iris/alloy/tuple.hpp>

#include <iris/container_traits.hpp>

#include <concepts>
#include <string>
#include <type_traits>
#include <vector>

namespace iris::x4::traits {

template<class T>
concept X4Container = requires {
    requires std::default_initializable<std::remove_cvref_t<T>>;
    requires iris::container::growable_array<std::remove_cvref_t<T>>;
};

// The attribute category of `unused_container_type` is `container_tag`, but it is not `X4Container`

// -------------------------------------------------

// Customization point
template<class T>
struct default_container
{
    using type = std::vector<T>;
};

template<class T>
struct default_container<alloy::tuple<T>> : default_container<T> {};

template<>
struct default_container<unused_type>
{
    using type = unused_container_type;
};

template<>
struct default_container<unused_container_type>
{
    using type = unused_container_type;
};

template<>
struct default_container<char>
{
    using type = std::basic_string<char>;
};

template<>
struct default_container<wchar_t>
{
    using type = std::basic_string<wchar_t>;
};

template<>
struct default_container<char8_t>
{
    using type = std::basic_string<char8_t>;
};

template<>
struct default_container<char16_t>
{
    using type = std::basic_string<char16_t>;
};

template<>
struct default_container<char32_t>
{
    using type = std::basic_string<char32_t>;
};

} // iris::x4::traits

#endif
