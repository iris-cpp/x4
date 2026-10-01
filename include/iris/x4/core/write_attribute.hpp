#ifndef IRIS_ZZ_X4_CORE_WRITE_ATTRIBUTE_HPP
#define IRIS_ZZ_X4_CORE_WRITE_ATTRIBUTE_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/traits/write_rank.hpp>
#include <iris/x4/traits/container_traits.hpp>

#include <iris/rvariant/rvariant.hpp>
#include <iris/alloy/traits.hpp>
#include <iris/type_list.hpp>

#include <ranges>
#include <type_traits>
#include <utility>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4 {

namespace planner {

namespace detail {

// The plan of a node: the branch the selection in `Graph` chose, executed recursively
template<class Graph, class Node>
struct write_plan;

template<class Node>
struct node_types;

template<template<class, class> class Node, class S, class V>
struct node_types<Node<S, V>>
{
    using storage = S;
    using value = V;
};

// The argument which constructs `T` from the value, as `T t = value;` does: the value itself if of
// the type `T`, else a `T` copy-initialized from it
template<class T, class V>
[[nodiscard]] constexpr std::conditional_t<std::same_as<std::remove_cvref_t<V>, T>, V&&, T> construction_argument(V&& value)
{
    return std::forward<V>(value);
}

// Into the content of an optional if any, else a content made from the value, or by default and written into
template<class Graph, class Child, class Optional>
constexpr void engage(Optional& s, typename node_types<Child>::value&& value)
{
    using T = node_types<Child>::storage;
    using V = node_types<Child>::value;

    if (s) {
        write_plan<Graph, Child>::apply(iris::unwrap_recursive(*s), std::forward<V>(value));
        return;
    }
    if constexpr (constructible_from_value<T, V>) {
        s.emplace(detail::construction_argument<T, V>(std::forward<V>(value)));
    } else {
        s.emplace();
        write_plan<Graph, Child>::apply(iris::unwrap_recursive(*s), std::forward<V>(value));
    }
}

// Into the alternative `J` if held, else one made from the value, or by default and written into
template<class Graph, std::size_t J, class Child, class Variant>
constexpr void write_alternative(Variant& s, typename node_types<Child>::value&& value)
{
    using T = node_types<Child>::storage;
    using V = node_types<Child>::value;

    if (auto* const held = iris::get_if<J>(&s)) {
        write_plan<Graph, Child>::apply(*held, std::forward<V>(value));
        return;
    }
    if constexpr (constructible_from_value<T, V>) {
        s.template emplace<J>(detail::construction_argument<T, V>(std::forward<V>(value)));
    } else {
        write_plan<Graph, Child>::apply(s.template emplace<J>(), std::forward<V>(value));
    }
}

// A new element made from the value, or by default and written into
template<class Graph, class Child, class Container>
constexpr void push_new_element(Container& s, typename node_types<Child>::value&& value)
{
    using element_type = iris::container::element_t<Container>;
    using T = node_types<Child>::storage;
    using V = node_types<Child>::value;

    if constexpr (constructible_from_value<T, V>) {
        element_type element = std::forward<V>(value);
        iris::container::append(s, std::move(element));

    } else {
        element_type element{};
        write_plan<Graph, Child>::apply(iris::unwrap_recursive(element), std::forward<V>(value));
        iris::container::append(s, std::move(element));
    }
}

template<class Graph, class Branch, std::size_t Alternative>
struct write_step;

template<class Graph, branch_kind Kind, std::size_t Alternative, class... Edges>
struct write_step<Graph, branch<Kind, Alternative, true, Edges...>, Alternative>
{
    template<std::size_t I>
    using child = pack_indexing_t<I, Edges...>::node;

    template<class S, class V>
    static constexpr void apply(S& s, V&& v)
    {
        if constexpr (Kind == branch_kind::assign_same || Kind == branch_kind::assign) {
            s = std::forward<V>(v);

        } else if constexpr (Kind == branch_kind::transparent) {
            if (v) {
                write_plan<Graph, child<0>>::apply(s, iris::unwrap_recursive(std::forward_like<V>(*v)));
            }

        } else if constexpr (Kind == branch_kind::slots) {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                (write_plan<Graph, child<Is>>::apply(
                    iris::unwrap_recursive(alloy::get<Is>(s)),
                    iris::unwrap_recursive(std::forward_like<V>(alloy::get<Is>(v)))
                ), ...);
            }(std::index_sequence_for<Edges...>{});

        } else if constexpr (Kind == branch_kind::single_slot) {
            write_plan<Graph, child<0>>::apply(iris::unwrap_recursive(alloy::get<0>(s)), std::forward<V>(v));

        } else if constexpr (Kind == branch_kind::engage_optional) {
            if (!v) {
                s.reset();
                return;
            }
            detail::engage<Graph, child<0>>(s, iris::unwrap_recursive(std::forward_like<V>(*v)));

        } else if constexpr (Kind == branch_kind::engage) {
            detail::engage<Graph, child<0>>(s, std::forward<V>(v));

        } else if constexpr (Kind == branch_kind::whole || Kind == branch_kind::new_element) {
            detail::push_new_element<Graph, child<0>>(s, std::forward<V>(v));

        } else if constexpr (Kind == branch_kind::new_default_element) {
            iris::container::element_t<S> element{};
            write_plan<Graph, child<0>>::apply(iris::unwrap_recursive(element), std::forward<V>(v));
            iris::container::append(s, std::move(element));

        } else if constexpr (Kind == branch_kind::append_all) {
            iris::container::append_range(s, detail::appended_elements<V>(v));

        } else if constexpr (Kind == branch_kind::each) {
            for (auto&& element : detail::appended_elements<V>(v)) {
                write_plan<Graph, child<0>>::apply(s, iris::unwrap_recursive(std::forward<decltype(element)>(element)));
            }

        } else if constexpr (Kind == branch_kind::same_alternative || Kind == branch_kind::structural) {
            detail::write_alternative<Graph, Alternative, child<0>>(s, std::forward<V>(v));

        } else if constexpr (Kind == branch_kind::conversion) {
            using T = storage_t<variant_alternative_t<Alternative, S>>;

            if (auto* const held = iris::get_if<Alternative>(&s)) {
                *held = std::forward<V>(v);
                return;
            }
            if constexpr (constructible_from_value<T, V>) {
                s.template emplace<Alternative>(detail::construction_argument<T, V>(std::forward<V>(v)));
            } else {
                s.template emplace<Alternative>() = std::forward<V>(v);
            }

        } else if constexpr (Kind == branch_kind::wrapping) {
            auto* const held = iris::get_if<Alternative>(&s);
            auto& wrapper = held ? *held : s.template emplace<Alternative>();
            write_plan<Graph, child<0>>::apply(iris::unwrap_recursive(alloy::get<0>(wrapper)), std::forward<V>(v));

        } else if constexpr (Kind == branch_kind::split) {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                (void)((v.index() == Is && (write_plan<Graph, child<Is>>::apply(s, iris::unwrap_recursive(std::forward_like<V>(iris::get<Is>(v)))), true)) || ...);
            }(std::index_sequence_for<Edges...>{});

        } else if constexpr (Kind == branch_kind::parts) {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                (write_plan<Graph, child<Is>>::apply(s, iris::unwrap_recursive(std::forward_like<V>(alloy::get<Is>(v)))), ...);
            }(std::index_sequence_for<Edges...>{});

        } else {
            static_assert(Kind == branch_kind::range);
            write_plan<Graph, child<0>>::apply(s, std::forward<V>(v));
        }
    }
};

// The candidate of a group for the alternative chosen
template<class Graph, class... Branches, std::size_t Alternative>
struct write_step<Graph, branch_group<Branches...>, Alternative>
{
    template<class Branch>
    struct chosen
    {
        using type = type_list<>;
    };

    template<branch_kind Kind, class... Edges>
    struct chosen<branch<Kind, Alternative, true, Edges...>>
    {
        using type = type_list<branch<Kind, Alternative, true, Edges...>>;
    };

    template<class S, class V>
    static constexpr void apply(S& s, V&& v)
    {
        using branch_type = at_c_t<0, typename concat_type_list<typename chosen<Branches>::type...>::type>;
        write_step<Graph, branch_type, Alternative>::template apply<S, V>(s, std::forward<V>(v));
    }
};

template<class Graph, template<class, class> class Node, class S, class V>
struct write_plan<Graph, Node<S, V>>
{
    static constexpr selection chosen = Graph::template selection_of<Node<S, V>>;

    static constexpr void apply(S& s, V&& v)
    {
        using branch_type = at_c_t<chosen.position, typename branches_of<Node<S, V>>::type>;
        write_step<Graph, branch_type, chosen.alternative>::template apply<S, V>(s, std::forward<V>(v));
    }
};

struct write_attribute_fn
{
    template<class S, class V>
        requires X4UnusedAttribute<S> || X4UnusedAttribute<std::remove_reference_t<V>>
    static constexpr void operator()(S&, V&&) noexcept
    {}

    // Not `noexcept`. Deriving it from the plan is not planned; reconsider it when a concrete need arises.
    template<class S, class V>
        requires (!X4UnusedAttribute<S> && !X4UnusedAttribute<std::remove_reference_t<V>>) && is_writable_v<S&, V>
    static constexpr void operator()(S& s, V&& v)
    {
        using root = write_node<storage_t<S>, model_value_t<V>>;
        write_plan<graph_of<root>, root>::apply(iris::unwrap_recursive(s), iris::unwrap_recursive(std::forward<V>(v)));
    }
};

} // detail

// Passes `v`, the attribute of `x4::rule` or `x4::as<T>`, to the exposed attribute `s` by the ordinary
// assignment, which the caller has checked by `X4StrictlyWritable<S&, V&&>`. A container which holds
// the preceding results keeps them: the value assigned to a new container is appended to it.
template<class S, class V>
constexpr void pass_declared_attribute(S& s, V&& v)
{
    if constexpr (traits::X4Container<S>) {
        if (!std::ranges::empty(s)) {
            S assigned{};
            assigned = std::forward<V>(v);
            iris::container::append_range(s, assigned | std::views::as_rvalue);
            return;
        }
    }
    s = std::forward<V>(v);
}

// Writes `v`, a part of a sequence or the value of one parse of a repetition, into the container
// `s`: as a new element, part by part, or a range appended (P(S, y))
template<class S, class V>
constexpr void write_part(S& s, V&& v)
{
    using node = parse_part_node<storage_t<S>, model_value_t<V>>;
    static_assert(node_write_of<node>.writable, "The value is not written into the container, as a new element or otherwise");

    detail::write_plan<graph_of<node>, node>::apply(
        iris::unwrap_recursive(s),
        iris::unwrap_recursive(std::forward<V>(v))
    );
}

} // planner

// Writes `v` into the attribute `s`, as `write_rank_v<S&, V>` rates it. A container is appended
// into, and an existing content or alternative is written into rather than replaced.
inline constexpr planner::detail::write_attribute_fn write_attribute{};

} // iris::x4

#endif
