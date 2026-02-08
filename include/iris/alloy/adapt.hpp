#ifndef IRIS_ALLOY_ADAPT_HPP
#define IRIS_ALLOY_ADAPT_HPP

/*=============================================================================
    Copyright (c) 2025 Yaito Kakeyama
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

namespace iris::alloy {

namespace detail {

template<auto... Vs>
struct non_type_list;

} // detail

template<class T>
struct adaptor;

template<auto... Getters>
using make_getters_list = detail::non_type_list<Getters...>;

} // iris::alloy

#endif
