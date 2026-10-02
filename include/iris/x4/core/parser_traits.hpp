#ifndef IRIS_ZZ_X4_CORE_PARSER_TRAITS_HPP
#define IRIS_ZZ_X4_CORE_PARSER_TRAITS_HPP

/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/traits/write_rank.hpp>

#include <iris/type_list.hpp>

#include <concepts>
#include <type_traits>

#include <cstddef> // IWYU pragma: keep

// If a user-provided program declares an explicit or partial
// specialization of any entity defined in this header, the
// program is ill-formed, no diagnostic required.

namespace iris::x4 {

struct unused_type;
struct unused_container_type;


namespace detail {

template<class Parser>
struct get_attribute_type
{
    using type = unused_type;
};

template<class Parser>
    requires
        requires { typename Parser::attribute_type; }
struct get_attribute_type<Parser>
{
    using type = Parser::attribute_type;
};

template<class Parser, class Container>
struct get_accepts_container
{
    static constexpr bool value = is_writable_v<Container&, typename get_attribute_type<Parser>::type>;
};

template<class Parser, class Container>
    requires
        requires { Parser::template accepts_container<Container>; }
struct get_accepts_container<Parser, Container>
{
    static constexpr bool value = Parser::template accepts_container<Container>;
};

} // detail


template<class Parser>
struct has_attribute : std::bool_constant<
    !std::same_as<typename detail::get_attribute_type<Parser>::type, unused_type> &&
    !std::same_as<typename detail::get_attribute_type<Parser>::type, unused_container_type>
>
{};

template<class Parser>
inline constexpr bool has_attribute_v = has_attribute<Parser>::value;

template<class Parser>
    requires
        requires { Parser::has_attribute; }
struct has_attribute<Parser> : std::bool_constant<Parser::has_attribute>
{};


namespace detail {

template<class Parser>
struct get_sequence_size
{
    static constexpr std::size_t value = has_attribute<Parser>::value;
};

template<class Parser>
    requires
        requires { Parser::sequence_size; }
struct get_sequence_size<Parser>
{
    static constexpr std::size_t value = Parser::sequence_size;
};

template<class Parser>
concept attribute_passing_proxy =
    requires { typename Parser::proxy_backend_type; } &&
    std::same_as<typename get_attribute_type<Parser>::type, typename get_attribute_type<typename Parser::proxy_backend_type>::type>;

// Whether the parser may succeed without writing its attribute, i.e., an alternative with a
// branch without an attribute, or a parser that passes its attribute to such an alternative.
//
// The attribute given to such a parser is NOT prepared for its attribute type (`prepare_attribute_for`),
// as the part made for that type would remain even when nothing is written (e.g., `int` emplaced
// into `rvariant<std::string, int>` by `int_ | lit("auto")`, left as `0` when "auto" matches).
//
// Instead, the attribute is handed over whole in its default state, which then remains when
// the parser writes nothing.
template<class Parser>
struct may_leave_attribute_unwritten : std::false_type {};

template<attribute_passing_proxy Parser>
struct may_leave_attribute_unwritten<Parser> : may_leave_attribute_unwritten<typename Parser::proxy_backend_type> {};

template<class Parser>
inline constexpr bool may_leave_attribute_unwritten_v = may_leave_attribute_unwritten<Parser>::value;

template<class Parser>
struct attribute_candidates
{
    using type = std::conditional_t<
        std::same_as<typename get_attribute_type<Parser>::type, unused_type>,
        type_list<>,
        type_list<typename get_attribute_type<Parser>::type>
    >;
};

template<attribute_passing_proxy Parser>
struct attribute_candidates<Parser> : attribute_candidates<typename Parser::proxy_backend_type> {};

template<class Parser>
using attribute_candidates_t = attribute_candidates<Parser>::type;

} // detail


// A global facade exposing all traits recognized and used by X4 core.
//
// This class is not placed under the `traits/` directory because this is NOT
// a customization point.
//
// The purposes of this class are as follows:
//   - Provide some defaulted or automatically deduced characteristics for any
//     legitimate parser class, similar to `std::allocator_traits`.
//   - Gather all miscellaneous trait interfaces that were previously
//     scattered across the `traits/` headers.
//   - Make sure these core traits are not allowed to be specialized in user
//     applications.
template<class Parser>
struct parser_traits
{
    static_assert(!requires {
        Parser::is_pass_through_unary;
    }, "`::is_pass_through_unary` is obsolete. Derive from `x4::proxy_parser`.");

    using attribute_type = detail::get_attribute_type<Parser>::type;

    // Equivalent to `true` if and only if the deduced `attribute_type` is neither
    // `unused_type` nor `unused_container_type`.
    //
    // A user-defined parser class may define `::has_attribute` member explicitly
    // to bypass the type deduction for better compile times. If such member is
    // defined as a contradicting value, the program is ill-formed, no diagnostic
    // required.
    static constexpr bool has_attribute = x4::has_attribute<Parser>::value;

    static constexpr std::size_t sequence_size = detail::get_sequence_size<Parser>::value;

    // If true, X4 may pass `Container` itself to the parser in place of a new element of it,
    // and the parser appends into it. Whether X4 does so is decided by the priority of writes
    // into a container (`detail::container_parse_strategy_for`).
    template<class Container>
    static constexpr bool accepts_container = detail::get_accepts_container<Parser, Container>::value;

    static constexpr bool has_action = Parser::has_action;
    static constexpr bool need_rcontext = Parser::need_rcontext;
};

namespace detail {

// The value of `Parser` is written into `Container` as a part: a new element, part by part, or a range appended
template<class Parser, class Container>
concept writes_as_part =
    has_attribute_v<Parser> &&
    planner::node_write_strategy_of<
        planner::sequence_part_node<
            planner::storage_t<Container>,
            planner::model_value_t<typename parser_traits<Parser>::attribute_type>
        >
    >.is_writable;

} // detail

// `Parser` writes into `Container`: it is passed the container itself and appends into it, or its value is written into it as a part
template<class Parser, class Container>
concept writes_into_container =
    parser_traits<Parser>::template accepts_container<Container> ||
    detail::writes_as_part<Parser, Container>;

} // iris::x4

#endif
