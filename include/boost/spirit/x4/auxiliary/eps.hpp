/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_SPIRIT_X4_EPS_MARCH_23_2007_0454PM
#define BOOST_SPIRIT_X4_EPS_MARCH_23_2007_0454PM

#include <boost/spirit/x4/core/skip_over.hpp>
#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/unused.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4
{
    struct rule_context_tag;

    struct semantic_predicate : parser<semantic_predicate>
    {
        using attribute_type = unused_type;

        static constexpr bool has_attribute = false;

        constexpr explicit semantic_predicate(bool predicate) noexcept
            : predicate_(predicate)
        {}

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename RContext, typename Attribute>
        [[nodiscard]] constexpr bool
        parse(It& first, Se const& last, Context const& context, RContext const&, Attribute&) const
            noexcept(noexcept(x4::skip_over(first, last, context)))
        {
            x4::skip_over(first, last, context);
            return predicate_;
        }

    private:
        bool predicate_;
    };

    template <typename F>
    struct lazy_semantic_predicate : parser<lazy_semantic_predicate<F>>
    {
        using attribute_type = unused_type;

        static constexpr bool has_attribute = false;

        template <typename F_>
            requires
                (!std::is_same_v<std::remove_cvref_t<F_>, lazy_semantic_predicate>) &&
                std::is_constructible_v<F, F_>
        constexpr explicit lazy_semantic_predicate(F_&& f)
            noexcept(std::is_nothrow_constructible_v<F, F_>)
            : f_(std::forward<F_>(f))
        {}

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename RContext, typename Attribute>
        [[nodiscard]] constexpr bool
        parse(It& first, Se const& last, Context const& context, RContext const&, Attribute& /* attr */) const
        {
            static_assert(std::is_invocable_r_v<bool, F const&, decltype(x4::get<rule_context_tag>(context))>);

            x4::skip_over(first, last, context);
            return f_(x4::get<rule_context_tag>(context));
        }

    private:
        F f_;
    };

    struct eps_parser : parser<eps_parser>
    {
        using attribute_type = unused_type;

        static constexpr bool has_attribute = false;

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename RContext, typename Attribute>
        [[nodiscard]] constexpr bool
        parse(It& first, Se const& last, Context const& context, RContext const&, Attribute&) const
            noexcept(noexcept(x4::skip_over(first, last, context)))
        {
            x4::skip_over(first, last, context);
            return true;
        }

        [[nodiscard]] static constexpr semantic_predicate
        operator()(bool predicate) noexcept
        {
            return semantic_predicate{ predicate };
        }

        template <typename F>
        [[nodiscard]] static constexpr lazy_semantic_predicate<std::remove_cvref_t<F>>
        operator()(F&& f)
            noexcept(std::is_nothrow_constructible_v<lazy_semantic_predicate<std::remove_cvref_t<F>>, F>)
        {
            return lazy_semantic_predicate<std::remove_cvref_t<F>>{ std::forward<F>(f) };
        }
    };

    inline namespace cpos
    {
        inline constexpr eps_parser eps{};

    } // cpos

} // boost::spirit::x4

#endif
