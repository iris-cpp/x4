#ifndef IRIS_X4_DEBUG_ANNOTATE_HPP
#define IRIS_X4_DEBUG_ANNOTATE_HPP

/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/context.hpp>
#include <iris/x4/core/attribute.hpp>

#include <iris/x4/debug/error_handler.hpp>

#include <iris/enum_bitops.hpp>

#include <iterator>
#include <type_traits>
#include <string_view>

namespace iris::x4 {

namespace contexts {
struct error_handler;
} // contexts

enum struct annotated_rule_kind : unsigned
{
    annotate_success             = 1 << 0, // suitable for any child parsers
    annotate_expectation_failure = 1 << 1, // suitable for root parser
    annotate_trace               = 1 << 2, // suitable for any child parsers (except for the primitive ones)
};

} // iris::x4

namespace iris {

template<>
struct bitops_enabled<x4::annotated_rule_kind> : std::true_type {};

} // iris

namespace iris::x4 {

using iris::operator|;
using iris::operator&;

template<std::forward_iterator It>
struct expectation_failure;

template<annotated_rule_kind anno>
struct annotated_rule
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires
            (iris::contains_single_bit(anno, annotated_rule_kind::annotate_success)) &&
            has_on_success<get_context_plain_t<contexts::error_handler, Context>, It, Se, Context, Attr>::value
    static constexpr void on_success(It const& first, Se const& last, Context const& ctx, Attr& attr)
    {
        auto&& error_handler = x4::get<contexts::error_handler>(ctx);
        error_handler.on_success(first, last, ctx, attr);
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
        requires
            (iris::contains_single_bit(anno, annotated_rule_kind::annotate_expectation_failure)) &&
            has_on_expectation_failure<get_context_plain_t<contexts::error_handler, Context>, It, Se, Context>::value
    static constexpr void on_expectation_failure(It const& first, Se const& last, Context const& ctx, expectation_failure<It> const& failure)
    {
        auto&& error_handler = x4::get<contexts::error_handler>(ctx);
        error_handler.on_expectation_failure(first, last, ctx, failure);
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires
            (iris::contains_single_bit(anno, annotated_rule_kind::annotate_trace)) &&
            has_on_trace<get_context_plain_t<contexts::error_handler, Context>, It, Se, Context, Attr>::value
    static constexpr void on_trace(
        It const& first, Se const& last, Context const& ctx, Attr& attr,
        std::string_view rule_name,
        tracer_state state
    )
    {
        auto&& error_handler = x4::get<contexts::error_handler>(ctx);
        error_handler.on_trace(first, last, ctx, attr, rule_name, state);
    }
};

using annotate_on_success [[deprecated("Use `annotated_rule`")]] =
    annotated_rule<annotated_rule_kind::annotate_success | annotated_rule_kind::annotate_trace>;

} // iris::x4

#endif
