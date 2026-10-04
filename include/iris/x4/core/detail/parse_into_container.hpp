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
#include <iris/x4/core/detail/action_slot.hpp>

#include <iris/alloy/tuple.hpp>
#include <iris/rvariant/rvariant.hpp>
#include <iris/type_list.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct optional;

} // iris::x4

namespace iris::x4::detail {

enum class container_parse_strategy : unsigned char
{
    // The value of the parser cannot be written into the container in any way.
    none,

    // The container itself is passed to the parser, which appends into it.
    container_itself,

    // The value of the parser is written into the container as a "part", i.e.,
    // a new element, the parts of a sequence, or an appended range.
    as_part,

    // The parser may succeed without writing its value. It is passed a slot, and the value
    // written (if any) is written into the container as a part.
    as_part_if_written,
};

template<class Container, class Candidates>
inline constexpr bool candidates_write_as_part = false;

template<class Container, class... Cs>
inline constexpr bool candidates_write_as_part<Container, type_list<Cs...>> =
    (planner::node_write_strategy_of<planner::sequence_part_node<Container, planner::model_value_t<Cs>>>.is_writable && ...);

template<class Parser, traits::X4Container Container>
inline constexpr container_parse_strategy container_parse_strategy_for = [] {
    using planner::branch_kind;
    using container_type = planner::storage_t<Container>;
    using value_type = planner::model_value_t<typename parser_traits<Parser>::attribute_type>;

    constexpr planner::node_write_strategy strategy = planner::node_write_strategy_of<planner::sequence_part_node<container_type, value_type>>;

    if constexpr (may_leave_attribute_unwritten_v<Parser>) {
        if constexpr (candidates_write_as_part<container_type, attribute_candidates_t<Parser>>) {
            return container_parse_strategy::as_part_if_written;

        } else if constexpr (parser_traits<Parser>::template accepts_container<Container>) {
            return container_parse_strategy::container_itself;

        } else {
            return container_parse_strategy::none;
        }

    } else if constexpr (strategy.is_writable && strategy.kind == branch_kind::new_element) {
        return container_parse_strategy::as_part;

    } else if constexpr (
        strategy.is_writable && strategy.kind == branch_kind::range &&
        planner::node_write_strategy_of<planner::write_node<container_type, value_type>>.kind == branch_kind::whole
    ) {
        return container_parse_strategy::as_part;

    } else if constexpr (parser_traits<Parser>::template accepts_container<Container>) {
        return container_parse_strategy::container_itself;

    } else if constexpr (strategy.is_writable) {
        return container_parse_strategy::as_part;

    } else {
        return container_parse_strategy::none;
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

        constexpr planner::node_write_strategy strategy = planner::node_write_strategy_of<
            planner::write_node<S, planner::model_value_t<ParserAttr>>
        >;

        if constexpr (!strategy.is_writable) {
            return s;

        } else if constexpr (strategy.alternative_index != planner::no_index) {
            if constexpr (std::is_default_constructible_v<variant_alternative_t<strategy.alternative_index, S>>) {
                if (auto* const existing_alt = iris::get_if<strategy.alternative_index>(&s)) {
                    return ref_or_init_attribute_fn{}(*existing_alt);
                }
                return ref_or_init_attribute_fn{}(s.template emplace<strategy.alternative_index>());

            } else {
                return s;
            }

        } else if constexpr (strategy.kind == branch_kind::engage) {
            if constexpr (std::is_default_constructible_v<typename S::value_type>) {
                if (!s) {
                    s.emplace();
                }
                return ref_or_init_attribute_fn{}(*s);

            } else {
                return s;
            }

        } else if constexpr (strategy.kind == branch_kind::single_slot) {
            return ref_or_init_attribute_fn{}(alloy::get<0>(s));

        } else {
            return s;
        }
    }
};

template<class ParserAttr>
inline constexpr ref_or_init_attribute_fn<ParserAttr> ref_or_init_attribute_for{};

// A new element is parsed into in place only where its write converts nothing. A plain object created on
// the way then has the type of the value, so the parser writes into it as it would into a temporary of
// that type, from which the object is otherwise moved.
template<class S, class V>
concept parses_into_new_element =
    (
        planner::node_write_strategy_of<planner::sequence_part_node<S, V>>.kind == planner::branch_kind::new_element ||
        (
            planner::node_write_strategy_of<planner::sequence_part_node<S, V>>.kind == planner::branch_kind::range &&
            planner::node_write_strategy_of<planner::write_node<S, V>>.kind == planner::branch_kind::whole
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

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, traits::X4Container ContainerAttr>
    static constexpr bool parse_written_part(Parser const& parser, It& first, Se const& last, Context& ctx, ContainerAttr& container)
    {
        detail::action_slot<typename parser_traits<Parser>::attribute_type> slot;
        if (!parser.parse(first, last, ctx, slot)) return false;

        if (slot.is_generated()) {
            detail::write_slot_value<attribute_candidates_t<Parser>::size >= 2>(slot, [&container]<class V>(V&& value) {
                planner::write_part(container, std::forward<V>(value));
            });
        }
        return true;
    }
};

// Internal customization point. A specialization that has `call` replaces how `parse_into_container`
// parses the parser into a container; otherwise `parse_into_container` uses the strategy of the parser.
template<class Parser>
struct parse_into_container_impl {};

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
        constexpr std::size_t alt_index = variant_alternative_for_v<Attr, container_type>;
        auto* const existing_alt = iris::get_if<alt_index>(&attr);
        auto& variant_alt = existing_alt ? *existing_alt : attr.template emplace<alt_index>();
        return detail::parse_into_container(parser, first, last, ctx, variant_alt);

    } else if constexpr (requires { parse_into_container_impl<Parser>::call(parser, first, last, ctx, attr); }) {
        static_assert(traits::X4Container<Attr>);
        return parse_into_container_impl<Parser>::call(parser, first, last, ctx, attr);

    } else {
        // Choose the strategy here rather than in a function of its own, to keep the call stack short
        static_assert(traits::X4Container<Attr>);
        constexpr container_parse_strategy strategy = container_parse_strategy_for<Parser, Attr>;
        static_assert(
            strategy != container_parse_strategy::none,
            "The value of this parser cannot be added to the container, as a new element, part by part, or as a range. "
            "A new element of a plain type must be constructible from the value. Note: a default constructor and an assignment "
            "are not enough."
        );
        if constexpr (strategy == container_parse_strategy::as_part_if_written) {
            return parse_into_container_impl_default<Parser>::parse_written_part(parser, first, last, ctx, attr);

        } else if constexpr (strategy == container_parse_strategy::container_itself) {
            return parser.parse(first, last, ctx, attr); // the parser appends into the container itself

        } else {
            return parse_into_container_impl_default<Parser>::parse_part(parser, first, last, ctx, attr);
        }
    }
}

} // iris::x4::detail

#endif
