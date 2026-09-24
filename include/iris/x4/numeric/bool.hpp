#ifndef IRIS_ZZ_X4_NUMERIC_BOOL_HPP
#define IRIS_ZZ_X4_NUMERIC_BOOL_HPP

/*=============================================================================
    Copyright (c) 2009 Hartmut Kaiser
    Copyright (c) 2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/traits/numeric_traits.hpp>
#include <iris/x4/core/traits/char_encoding_traits.hpp>

#include <iris/x4/core/move_to.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/skip_over.hpp>

#include <iris/x4/string/detail/string_parse.hpp>

#include <concepts>
#include <string_view>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class T = bool>
struct bool_policies
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Attribute Attr, class CaseCompare>
    [[nodiscard]] static constexpr bool
    parse_true(It& first, Se const& last, Attr& attr_, CaseCompare const& compare)
        noexcept(noexcept(x4::move_to(T(true), attr_)))
    {
        using token_def = numeric_token<std::iter_value_t<It>>;
        if (detail::string_parse(std::basic_string_view{token_def::true_}, first, last, unused_container, compare)) {
            x4::move_to(T(true), attr_);
            return true;
        }
        return false;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Attribute Attr, class CaseCompare>
    [[nodiscard]] static constexpr bool
    parse_false(It& first, Se const& last, Attr& attr_, CaseCompare const& compare)
        noexcept(noexcept(x4::move_to(T(false), attr_)))
    {
        using token_def = numeric_token<std::iter_value_t<It>>;
        if (detail::string_parse(std::basic_string_view{token_def::false_}, first, last, unused_container, compare)) {
            x4::move_to(T(false), attr_);
            return true;
        }
        return false;
    }
};

template<class T, class Policy = bool_policies<T>>
struct bool_parser : parser<bool_parser<T, Policy>>
{
    static_assert(X4Attribute<T>);
    static_assert(std::default_initializable<T>);

    using attribute_type = T;
    using policy_type = Policy;

    static constexpr bool has_attribute = true;

    // Attribute is `T` or `unused_type`
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, class U>
        requires
            std::same_as<std::remove_const_t<U>, T> ||
            std::same_as<std::remove_const_t<U>, unused_type>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Context const& ctx, U& attr)
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(Policy::parse_true(first, last, attr, x4::get_case_compare<char_encoding_for<std::iter_value_t<It>>>(ctx))) &&
            noexcept(Policy::parse_false(first, last, attr, x4::get_case_compare<char_encoding_for<std::iter_value_t<It>>>(ctx)))
        )
    {
        auto it = first;
        x4::skip_over(it, last, ctx);

        auto const& compare = x4::get_case_compare<char_encoding_for<std::iter_value_t<It>>>(ctx);
        bool const ok = Policy::parse_true(it, last, attr, compare)
            || Policy::parse_false(it, last, attr, compare);

        if (ok) first = it;
        return ok;
    }

    // Attribute is NOT (`T` or `unused_type`)
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attr>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr)
        noexcept(
            std::is_nothrow_default_constructible_v<T> &&
            noexcept(bool_parser::parse(first, last, ctx, std::declval<T&>())) &&
            noexcept(x4::move_to(std::declval<T>(), attr))
        )
    {
        static_assert(X4NonUnusedAttribute<Attr>);

        T attr_{};
        if (bool_parser::parse(first, last, ctx, attr_)) {
            x4::move_to(std::move(attr_), attr);
            return true;
        }
        return false;
    }

    [[nodiscard]] static std::string get_x4_info()
    {
        return "`bool`";
    }
};

template<auto ExpectedValue, class Policy = bool_policies<decltype(ExpectedValue)>>
struct literal_bool_parser : parser<literal_bool_parser<ExpectedValue, Policy>>
{
    using T = decltype(ExpectedValue);
    static_assert(requires { static_cast<bool>(ExpectedValue); });
    static_assert(X4Attribute<T>);
    static_assert(std::default_initializable<T>);

    using attribute_type = T;

    static constexpr bool has_attribute = true;

    // Attribute is `T` or `unused_type`
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, class U>
        requires
            std::same_as<std::remove_const_t<U>, T> ||
            std::same_as<std::remove_const_t<U>, unused_type>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, U& attr) const
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(Policy::parse_true(first, last, attr, x4::get_case_compare<char_encoding_for<std::iter_value_t<It>>>(ctx))) &&
            noexcept(Policy::parse_false(first, last, attr, x4::get_case_compare<char_encoding_for<std::iter_value_t<It>>>(ctx)))
        )
    {
        auto it = first;
        x4::skip_over(it, last, ctx);

        auto const& compare = x4::get_case_compare<char_encoding_for<std::iter_value_t<It>>>(ctx);
        bool const ok = (static_cast<bool>(ExpectedValue) && Policy::parse_true(it, last, attr, compare))
            || (!static_cast<bool>(ExpectedValue) && Policy::parse_false(it, last, attr, compare));

        if (ok) first = it;
        return ok;
    }

    // Attribute is NOT (`T` or `unused_type`)
    // Needs temporary instance of `T` and do conversion
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(
            std::is_nothrow_default_constructible_v<T> &&
            noexcept(literal_bool_parser::parse(first, last, ctx, std::declval<T&>())) &&
            noexcept(x4::move_to(std::declval<T>(), attr))
        )
    {
        static_assert(X4NonUnusedAttribute<Attr>);

        T attr_;
        if (literal_bool_parser::parse(first, last, ctx, attr_)) {
            x4::move_to(std::move(attr_), attr);
            return true;
        }
        return false;
    }

    [[nodiscard]] std::string get_x4_info() const
    {
        return static_cast<bool>(ExpectedValue) ? "`true`" : "`false`";
    }
};

namespace parsers {

[[maybe_unused]] inline constexpr bool_parser<bool> bool_{};
[[maybe_unused]] inline constexpr literal_bool_parser<true> true_{};
[[maybe_unused]] inline constexpr literal_bool_parser<false> false_{};

} // parsers

using parsers::bool_;
using parsers::true_;
using parsers::false_;

} // iris::x4

#endif
