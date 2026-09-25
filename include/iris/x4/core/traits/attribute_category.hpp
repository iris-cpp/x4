#ifndef IRIS_ZZ_X4_CORE_TRAITS_ATTRIBUTE_CATEGORY_HPP
#define IRIS_ZZ_X4_CORE_TRAITS_ATTRIBUTE_CATEGORY_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/attribute.hpp>
#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/core/traits/variant_class.hpp>
#include <iris/x4/core/traits/optional_traits.hpp>

#include <iris/alloy/traits.hpp>

#include <concepts>
#include <type_traits>

namespace iris::x4 {

struct unused_type;
struct unused_container_type;

struct unused_tag {};
struct plain_tag {};
struct container_tag {};
struct tuple_tag {};
struct variant_tag {};
struct optional_tag {};

namespace detail {

template<class T>
struct attribute_category
{
    static_assert(X4NonUnusedAttribute<T>);
    using type = plain_tag;
};

template<class T>
struct attribute_category<T const> : attribute_category<T> {};

template<class T>
struct attribute_category<T&> : attribute_category<T> {};

template<class T>
struct attribute_category<T const&> : attribute_category<T> {};

template<class T>
struct attribute_category<T&&> : attribute_category<T> {};

template<class T>
struct attribute_category<T const&&> : attribute_category<T> {};

template<>
struct attribute_category<unused_type>
{
    using type = unused_tag;
};

template<>
struct attribute_category<unused_container_type>
{
    using type = container_tag;

    // The attribute category type for `unused_container_type` is
    // `container_attribute`, but it does not satisfy `is_container`.
};

template<class T>
    requires alloy::is_tuple_like_v<T>
struct attribute_category<T>
{
    using type = tuple_tag;
};

template<class T>
    requires is_variant_v<std::remove_cvref_t<T>>
struct attribute_category<T>
{
    using type = variant_tag;
};

template<class T>
    requires is_optional_v<std::remove_cvref_t<T>>
struct attribute_category<T>
{
    using type = optional_tag;
};

template<class T>
    requires traits::is_container_v<std::remove_cvref_t<T>>
struct attribute_category<T>
{
    using type = container_tag;
};

} // detail

template<class T>
using attribute_category_t = detail::attribute_category<T>::type;

template<class T, typename AttrCategoryTag>
concept CategorizedAttr =
    X4Attribute<std::remove_reference_t<T>> &&
    // Don't use `std::same_as` here, it bloats the compilation error.
    std::same_as<attribute_category_t<T>, AttrCategoryTag>;

template<class T>
concept NonUnusedCategorizedAttr =
    X4Attribute<std::remove_reference_t<T>> &&
    !std::same_as<attribute_category_t<T>, unused_tag>;

} // iris::x4

#endif
