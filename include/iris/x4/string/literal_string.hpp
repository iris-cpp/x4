#ifndef IRIS_ZZ_X4_STRING_LITERAL_STRING_HPP
#define IRIS_ZZ_X4_STRING_LITERAL_STRING_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/string/detail/string_parse.hpp>

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/unused.hpp>

#include <iris/x4/string/case_compare.hpp>

#include <iris/unicode/string.hpp>

#include <format>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class StoredStringT, class Encoding, X4Attribute Attr = std::basic_string<typename Encoding::char_type>>
struct literal_string : parser<literal_string<StoredStringT, Encoding, Attr>>
{
    static_assert(
        !std::is_pointer_v<std::decay_t<StoredStringT>>,
        "`literal_string` for raw character pointer/array is banned; it has an undetectable risk of holding a dangling pointer."
    );

    using char_type = Encoding::char_type;
    using encoding = Encoding;
    using attribute_type = Attr;

    static_assert(!std::is_same_v<Attr, unused_container_type>, "`literal_string` with `unused_container_type` is not supported");

    static constexpr bool has_attribute = !std::is_same_v<Attr, unused_type>;

    constexpr literal_string() = default;

    template<class... Args>
        requires std::is_constructible_v<StoredStringT, Args...>
    constexpr /*explicit*/ literal_string(Args&&... args)
        noexcept(std::is_nothrow_constructible_v<StoredStringT, Args...>)
        : str_(std::forward<Args>(args)...)
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute ExposedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, ExposedAttr& exposed_attr) const
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(detail::string_parse(this->str_, first, last, x4::assume_container(exposed_attr), x4::get_case_compare<encoding>(ctx)))
        )
    {
        static_assert(std::same_as<std::iter_value_t<It>, char_type>, "Mixing incompatible char types is not allowed");
        auto it = first;
        x4::skip_over(it, last, ctx);
        bool const ok = detail::string_parse(this->str_, it, last, x4::assume_container(exposed_attr), x4::get_case_compare<encoding>(ctx));
        if (ok) first = it;
        return ok;
    }

    [[nodiscard]] std::string get_x4_info() const
    {
        // TODO: escape quotes
        return std::format("\"{}\"", iris::unicode::transcode<char>(std::basic_string_view{this->str_}));
    }

private:
    StoredStringT str_{};
};

} // iris::x4

#endif
