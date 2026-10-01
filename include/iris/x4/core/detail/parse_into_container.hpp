#ifndef IRIS_ZZ_X4_CORE_DETAIL_PARSE_INTO_CONTAINER_HPP
#define IRIS_ZZ_X4_CORE_DETAIL_PARSE_INTO_CONTAINER_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/attribute.hpp>
#include <iris/x4/core/parser_traits.hpp>

#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/core/traits/tuple_traits.hpp>
#include <iris/x4/core/traits/variant_traits.hpp>

#include <iris/alloy/tuple.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct optional;

} // iris::x4

namespace iris::x4::detail {

// A value is pushed into a container as an element by an implicit conversion; `unused_type` is pushed as nothing
template<class Container, class V>
concept pushable_into_container =
    std::same_as<std::remove_cvref_t<V>, unused_type> ||
    (std::convertible_to<V, std::ranges::range_value_t<Container>> && iris::container::appendable<Container, V>);

template<class Parser, traits::X4Container Container>
struct parser_accepts_container
{
    static constexpr bool value =
        parser_traits<Parser>::template accepts_container<Container> &&
        !pushable_into_container<Container, typename parser_traits<Parser>::attribute_type>;
};

template<class Parser, traits::X4Container Container>
    requires is_variant_v<typename parser_traits<Parser>::attribute_type>
struct parser_accepts_container<Parser, Container>
{
    using alternative_type = variant_find_holdable_type<
        typename parser_traits<Parser>::attribute_type,
        Container
    >::type;

    static constexpr bool value =
        parser_traits<Parser>::template accepts_container<Container> &&
        !pushable_into_container<Container, alternative_type>;
};

template<class Parser>
struct parse_into_container_impl_default
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    static constexpr bool call(Parser const& parser, It& first, Se const& last, Context& ctx, Attr& attr)
    {
        using unwrapped_attribute_type = iris::unwrap_recursive_t<Attr>;
        auto& unwrapped_attr = iris::unwrap_recursive(attr);

        if constexpr (traits::X4Container<unwrapped_attribute_type>) { // Attr is a container
            if constexpr (parser_accepts_container<Parser, unwrapped_attribute_type>::value) {
                // `Parser` accepts the exact `Container`; let parser append directly
                return parser.parse(first, last, ctx, unwrapped_attr);

            } else {
                // `Parser` DOES NOT accept the exact `Container`; parse into `value_type` and append it.
                iris::container::element_t<unwrapped_attribute_type> value{}; // value-initialize
                if (!parser.parse(first, last, ctx, value)) return false;
                iris::container::append(unwrapped_attr, std::move(value));
                return true;
            }

        } else {
            if constexpr (SingleElementTupleLike<unwrapped_attribute_type>) {
                // attribute is a single-element tuple-like; unwrap and try again
                return parse_into_container_impl_default<Parser>::call(parser, first, last, ctx, alloy::get<0>(unwrapped_attr));
            } else {
                static_assert(false, "[BUG] parse_into_container accepts a container, a variant of container or a single-element tuple-like of container");
                return false;
            }
        }
    }
};

// internal customization point
template<class Parser>
struct parse_into_container_impl
    : parse_into_container_impl_default<Parser>
{};

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
[[nodiscard]] constexpr bool
parse_into_container(Parser const& parser, It& first, Se const& last, Context const& ctx, Attr& attr)
{
    if constexpr (X4UnusedAttribute<Attr> || !has_attribute_v<Parser>) {
        return parser.parse(first, last, ctx, unused);

    } else if constexpr (is_recursive_wrapper_v<Attr>) {
        return detail::parse_into_container(parser, first, last, ctx, *attr);

    } else if constexpr (SingleElementTupleLike<Attr>) {
        // A tuple-like holding a single container; parse into that container
        return detail::parse_into_container(parser, first, last, ctx, alloy::get<0>(attr));

    } else if constexpr (is_variant_v<Attr>) {
         // e.g. `char` when the caller is `+char_`
        using attribute_type = parser_traits<Parser>::attribute_type;

        // e.g. `std::string` when the attribute_type is `char`
        using substitute_type = variant_find_holdable_type<Attr, typename traits::default_container<attribute_type>::type>::type;

        // append directly into the alternative, held or else emplaced, instead of into a temporary
        auto* const held = iris::get_if<substitute_type>(&attr);
        auto& variant_alt = held ? *held : attr.template emplace<substitute_type>();
        return parse_into_container_impl<Parser>::call(parser, first, last, ctx, variant_alt);

    } else {
        static_assert(traits::X4Container<Attr>);
        return parse_into_container_impl<Parser>::call(parser, first, last, ctx, attr);
    }
}

} // iris::x4::detail

#endif
