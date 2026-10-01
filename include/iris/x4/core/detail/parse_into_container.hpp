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
#include <iris/x4/core/write_attribute.hpp>

#include <iris/alloy/tuple.hpp>
#include <iris/rvariant/rvariant.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct optional;

} // iris::x4

namespace iris::x4::detail {

// How a parser writes into a container, following the write of its value as a part (P(S, y)), by
// one of two paths:
//   "part": the value of the parser is written into the container as a part. It is parsed into a
//     new element directly where the element takes the value by its shape, else into a temporary
//     which `write_part` writes; a range may be appended, so not always one element is added.
//   "container": the container itself is passed to the parser, which appends into it (a range
//     appended, the parts of a sequence, the content of an optional).
//
// The names follow the two roles in `writes_into_container` (`writes_as_part`, `accepts_container`),
// but the path is chosen by the priority below, not by which of them holds:
//
//   1. A value which becomes one new element (a range included, when it becomes one element as a
//      whole) is written as a part.
//   2. Otherwise, a parser which accepts the container is passed it.
//   3. Otherwise, a value which can be written as a part is written so.
enum class container_parse { none, part, container };

template<class Parser, traits::X4Container Container>
inline constexpr container_parse container_parse_for = [] {
    using planner::branch_kind;
    using container_type = planner::storage_t<Container>;
    using value_type = planner::model_value_t<typename parser_traits<Parser>::attribute_type>;

    constexpr planner::node_write part = planner::node_write_of<planner::parse_part_node<container_type, value_type>>;

    if constexpr (part.writable && part.kind == branch_kind::new_default_element) {
        return container_parse::part;

    } else if constexpr (
        part.writable && part.kind == branch_kind::range &&
        planner::node_write_of<planner::write_node<container_type, value_type>>.kind == branch_kind::whole &&
        std::is_default_constructible_v<iris::container::element_t<container_type>>
    ) {
        return container_parse::part;

    } else if constexpr (parser_traits<Parser>::template accepts_container<Container>) {
        return container_parse::container;

    } else if constexpr (part.writable) {
        return container_parse::part;

    } else {
        return container_parse::none;
    }
}();

// The part of `attr` which a parser yielding `ParserAttr` is given, following the selections of
// `write_rank`: an existing part is referred to as it is (never cleared), a missing one (a
// disengaged optional, another alternative of a variant) is constructed by default. A list-like
// parser appends into the container referred to this way (`list_like_parser::chunk_buffer`).
template<class ParserAttr>
struct ref_or_init_attribute_fn
{
    template<class Attr>
    [[nodiscard]] static constexpr auto& operator()(Attr& attr)
    {
        using planner::branch_kind;

        using S = planner::storage_t<Attr>;
        S& s = iris::unwrap_recursive(attr);

        constexpr planner::node_write write = planner::node_write_of<
            planner::write_node<S, planner::model_value_t<ParserAttr>>
        >;

        if constexpr (!write.writable) {
            return s;

        } else if constexpr (write.alternative != planner::no_index) {
            if constexpr (std::is_default_constructible_v<variant_alternative_t<write.alternative, S>>) {
                if (auto* const held = iris::get_if<write.alternative>(&s)) {
                    return ref_or_init_attribute_fn{}(*held);
                }
                return ref_or_init_attribute_fn{}(s.template emplace<write.alternative>());

            } else {
                return s;
            }

        } else if constexpr (write.kind == branch_kind::engage) {
            if constexpr (std::is_default_constructible_v<typename S::value_type>) {
                if (!s) {
                    s.emplace();
                }
                return ref_or_init_attribute_fn{}(*s);

            } else {
                return s;
            }

        } else if constexpr (write.kind == branch_kind::single_slot) {
            return ref_or_init_attribute_fn{}(alloy::get<0>(s));

        } else {
            return s;
        }
    }
};

template<class ParserAttr>
inline constexpr ref_or_init_attribute_fn<ParserAttr> ref_or_init_attribute_for{};

template<class S, class V>
concept parses_into_new_element =
    (
        planner::node_write_of<planner::parse_part_node<S, V>>.kind == planner::branch_kind::new_default_element ||
        (
            planner::node_write_of<planner::parse_part_node<S, V>>.kind == planner::branch_kind::range &&
            planner::node_write_of<planner::write_node<S, V>>.kind == planner::branch_kind::whole
        )
    ) &&
    std::is_default_constructible_v<iris::container::element_t<S>> &&
    planner::graph_of<
        planner::write_node<planner::storage_t<iris::container::element_t<S>>, V>
    >::solution.result.rank >= write_rank::structural;

template<class Parser>
struct parse_into_container_impl_default
{
    // parse the value and write it into `container` as a part: into a new element directly
    // where the element takes the value by its shape, else through a temporary
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, traits::X4Container ContainerAttr>
    static constexpr bool parse_part(Parser const& parser, It& first, Se const& last, Context& ctx, ContainerAttr& container)
    {
        using attribute_type = parser_traits<Parser>::attribute_type;
        using container_type = planner::storage_t<ContainerAttr>;
        using element_type = iris::container::element_t<container_type>;

        if constexpr (parses_into_new_element<container_type, planner::model_value_t<attribute_type>>) {
            element_type value{};
            if (!parser.parse(first, last, ctx, detail::ref_or_init_attribute_for<attribute_type>(value))) return false;
            iris::container::append(container, std::move(value));

        } else {
            attribute_type attr{};
            if (!parser.parse(first, last, ctx, attr)) return false;
            planner::write_part(container, std::move(attr));
        }
        return true;
    }

    // pass `container` itself to the parser, which appends into it
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, traits::X4Container Container>
    static constexpr bool parse_container(Parser const& parser, It& first, Se const& last, Context& ctx, Container& container)
    {
        return parser.parse(first, last, ctx, container);
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    static constexpr bool call(Parser const& parser, It& first, Se const& last, Context& ctx, Attr& attr)
    {
        using unwrapped_attribute_type = iris::unwrap_recursive_t<Attr>;
        auto& unwrapped_attr = iris::unwrap_recursive(attr);

        if constexpr (traits::X4Container<unwrapped_attribute_type>) { // Attr is a container
            constexpr container_parse write = container_parse_for<Parser, unwrapped_attribute_type>;
            static_assert(
                write != container_parse::none,
                "The parser neither accepts this container nor yields an element of it"
            );
            if constexpr (write == container_parse::container) {
                return parse_into_container_impl_default::parse_container(parser, first, last, ctx, unwrapped_attr);
            } else {
                return parse_into_container_impl_default::parse_part(parser, first, last, ctx, unwrapped_attr);
            }

        } else {
            if constexpr (SingleElementTupleLike<unwrapped_attribute_type>) {
                // Unwrap and try again
                return parse_into_container_impl_default::call(parser, first, last, ctx, alloy::get<0>(unwrapped_attr));

            } else {
                static_assert(false, "[BUG] parse_into_container accepts a container, a variant of container or a single-element tuple-like of container");
                return false;
            }
        }
    }
};

// Internal customization point
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
        using container_type = traits::default_container<attribute_type>::type;
        static_assert(
            variant_has_alternative_for_v<Attr, container_type>,
            "The variant has no alternative which can hold the container of the parser's attribute"
        );
        // append directly into the alternative, held or else emplaced, instead of into a temporary
        constexpr std::size_t index = variant_alternative_for_v<Attr, container_type>;
        auto* const held = iris::get_if<index>(&attr);
        auto& variant_alt = held ? *held : attr.template emplace<index>();
        return parse_into_container_impl<Parser>::call(parser, first, last, ctx, variant_alt);

    } else {
        static_assert(traits::X4Container<Attr>);
        return parse_into_container_impl<Parser>::call(parser, first, last, ctx, attr);
    }
}

} // iris::x4::detail

#endif
