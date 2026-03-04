#ifndef IRIS_X4_OPERATOR_SEQUENCE_HPP
#define IRIS_X4_OPERATOR_SEQUENCE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/detail/parse_sequence.hpp>

#include <iris/x4/traits/attribute_of_binary.hpp>

#include <iris/x4/directive/expect.hpp>

#include <iris/alloy/tuple.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace detail {

template<class LeftAttr, class RightAttr, class Container>
struct is_sequence_suitable_for_container_impl // e.g. `char_ >> char_` into `std::string`
    : std::conjunction<
        std::is_same<LeftAttr, RightAttr>,
        traits::can_hold<LeftAttr, traits::container_value_t<Container>>
    >
{};

template<traits::X4Container LeftAttr, class RightAttr, class Container>
struct is_sequence_suitable_for_container_impl<LeftAttr, RightAttr, Container>  // e.g. `*char_ >> char_` into `std::string`
    : std::conjunction<
        std::is_same<traits::container_value_t<LeftAttr>, RightAttr>,
        traits::can_hold<LeftAttr, Container>
    >
{};

template<class LeftAttr, traits::X4Container RightAttr, class Container>
struct is_sequence_suitable_for_container_impl<LeftAttr, RightAttr, Container>  // e.g. `char_ >> *char_` into `std::string`
    : std::conjunction<
        std::is_same<LeftAttr, traits::container_value_t<RightAttr>>,
        traits::can_hold<RightAttr, Container>
    >
{};

template<traits::X4Container LeftAttr, traits::X4Container RightAttr, class Container>
struct is_sequence_suitable_for_container_impl<LeftAttr, RightAttr, Container>  // e.g. `*char_ >> *char_` into `std::string`
    : std::conjunction <
        std::is_same<LeftAttr, RightAttr>,
        traits::can_hold<LeftAttr, Container>
    >
{};

template<class LeftAttr, class RightAttr, class Container>
struct is_sequence_suitable_for_container
    : is_sequence_suitable_for_container_impl<LeftAttr, RightAttr, Container>
{};

template<X4UnusedAttribute LeftAttr, class RightAttr, class Container>
struct is_sequence_suitable_for_container<LeftAttr, RightAttr, Container>
    : traits::can_hold<RightAttr, Container>
{};

template<class LeftAttr, X4UnusedAttribute RightAttr, class Container>
struct is_sequence_suitable_for_container<LeftAttr, RightAttr, Container>
    : traits::can_hold<LeftAttr, Container>
{};

template<X4UnusedAttribute LeftAttr, X4UnusedAttribute RightAttr, class Container>
struct is_sequence_suitable_for_container<LeftAttr, RightAttr, Container>
    : std::false_type
{};

template<class LeftAttr, class RightAttr, class Container>
inline constexpr bool is_sequence_suitable_for_container_v = is_sequence_suitable_for_container<LeftAttr, RightAttr, Container>::value;

} // detail

template<class Left, class Right>
struct sequence : binary_parser<Left, Right, sequence<Left, Right>>
{
    using attribute_type = traits::attribute_of_binary<alloy::tuple, Left, Right>::type;

    static constexpr std::size_t sequence_size =
        parser_traits<Left>::sequence_size + parser_traits<Right>::sequence_size;

    template<class Container>
    static constexpr bool handles_container =
        (parser_traits<Left>::template handles_container<Container> && parser_traits<Right>::template handles_container<Container>) ||
        detail::is_sequence_suitable_for_container_v<typename parser_traits<Left>::attribute_type, typename parser_traits<Right>::attribute_type, Container>;

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
