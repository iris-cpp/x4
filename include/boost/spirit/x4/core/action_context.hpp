#ifndef BOOST_SPIRIT_X4_CORE_ACTION_CONTEXT_HPP
#define BOOST_SPIRIT_X4_CORE_ACTION_CONTEXT_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/config.hpp>
#include <boost/spirit/x4/core/context.hpp>

namespace boost::spirit::x4 {

namespace contexts {

// _val
// Refers to the attribute of `x4::rule`.
// This always points to the *innermost* attribute for the (potentially recursive) invocation of `x4::rule`.
// Note: we currently provide no method to obtain the outer attribute, as it is discouraged for maintainability.
//
// This does NOT point to the attribute variable created by `x4::as<T>(...)`.
struct rule_val
{
    static constexpr bool is_unique = true;
};

// _as_val
// Refers to the attribute of `x4::as<T>(...)`.
struct as_val
{
    static constexpr bool is_unique = true;
};

// _attr
struct attr
{
    static constexpr bool is_unique = true;
};

} // contexts

using rule_val_context_tag [[deprecated("Use `x4::contexts::rule_val`")]] = contexts::rule_val;
using attr_context_tag [[deprecated("Use `x4::contexts::attr`")]] = contexts::attr;


namespace detail {

struct _pass_fn
{
    template<class Context>
    static constexpr void
    operator()(Context const&) = delete; // `_pass(ctx)` is obsolete. Just return bool from semantic action.
};

struct _val_fn
{
    template<class Context>
    [[nodiscard]] static constexpr auto&&
    operator()(Context const& ctx BOOST_SPIRIT_LIFETIMEBOUND) noexcept
    {
        return x4::get<contexts::rule_val>(ctx);
    }

    template<class Context>
    static void operator()(Context const&&) = delete; // dangling
};

struct _as_val_fn
{
    template<class Context>
    [[nodiscard]] static constexpr auto&&
    operator()(Context const& ctx BOOST_SPIRIT_LIFETIMEBOUND) noexcept
    {
        return x4::get<contexts::as_val>(ctx);
    }

    template<class Context>
    static void operator()(Context const&&) = delete; // dangling
};

struct _where_fn
{
    template<class Context>
    static constexpr void
    operator()(Context const&) = delete; // `_where` is obsolete. Use `raw[...]` directive.
};

struct _attr_fn
{
    template<class Context>
    [[nodiscard]] static constexpr auto&&
    operator()(Context const& ctx BOOST_SPIRIT_LIFETIMEBOUND) noexcept
    {
        return x4::get<contexts::attr>(ctx);
    }

    template<class Context>
    static void operator()(Context const&&) = delete; // dangling
};

} // detail

inline namespace cpos {

[[maybe_unused]] inline constexpr detail::_pass_fn _pass{};
[[maybe_unused]] inline constexpr detail::_val_fn _val{};
[[maybe_unused]] inline constexpr detail::_as_val_fn _as_val{};
[[maybe_unused]] inline constexpr detail::_where_fn _where{};
[[maybe_unused]] inline constexpr detail::_attr_fn _attr{};

} // cpos

} // boost::spirit::x4

#endif
