/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"

#include <boost/spirit/x4/core/context.hpp>

#include <concepts>
#include <memory>
#include <utility>

int main()
{
    using x4::context;
    using x4::owning_context;

    struct tag;
    struct nonexistent_tag;

    {
        int i = 42;
        auto ctx = x4::make_context<tag>(std::as_const(i));
        BOOST_TEST(x4::get<tag>(ctx) == 42);
        static_assert(std::same_as<decltype(x4::get<tag>(ctx)), int const&>);
        static_assert(std::same_as<decltype(x4::get<tag>(std::as_const(ctx))), int const&>);
    }

    // Start with plain `context<tag, int>`
    {
        int i = 42;
        auto ctx = x4::make_context<tag>(i);
        static_assert(std::same_as<decltype(ctx), context<tag, int>>);
        BOOST_TEST(x4::get<tag>(ctx) == 42);
        static_assert(std::same_as<decltype(x4::get<tag>(ctx)), int&>);
        static_assert(std::same_as<decltype(x4::get<tag>(std::as_const(ctx))), int&>);
        BOOST_TEST(std::addressof(ctx.val) == std::addressof(i));

        {
            int j = 999;
            auto&& replaced_ctx = x4::replace_first_context<tag>(ctx, j);
            static_assert(std::same_as<decltype(replaced_ctx), context<tag, int>&&>);
            BOOST_TEST(x4::get<tag>(replaced_ctx) == 999);
            BOOST_TEST(std::addressof(ctx.val) == std::addressof(i));
            BOOST_TEST(std::addressof(replaced_ctx.val) == std::addressof(j));

            {
                auto&& removed_ctx = x4::remove_first_context<nonexistent_tag>(replaced_ctx);
                static_assert(std::same_as<decltype(removed_ctx), context<tag, int> const&>);
                BOOST_TEST(std::addressof(removed_ctx) == std::addressof(replaced_ctx));
            }
            {
                auto&& removed_ctx = x4::remove_first_context<tag>(replaced_ctx);
                static_assert(std::same_as<decltype(removed_ctx), x4::detail::monostate_context&&>);

                {
                    [[maybe_unused]] auto&& removed_removed_ctx = x4::remove_first_context<tag>(removed_ctx);
                    static_assert(std::same_as<decltype(removed_removed_ctx), x4::detail::monostate_context const&>);
                }
                {
                    [[maybe_unused]] auto&& new_ctx = x4::make_context<tag>(i, removed_ctx);
                    static_assert(std::same_as<decltype(new_ctx), context<tag, int>&&>);
                }
                {
                    auto&& new_ctx = x4::replace_first_context<tag>(removed_ctx, i);
                    static_assert(std::same_as<decltype(new_ctx), context<tag, int>&&>);
                    BOOST_TEST(std::addressof(new_ctx.val) == std::addressof(i));
                }
            }
        }
        {
            double d = 3.14;
            auto replaced_ctx = x4::replace_first_context<tag>(ctx, d);
            static_assert(std::same_as<decltype(replaced_ctx), context<tag, double>>);
            BOOST_TEST(x4::get<tag>(replaced_ctx) == 3.14);
            BOOST_TEST(std::addressof(ctx.val) == std::addressof(i));
            BOOST_TEST(std::addressof(replaced_ctx.val) == std::addressof(d));
        }
        {
            double d = 3.14;
            auto replaced_ctx = x4::replace_first_context<nonexistent_tag>(ctx, d);
            static_assert(std::same_as<decltype(replaced_ctx), context<tag, int, owning_context<context<nonexistent_tag, double>>>>);
            BOOST_TEST(x4::get<tag>(replaced_ctx) == 42);
            BOOST_TEST(std::addressof(ctx.val) == std::addressof(i));
            BOOST_TEST(std::addressof(x4::get<nonexistent_tag>(replaced_ctx)) == std::addressof(d));
        }
    }

    // Start with nested; `context<dummy_tag, dummy_t const, context<tag, int>>`
    {
        struct dummy_tag;
        struct dummy_t {};
        constexpr dummy_t dummy;

        auto const dummy_ctx = x4::make_context<dummy_tag>(dummy);
        using dummy_ctx_t = context<dummy_tag, dummy_t const>;
        static_assert(std::same_as<decltype(dummy_ctx), dummy_ctx_t const>);

        int i = 42;
        auto ctx = x4::make_context<tag>(i, dummy_ctx);
        static_assert(std::same_as<decltype(ctx), context<tag, int, dummy_ctx_t>>);
        BOOST_TEST(x4::get<tag>(ctx) == 42);
        static_assert(std::same_as<decltype(x4::get<tag>(ctx)), int&>);
        static_assert(std::same_as<decltype(x4::get<tag>(std::as_const(ctx))), int&>);
        BOOST_TEST(std::addressof(ctx.val) == std::addressof(i));

        {
            int j = 999;
            auto&& replaced_ctx = x4::replace_first_context<tag>(ctx, j);
            static_assert(std::same_as<decltype(replaced_ctx), context<tag, int, dummy_ctx_t>&&>);
            BOOST_TEST(x4::get<tag>(replaced_ctx) == 999);
            BOOST_TEST(std::addressof(ctx.val) == std::addressof(i));
            BOOST_TEST(std::addressof(replaced_ctx.val) == std::addressof(j));

            {
                auto&& removed_ctx = x4::remove_first_context<nonexistent_tag>(replaced_ctx);
                static_assert(std::same_as<decltype(removed_ctx), context<tag, int, dummy_ctx_t> const&>);
                BOOST_TEST(std::addressof(removed_ctx) == std::addressof(replaced_ctx));

                {
                    [[maybe_unused]] auto&& removed_removed_ctx = x4::remove_first_context<tag>(removed_ctx);
                    static_assert(std::same_as<decltype(removed_removed_ctx), dummy_ctx_t const&>);
                }
                {
                    [[maybe_unused]] auto&& new_ctx = x4::make_context<nonexistent_tag>(i, removed_ctx);
                    static_assert(std::same_as<decltype(new_ctx), context<nonexistent_tag, int, context<tag, int, dummy_ctx_t>>&&>);
                }
                {
                    auto&& new_ctx = x4::replace_first_context<tag>(removed_ctx, i);
                    static_assert(std::same_as<decltype(new_ctx), context<tag, int, dummy_ctx_t>&&>);
                    BOOST_TEST(std::addressof(new_ctx.val) == std::addressof(i));
                }
            }
        }
        {
            double d = 3.14;
            auto&& replaced_ctx = x4::replace_first_context<tag>(ctx, d);
            static_assert(std::same_as<decltype(replaced_ctx), context<tag, double, dummy_ctx_t>&&>);
            BOOST_TEST(x4::get<tag>(replaced_ctx) == 3.14);
            BOOST_TEST(std::addressof(ctx.val) == std::addressof(i));
            BOOST_TEST(std::addressof(replaced_ctx.val) == std::addressof(d));
        }
        {
            double d = 3.14;
            auto&& replaced_ctx = x4::replace_first_context<dummy_tag>(ctx, d);
            static_assert(std::same_as<
                decltype(replaced_ctx),
                context<
                    tag, int,
                    owning_context<context<
                        dummy_tag, double
                    >>
                >&&
            >);
            BOOST_TEST(x4::get<tag>(replaced_ctx) == 42);
            BOOST_TEST(std::addressof(ctx.val) == std::addressof(i));
            BOOST_TEST(std::addressof(x4::get<dummy_tag>(replaced_ctx)) == std::addressof(d));
        }
        {
            double d = 3.14;
            auto&& replaced_ctx = x4::replace_first_context<nonexistent_tag>(ctx, d);
            static_assert(std::same_as<
                decltype(replaced_ctx),
                context<
                    tag, int,
                    owning_context<context<
                        dummy_tag, dummy_t const,
                        owning_context<context<
                            nonexistent_tag, double
                        >>
                    >>
                >&&
            >);
            BOOST_TEST(x4::get<tag>(replaced_ctx) == 42);
            BOOST_TEST(std::addressof(ctx.val) == std::addressof(i));
            BOOST_TEST(std::addressof(x4::get<nonexistent_tag>(replaced_ctx)) == std::addressof(d));
        }
    }

    return boost::report_errors();
}
