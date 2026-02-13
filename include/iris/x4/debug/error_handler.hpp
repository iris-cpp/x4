#ifndef IRIS_X4_DEBUG_ERROR_HANDLER_HPP
#define IRIS_X4_DEBUG_ERROR_HANDLER_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/attribute.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace contexts {

// Tag used to get our error handler from the context
struct error_handler
{
    static constexpr bool is_unique = true;
};

} // contexts

using error_handler_tag [[deprecated("Use `x4::contexts::error_handler`")]] = contexts::error_handler;


enum class tracer_state : char
{
    pre_parse,
    parse_succeeded,
    parse_failed,
};


// `T` is `RuleID` or some custom error handler type
template<class T, std::forward_iterator It, std::sentinel_for<It> Se, class Context>
struct has_on_expectation_failure : std::false_type
{
    static_assert(
        !requires(T id) {
            id.on_error(
                std::declval<It const&>(),
                std::declval<Se const&>(),
                std::declval<Context const&>(),
                std::declval<expectation_failure<It> const&>()
            );
        },
        "`on_error` is deprecated due to its confusing name; use `on_expectation_failure` instead."
    );
};

// `T` is `RuleID` or some custom error handler type
template<class T, std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    requires requires(T id) {
        id.on_expectation_failure(
            std::declval<It const&>(),
            std::declval<Se const&>(),
            std::declval<Context const&>(),
            std::declval<expectation_failure<It> const&>()
        );
    }
struct has_on_expectation_failure<T, It, Se, Context> : std::true_type
{
    static_assert(
        std::is_void_v<decltype(
            std::declval<T&>().on_expectation_failure(
                std::declval<It const&>(),
                std::declval<Se const&>(),
                std::declval<Context const&>(),
                std::declval<expectation_failure<It> const&>()
            )
        )>,
        "`on_expectation_failure` should not return a value"
    );
};


// `T` is `RuleID` or some custom error handler type
template<class T, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
struct has_on_success : std::false_type {};

// `T` is `RuleID` or some custom error handler type
template<class T, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    requires requires(T id) {
        id.on_success(
            std::declval<It const&>(),
            std::declval<Se const&>(),
            std::declval<Context const&>(),
            std::declval<Attr&>()
        );
    }
struct has_on_success<T, It, Se, Context, Attr> : std::true_type
{};


// `T` is `RuleID` or some custom error handler type
template<class T, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
struct has_on_trace : std::false_type {};

// `T` is `RuleID` or some custom error handler type
template<class T, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    requires requires(T id) {
        id.on_trace(
            std::declval<It const&>(),
            std::declval<Se const&>(),
            std::declval<Context const&>(),
            std::declval<Attr const&>(),
            std::declval<std::string_view>(), // rule_name
            std::declval<tracer_state>()
        );
    }
struct has_on_trace<T, It, Se, Context, Attr> : std::true_type
{};


namespace detail {

template<class RuleID, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
struct [[nodiscard]] scoped_tracer
{
    template<class... Args>
    constexpr explicit scoped_tracer(Args&&...) noexcept
    {}
};

template<class RuleID, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    requires has_on_trace<RuleID, It, Se, Context, Attr>::value
struct [[nodiscard]] scoped_tracer<RuleID, It, Se, Context, Attr>
{
    constexpr scoped_tracer(
        It const& first, Se const& last,
        Context const& ctx,
        Attr const& attr_,
        std::string_view rule_name,
        bool const* parse_ok
    )
        : first_(first)
        , last_(last)
        , ctx_(ctx)
        , attr_(attr_)
        , rule_name_(rule_name)
        , parse_ok_(parse_ok)
    {
        RuleID{}.on_trace(first, last, ctx, attr_, rule_name, tracer_state::pre_parse);
    }

    constexpr ~scoped_tracer()
    {
        RuleID{}.on_trace(
            first_, last_,
            ctx_,
            attr_,
            rule_name_,
            *parse_ok_ ? tracer_state::parse_succeeded : tracer_state::parse_failed
        );
    }

private:
    It const& first_;
    Se const& last_;
    Context const& ctx_;
    Attr const& attr_;
    std::string_view rule_name_;
    bool const* parse_ok_ = nullptr;
};

} // detail

} // iris::x4

#endif
