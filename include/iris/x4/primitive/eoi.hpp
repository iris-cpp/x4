#ifndef IRIS_ZZ_X4_PRIMITIVE_EOI_HPP
#define IRIS_ZZ_X4_PRIMITIVE_EOI_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/unused.hpp>

#include <iterator>
#include <type_traits>

namespace iris::x4 {

struct eoi_parser : parser<eoi_parser>
{
    using attribute_type = unused_type;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr&)
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(first == last)
        )
    {
        auto it = first;
        x4::skip_over(it, last, ctx);
        bool const ok = it == last;
        if (ok) first = it;
        return ok;
    }
};

template<>
struct get_info<eoi_parser>
{
    using result_type = std::string;
    [[nodiscard]] result_type operator()(eoi_parser const &) const { return "eoi"; }
};

namespace parsers {

[[maybe_unused]] inline constexpr eoi_parser eoi{};

} // parsers

using parsers::eoi;

} // iris::x4

#endif
