#ifndef IRIS_ZZ_X4_PRIMITIVE_EOL_HPP
#define IRIS_ZZ_X4_PRIMITIVE_EOL_HPP

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

namespace iris::x4 {

struct eol_parser : parser<eol_parser>
{
    using attribute_type = unused_type;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr&)
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(first != last) &&
            noexcept(*first == static_cast<std::iter_value_t<It>>('\r')) &&
            noexcept(++first)
        )
    {
        auto it = first;
        x4::skip_over(it, last, ctx);
        bool matched = false;

        using iter_value_type = std::iter_value_t<It>;

        if (it != last && *it == static_cast<iter_value_type>('\r')) {
            matched = true;
            ++it;
        }
        if (it != last && *it == static_cast<iter_value_type>('\n')) {
            matched = true;
            ++it;
        }

        if (matched) first = it;
        return matched;
    }
};

template<>
struct get_info<eol_parser>
{
    using result_type = std::string;
    [[nodiscard]] result_type operator()(eol_parser const &) const { return "eol"; }
};

namespace parsers {

[[maybe_unused]] inline constexpr eol_parser eol{};

} // parsers

using parsers::eol;

} // iris::x4

#endif
