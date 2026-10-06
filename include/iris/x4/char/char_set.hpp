#ifndef IRIS_ZZ_X4_CHAR_CHAR_SET_HPP
#define IRIS_ZZ_X4_CHAR_CHAR_SET_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/char/char_parser.hpp>
#include <iris/x4/char/detail/check_char.hpp>
#include <iris/x4/char/detail/chset.hpp>
#include <iris/x4/string/case_compare.hpp>

#include <iris/x4/core/traits/char_traits.hpp>

#include <iris/unicode/string.hpp>

#include <string_view>
#include <type_traits>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4 {

// Parser for a character range
template<class Encoding, class Attr = typename Encoding::char_type>
struct char_range : char_parser<char_range<Encoding, Attr>, Encoding>
{
    static_assert(X4Attribute<Attr>);

    using encoding_type = Encoding;
    using char_type = Encoding::char_type;
    using classify_type = Encoding::classify_type;
    using attribute_type = Attr;

    static constexpr bool has_attribute = !std::is_same_v<unused_type, attribute_type>;

    constexpr char_range(std::same_as<char_type> auto const from, std::same_as<char_type> auto const to)
        : from_(static_cast<classify_type>(from)), to_(static_cast<classify_type>(to))
    {
        detail::check_char_range<Encoding>(from, to);
    }

    template<class Context>
    [[nodiscard]] constexpr bool test(std::same_as<char_type> auto const ch, Context const& ctx) const noexcept
    {
        auto const classify_ch = static_cast<classify_type>(ch);
        static_assert(noexcept(x4::get_case_compare<encoding_type>(ctx)(classify_ch, from_)));

        return x4::get_case_compare<encoding_type>(ctx)(classify_ch, from_) >= 0 &&
            x4::get_case_compare<encoding_type>(ctx)(classify_ch, to_) <= 0;
    }

    [[nodiscard]] std::string get_x4_info() const
    {
        // TODO: escape
        return std::string("char_(\"")
            + iris::unicode::transcode<char>(typename Encoding::string_type(1, static_cast<char_type>(from_)))
            + '-' + iris::unicode::transcode<char>(typename Encoding::string_type(1, static_cast<char_type>(to_)))
            + "\")";
    }

private:
    classify_type from_, to_;
};

// Parser for a character set
template<class Encoding, class Chset, X4Attribute Attr = typename Encoding::char_type>
struct char_set : char_parser<char_set<Encoding, Chset, Attr>, Encoding>
{
    using char_type = Encoding::char_type;
    using classify_type = Encoding::classify_type;
    using encoding_type = Encoding;
    using attribute_type = Attr;

    static constexpr bool has_attribute = !std::is_same_v<unused_type, attribute_type>;

    constexpr explicit char_set(std::basic_string_view<char_type> const definition)
    {
        for (std::size_t i = 0; i < definition.size();) {
            bool const is_range = i + 2 < definition.size() && definition[i + 1] == hyphen;
            char_type const from = definition[i];
            char_type const to = definition[is_range ? i + 2 : i];
            detail::check_char_range<Encoding>(from, to);
            chset_.set(static_cast<classify_type>(from), static_cast<classify_type>(to));
            i += is_range ? 3 : 1;
        }
    }

    template<class Char, class Context>
    [[nodiscard]] constexpr bool test(Char ch_, Context const& ctx) const noexcept
    {
        static_assert(noexcept(x4::get_case_compare<encoding_type>(ctx).in_set(ch_, chset_)));
        return x4::get_case_compare<encoding_type>(ctx).in_set(ch_, chset_);
    }

    [[nodiscard]] std::string get_x4_info() const
    {
        // TODO: escape
        typename Encoding::string_type definition;
        bool has_hyphen = false;
        chset_.for_each_range([&](classify_type const first, classify_type const last) {
            if (first == last && static_cast<char_type>(first) == hyphen) {
                has_hyphen = true;
                return;
            }
            definition += static_cast<char_type>(first);
            if (first != last) {
                definition += hyphen;
                definition += static_cast<char_type>(last);
            }
        });
        if (has_hyphen) {
            definition += hyphen;
        }
        return "char_(\"" + iris::unicode::transcode<char>(definition) + "\")";
    }

private:
    static constexpr char_type hyphen = detail::char_tokens<char_type>::hyphen;

    Chset chset_;
};

} // iris::x4

#endif
