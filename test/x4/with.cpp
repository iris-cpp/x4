/*=============================================================================
    Copyright (c) 2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/directive/with.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/operator/delimited_list.hpp>

#include <concepts>
#include <utility>
#include <type_traits>

// NOLINTBEGIN(readability-container-size-empty)

namespace {

struct my_tag;

using x4::int_;
using x4::with;
using x4::_attr;

template<class T>
constexpr auto value_equals = int_.on_match([](auto&& ctx) {
    auto&& with_val = x4::get<my_tag>(ctx);
    static_assert(std::same_as<decltype(with_val), T>);
    return with_val == _attr(ctx);
});

} // anonymous

TEST_CASE("with")
{
    IRIS_X4_ASSERT_CONSTEXPR_CTORS(with<my_tag>(0)['x']);

    {
        constexpr int i = 0;
        IRIS_X4_ASSERT_CONSTEXPR_CTORS(with<my_tag>(i)['x']);
    }

    // check various value categories
    {
        {
            int i = 42;
            CHECK(parse("42", with<my_tag>(i)[value_equals<int&>]));
        }
        {
            int const i = 42;
            CHECK(parse("42", with<my_tag>(i)[value_equals<int const&>]));
        }
        {
            int i = 42;
            CHECK(parse("42", with<my_tag>(std::move(i))[value_equals<int&>]));
        }
        {
            int const i = 42;
            CHECK(parse("42", with<my_tag>(std::move(i))[value_equals<int const&>]));
        }

        {
            int i = 42;
            auto with_gen = with<my_tag>(i);
            CHECK(parse("42", with_gen[value_equals<int&>]));
        }
        {
            int const i = 42;
            auto with_gen = with<my_tag>(i);
            CHECK(parse("42", with_gen[value_equals<int const&>]));
        }
        {
            int i = 42;
            auto with_gen = with<my_tag>(std::move(i));
            CHECK(parse("42", with_gen[value_equals<int&>]));
        }
        {
            int const i = 42;
            auto with_gen = with<my_tag>(std::move(i));
            CHECK(parse("42", with_gen[value_equals<int const&>]));
        }

        // lvalue `move_only`
        {
            x4_test::move_only mo;
            (void)with<my_tag>(mo)[int_];
        }
        {
            x4_test::move_only mo;
            auto with_gen = with<my_tag>(mo); // passed-by-reference
            (void)with_gen[int_]; // permitted, never copies
            (void)std::move(with_gen)[int_];
        }

        // rvalue `move_only`
        {
            (void)with<my_tag>(x4_test::move_only{})[int_];
        }
        {
            auto with_gen = with<my_tag>(x4_test::move_only{});
            // (void)with_gen[int_]; // requires copy-constructible
            (void)std::move(with_gen)[int_];
        }
    }

    {
        // injecting non-const lvalue into the context
        int val = 0;
        auto const r = int_.on_match([](auto&& ctx){
            x4::get<my_tag>(ctx) += x4::_attr(ctx);
        });
        REQUIRE(parse("123,456", with<my_tag>(val)[r % ',']));
        CHECK(val == 579);
    }
}

// NOLINTEND(readability-container-size-empty)
