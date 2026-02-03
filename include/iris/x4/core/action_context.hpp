#ifndef BOOST_SPIRIT_X4_CORE_ACTION_CONTEXT_HPP
#define BOOST_SPIRIT_X4_CORE_ACTION_CONTEXT_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp>
#include <iris/x4/core/context.hpp>

namespace boost::spirit::x4 {

namespace contexts {

// _rule_var
// Refers to the attribute of `x4::rule`.
// This always points to the *innermost* attribute for the (potentially recursive) invocation of `x4::rule`.
// Note: we currently provide no method to obtain the outer attribute, as it is discouraged for maintainability.
//
// This does NOT point to the attribute variable created by `x4::as<T>(...)`.
struct rule_var
{
    static constexpr bool is_unique = true;
};

// _as_var
// Refers to the attribute of `x4::as<T>(...)`.
struct as_var
{
    static constexpr bool is_unique = true;
};

// _attr
struct attr
{
    static constexpr bool is_unique = true;
};

} // contexts


namespace detail {

struct _pass_fn
{
    template<class Context>
    static constexpr void
    operator()(Context const&) = delete; // `_pass(ctx)` is obsolete. Just return bool from semantic action.
};

struct _rule_var_fn
{
    template<class Context>
    [[nodiscard]] static constexpr auto&&
    operator()(Context const& ctx BOOST_SPIRIT_LIFETIMEBOUND) noexcept
    {
        return x4::get<contexts::rule_var>(ctx);
    }

    template<class Context>
    static void operator()(Context const&&) = delete; // dangling
};

struct _as_var_fn
{
    template<class Context>
    [[nodiscard]] static constexpr auto&&
    operator()(Context const& ctx BOOST_SPIRIT_LIFETIMEBOUND) noexcept
    {
        return x4::get<contexts::as_var>(ctx);
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

[[maybe_unused]] inline constexpr detail::_rule_var_fn _rule_var{};

[[maybe_unused, deprecated("Use `_rule_var`")]]
inline constexpr detail::_rule_var_fn _val{};

[[maybe_unused]] inline constexpr detail::_as_var_fn _as_var{};
[[maybe_unused]] inline constexpr detail::_where_fn _where{}; // obsolete
[[maybe_unused]] inline constexpr detail::_attr_fn _attr{};

} // cpos

} // boost::spirit::x4

#endif
