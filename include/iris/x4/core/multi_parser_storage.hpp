#ifndef IRIS_ZZ_X4_CORE_MULTI_PARSER_STORAGE_HPP
#define IRIS_ZZ_X4_CORE_MULTI_PARSER_STORAGE_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <utility>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4 {

namespace detail {

template<std::size_t I, class P>
struct multi_parser_element : P
{};

template<class Indices, class... Ps>
struct multi_parser_storage;

template<std::size_t... Is, class... Ps>
struct multi_parser_storage<std::index_sequence<Is...>, Ps...>
    : multi_parser_element<Is, Ps>...
{};

} // detail

template<class... Ps>
using multi_parser_storage_t = detail::multi_parser_storage<std::index_sequence_for<Ps...>, Ps...>;

// --------------------------------------------------------

template<std::size_t I, class P>
[[nodiscard]] constexpr P const&
multi_parser_get(detail::multi_parser_element<I, P> const& elem) noexcept
{
    return elem;
}

template<std::size_t I, class P>
[[nodiscard]] constexpr P const&
multi_parser_forward(detail::multi_parser_element<I, P> const& elem) noexcept
{
    return elem;
}

template<std::size_t I, class P>
[[nodiscard]] constexpr P&&
multi_parser_forward(detail::multi_parser_element<I, P>&& elem) noexcept
{
    return static_cast<P&&>(elem);
}

namespace detail {

template<std::size_t I, class P>
[[nodiscard]] P deduce_multi_parser_element(multi_parser_element<I, P> const&) noexcept;

} // detail

template<std::size_t I, class... Ps>
using multi_parser_t = decltype(detail::deduce_multi_parser_element<I>(
    std::declval<detail::multi_parser_storage<std::index_sequence_for<Ps...>, Ps...> const&>()
));


template<std::size_t I, class P>
[[nodiscard]] constexpr P const&
get_parser(detail::multi_parser_element<I, P> const& elems IRIS_LIFETIMEBOUND) noexcept
{
    return elems;
}
template<std::size_t I, class P>
[[nodiscard]] constexpr P&&
get_parser(detail::multi_parser_element<I, P>&& elems IRIS_LIFETIMEBOUND) noexcept
{
    return static_cast<P&&>(elems);
}

} // iris::x4

#endif
