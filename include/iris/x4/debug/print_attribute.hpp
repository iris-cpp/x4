#ifndef IRIS_ZZ_X4_DEBUG_PRINT_ATTRIBUTE_HPP
#define IRIS_ZZ_X4_DEBUG_PRINT_ATTRIBUTE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
================================================_==============================*/

#include <iris/x4/core/attribute.hpp>

#include <iris/x4/char_encoding/unicode/classify_category.hpp>

#include <iris/x4/traits/attribute_category.hpp>

#include <iris/unicode/string.hpp>
#include <iris/rvariant/rvariant_io.hpp>
#include <iris/alloy/utility.hpp>

#include <iris/string.hpp>

#include <print>
#include <iterator>
#include <concepts>

#include <cstdint>

namespace iris::x4 {

template<std::forward_iterator It>
[[nodiscard]] int calc_line_number(It const source_first, It const current_pos)
{
    int line = 1;
    char32_t prev_ch = U'\0';

    iris::unicode::code_point_iterator<It> code_point_it{source_first, source_first, current_pos};

    for (; code_point_it.base() != current_pos; ++code_point_it) {
        char32_t const ch = *code_point_it;
        switch (ch) {
        case U'\n':
            if (prev_ch != U'\r') ++line;
            break;
        case U'\r':
            ++line;
            break;
        default:
            break;
        }
        prev_ch = ch;
    }
    return line;
}

template<std::bidirectional_iterator It>
[[nodiscard]] It fetch_line_start(It const source_first, It const current_pos)
{
    if (current_pos == source_first) {
        return current_pos;
    }

    iris::unicode::code_point_iterator<It> code_point_it{current_pos, source_first, current_pos};

    auto last_it = code_point_it--;
    for (;; --code_point_it) {
        switch (*code_point_it) {
        case U'\n':
        case U'\r':
            return last_it.base();
        default:
            break;
        }
        last_it = code_point_it;
        if (code_point_it.base() == source_first) break;
    }
    return code_point_it.base();
}

template<std::forward_iterator It, std::sentinel_for<It> Se>
[[nodiscard]] It fetch_line_last(It const current_pos, Se const source_last)
{
    iris::unicode::code_point_iterator<It> code_point_it{current_pos, current_pos, source_last};

    for (; code_point_it.base() != source_last; ++code_point_it) {
        switch (*code_point_it) {
        case U'\n':
        case U'\r':
            return code_point_it.base();
        default:
            break;
        }
    }
    return code_point_it.base();
}

template<std::forward_iterator It, std::sentinel_for<It> Se>
void skip_whitespace_for_print(It& it, Se const source_last)
{
    iris::unicode::code_point_iterator<It> code_point_it{it, it, source_last};

    for (; code_point_it.base() != source_last; ++code_point_it) {
        switch (*code_point_it) {
        case U'\r':
        case U'\n':
        case U'\t':
        case U' ':
            continue;
        default:
            break;
        }
        break;
    }
    it = code_point_it.base();
}

inline void print_chars(std::ostream& os, char32_t const ch)
{
    // https://en.cppreference.com/w/cpp/utility/format/spec.html#Formatting_escaped_characters_and_strings

    switch (ch) {
    case U'\t': os << "\\t"; return;
    case U'\n': os << "\\n"; return;
    case U'\r': os << "\\r"; return;
    case U'\\': os << "\\"; return;
    case U' ': os << ' '; return;
    // fullwidth CJK space; we don't want this to be printed like `\u{3000}`
    case U'　': os << "　"; return;
    default: break;
    }

    auto const major_cat = unicode::get_major_category(ch);
    if (
        major_cat == unicode::properties::separator || major_cat == unicode::properties::other
        // TODO: handle Grapheme_Extend=Yes
    ) {
        std::print(os, "\\u{{{:x}}}", static_cast<std::int32_t>(ch));
        return;
    }

    iris::unicode::append8(ch, std::ostreambuf_iterator(os));
}

template<std::forward_iterator It, std::sentinel_for<It> Se>
void print_chars(std::ostream& os, It it, Se const se, std::size_t const max_code_points)
{
    iris::unicode::code_point_iterator<It> code_point_it{it, it, se};
    iris::unicode::code_point_iterator<It> const code_point_se{se, it, se};

    for (std::size_t printed_code_points = 0; printed_code_points < max_code_points && code_point_it != code_point_se; ++printed_code_points, ++code_point_it) {
        x4::print_chars(os, *code_point_it);
    }
}

// --------------------------------------

template<X4Attribute Attr>
void print_attribute(std::ostream& os, Attr const& attr_);

namespace detail {

template<class Out>
struct print_tuple_like
{
    print_tuple_like(Out& out)
        : out(out)
        , is_first(true)
    {}

    using result_type = void;

    template<class T>
    void operator()(T const& val) const
    {
        if (is_first) {
            is_first = false;
        } else {
            out << ", ";
        }
        x4::print_attribute(out, val);
    }

    Out& out;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    mutable bool is_first;
};

template<class Out>
struct print_visitor
{
    explicit print_visitor(Out& out)
        : out(out)
    {}

    template<class T>
    void operator()(T const& val) const
    {
        x4::print_attribute(out, val);
    }

    Out& out;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

} // detail

template<class T>
struct print_attribute_debug
{
    static void call(std::ostream& out, unused_type const&)
    {
        out << "unused";
    }

    static void call(std::ostream& out, unused_container_type const&)
    {
        out << "unused_container";
    }

    static void call(std::ostream& out, traits::CategorizedAttr<traits::plain_attr> auto const& val)
    {
        if constexpr (std::formattable<T, char>) {
            std::format_to(std::ostreambuf_iterator{out}, "{}", val);
        } else {
            // TODO: https://github.com/iris-cpp/iris/issues/51
            //static_assert(iris::req::ADL_ostreamable_v<T>);
            out << val;
        }
    }

    // for tuple-likes
    static void call(std::ostream& out, traits::CategorizedAttr<traits::tuple_attr> auto const& val)
    {
        out << '[';
        alloy::for_each(val, detail::print_tuple_like<std::ostream>(out));
        out << ']';
    }

    template<traits::CategorizedAttr<traits::container_attr> T_>
        requires (!std::is_same_v<T_, unused_container_type>)
    static void call(std::ostream& out, T_ const& val)
    {
        if constexpr (iris::StringLike<T_>) {
            out << iris::unicode::transcode_ref<char>(std::basic_string_view{val});

        } else {
            out << '[';
            bool is_first = true;
            auto last = traits::end(val);
            for (auto it = traits::begin(val); it != last; ++it) {
                if (is_first) {
                    is_first = false;
                } else {
                    out << ", ";
                }
                x4::print_attribute(out, *it);
            }
            out << ']';
        }
    }

    // for variant types
    static void call(std::ostream& out, traits::CategorizedAttr<traits::variant_attr> auto const& val)
    {
        iris::visit(detail::print_visitor{out}, val);
    }

    static void call(std::ostream& out, traits::CategorizedAttr<traits::optional_attr> auto const& val)
    {
        if (val) {
            x4::print_attribute(out, *val);
        } else {
            out << "[empty]";
        }
    }
};

template<X4Attribute Attr>
void print_attribute(std::ostream& os, Attr const& attr_)
{
    print_attribute_debug<Attr>::call(os, attr_);
}

} // iris::x4

#endif
