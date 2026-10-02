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

#include <concepts>
#include <optional>
#include <ranges>
#include <type_traits>
#include <utility>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4 {

namespace planner {

namespace detail {

// The plan of a node: the branch the selection in `Graph` chose, executed recursively
template<class Graph, class NodeT>
struct write_plan;

// Constructs a new object for the value by invoking `construct(fill, args...)`, which constructs
// the object from `args` and then calls `fill` with it.
//
// - A plain type, or one of the type of the value, is constructed from the value.
// - A variant is constructed holding the alternative its write selects, which is
//   made by the same rule, so that no alternative is constructed by default and
//   then written into.
// - Any other type is constructed by default and written into by its shape (by `fill`).
template<class Graph, class NodeT, class Construct>
constexpr void construct_node(Construct const& construct, typename NodeT::value_type&& value)
{
    using T = NodeT::storage_type;
    using V = NodeT::value_type;
    constexpr branch_selection selection = Graph::template selection_of<NodeT>;
    constexpr auto fill_nothing = [](auto&) noexcept {};

    if constexpr (constructible_from_value<T, V>) {
        construct(fill_nothing, std::forward<V>(value));

    } else if constexpr (
        std::same_as<attribute_category_t<T>, variant_tag> &&
        (selection.kind == branch_kind::same_alternative || selection.kind == branch_kind::structural || selection.kind == branch_kind::conversion)
    ) {
        constexpr std::size_t J = selection.alternative_index;

        if constexpr (selection.kind == branch_kind::conversion) {
            if constexpr (is_wrapped_alternative_v<J, T>) {
                construct(fill_nothing, std::in_place_index<J>, std::in_place, std::forward<V>(value));
            } else {
                construct(fill_nothing, std::in_place_index<J>, std::forward<V>(value));
            }

        } else {
            detail::construct_node<Graph, write_node<variant_alternative_t<J, T>, V>>(
                [&construct]<class Fill, class... Args>(Fill const& fill, Args&&... args) {
                    auto const fill_alternative = [&fill](auto& variant) { fill(iris::unsafe_get<J>(variant)); };
                    if constexpr (is_wrapped_alternative_v<J, T>) {
                        construct(fill_alternative, std::in_place_index<J>, std::in_place, std::forward<Args>(args)...);
                    } else {
                        construct(fill_alternative, std::in_place_index<J>, std::forward<Args>(args)...);
                    }
                },
                std::forward<V>(value)
            );
        }

    } else if constexpr (std::same_as<attribute_category_t<T>, variant_tag> && selection.kind == branch_kind::split) {
        [&construct, &value]<std::size_t... Is>(std::index_sequence<Is...>) {
            (void)(
                (
                    value.index() == Is &&
                    (
                        detail::construct_node<Graph, write_node<T, part_value_t<V, decltype(iris::get<Is>(std::declval<V&>()))>>>(
                            construct,
                            std::forward_like<V>(iris::get<Is>(value))
                        ),
                        true
                    )
                ) || ...
            );
        }(std::make_index_sequence<variant_size_v<std::remove_cvref_t<V>>>{});

    } else {
        construct([&value](auto& object) { write_plan<Graph, NodeT>::apply(object, std::forward<V>(value)); });
    }
}

// Engage into the content of an optional if any, else into a new content that `construct_node` makes
template<class Graph, class NodeT, class Optional>
constexpr void engage(Optional& s, typename NodeT::value_type&& value)
{
    using V = NodeT::value_type;

    if (s) {
        write_plan<Graph, NodeT>::apply(iris::unwrap_recursive(*s), std::forward<V>(value));
        return;
    }
    detail::construct_node<Graph, NodeT>(
        [&s]<class Fill, class... Args>(Fill const& fill, Args&&... args) {
            if constexpr (is_recursive_wrapper_v<typename Optional::value_type>) {
                fill(iris::unwrap_recursive(s.emplace(std::in_place, std::forward<Args>(args)...)));
            } else {
                fill(iris::unwrap_recursive(s.emplace(std::forward<Args>(args)...)));
            }
        },
        std::forward<V>(value)
    );
}

// Write into the alternative `J` if held, else into a new one that `construct_node` makes
template<class Graph, std::size_t J, class NodeT, class Variant>
constexpr void write_alternative(Variant& s, typename NodeT::value_type&& value)
{
    using V = NodeT::value_type;

    if (auto* const existing_alt = iris::get_if<J>(&s)) {
        write_plan<Graph, NodeT>::apply(*existing_alt, std::forward<V>(value));
        return;
    }
    detail::construct_node<Graph, NodeT>(
        [&s]<class Fill, class... Args>(Fill const& fill, Args&&... args) {
            if constexpr (is_wrapped_alternative_v<J, Variant>) {
                fill(s.template emplace<J>(std::in_place, std::forward<Args>(args)...));
            } else {
                fill(s.template emplace<J>(std::forward<Args>(args)...));
            }
        },
        std::forward<V>(value)
    );
}

// Appends an element constructed from `args` into the container, and calls `fill` with it.
//
// - If the element can be constructed directly in the container from `args`, and written in
//   place after it is appended (e.g. `std::vector`), it is constructed so.
//   - In this case, the value written must not refer into the container; otherwise the
//     behavior is undefined.
//
// - Otherwise, the element is made outside the container first and then appended; e.g. an
//   element of `std::set`, which is const, or of a container which appends a complete element
//   only (`push_back(T)`).
template<class Container, class Fill, class... Args>
constexpr void append_element(Container& s, Fill const& fill, Args&&... args)
{
    using element_type = iris::container::element_t<Container>;

    if constexpr (
        std::same_as<std::ranges::range_reference_t<Container>, element_type&> &&
        std::invocable<decltype(iris::container::append_return) const&, Container&, Args...>
    ) {
        fill(iris::unwrap_recursive(iris::container::append_return(s, std::forward<Args>(args)...)));

    } else {
        std::optional<element_type> element;
        fill(iris::unwrap_recursive(element.emplace(std::forward<Args>(args)...)));
        iris::container::append(s, std::move(*element));
    }
}

// Appends an element made by `construct_node` into the container (see `append_element`)
template<class Graph, class NodeT, class Container>
constexpr void push_new_element(Container& s, typename NodeT::value_type&& value)
{
    using T = NodeT::storage_type;
    using V = NodeT::value_type;

    if constexpr (constructible_from_value<T, V>) {
        iris::container::append(s, std::forward<V>(value));

    } else {
        detail::construct_node<Graph, NodeT>(
            [&s]<class Fill, class... Args>(Fill const& fill, Args&&... args) {
                if constexpr (is_recursive_wrapper_v<iris::container::element_t<Container>>) {
                    detail::append_element(s, fill, std::in_place, std::forward<Args>(args)...);
                } else {
                    detail::append_element(s, fill, std::forward<Args>(args)...);
                }
            },
            std::forward<V>(value)
        );
    }
}

template<class Graph, class Branch, std::size_t AlternativeI>
struct write_step;

template<class Graph, branch_kind Kind, std::size_t AlternativeI, class... Edges>
struct write_step<Graph, branch<Kind, AlternativeI, true, Edges...>, AlternativeI>
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

        } else if constexpr (Kind == branch_kind::append_all) {
            iris::container::append_range(s, detail::appended_elements<V>(v));

        } else if constexpr (Kind == branch_kind::each) {
            for (auto&& element : detail::appended_elements<V>(v)) {
                write_plan<Graph, child<0>>::apply(s, iris::unwrap_recursive(std::forward<decltype(element)>(element)));
            }

        } else if constexpr (Kind == branch_kind::same_alternative || Kind == branch_kind::structural) {
            detail::write_alternative<Graph, AlternativeI, child<0>>(s, std::forward<V>(v));

        } else if constexpr (Kind == branch_kind::conversion) {
            if (auto* const existing_alt = iris::get_if<AlternativeI>(&s)) {
                *existing_alt = std::forward<V>(v);
                return;
            }
            s.template emplace<AlternativeI>(std::forward<V>(v));

        } else if constexpr (Kind == branch_kind::wrapping) {
            auto* const existing_alt = iris::get_if<AlternativeI>(&s);
            auto& wrapper = existing_alt ? *existing_alt : s.template emplace<AlternativeI>();
            write_plan<Graph, child<0>>::apply(iris::unwrap_recursive(alloy::get<0>(wrapper)), std::forward<V>(v));

        } else if constexpr (Kind == branch_kind::split) {
            [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                (void)((v.index() == Is && (write_plan<Graph, child<Is>>::apply(s, std::forward_like<V>(iris::get<Is>(v))), true)) || ...);
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

// The candidate of a group for the alternative selected
template<class Graph, class... Branches, std::size_t AlternativeI>
struct write_step<Graph, branch_group<Branches...>, AlternativeI>
{
    template<class Branch>
    struct chosen
    {
        using type = type_list<>;
    };

    template<branch_kind Kind, class... Edges>
    struct chosen<branch<Kind, AlternativeI, true, Edges...>>
    {
        using type = type_list<branch<Kind, AlternativeI, true, Edges...>>;
    };

    template<class S, class V>
    static constexpr void apply(S& s, V&& v)
    {
        using branch_type = at_c_t<0, typename concat_type_list<typename chosen<Branches>::type...>::type>;
        write_step<Graph, branch_type, AlternativeI>::template apply<S, V>(s, std::forward<V>(v));
    }
};

template<class Graph, template<class, class> class NodeTT, class S, class V>
struct write_plan<Graph, NodeTT<S, V>>
{
    static constexpr branch_selection selection = Graph::template selection_of<NodeTT<S, V>>;

    static constexpr void apply(S& s, V&& v)
    {
        using branch_type = at_c_t<selection.branch_index, typename branches_of<NodeTT<S, V>>::type>;
        write_step<Graph, branch_type, selection.alternative_index>::template apply<S, V>(s, std::forward<V>(v));
    }
};

struct write_attribute_fn
{
    template<class S, class V>
        requires X4UnusedAttribute<S> || X4UnusedAttribute<std::remove_reference_t<V>>
    static constexpr void operator()(S&, V&&) noexcept
    {}

    // Not `noexcept`. Deriving it from the plan is not planned; reconsider it when a concrete need arises.
    // `is_writable_v` is spelled out, as ReSharper judges a constraint of that variable alone as false.
    template<class S, class V>
        requires (!X4UnusedAttribute<S> && !X4UnusedAttribute<std::remove_reference_t<V>>) && (write_rank_v<S&, V> != write_rank::none)
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
    using node = sequence_part_node<storage_t<S>, model_value_t<V>>;
    static_assert(node_write_strategy_of<node>.is_writable, "The value is not written into the container, as a new element or otherwise");

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
