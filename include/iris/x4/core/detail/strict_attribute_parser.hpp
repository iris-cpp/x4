#ifndef IRIS_ZZ_X4_CORE_DETAIL_STRICT_ATTRIBUTE_PARSER_HPP
#define IRIS_ZZ_X4_CORE_DETAIL_STRICT_ATTRIBUTE_PARSER_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/traits/container_traits.hpp>

#include <iris/container_traits.hpp>

#include <ranges> // IWYU pragma: keep
#include <utility>

namespace iris::x4 {

namespace detail {

// Passes `v`, the attribute of the strict-attribute parser (e.g. `x4::rule` or `x4::as<T>`), to the
// exposed attribute `s` by the ordinary assignment. A container which holds the preceding results
// keeps them, and the value assigned to a new container is appended to it.
//
// Caller must preliminarily check the validity of such operation by `X4StrictlyWritable<S&, V&&>`.
template<class S, class V>
constexpr void pass_declared_attribute(S& s, V&& v)
{
    if constexpr (traits::X4Container<S>) {
        if (!std::ranges::empty(s)) {
            S assigned{};
            assigned = std::forward<V>(v);
            iris::container::transfer_from(s, std::move(assigned));
            return;
        }
    }
    s = std::forward<V>(v);
}

} // detail

} // iris::x4

#endif
