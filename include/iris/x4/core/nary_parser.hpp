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
struct nary_parser_element : P
{};

template<class Indices, class... Ps>
struct parser_storage;

template<std::size_t... Is, class... Ps>
struct parser_storage<std::index_sequence<Is...>, Ps...>
    : nary_parser_element<Is, Ps>...
{};

} // detail

template<class... Ps>
using parser_storage_t = detail::parser_storage<std::index_sequence_for<Ps...>, Ps...>;

// --------------------------------------------------------

namespace detail {

template<std::size_t I, class P>
[[nodiscard]] P deduce_nary_parser_element(nary_parser_element<I, P> const&) noexcept;

} // detail

template<std::size_t I, class... Ps>
using parser_t = decltype(detail::deduce_nary_parser_element<I>(
    std::declval<detail::parser_storage<std::index_sequence_for<Ps...>, Ps...> const&>()
));


template<std::size_t I, class P>
[[nodiscard]] constexpr P const&
get(detail::nary_parser_element<I, P> const& elems IRIS_LIFETIMEBOUND) noexcept
{
    return elems;
}
template<std::size_t I, class P>
[[nodiscard]] constexpr P&&
get(detail::nary_parser_element<I, P>&& elems IRIS_LIFETIMEBOUND) noexcept
{
    return static_cast<P&&>(elems);
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
