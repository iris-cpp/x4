#ifndef IRIS_ZZ_X4_CORE_MULTI_PARSER_HPP
#define IRIS_ZZ_X4_CORE_MULTI_PARSER_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/multi_parser_storage.hpp>
#include <iris/x4/core/parser.hpp>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4 {

template<class Derived, class... Ps>
struct multi_parser : parser<Derived>
{
    static_assert(sizeof...(Ps) >= 2);
    static constexpr std::size_t element_count = sizeof...(Ps);

    static constexpr bool has_action = (Ps::has_action || ...);
    static constexpr bool need_rcontext = (Ps::need_rcontext || ...);

    IRIS_NO_UNIQUE_ADDRESS multi_parser_storage_t<Ps...>
    elems;
};

} // iris::x4

#endif
