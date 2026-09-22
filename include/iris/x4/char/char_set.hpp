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
#include <iris/x4/char/detail/basic_chset.hpp>
#include <iris/x4/string/case_compare.hpp>
#include <iris/x4/core/char_traits.hpp>

#include <iris/unicode/string.hpp>

#include <string_view>
#include <format>
#include <ranges>
#include <type_traits>

namespace iris::x4 {

// Parser for a character range
template<class Encoding, X4Attribute Attr = typename Encoding::char_type>
struct char_range : char_parser<char_range<Encoding, Attr>, Encoding>
{
    using char_type = Encoding::char_type;
    using encoding_type = Encoding;
    using attribute_type = Attr;

    static constexpr bool has_attribute = !std::is_same_v<unused_type, attribute_type>;

    constexpr char_range(char_type from_, char_type to_) noexcept
        : from(from_), to(to_)
    {}

    template<class Char, class Context>
        requires (std::is_convertible_v<std::remove_cvref_t<Char>, char_type>)
    [[nodiscard]] constexpr bool test(Char ch_, Context const& ctx) const noexcept
    {
        char_type ch = static_cast<char_type>(ch_);  // optimize for token based parsing
        static_assert(noexcept(x4::get_case_compare<encoding_type>(ctx)(ch, from)));

        return x4::get_case_compare<encoding_type>(ctx)(ch, from) >= 0
            && x4::get_case_compare<encoding_type>(ctx)(ch , to) <= 0;
    }

    char_type from, to;

    [[nodiscard]] std::string get_x4_info() const
    {
        // TODO: make more user-friendly && make the format consistent with above
        // TODO: escape
        return std::format(
            "char_range \"{}-{}\"",
            iris::unicode::transcode<char>(typename Encoding::string_type(1, this->from)),
            iris::unicode::transcode<char>(typename Encoding::string_type(1, this->to))
        );
    }
};

// Parser for a character set
template<class Encoding, X4Attribute Attr = typename Encoding::char_type>
struct char_set : char_parser<char_set<Encoding, Attr>, Encoding>
{
    using char_type = Encoding::char_type;
    using encoding_type = Encoding;
    using attribute_type = Attr;

    static constexpr bool has_attribute = !std::is_same_v<unused_type, attribute_type>;

    constexpr explicit char_set(std::basic_string_view<char_type> const str)
        // never noexcept; requires vector insertion
    {
        for (auto definition = std::ranges::begin(str); definition != std::ranges::end(str);) {
            auto const ch = *definition;
            auto next_definition = std::next(definition);
            if (next_definition == std::ranges::end(str)) {
                chset_.set(ch);
                break;
            }

            auto next_ch = *next_definition;
            if (next_ch == detail::char_tokens<char_type>::hyphen) {
                next_definition = std::next(next_definition);
                if (next_definition == std::ranges::end(str)) {
                    chset_.set(ch);
                    chset_.set(detail::char_tokens<char_type>::hyphen);
                    break;
                }
                chset_.set(ch, *next_definition);

            } else {
                chset_.set(ch);
            }

            definition = next_definition;
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
        return "char-set"; // TODO: make more user-friendly
    }

private:
    detail::basic_chset<char_type> chset_;
};

} // iris::x4

#endif
