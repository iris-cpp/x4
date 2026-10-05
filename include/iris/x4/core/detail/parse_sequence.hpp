#ifndef IRIS_ZZ_X4_CORE_DETAIL_PARSE_SEQUENCE_HPP
#define IRIS_ZZ_X4_CORE_DETAIL_PARSE_SEQUENCE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp>

#include <iris/x4/core/traits/tuple_traits.hpp>
#include <iris/x4/core/traits/write_rank.hpp>

#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/nary_parser.hpp>
#include <iris/x4/core/detail/parse_into_container.hpp>

#include <iris/alloy/tuple.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4 {

template<class... Ps>
struct sequence;

} // iris::x4

namespace iris::x4::detail {

template<class... Ps>
struct sequence_layout
{
    static constexpr std::size_t parser_count = sizeof...(Ps);

    static constexpr std::size_t elem_sequence_sizes[parser_count]{parser_traits<Ps>::sequence_size...};
    static constexpr std::size_t total_sequence_size = (std::size_t{0} + ... + parser_traits<Ps>::sequence_size);

    [[nodiscard]] static constexpr std::size_t elem_offset(std::size_t const index) noexcept
    {
        std::size_t offset = 0;
        for (std::size_t i = 0; i < index; ++i) {
            offset += elem_sequence_sizes[i];
        }
        return offset;
    }

    static constexpr std::size_t attributed_count = (std::size_t{0} + ... + std::size_t{has_attribute_v<Ps>});

    static constexpr std::size_t single_attributed_index = [] {
        bool const is_attributed[parser_count]{has_attribute_v<Ps>...};
        for (std::size_t i = 0; i < parser_count; ++i) {
            if (is_attributed[i]) return i;
        }
        return parser_count;
    }();
};

template<class... Ps>
    requires (sequence_layout<Ps...>::attributed_count == 1)
struct may_leave_attribute_unwritten<sequence<Ps...>>
    : may_leave_attribute_unwritten<nary::parser_t<sequence_layout<Ps...>::single_attributed_index, Ps...>>
{};

template<class... Ps>
    requires
        (sequence_layout<Ps...>::attributed_count == 1) &&
        std::same_as<
            typename get_attribute_type<sequence<Ps...>>::type,
            typename get_attribute_type<nary::parser_t<sequence_layout<Ps...>::single_attributed_index, Ps...>>::type
        >
struct attribute_candidates<sequence<Ps...>>
    : attribute_candidates<nary::parser_t<sequence_layout<Ps...>::single_attributed_index, Ps...>>
{};

template<class P>
struct sequence_passes_view : std::false_type {};

template<class... Ps>
struct sequence_passes_view<sequence<Ps...>> : std::true_type {};

template<class P>
    requires requires { typename P::proxy_backend_type; }
struct sequence_passes_view<P> : sequence_passes_view<typename P::proxy_backend_type> {};


// Selects the attribute for the `I`-th element of the sequence. This returns before
// the element is parsed, so it does not stay in the call stack.
template<std::size_t I, class... Ps, class Attr>
[[nodiscard]] constexpr decltype(auto) sequence_attribute_for(sequence<Ps...> const&, Attr& attr) noexcept
{
    using layout = sequence_layout<Ps...>;
    using parser_type = nary::parser_t<I, Ps...>;
    constexpr std::size_t sequence_size = layout::elem_sequence_sizes[I];
    constexpr std::size_t offset = layout::elem_offset(I);

    if constexpr (X4UnusedAttribute<Attr>) {
        return (unused);

    } else if constexpr (layout::attributed_count == 1) {
        if constexpr (I != layout::single_attributed_index) {
            return (unused);

        } else if constexpr (SingleElementTupleLikeView<Attr> && !sequence_passes_view<parser_type>::value) {
            return alloy::get<0>(attr);

        } else {
            return (attr);
        }

    } else if constexpr (sequence_size == 0) {
        return (unused);

    } else if constexpr (sequence_size == 1 && !sequence_passes_view<parser_type>::value) {
        return alloy::get<offset>(attr);

    } else {
        return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            return alloy::tuple<alloy::tuple_element_t<offset + Is, Attr>&...>(
                alloy::get<offset + Is>(attr)...
            );
        }(std::make_index_sequence<sequence_size>{});
    }
}

// The attribute for an element refers into the attribute of the sequence, or is a temporary slice viewing it.
// The slice lives until the end of the full-expression that parses the element.
template<class T>
[[nodiscard]] constexpr T& to_lvalue(T&& attr IRIS_LIFETIMEBOUND) noexcept
{
    static_assert(
        std::is_lvalue_reference_v<T> || alloy::TupleLikeView<T>,
        "Only a slice viewing the attribute of the sequence can be a temporary; a value written into any other temporary is lost."
    );
    return static_cast<T&>(attr);
}

template<class Seq, std::size_t... Is, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attr>
[[nodiscard]] constexpr bool
parse_sequence_all(Seq const& seq, std::index_sequence<Is...>, It& first, Se const& last, Context const& ctx, Attr& attr)
{
    return (nary::get<Is>(seq.elems).parse(
        first, last, ctx, detail::to_lvalue(detail::sequence_attribute_for<Is>(seq, attr))
    ) && ...);
}

// The whole sequence yields one element when its value is written into a new element (nothing is
// left behind when a later part fails); otherwise each element of the sequence writes into the
// container on its own, as the value is written part by part.
//
// Note: A sequence that may succeed without writing its value does not yield a new element
//       (see `container_parse_strategy`).
template<traits::X4Container Container, class... Ps>
inline constexpr bool sequence_parses_as_new_element = [] {
    using value_type = planner::model_value_t<typename parser_traits<sequence<Ps...>>::attribute_type>;
    constexpr planner::node_write_strategy strategy = planner::node_write_strategy_of<
        planner::sequence_part_node<planner::storage_t<Container>, value_type>
    >;
    return
        strategy.is_writable && strategy.kind == planner::branch_kind::new_element &&
        !may_leave_attribute_unwritten_v<sequence<Ps...>>;
}();

// A sequence written part by part leaves the parts before a failed one in the container
template<class... Ps, traits::X4Container Container>
inline constexpr bool needs_chunk_buffer<sequence<Ps...>, Container> = !sequence_parses_as_new_element<Container, Ps...>;

template<class... Ps>
struct parse_into_container_impl<sequence<Ps...>>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    call(
        sequence<Ps...> const& seq, It& first, Se const& last,
        Context const& ctx, Attr& attr
    )
    {
        if constexpr (sequence_parses_as_new_element<Attr, Ps...>) {
            return parse_into_container_impl_default<sequence<Ps...>>::parse_part(seq, first, last, ctx, attr);

        } else {
            static_assert(
                parser_traits<sequence<Ps...>>::template accepts_container<Attr>,
                "No element of this sequence can write into the container, nor can the sequence as a whole"
            );
            return seq.parse(first, last, ctx, attr);
        }
    }
};

} // iris::x4::detail

#endif
