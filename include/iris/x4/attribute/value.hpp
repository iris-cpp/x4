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

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/move_to.hpp>
#include <iris/x4/core/char_traits.hpp>

#include <iris/x4/traits/container_traits.hpp>

#include <string>
#include <string_view>
#include <iterator>
#include <type_traits>
#include <utility>

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

    static_assert(X4Movable<HeldValueT const&, T>);

    using attribute_type = T;
    using held_value_type = HeldValueT;

    template<class U>
        requires
            (!std::is_same_v<std::remove_cvref_t<U>, fixed_value_parser>) &&
            std::is_constructible_v<HeldValueT, U>
    constexpr explicit fixed_value_parser(U&& value)
        noexcept(std::is_nothrow_constructible_v<HeldValueT, U>)
        : held_value_(std::forward<U>(value))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It&, Se const&, Context const&, Attr& attr_) const
        noexcept(noexcept(x4::move_to(std::as_const(held_value_), attr_)))
    {
        // Always copy (need reuse in repetitive invocations)
        x4::move_to(std::as_const(held_value_), attr_);
        return true;
    }

private:
    HeldValueT held_value_;
};

// `reset_value<T>`
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

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute ContainerAttr>
        requires traits::CategorizedAttr<ContainerAttr, traits::container_attr>
    [[nodiscard]] static constexpr bool
    parse(It&, Se const&, Context const&, ContainerAttr& container_attr) noexcept
    {
        traits::clear(container_attr);
        return true;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
        requires (!traits::CategorizedAttr<Attr, traits::container_attr>)
    [[nodiscard]] static constexpr bool
    parse(It&, Se const&, Context const&, Attr& attr_)
        noexcept(noexcept(attr_ = Attr{}))
    {
        attr_ = Attr{};
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
            return "reset_value<T>";
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

// A special `fixed_value` parser that resets the variable and always succeeds.
//
// This can be used for constructing `constexpr` instance of a parser
// even when `T` has dynamically allocated storage.
// For example, normal `fixed_value(std::vector<int>{})` cannot be assigned
// to a `constexpr` instance, but `reset_value<std::vector<int>>` can.
template<class T>
[[maybe_unused]] inline constexpr fixed_value_parser<T, void> reset_value{};

} // parsers

using parsers::fixed_value;
using parsers::reset_value;

} // iris::x4

#endif
