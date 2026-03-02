#ifndef IRIS_X4_CORE_DETAIL_PARSE_INTO_CONTAINER_HPP
#define IRIS_X4_CORE_DETAIL_PARSE_INTO_CONTAINER_HPP

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
#include <iris/x4/traits/can_hold.hpp>

#include <iris/alloy/tuple.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct optional;

} // iris::x4

namespace iris::x4::detail {

template<class Parser, class Container>
struct parser_accepts_container {};

template<class Parser, traits::X4Container Container>
struct parser_accepts_container<Parser, Container>
    : std::disjunction<
        traits::can_hold<typename parser_traits<Parser>::attribute_type, Container>,
        std::bool_constant<parser_traits<Parser>::maybe_handles_container>
    >
{};

template<class Subject, traits::X4Container Container>
struct parser_accepts_container<optional<Subject>, Container>
    : std::true_type
{};

template<class Parser, class Container>
inline constexpr bool parser_accepts_container_v = parser_accepts_container<Parser, Container>::value;

template<class Parser>
struct parse_into_container_impl_default
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    static constexpr bool call(Parser const& parser, It& first, Se const& last, Context& ctx, Attr& attr) // TODO: add noexcept
    {
        using unwrapped_attribute_type = iris::unwrap_recursive_type<Attr>;
        auto& unwrapped_attr = iris::unwrap_recursive(attr);

        if constexpr (traits::is_container_v<unwrapped_attribute_type>) { // Attr is a container
            if constexpr (parser_accepts_container_v<Parser, unwrapped_attribute_type>) { // parser accepts the container; make parser append directly
                auto&& appender = x4::make_container_appender(unwrapped_attr);
                return parser.parse(first, last, ctx, appender);
            } else { // parser DOES NOT accept the container; parse into value type and append it
                using value_type = traits::container_value_t<unwrapped_attribute_type>;
                value_type value{}; // value-initialize
                if (!parser.parse(first, last, ctx, value)) return false;
                traits::push_back(unwrapped_attr, std::move(value));
                return true;
            }
        } else {
            if constexpr (traits::is_size_one_sequence_v<unwrapped_attribute_type>) { // attribute is single element tuple-like; unwrap and try again
                return parse_into_container_impl_default<Parser>::call(parser, first, last, ctx, alloy::get<0>(unwrapped_attr));
            } else {
                static_assert(false, "parse_into_container accepts a container, a variant of container or a single element tuple-like of container");
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
parse_into_container(
    Parser const& parser, It& first, Se const& last,
    Context const& ctx, Attr& attr
) // TODO: add noexcept
{
    if constexpr (X4UnusedAttribute<Attr> || !has_attribute_v<Parser>) { // handle unused types first
        return parser.parse(first, last, ctx, unused);
    } else {
        if constexpr (traits::is_variant_v<Attr>) {
             // e.g. `char` when the caller is `+char_`
            using attribute_type = parser_traits<Parser>::attribute_type;

            // e.g. `std::string` when the attribute_type is `char`
            using substitute_type = traits::variant_find_holdable_type_t<Attr, traits::build_container_t<attribute_type>>;

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
