#ifndef IRIS_ZZ_X4_CORE_DETAIL_PARSE_INTO_CONTAINER_HPP
#define IRIS_ZZ_X4_CORE_DETAIL_PARSE_INTO_CONTAINER_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp>

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/container_appender.hpp>

#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/traits/tuple_traits.hpp>

#include <iris/alloy/tuple.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct optional;

} // iris::x4

namespace iris::x4::detail {

template<class Parser, traits::X4Container Container>
struct parser_accepts_container
{
    static constexpr bool value =
        parser_traits<Parser>::template handles_container<Container> &&
        !requires (Container& c, typename parser_traits<Parser>::attribute_type&& v) {
            traits::push_back(c, std::move(v));
        };
};

template<class Parser, traits::X4Container Container>
    requires traits::is_variant_v<typename parser_traits<Parser>::attribute_type>
struct parser_accepts_container<Parser, Container>
{
    using alternative_type = traits::variant_find_holdable_type<
        typename parser_traits<Parser>::attribute_type,
        Container
    >::type;

    static constexpr bool value =
        parser_traits<Parser>::template handles_container<Container> &&
        !requires (Container& c, alternative_type&& v) {
            traits::push_back(c, std::move(v));
        };
};

template<class Parser>
struct parse_into_container_impl_default
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    static constexpr bool call(Parser const& parser, It& first, Se const& last, Context& ctx, Attr& attr)
        // never noexcept (requires container insertion)
    {
        using unwrapped_attribute_type = iris::unwrap_recursive_type<Attr>;
        auto& unwrapped_attr = iris::unwrap_recursive(attr);

        if constexpr (traits::is_container_v<unwrapped_attribute_type>) { // Attr is a container
            if constexpr (parser_accepts_container<Parser, unwrapped_attribute_type>::value) {
                // `Parser` accepts the exact `Container`; let parser append directly
                auto&& appender = x4::make_container_appender(unwrapped_attr);
                return parser.parse(first, last, ctx, appender);

            } else {
                // `Parser` DOES NOT accept the exact `Container`; parse into `value_type` and append it.
                typename traits::container_value<unwrapped_attribute_type>::type value{}; // value-initialize
                if (!parser.parse(first, last, ctx, value)) return false;
                traits::push_back(unwrapped_attr, std::move(value));
                return true;
            }

        } else {
            if constexpr (traits::is_size_one_sequence_v<unwrapped_attribute_type>) {
                // attribute is single element tuple-like; unwrap and try again
                return parse_into_container_impl_default<Parser>::call(parser, first, last, ctx, alloy::get<0>(unwrapped_attr));
            } else {
                //attr = nullptr;
                static_assert(false, "[BUG] parse_into_container accepts a container, a variant of container or a single element tuple-like of container");
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

template<class Parser, class It, class Se, class Context, class Attr>
struct parse_into_container_noexcept : std::false_type {};

template<class Parser, class It, class Se, class Context, class Attr>
    requires X4UnusedAttribute<Attr> || (!has_attribute_v<Parser>)
struct parse_into_container_noexcept<Parser, It, Se, Context, Attr> : is_nothrow_parsable<Parser, It, Se, Context, unused_type> {};

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
[[nodiscard]] constexpr bool
parse_into_container(
    Parser const& parser, It& first, Se const& last,
    Context const& ctx, Attr& attr
) noexcept(parse_into_container_noexcept<Parser, It, Se, Context, Attr>::value)
{
    if constexpr (X4UnusedAttribute<Attr> || !has_attribute_v<Parser>) { // handle unused types first
        return parser.parse(first, last, ctx, unused);
    } else {
        if constexpr (traits::is_variant_v<Attr>) {
             // e.g. `char` when the caller is `+char_`
            using attribute_type = parser_traits<Parser>::attribute_type;

            // e.g. `std::string` when the attribute_type is `char`
            using substitute_type = traits::variant_find_holdable_type<Attr, typename traits::default_container<attribute_type>::type>::type;

            // instead of creating a temporary `substitute_type`, append directly into the emplaced alternative
            auto& variant_alt = attr.template emplace<substitute_type>();
            return parse_into_container_impl<Parser>::call(parser, first, last, ctx, variant_alt);
        } else {
            return parse_into_container_impl<Parser>::call(parser, first, last, ctx, attr);
        }
    }
}

} // iris::x4::detail

#endif
