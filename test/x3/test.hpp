/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2019 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_TEST_FEBRUARY_01_2007_0605PM
#define BOOST_SPIRIT_TEST_FEBRUARY_01_2007_0605PM

// TODO: remove this
#define BOOST_SPIRIT_X3_THROW_EXPECTATION_FAILURE 0

#include <boost/spirit/home/x3/core/parse.hpp>
#include <boost/spirit/home/x3/core/parser.hpp>
#include <boost/spirit/home/x3/support/traits/move_to.hpp>

#include <boost/core/lightweight_test.hpp>

#include <iterator>
#include <optional>
#include <string_view>
#include <iostream>
#include <type_traits>
#include <concepts>
#include <utility>
#include <print>

namespace x3 = boost::spirit::x3;

namespace spirit_test
{
    namespace detail
    {
        // Provide `x3::unused` default arg fallback
        struct parse_overloads : x3::detail::parse_fn_main
        {
            using x3::detail::parse_fn_main::operator();

            // It/Se + Parser
            template <std::forward_iterator It, std::sentinel_for<It> Se, x3::X3Parser<It, Se> Parser>
            static constexpr x3::parse_result<It, Se>
            operator()(It first, Se last, Parser&& p)
            {
                return x3::parse(first, last, std::forward<Parser>(p), x3::unused);
            }

            // parse_result + It/Se + Parser
            template <std::forward_iterator It, std::sentinel_for<It> Se, x3::X3Parser<It, Se> Parser>
            static constexpr void
            operator()(x3::parse_result<It, Se>& res, It first, Se last, Parser&& p)
            {
                return x3::parse(res, first, last, std::forward<Parser>(p), x3::unused);
            }

            // R + Parser
            template <std::ranges::forward_range R, x3::detail::X3RangeParseParser<R> Parser>
            static constexpr x3::parse_result_for<R>
            operator()(R&& r, Parser&& p)
            {
                return x3::parse(std::forward<R>(r), std::forward<Parser>(p), x3::unused);
            }

            // parse_result + R + Parser
            template <std::ranges::forward_range R, x3::detail::X3RangeParseParser<R> Parser>
            static constexpr void
            operator()(x3::parse_result_for<R>& res, R&& r, Parser&& p)
            {
                return x3::parse(res, std::forward<R>(r), std::forward<Parser>(p), x3::unused);
            }

            // It/Se + Parser + Skipper
            template <std::forward_iterator It, std::sentinel_for<It> Se, x3::X3Parser<It, Se> Parser, x3::X3ExplicitParser<It, Se> Skipper>
            static constexpr x3::parse_result<It, Se>
            operator()(It first, Se last, Parser&& p, Skipper&& s, x3::root_skipper_flag flag = x3::root_skipper_flag::do_post_skip)
            {
                return x3::parse(first, last, std::forward<Parser>(p), x3::unused, std::forward<Skipper>(s), flag);
            }

            // parse_result + It/Se + Parser + Skipper
            template <std::forward_iterator It, std::sentinel_for<It> Se, x3::X3Parser<It, Se> Parser, x3::X3ExplicitParser<It, Se> Skipper>
            static constexpr void
            operator()(x3::parse_result<It, Se>& res, It first, Se last, Parser&& p, Skipper&& s, x3::root_skipper_flag flag = x3::root_skipper_flag::do_post_skip)
            {
                return x3::parse(res, first, last, std::forward<Parser>(p), x3::unused, std::forward<Skipper>(s), flag);
            }

            // R + Parser + Skipper
            template <
                std::ranges::forward_range R,
                x3::detail::X3RangeParseParser<R> Parser,
                x3::detail::X3RangeParseSkipper<R> Skipper
            >
            static constexpr x3::parse_result_for<R>
            operator()(R&& r, Parser&& p, Skipper&& s, x3::root_skipper_flag flag = x3::root_skipper_flag::do_post_skip)
            {
                return x3::parse(std::forward<R>(r), std::forward<Parser>(p), x3::unused, std::forward<Skipper>(s), flag);
            }

            // parse_result + R + Parser + Skipper
            template <
                std::ranges::forward_range R,
                x3::detail::X3RangeParseParser<R> Parser,
                x3::detail::X3RangeParseSkipper<R> Skipper
            >
            static constexpr void
            operator()(x3::parse_result_for<R>& res, R&& r, Parser&& p, Skipper&& s, x3::root_skipper_flag flag = x3::root_skipper_flag::do_post_skip)
            {
                return x3::parse(res, std::forward<R>(r), std::forward<Parser>(p), x3::unused, std::forward<Skipper>(s), flag);
            }
        }; // parse_overload

    } // detail

    inline namespace cpos
    {
        inline constexpr detail::parse_overloads parse{};

    } // cpos

    template <typename Parser>
    [[nodiscard]] constexpr bool test_constexpr_copy_move_ctors(Parser const& p)
    {
        [[maybe_unused]] Parser copy_ctor(p);
        [[maybe_unused]] Parser move_ctor(std::move(copy_ctor));
        return true;
    }

    struct move_only
    {
        move_only() = default;
        move_only(move_only&&) = default;
        move_only& operator=(move_only&&) = default;
    };

    template <typename T>
    struct synth_parser : boost::spirit::x3::parser<synth_parser<T>>
    {
        using attribute_type = T;

        static constexpr bool has_attribute = true;
        static constexpr bool handles_container = false;

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename RContext, typename Attribute>
        [[nodiscard]] constexpr bool parse(
            It& iter, Se const& last, Context const&,
            RContext&, Attribute& attr
        ) const
        {
            if (iter != last && *iter == 's') {
                ++iter;
                boost::spirit::x3::traits::move_to(attribute_type{}, attr);
                return true;
            }
            return false;
        }
    };

    template <typename T>
    constexpr synth_parser<T> synth{};

    constexpr synth_parser<move_only> synth_move_only{};

} // spirit_test

using spirit_test::parse;

#define BOOST_SPIRIT_X3_ASSERT_CONSTEXPR_CTORS(...) \
    static_assert(::spirit_test::test_constexpr_copy_move_ctors(__VA_ARGS__))

#endif
