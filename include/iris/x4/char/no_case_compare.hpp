#ifndef IRIS_ZZ_X4_CHAR_NO_CASE_COMPARE_HPP
#define IRIS_ZZ_X4_CHAR_NO_CASE_COMPARE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/char_encoding/char_properties.hpp>
#include <iris/x4/char/char_class_tags.hpp>

#include <concepts>

namespace iris::x4 {

template<class Encoding>
struct no_case_compare
{
    using encoding_type = Encoding;
    using char_type = Encoding::char_type;
    using classify_type = Encoding::classify_type;

    template<class Char, class CharSet>
    [[nodiscard]] static constexpr bool in_set(Char ch, CharSet const& set) noexcept
    {
        static_assert(std::same_as<Char, char_type> || std::same_as<Char, classify_type>);
        auto const classify_ch = static_cast<classify_type>(ch);
        static_assert(noexcept(set.test(classify_ch)));
        return set.test(classify_ch)
            || set.test(static_cast<classify_type>(
                char_encoding::char_properties<Encoding>::islower(classify_ch)
                ? char_encoding::char_properties<Encoding>::toupper(classify_ch)
                : char_encoding::char_properties<Encoding>::tolower(classify_ch))
            );
    }

    template<class LChar, class RChar>
    [[nodiscard]] static constexpr int
    operator()(LChar lc, RChar rc) noexcept
    {
        static_assert(std::same_as<LChar, char_type> || std::same_as<LChar, classify_type>);
        static_assert(std::same_as<RChar, char_type> || std::same_as<RChar, classify_type>);

        auto const classify_lc = static_cast<classify_type>(lc);
        auto const classify_rc = static_cast<classify_type>(rc);
        return char_encoding::char_properties<Encoding>::islower(classify_rc)
            ? char_encoding::char_properties<Encoding>::tolower(classify_lc) - classify_rc
            : char_encoding::char_properties<Encoding>::toupper(classify_lc) - classify_rc;
    }

    template<class CharClassTag>
    [[nodiscard]] static constexpr CharClassTag get_char_class_tag(CharClassTag tag) noexcept
    {
        return tag;
    }

    [[nodiscard]] static constexpr char_classes::alpha_tag get_char_class_tag(char_classes::lower_tag) noexcept
    {
        return {};
    }

    [[nodiscard]] static constexpr char_classes::alpha_tag get_char_class_tag(char_classes::upper_tag) noexcept
    {
        return {};
    }
};

namespace detail {
struct case_compare_no_case_t { constexpr explicit case_compare_no_case_t() = default; };
[[maybe_unused]] inline constexpr case_compare_no_case_t case_compare_no_case{};
} // detail

} // iris::x4

#endif
