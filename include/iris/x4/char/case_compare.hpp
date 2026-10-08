#ifndef IRIS_ZZ_X4_CHAR_CASE_COMPARE_HPP
#define IRIS_ZZ_X4_CHAR_CASE_COMPARE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

// Do NOT include `char_encoding/` related headers in this file

#include <iris/x4/core/context.hpp>

#include <concepts>

namespace iris::x4 {

template<class Encoding>
struct case_compare
{
    using encoding_type = Encoding;
    using char_type = Encoding::char_type;
    using classify_type = Encoding::classify_type;

    template<class Char, class CharSet>
    [[nodiscard]] static constexpr bool in_set(Char ch, CharSet const& set) noexcept
    {
        static_assert(std::same_as<Char, char_type> || std::same_as<Char, classify_type>);
        static_assert(noexcept(set.test(static_cast<classify_type>(ch))));
        return set.test(static_cast<classify_type>(ch));
    }

    template<class LChar, class RChar>
    [[nodiscard]] static constexpr int
    operator()(LChar lc, RChar rc) noexcept
    {
        static_assert(std::same_as<LChar, char_type> || std::same_as<LChar, classify_type>);
        static_assert(std::same_as<RChar, char_type> || std::same_as<RChar, classify_type>);
        return lc - rc;
    }

    template<class CharClassTag>
    [[nodiscard]] static constexpr CharClassTag get_char_class_tag(CharClassTag tag) noexcept
    {
        return tag;
    }
};

template<class Encoding>
struct no_case_compare;

namespace detail {

struct case_compare_tag
{
    static constexpr bool is_unique = false;
};

struct case_compare_no_case_t;

} // detail

template<class Encoding, class Context>
[[nodiscard]] constexpr auto
get_case_compare(Context const&) noexcept
{
    if constexpr (has_context_of<Context, detail::case_compare_tag, detail::case_compare_no_case_t>) {
        return no_case_compare<Encoding>{};
    } else {
        return case_compare<Encoding>{};
    }
}

} // iris::x4

#endif
