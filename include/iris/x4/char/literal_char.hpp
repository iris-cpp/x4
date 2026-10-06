#ifndef IRIS_ZZ_X4_CHAR_LITERAL_CHAR_HPP
#define IRIS_ZZ_X4_CHAR_LITERAL_CHAR_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/char/char_parser.hpp>
#include <iris/x4/char/detail/check_char.hpp>
#include <iris/x4/string/case_compare.hpp>

#include <iris/unicode/string.hpp>

#include <concepts>
#include <type_traits>

namespace iris::x4 {

template<class Encoding, class Attr = typename Encoding::char_type>
struct literal_char : char_parser<literal_char<Encoding, Attr>, Encoding>
{
    static_assert(X4Attribute<Attr>);

    using encoding_type = Encoding;
    using attribute_type = Attr;
    using char_type = Encoding::char_type;
    using classify_type = Encoding::classify_type;

    static constexpr bool has_attribute = !std::is_same_v<unused_type, attribute_type>;

    template<std::same_as<char_type> Char>
    constexpr explicit literal_char(Char const ch)
        : classify_ch_(static_cast<classify_type>(ch))
    {
        detail::check_char<Encoding>(ch);
    }

    [[nodiscard]] constexpr bool
    test(std::same_as<char_type> auto const ch, auto const& ctx) const noexcept
    {
        static_assert(noexcept(x4::get_case_compare<encoding_type>(ctx)(classify_ch_, static_cast<classify_type>(ch))));
        return x4::get_case_compare<encoding_type>(ctx)(classify_ch_, static_cast<classify_type>(ch)) == 0;
    }

    constexpr void
    test(auto const, auto const&) const = delete; // Mixing incompatible character types is not allowed

    [[nodiscard]] constexpr classify_type classify_ch() const noexcept { return classify_ch_; }

    [[nodiscard]] std::string get_x4_info() const
    {
        // TODO: escape quote
        return '\'' + iris::unicode::transcode<char>(typename Encoding::string_type(1, this->classify_ch_)) + '\'';
    }

private:
    classify_type classify_ch_{};
};

} // iris::x4

#endif
