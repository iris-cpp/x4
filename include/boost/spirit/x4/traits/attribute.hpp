/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2020 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_X3_ATTRIBUTE_OF_JAN_7_2012_0914AM
#define BOOST_SPIRIT_X3_ATTRIBUTE_OF_JAN_7_2012_0914AM

#include <boost/spirit/x4/core/unused.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>

namespace boost::spirit::x3::traits
{
    // Retrieve the attribute type to use from the given type.
    // This is needed to extract the correct attribute type from proxy classes
    // as utilized in `FUSION_ADAPT_ADT` et al.
    template <typename Attribute, typename Enable = void>
    struct attribute_type
    {
        using type = Attribute;
    };

    template <typename Attribute>
    using attribute_type_t = typename attribute_type<Attribute>::type;


    // Pseudo attributes are placeholders for parsers that can only know
    // its actual attribute at parse time. This trait customization point
    // provides a mechanism to convert the trait to the actual trait at
    // parse time.
    template <typename Context, typename Attribute, std::forward_iterator It, std::sentinel_for<It> Se = It, typename Enable = void>
    struct pseudo_attribute
    {
        using attribute_type = Attribute;
        using type = Attribute;

        [[nodiscard]] static constexpr type&& call(It&, Se const&, Attribute&& attribute) noexcept
        {
            return static_cast<type&&>(attribute);
        }
    };

    template <typename Context, typename Attribute, std::forward_iterator It, std::sentinel_for<It> Se>
    using pseudo_attribute_t = typename pseudo_attribute<Context, Attribute, It, Se>::type;


    // Get the attribute type of the component. By default, this gets the
    // Component's attribute_type typedef or instantiates a nested attribute
    // metafunction. Components may specialize this if such an attribute_type
    // is not readily available (e.g. expensive to compute at compile time).
    template <typename Component, typename Context, typename Enable = void>
    struct attribute_of;

    template <typename Component, typename Context, typename Enable = void>
    using attribute_of_t = typename attribute_of<Component, Context>::type;

    template <typename Component, typename Context>
        requires requires {
            typename Component::attribute_type;
        }
    struct attribute_of<Component, Context>
    {
        using type = typename Component::attribute_type;
    };

    template <typename Component, typename Context>
        requires requires {
            typename Component::template attribute<Context>::type;
        }
    struct attribute_of<Component, Context>
    {
        using type = typename Component::template attribute<Context>::type;
    };

    template <typename Component, typename Context>
        requires Component::is_pass_through_unary
    struct attribute_of<Component, Context>
    {
        static_assert(requires {
            typename Component::subject_type;
        });
        using type = typename attribute_of<typename Component::subject_type, Context>::type;
    };


    // TODO: make these 'Component' depend on the concept.
    // It is currently hard to implement due to circular dependency.

    // Whether a component has an attribute. By default, this compares the
    // component attribute against unused_type. If the component provides a
    // nested constant expression has_attribute as a hint, that value is used
    // instead. Components may specialize this.
    template <typename Component, typename Context, typename Enable = void>
    struct has_attribute
    {
        static_assert(requires {
            typename attribute_of<Component, Context>::type;
        });
        static constexpr bool value = !std::is_same_v<attribute_of_t<Component, Context>, unused_type>;
    };

    template <typename Component, typename Context>
    constexpr bool has_attribute_v = has_attribute<Component, Context>::value;

    template <typename Component, typename Context>
        requires requires {
            { Component::has_attribute } -> std::same_as<bool>;
        }
    struct has_attribute<Component, Context>
        : std::bool_constant<Component::has_attribute>
    {};

    template <typename Component, typename Context>
        requires Component::is_pass_through_unary
    struct has_attribute<Component, Context>
    {
        static_assert(requires {
            typename Component::subject_type;
        });
        static constexpr bool value = has_attribute_v<typename Component::subject_type, Context>;
    };

} // boost::spirit::x3::traits

namespace boost::spirit::x3::detail
{
    template <typename... T>
    struct type_sequence
    {
        using type = type_sequence;

        static constexpr std::size_t size = sizeof...(T);

        template <typename... U>
        using append = type_sequence<T..., U...>;

        template <typename... U>
        using prepend = type_sequence<U..., T...>;

        template <typename U>
        using extend = typename U::template prepend<T...>;

        template <template <typename...> class U>
        using transfer_to = U<T...>;
    };

    template <typename Attribute>
    struct types_of_binary_init : type_sequence<Attribute>
    {};

    template <>
    struct types_of_binary_init<unused_type> : type_sequence<>
    {};

    template <>
    struct types_of_binary_init<unused_type const> : type_sequence<>
    {};

    template <template <typename, typename> class B, typename P, typename C>
    struct get_types_of_binary
        : types_of_binary_init<typename traits::attribute_of<P, C>::type>
    {};

    template <template <typename, typename> class B, typename L, typename R, typename C>
    struct get_types_of_binary<B, B<L, R>, C>
        : get_types_of_binary<B, L, C>::template extend<get_types_of_binary<B, R, C>>
    {};

    template <template <typename...> class A, typename T, int = T::size>
    struct type_sequence_to_attribute
    {
        using type = typename T::template transfer_to<A>;
    };

    template <template <typename...> class A, typename T>
    struct type_sequence_to_attribute<A, T, 1>
        : T::template transfer_to<std::type_identity>
    {};

    template <template <typename...> class A, typename T>
    struct type_sequence_to_attribute<A, T, 0>
    {
        using type = unused_type;
    };

    template <
        template <typename...> class A,
        template <typename, typename> class B,
        typename L, typename R, typename C
    >
    using attribute_of_binary = type_sequence_to_attribute<
        A,
        typename get_types_of_binary<B, B<L, R>, C>::type
    >;

} // boost::spirit::x3::detail

#endif
