/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2012 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_X3_ATTRIBUTE_TRANSFORM_JAN_8_2012_0721PM
#define BOOST_SPIRIT_X3_ATTRIBUTE_TRANSFORM_JAN_8_2012_0721PM

#include <boost/spirit/x4/traits/move_to.hpp>
#include <boost/spirit/x4/core/unused.hpp>

#include <type_traits>
#include <concepts>
#include <utility>

namespace boost::spirit::x3::traits
{
    template <typename Transformed, typename Exposed>
    struct transform_attribute
    {
        static_assert(!std::is_reference_v<Exposed>, "Exposed cannot be a reference type");
        static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");

        static_assert(std::assignable_from<Exposed&, Transformed&&>);

        using type = Transformed;

        [[nodiscard]] static constexpr Transformed pre(Exposed&)
            noexcept(std::is_nothrow_default_constructible_v<Transformed>)
        {
            return Transformed{};
        }

        template <typename TransformedT>
        static constexpr void post(Exposed& val, TransformedT&& attribute)
            noexcept(noexcept(traits::move_to(std::forward<TransformedT>(attribute), val)))
        {
            traits::move_to(std::forward<TransformedT>(attribute), val);
        }
    };

    template <typename Transformed, typename Exposed>
    concept Transformable = requires(Exposed& val) {
        typename transform_attribute<Transformed, Exposed>::type;

        { transform_attribute<Transformed, Exposed>::pre(val) }
            -> std::same_as<typename transform_attribute<Transformed, Exposed>::type>;

        { transform_attribute<Transformed, Exposed>::post(val, std::declval<Transformed>()) };
    };

    // Same attribute types; no transformation needed
    template <typename Attribute>
        requires
            (!std::is_same_v<std::remove_const_t<Attribute>, unused_type>) &&
            (!std::is_same_v<std::remove_const_t<Attribute>, unused_container_type>)
    struct transform_attribute<Attribute, Attribute>
    {
        static_assert(!std::is_reference_v<Attribute>, "Attribute cannot be a reference type");

        using type = Attribute&;
        static constexpr Attribute& pre(Attribute& val) noexcept { return val; }
        static constexpr void post(Attribute&, Attribute const&) noexcept {}
    };

    template <>
    struct transform_attribute<unused_type, unused_type>
    {
        using type = unused_type;
        static constexpr unused_type pre(unused_type) noexcept { return unused; }
        static constexpr void post(unused_type, unused_type) noexcept {}
    };

    template <>
    struct transform_attribute<unused_container_type, unused_container_type>
    {
        using type = unused_container_type;
        static constexpr unused_container_type pre(unused_container_type) noexcept { return unused_container; }
        static constexpr void post(unused_container_type, unused_container_type) noexcept {}
    };

    template <typename Transformed>
        requires (!std::is_same_v<Transformed, unused_type>)
    struct transform_attribute<Transformed, unused_type>
        : transform_attribute<unused_type, unused_type>
    {
        static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");
    };

    template <typename Transformed>
    struct transform_attribute<Transformed, unused_type const>
        : transform_attribute<unused_type, unused_type>
    {
        static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");
    };

    template <typename Transformed>
        requires (!std::is_same_v<Transformed, unused_container_type>)
    struct transform_attribute<Transformed, unused_container_type>
        : transform_attribute<unused_container_type, unused_container_type>
    {
        static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");
    };

    template <typename Transformed>
    struct transform_attribute<Transformed, unused_container_type const>
        : transform_attribute<unused_container_type, unused_container_type>
    {
        static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");
    };


} // boost::spirit::x3::traits

#endif
