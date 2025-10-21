/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"

#include <boost/spirit/x4/directive/with_local.hpp>
#include <boost/spirit/x4/directive/as.hpp>
#include <boost/spirit/x4/numeric/int.hpp>
#include <boost/spirit/x4/auxiliary/eps.hpp>
#include <boost/spirit/x4/operator/sequence.hpp>

#include <concepts>
#include <string_view>
#include <tuple>

#include <cmath>

TEST_CASE("with_local")
{
    using x4::with_local;
    using x4::as;
    using x4::_local_var;
    using x4::_attr;
    using x4::_as_var;
    using x4::int_;
    using x4::eps;

    // `as<int>` + `with_local<int>`
    {
        constexpr auto p = with_local<int>[
            as<int>(
                int_[([](auto&& ctx) {
                    CHECK(_local_var(ctx) == 0);
                    _local_var(ctx) = _attr(ctx) * 100;
                })] >>
                eps[([](auto&& ctx) {
                    _as_var(ctx) = _local_var(ctx);
                })]
            )
        ];
        constexpr std::string_view input = "42";
        auto first = input.begin();
        int i = -1;
        REQUIRE(p.parse(first, input.end(), unused, i));
        CHECK(i == 4200);
    }

    // `with_local<int>` with semantic action assignment
    {
        int i = -1;

        auto const p = with_local<int>[
            int_[([](auto&& ctx) {
                CHECK(_local_var(ctx) == 0);
                _local_var(ctx) = _attr(ctx) * 100;
            })] >>
            eps[([&](auto&& ctx) {
                i = _local_var(ctx);
            })]
        ];
        constexpr std::string_view input = "42";
        auto first = input.begin();
        REQUIRE(p.parse(first, input.end(), unused, unused));
        CHECK(i == 4200);
    }

    // `with_local<int>[with_local<int>[...]]`
    {
        int i = -1;

        auto const p = with_local<int>[
            eps[([](auto&& ctx) {
                CHECK(_local_var(ctx) == 0);
            })] >>

            with_local<int>[
                eps[([](auto&& ctx) {
                    CHECK(_local_var(ctx) == 0);
                })] >>
                int_[([](auto&& ctx) {
                    _local_var(ctx) = _attr(ctx);
                })] >>
                eps[([&](auto&& ctx) {
                    i = _local_var(ctx) * 100;
                })]
            ] >>

            eps[([](auto&& ctx) {
                CHECK(_local_var(ctx) == 0);
            })]
        ];
        constexpr std::string_view input = "42";
        auto first = input.begin();
        REQUIRE(p.parse(first, input.end(), unused, unused));
        CHECK(i == 4200);
    }

    // `with_local<int>[with_local<double>[...]]`
    {
        double d = -1.0;

        auto const p = with_local<int>[
            eps[([](auto&& ctx) {
                static_assert(std::same_as<decltype(_local_var(ctx)), int&>);
                CHECK(_local_var(ctx) == 0);
            })] >>

            with_local<double>[
                eps[([](auto&& ctx) {
                    static_assert(std::same_as<decltype(_local_var(ctx)), double&>);
                    CHECK(_local_var(ctx) == 0.0);
                })] >>
                int_[([](auto&& ctx) {
                    _local_var(ctx) = _attr(ctx);
                })] >>
                eps[([&](auto&& ctx) {
                    d = std::ceil(_local_var(ctx) / 10);
                })]
            ] >>

            eps[([](auto&& ctx) {
                CHECK(_local_var(ctx) == 0);
            })]
        ];
        constexpr std::string_view input = "42";
        auto first = input.begin();
        REQUIRE(p.parse(first, input.end(), unused, unused));
        CHECK(d == 5);
    }

    // `with_local<int, A_ID>[with_local<int, B_ID>[...]]`
    {
        std::tuple<int, int> res{};

        struct A_ID {};
        struct B_ID {};

        auto const p = with_local<int, A_ID>[
            eps[([](auto&& ctx) {
                CHECK(x4::get<A_ID>(ctx) == 0);
            })] >>

            with_local<int, B_ID>[
                eps[([](auto&& ctx) {
                    CHECK(x4::get<B_ID>(ctx) == 0.0);
                })] >>
                int_[([](auto&& ctx) {
                    x4::get<A_ID>(ctx) = _attr(ctx) * 10;
                    x4::get<B_ID>(ctx) = _attr(ctx) * 100;
                })] >>
                eps[([&](auto&& ctx) {
                    res = std::make_tuple(x4::get<A_ID>(ctx), x4::get<B_ID>(ctx));
                })]
            ] >>

            eps[([](auto&& ctx) {
                CHECK(x4::get<A_ID>(ctx) == 420);
            })]
        ];
        constexpr std::string_view input = "42";
        auto first = input.begin();
        REQUIRE(p.parse(first, input.end(), unused, unused));
        CHECK(std::get<0>(res) == 420);
        CHECK(std::get<1>(res) == 4200);
    }
}
