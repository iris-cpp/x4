/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#if !defined(BOOST_SPIRIT_X3_ATTRIBUTE_CATEGORY_JAN_4_2012_1150AM)
#define BOOST_SPIRIT_X3_ATTRIBUTE_CATEGORY_JAN_4_2012_1150AM

#include <boost/spirit/home/x3/support/traits/is_variant.hpp>
#include <boost/spirit/home/x3/support/traits/is_range.hpp>
#include <boost/spirit/home/x3/support/traits/container_traits.hpp>
#include <boost/spirit/home/x3/support/traits/optional_traits.hpp>

#include <boost/fusion/include/is_sequence.hpp>
#include <boost/fusion/support/category_of.hpp>

#include <type_traits>

namespace boost::spirit::x3
{
    struct unused_type;
    struct unused_container_type;

} // boost::spirit::x3

namespace boost::spirit::x3::traits
{
    struct unused_attribute {};
    struct plain_attribute {};
    struct container_attribute {};
    struct tuple_attribute {};
    struct associative_attribute {};
    struct variant_attribute {};
    struct optional_attribute {};
    struct range_attribute {};

    template <typename T, typename Enable = void>
    struct attribute_category
    {
        using type = plain_attribute;
    };

    template <typename T>
    struct attribute_category<T const> : attribute_category<T> {};

    template <typename T>
    struct attribute_category<T&> : attribute_category<T> {};

    template <typename T>
    struct attribute_category<T const&> : attribute_category<T> {};

    template <typename T>
    struct attribute_category<T&&> : attribute_category<T> {};

    template <typename T>
    struct attribute_category<T const&&> : attribute_category<T> {};

    template <typename T>
    using attribute_category_t = typename attribute_category<T>::type;

    template <>
    struct attribute_category<unused_type>
    {
        using type = unused_attribute;
    };

    template <>
    struct attribute_category<unused_container_type>
    {
        using type = container_attribute;

        // The attribute category type for `unused_container_type` is
        // `container_attribute`, but it does not satisfy `is_container`.
    };

    template <typename T, typename AttributeCategoryTag>
    concept CategorizedAttr = std::is_same_v<typename attribute_category<std::remove_cvref_t<T>>::type, AttributeCategoryTag>;

    template <typename T>
    concept NonUnusedAttr = !CategorizedAttr<T, unused_attribute>;

    template <typename T>
        requires
            fusion::traits::is_sequence<std::remove_cvref_t<T>>::value &&
            fusion::traits::is_associative<std::remove_cvref_t<T>>::value
    struct attribute_category<T>
    {
        using type = associative_attribute;
    };

    template <typename T>
        requires
            fusion::traits::is_sequence<std::remove_cvref_t<T>>::value &&
            (!fusion::traits::is_associative<std::remove_cvref_t<T>>::value)
    struct attribute_category<T>
    {
        using type = tuple_attribute;
    };

    template <typename T>
        requires is_variant_v<std::remove_cvref_t<T>>
    struct attribute_category<T>
    {
        using type = variant_attribute;
    };

    template <typename T>
        requires is_optional_v<std::remove_cvref_t<T>>
    struct attribute_category<T>
    {
        using type = optional_attribute;
    };

    template <typename T>
        requires is_range_v<std::remove_cvref_t<T>>
    struct attribute_category<T>
    {
        using type = range_attribute;
    };

    template <typename T>
        requires
            (!traits::is_range_v<std::remove_cvref_t<T>>) &&
            traits::is_container_v<std::remove_cvref_t<T>> &&
            (!fusion::traits::is_sequence<std::remove_cvref_t<T>>::value)
    struct attribute_category<T>
    {
        using type = container_attribute;
    };

} // boost::spirit::x3::traits

#endif
