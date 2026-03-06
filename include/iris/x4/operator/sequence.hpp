#ifndef IRIS_ZZ_X4_OPERATOR_SEQUENCE_HPP
#define IRIS_ZZ_X4_OPERATOR_SEQUENCE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/detail/parse_sequence.hpp>
#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/move_to.hpp>

#include <iris/x4/traits/attribute_of_binary.hpp>
#include <iris/x4/traits/container_traits.hpp>

#include <iris/x4/directive/expect.hpp>

#include <iris/alloy/tuple.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace detail {

template<traits::X4Container Container, class Elem>
struct container_can_hold_element : std::is_same<Container, Elem>
{};

template<traits::X4Container Container, class Elem>
    requires
        (!std::same_as<Container, Elem>) &&
        (!traits::X4Container<Elem>) &&
        requires (Container& c, Elem&& elem) {
            traits::push_back(c, std::move(elem));
        }
struct container_can_hold_element<Container, Elem>
    : std::true_type
{};

template<traits::X4Container Container, class ContainerElem>
    requires
        (!std::same_as<Container, ContainerElem>) &&
        traits::X4Container<ContainerElem> &&
        requires (Container& c, ContainerElem&& container_elem) {
            x4::move_to(
                std::make_move_iterator(traits::begin(container_elem)),
                std::make_move_iterator(traits::end(container_elem)),
                c
            );
        }
struct container_can_hold_element<Container, ContainerElem>
    : std::true_type
{};

template<traits::X4Container Container, class SequenceAttr_Maybe_Unwrapped>
struct container_can_hold_sequence : container_can_hold_element<Container, SequenceAttr_Maybe_Unwrapped>
{};

template<traits::X4Container Container, class... Ts>
struct container_can_hold_sequence<Container, alloy::tuple<Ts...>>
    // this should not delegate to `container_can_hold_sequence`; we don't want recursive expansion here.
    : std::conjunction<container_can_hold_element<Container, Ts>...>
{};

} // detail

template<class Left, class Right>
struct sequence : binary_parser<Left, Right, sequence<Left, Right>>
{
    using attribute_type = traits::detail::attribute_of_sequence<Left, Right>::type;

    static constexpr std::size_t sequence_size =
        parser_traits<Left>::sequence_size + parser_traits<Right>::sequence_size;

    template<traits::X4Container Container>
    static constexpr bool handles_container =
        (
            parser_traits<Left>::template handles_container<Container> &&
            parser_traits<Right>::template handles_container<Container>
        ) ||
        detail::container_can_hold_sequence<Container, attribute_type>::value;

    using binary_parser<Left, Right, sequence>::binary_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr const&) const
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            is_nothrow_parsable_v<Left, It, Se, Context, unused_type> &&
            is_nothrow_parsable_v<Right, It, Se, Context, unused_type>
        )
    {
        It const first_saved = first;

        if (this->left.parse(first, last, ctx, unused)
            && this->right.parse(first, last, ctx, unused)
        ) {
            return true;
        }

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) {
                // don't rollback iterator (mimicking exception-like behavior)
                return false;
            }
        }

        first = first_saved;
        return false;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(noexcept(detail::parse_sequence(*this, first, last, ctx, attr)))
    {
        return detail::parse_sequence(*this, first, last, ctx, attr);
    }
};

template<X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr sequence<as_parser_plain_t<Left>, as_parser_plain_t<Right>>
operator>>(Left&& left, Right&& right)
    noexcept(
        is_parser_nothrow_castable_v<Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<
            sequence<as_parser_plain_t<Left>, as_parser_plain_t<Right>>,
            as_parser_t<Left>,
            as_parser_t<Right>
        >
    )
{
    return {as_parser(std::forward<Left>(left)), as_parser(std::forward<Right>(right))};
}

template<X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr sequence<as_parser_plain_t<Left>, expect_directive<as_parser_plain_t<Right>>>
operator>(Left&& left, Right&& right)
    noexcept(
        is_parser_nothrow_castable_v<Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<
            expect_directive<as_parser_plain_t<Right>>,
            as_parser_t<Right>
        > &&
        std::is_nothrow_constructible_v<
            sequence<as_parser_plain_t<Left>, expect_directive<as_parser_plain_t<Right>>>,
            as_parser_t<Left>,
            expect_directive<as_parser_plain_t<Right>>
        >
    )
{
    return {
        as_parser(std::forward<Left>(left)),
        expect_directive<as_parser_plain_t<Right>>(as_parser(std::forward<Right>(right)))
    };
}

} // iris::x4

#endif
