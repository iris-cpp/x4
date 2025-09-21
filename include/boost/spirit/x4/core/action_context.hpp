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

namespace boost::spirit::x4
{
    struct parse_pass_context_tag; // _pass

    // _val
    struct rule_val_context_tag
    {
        static constexpr bool is_unique = true;
    };

    struct where_context_tag; // _where
    struct attr_context_tag; // _attr

    namespace detail
    {
        struct _pass_fn
        {
            template <typename Context>
            [[nodiscard]] static constexpr bool&
            operator()(Context const& context BOOST_SPIRIT_LIFETIMEBOUND) noexcept
            {
                return x4::get<parse_pass_context_tag>(context);
            }

            template <typename Context>
            static void operator()(Context const&&) = delete; // dangling
        };

        struct _val_fn
        {
            template <typename Context>
            [[nodiscard]] static constexpr auto&&
            operator()(Context const& context BOOST_SPIRIT_LIFETIMEBOUND) noexcept
            {
                return x4::get<rule_val_context_tag>(context);
            }

            template <typename Context>
            static void operator()(Context const&&) = delete; // dangling
        };

        struct _where_fn
        {
            template <typename Context>
            [[nodiscard]] static constexpr auto&&
            operator()(Context const& context BOOST_SPIRIT_LIFETIMEBOUND) noexcept
            {
                return x4::get<where_context_tag>(context);
            }

            template <typename Context>
            static void operator()(Context const&&) = delete; // dangling
        };

        struct _attr_fn
        {
            template <typename Context>
            [[nodiscard]] static constexpr auto&&
            operator()(Context const& context BOOST_SPIRIT_LIFETIMEBOUND) noexcept
            {
                return x4::get<attr_context_tag>(context);
            }

            template <typename Context>
            static void operator()(Context const&&) = delete; // dangling
        };

    } // detail

    inline namespace cpos
    {
        inline constexpr detail::_pass_fn _pass{};
        inline constexpr detail::_val_fn _val{};
        inline constexpr detail::_where_fn _where{};
        inline constexpr detail::_attr_fn _attr{};

    } // cpos

} // boost::spirit::x4

#endif
