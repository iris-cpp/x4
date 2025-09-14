/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_X3_NONTERMINAL_DETAIL_TRANSFORM_ATTRIBUTE_HPP
#define BOOST_SPIRIT_X3_NONTERMINAL_DETAIL_TRANSFORM_ATTRIBUTE_HPP

#include <boost/spirit/home/x3/support/traits/transform_attribute.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>
#include <boost/spirit/home/x3/support/unused.hpp>

#include <type_traits>
#include <concepts>
#include <utility>

namespace boost::spirit::x3
{
    namespace detail
    {
        struct parser_id;
    } // detail

    template <typename Exposed, typename Transformed>
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

    // Same attribute types; no transformation needed
    template <typename Attribute>
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

    template <typename Exposed, typename Transformed>
        requires
            std::is_same_v<std::remove_const_t<Exposed>, unused_type>
    struct transform_attribute<Exposed, Transformed>
        : transform_attribute<unused_type, unused_type>
    {
        static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");
    };

    template <typename Exposed, typename Transformed>
        requires
            std::is_same_v<std::remove_const_t<Exposed>, unused_container_type>
    struct transform_attribute<Exposed, Transformed>
        : transform_attribute<unused_container_type, unused_container_type>
    {
        static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");
    };

    template <typename Exposed, typename Transformed>
        requires
            (!std::is_same_v<std::remove_const_t<Exposed>, unused_type>) &&
            std::is_same_v<std::remove_const_t<Transformed>, unused_type>
    struct transform_attribute<Exposed, Transformed>
        : transform_attribute<unused_type, unused_type>
    {
        static_assert(!std::is_reference_v<Exposed>, "Exposed cannot be a reference type");
    };

    template <typename Exposed, typename Transformed>
        requires
            (!std::is_same_v<std::remove_const_t<Exposed>, unused_container_type>) &&
            std::is_same_v<std::remove_const_t<Transformed>, unused_container_type>
    struct transform_attribute<Exposed, Transformed>
        : transform_attribute<unused_container_type, unused_container_type>
    {
        static_assert(!std::is_reference_v<Exposed>, "Exposed cannot be a reference type");
    };

} // boost::spirit::x3

namespace boost::spirit::x3::traits
{
    template <typename Exposed, typename Transformed>
    struct transform_attribute<Exposed, Transformed, x3::detail::parser_id>
        : x3::transform_attribute<Exposed, Transformed>
    {};
} // boost::spirit::x3::traits

#endif
