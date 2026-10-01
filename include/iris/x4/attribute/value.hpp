#ifndef IRIS_ZZ_X4_ATTRIBUTE_VALUE_HPP
#define IRIS_ZZ_X4_ATTRIBUTE_VALUE_HPP

/*=============================================================================
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/core/traits/char_traits.hpp>

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/move_to.hpp>

#include <string>
#include <string_view>
#include <iterator>
#include <type_traits>
#include <utility>
#include <initializer_list>

namespace iris::x4 {

// `fixed_value(...)`
template<class T, class HeldValueT = T>
struct fixed_value_parser : parser<fixed_value_parser<T, HeldValueT>>
{
    static_assert(X4Attribute<T>);
    static_assert(!X4UnusedAttribute<T>, "fixed_value_parser with `unused_type` is meaningless");

    // `HeldValueT` is almost always equal to `T`.
    //
    // The most notable situation where they differ is when `fixed_value_parser` is initialized
    // by `char const (&)[N]`. In such case, `fixed_value_parser` must hold the value by
    // `std::string_view`, instead of `std::string`, to be constexpr.
    using attribute_type = T;
    using held_value_type = HeldValueT;

    constexpr fixed_value_parser() = default;

    template<class U, class... Rest>
        requires
            // This exclusion is mandatory because `HeldValueT` can be a weakly constrained
            // type such as `std::any`
            (!std::is_base_of_v<fixed_value_parser, std::remove_cvref_t<U>>) &&
            std::is_constructible_v<HeldValueT, U, Rest...>
    constexpr explicit fixed_value_parser(U&& arg, Rest&&... rest)
        noexcept(std::is_nothrow_constructible_v<HeldValueT, U, Rest...>)
        : held_value_(std::forward<U>(arg), std::forward<Rest>(rest)...)
    {}

    template<class U, class... Args>
        requires std::is_constructible_v<HeldValueT, std::initializer_list<U>&, Args...>
    constexpr explicit fixed_value_parser(std::initializer_list<U> il, Args&&... args)
        noexcept(std::is_nothrow_constructible_v<HeldValueT, std::initializer_list<U>&, Args...>)
        : held_value_(il, std::forward<Args>(args)...)
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It&, Se const&, Context const&, Attr& exposed_attr) const
        noexcept(noexcept(x4::move_to(std::declval<HeldValueT const&>(), exposed_attr)))
    {
        // Always copy (need reuse in repetitive invocations)
        x4::move_to(this->held_value_, exposed_attr);
        return true;
    }

private:
    IRIS_NO_UNIQUE_ADDRESS HeldValueT held_value_{};
};

// aka `default_value<T>`
template<class T>
struct fixed_value_parser<T, void> : parser<fixed_value_parser<T, void>>
{
    static_assert(X4Attribute<T>);
    static_assert(!X4UnusedAttribute<T>, "fixed_value_parser with `unused_type` is meaningless");

    using attribute_type = T;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] static constexpr bool
    parse(It&, Se const&, Context const&, UnusedAttr const&) noexcept
    {
        return true;
    }

    // `T{}` by the ordinary write rules: an empty `std::vector<int>` adds no element to a `std::vector<int>`,
    // but one to a `std::vector<std::vector<int>>`
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    [[nodiscard]] static constexpr bool
    parse(It&, Se const&, Context const&, Attr& exposed_attr)
    {
        x4::move_to(T{}, exposed_attr);
        return true;
    }
};

namespace detail {

template<CharArray R>
using string_array_attr_parser_t = fixed_value_parser<
    std::basic_string<std::remove_extent_t<std::remove_cvref_t<R>>>,
    std::basic_string_view<std::remove_extent_t<std::remove_cvref_t<R>>>
>;

} // detail

template<CharArray R>
fixed_value_parser(R const&) -> fixed_value_parser<
    std::basic_string<std::remove_extent_t<std::remove_cvref_t<R>>>,
    std::basic_string_view<std::remove_extent_t<std::remove_cvref_t<R>>>
>;

template<class T, class HeldValueT>
struct get_info<fixed_value_parser<T, HeldValueT>>
{
    using result_type = std::string;
    [[nodiscard]] constexpr std::string
    operator()(fixed_value_parser<T, HeldValueT> const&) const
    {
        if constexpr (std::is_void_v<HeldValueT>) {
            return "default_value<T>";
        } else {
            return "fixed_value<T>(...)";
        }
    }
};

namespace detail {

struct fixed_value_gen
{
    template<class T>
    [[nodiscard]] static constexpr fixed_value_parser<std::remove_cvref_t<T>>
    operator()(T&& value)
        noexcept(std::is_nothrow_constructible_v<fixed_value_parser<std::remove_cvref_t<T>>, T>)
    {
        return fixed_value_parser<std::remove_cvref_t<T>>{std::forward<T>(value)};
    }

    [[nodiscard]] static constexpr fixed_value_parser<std::string, std::string_view>
    operator()(std::string_view value)
        noexcept(std::is_nothrow_constructible_v<fixed_value_parser<std::string, std::string_view>, std::string_view>)
    {
        return fixed_value_parser<std::string, std::string_view>{value};
    }

    [[nodiscard]] static constexpr fixed_value_parser<std::u32string, std::u32string_view>
    operator()(std::u32string_view value)
        noexcept(std::is_nothrow_constructible_v<fixed_value_parser<std::u32string, std::u32string_view>, std::u32string_view>)
    {
        return fixed_value_parser<std::u32string, std::u32string_view>{value};
    }

    template<CharArray CharArrayT>
    [[nodiscard]] static constexpr string_array_attr_parser_t<CharArrayT>
    operator()(CharArrayT&& char_array)
        noexcept(std::is_nothrow_constructible_v<string_array_attr_parser_t<CharArrayT>, CharArrayT>)
    {
        return string_array_attr_parser_t<CharArrayT>{std::forward<CharArrayT>(char_array)};
    }
};

} // detail

namespace parsers {

// An always-succeeding parser that has the `attribute_type` equivalent
// to the given parameter. Copies the held instance on each invocation.
[[maybe_unused]] inline constexpr detail::fixed_value_gen fixed_value{};

// An always-succeeding parser which constructs `T{}` on each invocation and writes it by the ordinary
// write rules. Unlike `fixed_value(T{})` it holds no `T`, so it can be a `constexpr` variable even where
// a `T` cannot: given `struct Defaults { std::vector<int> values{1, 2, 3}; };`,
// `constexpr auto p = default_value<Defaults>;` is well-formed, `fixed_value(Defaults{})` is not.
template<class T>
[[maybe_unused]] inline constexpr fixed_value_parser<T, void> default_value{};

} // parsers

using parsers::fixed_value;
using parsers::default_value;

} // iris::x4

#endif
