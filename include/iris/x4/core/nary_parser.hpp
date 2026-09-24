#ifndef IRIS_ZZ_X4_CORE_NARY_PARSER_HPP
#define IRIS_ZZ_X4_CORE_NARY_PARSER_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/parser.hpp>

#include <utility>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4::nary {

namespace detail {

template<std::size_t I, class P>
struct parser_element : P
{};

template<class Indices, class... Ps>
struct parser_storage;

template<std::size_t... Is, class... Ps>
struct parser_storage<std::index_sequence<Is...>, Ps...>
    : parser_element<Is, Ps>...
{};

} // detail

template<class... Ps>
using parser_storage_t = detail::parser_storage<std::index_sequence_for<Ps...>, Ps...>;

// --------------------------------------------------------

namespace detail {

template<std::size_t I, class P>
[[nodiscard]] P deduce_nary_parser_element(parser_element<I, P> const&) noexcept;

} // detail

template<std::size_t I, class... Ps>
using parser_t = decltype(detail::deduce_nary_parser_element<I>(
    std::declval<detail::parser_storage<std::index_sequence_for<Ps...>, Ps...> const&>()
));


template<std::size_t I, class P>
[[nodiscard]] constexpr P const&
get(detail::parser_element<I, P> const& elems IRIS_LIFETIMEBOUND) noexcept
{
    return elems;
}
template<std::size_t I, class P>
[[nodiscard]] constexpr P&&
get(detail::parser_element<I, P>&& elems IRIS_LIFETIMEBOUND) noexcept
{
    return static_cast<P&&>(elems);
}

// -------------------------------------------------------

template<template<class...> class ParserTT, X4ExplicitSubject... Ps>
[[nodiscard]] constexpr ParserTT<std::remove_cvref_t<Ps>...>
make_parser(Ps&&... ps)
{
    return {{ {}, { {static_cast<Ps&&>(ps)}... } }};
}


namespace detail {

template<template<class...> class ParserTT, std::size_t I, X4ExplicitSubject P>
[[nodiscard]] constexpr decltype(auto)
parser_at(P&& parser) noexcept
{
    if constexpr (is_ttp_specialization_of_v<std::remove_cvref_t<P>, ParserTT>) {
        return nary::get<I>(static_cast<P&&>(parser).elems);

    } else {
        static_assert(I == 0);
        return static_cast<P&&>(parser);
    }
}

template<template<class...> class ParserTT, std::size_t... LeftIs, std::size_t... RightIs, X4ExplicitSubject Left, X4ExplicitSubject Right>
[[nodiscard]] constexpr auto
concat_impl(std::index_sequence<LeftIs...>, std::index_sequence<RightIs...>, Left&& left, Right&& right)
{
    return nary::make_parser<ParserTT>(
        detail::parser_at<ParserTT, LeftIs>(std::forward<Left>(left))...,
        detail::parser_at<ParserTT, RightIs>(std::forward<Right>(right))...
    );
}

// This is NOT the same as `parser_traits<P>::sequence_size` because we need the
// element count here, not the count of non-unused attributes
template<template<class...> class ParserTT, class P>
inline constexpr std::size_t nary_parser_count = 1;

template<template<class...> class ParserTT, class... Ps>
inline constexpr std::size_t nary_parser_count<ParserTT, ParserTT<Ps...>> = sizeof...(Ps);

} // detail

template<template<class...> class ParserTT, X4ExplicitSubject Left, X4ExplicitSubject Right>
[[nodiscard]] constexpr auto
concat(Left&& left, Right&& right)
{
    return detail::concat_impl<ParserTT>(
        std::make_index_sequence<detail::nary_parser_count<ParserTT, std::remove_cvref_t<Left>>>{},
        std::make_index_sequence<detail::nary_parser_count<ParserTT, std::remove_cvref_t<Right>>>{},
        std::forward<Left>(left), std::forward<Right>(right)
    );
}

} // iris::x4::nary

namespace iris::x4 {

template<class Derived, class... Ps>
struct nary_parser : parser<Derived>
{
    static_assert(sizeof...(Ps) >= 2);
    static constexpr std::size_t element_count = sizeof...(Ps);

    static constexpr bool has_action = (Ps::has_action || ...);
    static constexpr bool need_rcontext = (Ps::need_rcontext || ...);

    IRIS_NO_UNIQUE_ADDRESS nary::parser_storage_t<Ps...>
    elems;
};

} // iris::x4

#endif
