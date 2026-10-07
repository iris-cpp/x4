#ifndef IRIS_ZZ_X4_SYMBOLS_HPP
#define IRIS_ZZ_X4_SYMBOLS_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/char/case_compare.hpp>

#include <iris/x4/char_encoding/standard.hpp>

#ifdef IRIS_X4_UNICODE
# include <iris/x4/char_encoding/unicode.hpp>
#endif

#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/attribute.hpp>
#include <iris/x4/core/write_attribute.hpp>
#include <iris/x4/core/traits/char_traits.hpp>
#include <iris/x4/core/traits/char_encoding_traits.hpp>

#include <iris/error/throwf.hpp>
#include <iris/bits/specialization_of.hpp>

#include <algorithm>
#include <ranges>
#include <array>
#include <concepts>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4 {

// Non-owning key + owning value
template<class CharT, class T>
struct symbols_entry
{
    using char_type = CharT;
    using value_type = T;

    std::basic_string_view<CharT> key;
    IRIS_NO_UNIQUE_ADDRESS T value{};
};

// Matches the longest key that is a prefix of the input.
// Note: This class holds the keys by reference.
template<class Encoding, class T, std::size_t N>
struct symbols_parser : parser<symbols_parser<Encoding, T, N>>
{
    static_assert(N >= 1);
    static_assert(X4Attribute<T>);

    using encoding_type = Encoding;
    using char_type = Encoding::char_type;
    using value_type = T;
    using entry_type = symbols_entry<char_type, T>;

    using attribute_type = T;

    static constexpr bool has_attribute = !std::same_as<attribute_type, unused_type>;

    constexpr symbols_parser(std::string_view parser_name, entry_type const (&entries)[N])
        : symbols_parser(std::in_place, parser_name, std::to_array(entries))
    {
    }

    constexpr symbols_parser(std::string_view parser_name, std::basic_string_view<char_type> const (&keys)[N])
        requires (!has_attribute)
        : symbols_parser(
            std::in_place,
            parser_name,
            [&]<std::size_t... I>(std::index_sequence<I...>) {
                return std::array<entry_type, N>{entry_type{keys[I], T{}}...};
            }(std::make_index_sequence<N>{})
        )
    {
    }

    // Exact match, case-sensitive
    [[nodiscard]] constexpr value_type const* find(std::basic_string_view<char_type> const key) const noexcept
    {
        auto const it = std::lower_bound(
            entries_.begin(), entries_.end(), key,
            [](entry_type const& entry, std::basic_string_view<char_type> const k) { return entry.key < k; }
        );
        return it != entries_.end() && it->key == key ? &it->value : nullptr;
    }

    // Longest match, case-sensitive. Advances `first` past the matched key on success.
    template<std::forward_iterator It, std::sentinel_for<It> Se>
    [[nodiscard]] constexpr value_type const* prefix_find(It& first, Se const& last) const noexcept
    {
        entry_type const* entry = this->template longest_match<false>(first, last);
        return entry ? &entry->value : nullptr;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        It it = first;
        x4::skip_over(it, last, ctx);

        constexpr bool is_no_case = std::same_as<decltype(x4::get_case_compare<Encoding>(ctx)), no_case_compare<Encoding>>;
        if (entry_type const* entry = this->template longest_match<is_no_case>(it, last)) {
            x4::write_attribute(attr, entry->value);
            first = it;
            return true;
        }
        return false;
    }

    [[nodiscard]] constexpr std::string_view name() const noexcept
    {
        return parser_name_;
    }

    [[nodiscard]] constexpr std::string_view get_x4_info() const noexcept
    {
        return parser_name_;
    }

private:
    constexpr symbols_parser(std::in_place_t, std::string_view const parser_name, std::array<entry_type, N>&& entries)
        : parser_name_(parser_name)
        , entries_(std::move(entries))
    {
        std::sort(entries_.begin(), entries_.end(), [](entry_type const& a, entry_type const& b) { return a.key < b.key; });
        if (entries_.front().key.empty()) {
            iris::throwf<std::invalid_argument>("symbols parser cannot have an empty key");
        }
        auto const duplicate = std::adjacent_find(
            entries_.begin(), entries_.end(),
            [](entry_type const& a, entry_type const& b) { return a.key == b.key; }
        );
        if (duplicate != entries_.end()) {
            iris::throwf<std::invalid_argument>("symbols parser cannot have duplicate keys");
        }
    }

    template<bool IsNoCase, std::forward_iterator It, std::sentinel_for<It> Se>
    [[nodiscard]] constexpr entry_type const* longest_match(It& first, Se const& last) const noexcept
    {
        static_assert(!CharIncompatibleWith<std::iter_value_t<It>, char_type>, "Mixing incompatible char types is not allowed");

        It match_end = first;
        entry_type const* entry = symbols_parser::match_keys<IsNoCase>(entries_.data(), entries_.data() + N, 0, first, last, match_end);
        if (entry) first = match_end;
        return entry;
    }

    // [lo, hi) is a non-empty range of the sorted keys that share their first
    // `depth` characters with the input before `it`
    template<bool IsNoCase, std::forward_iterator It, std::sentinel_for<It> Se>
    [[nodiscard]] static constexpr entry_type const*
    match_keys(entry_type const* lo, entry_type const* hi, std::size_t const depth, It it, Se const& last, It& match_end) noexcept
    {
        entry_type const* found = nullptr;
        if (lo->key.size() == depth) {
            found = lo;
            match_end = it;
            ++lo;
        }
        if (lo == hi || it == last) return found;

        auto const ch = static_cast<char_type>(*it);
        ++it;

        auto const try_char = [&](char_type const key_ch) {
            entry_type const* sub_lo = std::lower_bound(lo, hi, key_ch, [depth](entry_type const& entry, char_type const c) {
                return std::char_traits<char_type>::lt(entry.key[depth], c);
            });
            entry_type const* sub_hi = std::upper_bound(sub_lo, hi, key_ch, [depth](char_type const c, entry_type const& entry) {
                return std::char_traits<char_type>::lt(c, entry.key[depth]);
            });
            if (sub_lo == sub_hi) return;

            It sub_end = it;
            entry_type const* sub = symbols_parser::match_keys<IsNoCase>(sub_lo, sub_hi, depth + 1, it, last, sub_end);
            if (sub && (!found || sub->key.size() > found->key.size())) {
                found = sub;
                match_end = sub_end;
            }
        };

        if constexpr (IsNoCase) {
            using classify_type = Encoding::classify_type;
            auto const lower = static_cast<char_type>(Encoding::tolower(static_cast<classify_type>(ch)));
            auto const upper = static_cast<char_type>(Encoding::toupper(static_cast<classify_type>(ch)));
            bool const can_be_lower = Encoding::islower(static_cast<classify_type>(lower));
            bool const can_be_upper = !Encoding::islower(static_cast<classify_type>(upper));

            if (ch == upper) {
                if (can_be_upper) try_char(upper);
                if (can_be_lower) try_char(lower);

            } else {
                if (can_be_lower) try_char(lower);
                if (can_be_upper) try_char(upper);
            }

        } else {
            try_char(ch);
        }
        return found;
    }

    std::string_view parser_name_;
    std::array<entry_type, N> entries_;
};

template<class T = unused_type, std::size_t N>
[[nodiscard]] constexpr symbols_parser<char_encoding::standard, T, N>
symbols(std::string_view parser_name, symbols_entry<char, T> const (&entries)[N])
{
    return {parser_name, entries};
}

template<std::size_t N>
[[nodiscard]] constexpr symbols_parser<char_encoding::standard, unused_type, N>
symbols(std::string_view parser_name, std::string_view const (&keys)[N])
{
    return {parser_name, keys};
}

#ifdef IRIS_X4_UNICODE
template<class T = unused_type, std::size_t N>
[[nodiscard]] constexpr symbols_parser<char_encoding::unicode, T, N>
symbols(std::string_view parser_name, symbols_entry<char32_t, T> const (&entries)[N])
{
    return {parser_name, entries};
}

template<std::size_t N>
[[nodiscard]] constexpr symbols_parser<char_encoding::unicode, unused_type, N>
symbols(std::string_view parser_name, std::u32string_view const (&keys)[N])
{
    return {parser_name, keys};
}
#endif


template<std::size_t N, std::ranges::forward_range R>
    requires
        std::ranges::sized_range<R> &&
        is_ttp_specialization_of_v<std::ranges::range_value_t<R>, symbols_entry>
[[nodiscard]] constexpr symbols_parser<
    char_encoding_for<typename std::ranges::range_value_t<R>::char_type>,
    typename std::ranges::range_value_t<R>::value_type,
    N
>
symbols(std::string_view parser_name, R&& r)
{
    if (std::ranges::size(r) != N) {
        iris::throwf<std::length_error>("input range's size does not match N");
    }

    symbols_entry<typename std::ranges::range_value_t<R>::char_type, typename std::ranges::range_value_t<R>::value_type>
    entries[N];

    std::ranges::copy(std::forward<R>(r), entries);
    return {parser_name, entries};
}


namespace parsers {
using x4::symbols;
} // parsers

} // iris::x4

#endif
