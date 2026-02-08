#ifndef IRIS_ALLOY_DETAIL_PACK_INDEXING_HPP
#define IRIS_ALLOY_DETAIL_PACK_INDEXING_HPP

/*=============================================================================
    Copyright (c) 2025 Yaito Kakeyama
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <version>

#include <cstddef>

namespace iris::alloy::detail {

template<std::size_t I, class... Ts>
struct type_pack_indexing;

template<std::size_t I, class... Ts>
using type_pack_indexing_t = typename type_pack_indexing<I, Ts...>::type;

#if __cpp_pack_indexing >= 202311L

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++26-extensions"

template<std::size_t I, class... Ts>
struct type_pack_indexing
{
    using type = Ts...[I];
};

#pragma clang diagnostic pop

#else

template<class T, class... Ts>
struct type_pack_indexing<0, T, Ts...>
{
    using type = T;
};

template<std::size_t I, class T, class... Ts>
struct type_pack_indexing<I, T, Ts...> : type_pack_indexing<I - 1, Ts...> {};

#endif

template<std::size_t I, auto... Vs>
struct non_type_pack_indexing;

template<std::size_t I, auto... Vs>
inline constexpr auto non_type_pack_indexing_v = non_type_pack_indexing<I, Vs...>::value;

#if __cpp_pack_indexing >= 202311L

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++26-extensions"

template<std::size_t I, auto... Vs>
struct non_type_pack_indexing
{
    static constexpr auto value = Vs...[I];
};

#pragma clang diagnostic pop

#else

template<auto V, auto... Vs>
struct non_type_pack_indexing<0, V, Vs...>
{
    static constexpr auto value = V;
};

template<std::size_t I, auto V, auto... Vs>
struct non_type_pack_indexing<I, V, Vs...> : non_type_pack_indexing<I - 1, Vs...> {};

#endif

} // iris::alloy::detail

#endif
