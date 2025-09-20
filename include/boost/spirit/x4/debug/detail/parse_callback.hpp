#ifndef BOOST_SPIRIT_X4_X4_DEBUG_DETAIL_PARSE_CALLBACK_HPP
#define BOOST_SPIRIT_X4_X4_DEBUG_DETAIL_PARSE_CALLBACK_HPP

/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/spirit/x4/core/expectation.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4::detail {

template <typename ID, typename It, typename Se, typename Context>
concept HasImmutableOnErrorOverload =
    std::forward_iterator<It> &&
    std::sentinel_for<Se, It> &&
    requires(ID& id) { // Note: `ID` should be non-const
        id.on_error(
            std::declval<It const&>(),
            std::declval<Se const&>(),
            std::declval<expectation_failure<It> const&>(),
            std::declval<Context const&>()
        );
    };

template <typename ID, typename It, typename Se, typename Context>
concept HasMutableOnErrorOverload =
    std::forward_iterator<It> &&
    std::sentinel_for<Se, It> &&
    requires(ID& id) { // Note: `ID` should be non-const
        id.on_error(
            std::declval<It&>(),
            std::declval<Se&>(),
            std::declval<expectation_failure<It> const&>(),
            std::declval<Context const&>()
        );
    };

template <typename ID, std::forward_iterator It, std::sentinel_for<It> Se, typename Context>
struct has_on_error : std::false_type {};

template <typename ID, std::forward_iterator It, std::sentinel_for<It> Se, typename Context>
    requires HasImmutableOnErrorOverload<ID, It, Se, Context>
struct has_on_error<ID, It, Se, Context> : std::true_type
{
    // We intentionally make this hard error to prevent error-prone definition
    static_assert(
        std::is_void_v<
            decltype(std::declval<ID&>().on_error(
                std::declval<It const&>(),
                std::declval<Se const&>(),
                std::declval<expectation_failure<It> const&>(),
                std::declval<Context const&>()
            ))
        >,
        "The return type of `on_error` should be `void`."
    );
};

template <typename ID, std::forward_iterator It, std::sentinel_for<It> Se, typename Context>
    requires
        (!HasImmutableOnErrorOverload<ID, It, Se, Context>) &&
        HasMutableOnErrorOverload<ID, It, Se, Context>
struct has_on_error<ID, It, Se, Context> : std::false_type
{
    // Historically, Spirit has passed mutable lvalue references of the iterators *as-is*
    // to the `on_success`/`on_error` handlers. This behavior was simply a mistake, because:
    //   (1) `on_success`/`on_error` mechanism was designed to be grammar-agnostic, and
    //   (2) it does not make sense to modify the grammar-specific iterator on the
    //       grammar-agnostic callback.
    //
    // Furthermore, any modification to X4's internal iterator variables may invoke
    // undefined behavior, since we never provide any kind of guarantee on how the
    // intermediate iterator variables are processed in X4's implementation details.
    static_assert(
        false,
        "The `on_error` callback should only accept const reference or passed-by-value iterators."
    );
};

template <typename ID, typename It, typename Se, typename Attribute, typename Context>
concept HasImmutableOnSuccessOverload =
    std::forward_iterator<It> &&
    std::sentinel_for<Se, It> &&
    requires(ID& id) { // Note: `ID` should be non-const
        id.on_success(
            std::declval<It const&>(),
            std::declval<Se const&>(),
            std::declval<Attribute&>(),
            std::declval<Context const&>()
        );
    };

template <typename ID, typename It, typename Se, typename Attribute, typename Context>
concept HasMutableOnSuccessOverload =
    std::forward_iterator<It> &&
    std::sentinel_for<Se, It> &&
    requires(ID& id) { // Note: `ID` should be non-const
        id.on_success(
            std::declval<It&>(),
            std::declval<Se&>(),
            std::declval<Attribute&>(),
            std::declval<Context const&>()
        );
    };

template <typename ID, std::forward_iterator It, std::sentinel_for<It> Se, typename Attribute, typename Context>
struct has_on_success : std::false_type {};

template <typename ID, std::forward_iterator It, std::sentinel_for<It> Se, typename Attribute, typename Context>
    requires HasImmutableOnSuccessOverload<ID, It, Se, Attribute, Context>
struct has_on_success<ID, It, Se, Attribute, Context> : std::true_type
{
    // We intentionally make this hard error to prevent error-prone definition
    static_assert(
        std::is_void_v<
            decltype(std::declval<ID&>().on_success(
                std::declval<It const&>(),
                std::declval<Se const&>(),
                std::declval<Attribute&>(),
                std::declval<Context const&>()
            ))
        >,
        "The return type of `on_success` should be `void`."
    );
};

template <typename ID, std::forward_iterator It, std::sentinel_for<It> Se, typename Attribute, typename Context>
    requires
        (!HasImmutableOnSuccessOverload<ID, It, Se, Attribute, Context>) &&
        HasMutableOnSuccessOverload<ID, It, Se, Attribute, Context>
struct has_on_success<ID, It, Se, Attribute, Context> : std::false_type
{
    // For details, see the comments on `has_on_error`.
    static_assert(
        false,
        "The `on_success` callback should only accept const reference or passed-by-value iterators."
    );
};

} // boost::spirit::x4::detail

#endif
