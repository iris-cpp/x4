/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/core/context.hpp>

#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

// Don't place these in anonymous namespace; they make debug harder

template<class T>
struct ref_holder
{
    T& ref;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

struct head_tag;
struct tail_tag;
struct absent_tag;

TEST_CASE("context")
{
    using x4::context;

    STATIC_CHECK(sizeof(context<head_tag, int>) == sizeof(ref_holder<int>));
    STATIC_CHECK(sizeof(context<head_tag, int, context<tail_tag, double>>) == sizeof(ref_holder<int>) * 2);
    STATIC_CHECK(sizeof(context<head_tag, int, context<tail_tag, double> const&>) == sizeof(ref_holder<int>) * 2);

    STATIC_CHECK(std::copy_constructible<context<head_tag, int>>);
    STATIC_CHECK(!std::is_copy_assignable_v<context<head_tag, int>>);

    int i = 42;
    double d = 3.14;
    auto const tail_ctx = x4::make_context<tail_tag>(d);

    {
        auto const ctx = x4::make_context<head_tag>(i);
        STATIC_CHECK(std::same_as<decltype(ctx), context<head_tag, int> const>);
        STATIC_CHECK(std::same_as<decltype(x4::make_context<head_tag>(i, x4::unused)), context<head_tag, int>>);

        STATIC_CHECK(std::same_as<decltype(x4::get<head_tag>(ctx)), int&>);
        STATIC_CHECK(std::same_as<decltype(x4::get<absent_tag>(ctx)), unused_type const&>);
        CHECK(std::addressof(x4::get<head_tag>(ctx)) == std::addressof(i));

    }
    {
        auto const ctx = x4::make_context<head_tag>(std::as_const(i));
        STATIC_CHECK(std::same_as<decltype(x4::get<head_tag>(ctx)), int const&>);
    }
    {
        auto const ctx = x4::make_context<head_tag>(i, tail_ctx);
        STATIC_CHECK(std::same_as<decltype(ctx), context<head_tag, int, context<tail_tag, double> const&> const>);
        CHECK(std::addressof(x4::get<tail_tag>(ctx)) == std::addressof(d));

        STATIC_CHECK(std::same_as<decltype(x4::remove_first_context<head_tag>(ctx)), context<tail_tag, double> const&>);
        STATIC_CHECK(std::same_as<decltype(x4::remove_first_context<tail_tag>(ctx)), context<head_tag, int>>);
        STATIC_CHECK(std::same_as<decltype(x4::remove_first_context<absent_tag>(ctx)), decltype(ctx)&>);
        CHECK(std::addressof(x4::remove_first_context<head_tag>(ctx)) == std::addressof(tail_ctx));
        CHECK(std::addressof(x4::remove_first_context<absent_tag>(ctx)) == std::addressof(ctx));
    }
    {
        auto const ctx = x4::make_context<head_tag>(i, x4::make_context<tail_tag>(d));
        STATIC_CHECK(std::same_as<decltype(ctx), context<head_tag, int, context<tail_tag, double>> const>);
        CHECK(std::addressof(x4::get<tail_tag>(ctx)) == std::addressof(d));

        STATIC_CHECK(std::same_as<decltype(x4::remove_first_context<head_tag>(ctx)), context<tail_tag, double> const&>);
        CHECK(std::addressof(x4::remove_first_context<head_tag>(ctx)) == std::addressof(ctx.next));
    }
    {
        auto const ctx = x4::make_context<head_tag>(i);
        STATIC_CHECK(std::same_as<decltype(x4::remove_first_context<head_tag>(ctx)), unused_type>);
        STATIC_CHECK(std::same_as<decltype(x4::remove_first_context<head_tag>(x4::unused)), unused_type const&>);
    }
}

TEST_CASE("replace_first_or_prepend_context")
{
    using x4::context;

    int i = 42;
    double d = 3.14;
    char c = 'x';
    auto const ctx = x4::make_context<head_tag>(i, x4::make_context<tail_tag>(d));

    {
        auto&& replaced = x4::replace_first_or_prepend_context<head_tag>(ctx, c);
        STATIC_CHECK(std::same_as<decltype(replaced), context<head_tag, char, context<tail_tag, double> const&>&&>);
        CHECK(std::addressof(x4::get<head_tag>(replaced)) == std::addressof(c));
        CHECK(std::addressof(x4::get<tail_tag>(replaced)) == std::addressof(d));
    }
    {
        auto&& replaced = x4::replace_first_or_prepend_context<tail_tag>(ctx, c);
        STATIC_CHECK(std::same_as<decltype(replaced), context<head_tag, int, context<tail_tag, char>>&&>);
        CHECK(std::addressof(x4::get<head_tag>(replaced)) == std::addressof(i));
        CHECK(std::addressof(x4::get<tail_tag>(replaced)) == std::addressof(c));
    }
    {
        auto&& replaced = x4::replace_first_or_prepend_context<absent_tag>(ctx, c);
        STATIC_CHECK(std::same_as<decltype(replaced), context<absent_tag, char, context<head_tag, int, context<tail_tag, double>> const&>&&>);
        CHECK(std::addressof(x4::get<head_tag>(replaced)) == std::addressof(i));
        CHECK(std::addressof(x4::get<absent_tag>(replaced)) == std::addressof(c));
    }
    {
        auto&& replaced = x4::replace_first_or_prepend_context<absent_tag>(x4::unused, c);
        STATIC_CHECK(std::same_as<decltype(replaced), context<absent_tag, char>&&>);
        CHECK(std::addressof(x4::get<absent_tag>(replaced)) == std::addressof(c));
    }
}

TEST_CASE("replace_first_or_append_context")
{
    using x4::context;

    int i = 42;
    double d = 3.14;
    char c = 'x';
    auto const ctx = x4::make_context<head_tag>(i, x4::make_context<tail_tag>(d));

    {
        auto&& replaced = x4::replace_first_or_append_context<head_tag>(ctx, c);
        STATIC_CHECK(std::same_as<decltype(replaced), context<head_tag, char, context<tail_tag, double> const&>&&>);
        CHECK(std::addressof(x4::get<head_tag>(replaced)) == std::addressof(c));
        CHECK(std::addressof(x4::get<tail_tag>(replaced)) == std::addressof(d));
    }
    {
        auto&& replaced = x4::replace_first_or_append_context<tail_tag>(ctx, c);
        STATIC_CHECK(std::same_as<decltype(replaced), context<head_tag, int, context<tail_tag, char>>&&>);
        CHECK(std::addressof(x4::get<head_tag>(replaced)) == std::addressof(i));
        CHECK(std::addressof(x4::get<tail_tag>(replaced)) == std::addressof(c));
    }
    {
        auto&& replaced = x4::replace_first_or_append_context<absent_tag>(ctx, c);
        STATIC_CHECK(std::same_as<decltype(replaced), context<head_tag, int, context<tail_tag, double, context<absent_tag, char>>>&&>);
        CHECK(std::addressof(x4::get<head_tag>(replaced)) == std::addressof(i));
        CHECK(std::addressof(x4::get<absent_tag>(replaced)) == std::addressof(c));
    }
    {
        auto&& replaced = x4::replace_first_or_append_context<absent_tag>(x4::unused, c);
        STATIC_CHECK(std::same_as<decltype(replaced), context<absent_tag, char>&&>);
        CHECK(std::addressof(x4::get<absent_tag>(replaced)) == std::addressof(c));
    }
}
