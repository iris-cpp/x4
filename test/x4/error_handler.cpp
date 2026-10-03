/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>

#include <iris/x4/primitive/eps.hpp>
#include <iris/x4/directive/expect.hpp>
#include <iris/x4/directive/with.hpp>

#include <iris/x4/debug/annotate.hpp>
#include <iris/x4/debug/error_handler.hpp>

#include <iterator>
#include <string>
#include <sstream>

template<std::forward_iterator It>
struct test_error_handler
{
    template<std::sentinel_for<It> Se>
    test_error_handler(It /*script_first*/, Se /*script_last*/, std::ostream& os)
        : os_(os)
    {}

    template<class Context, x4::X4Attribute Attr>
    void on_success(It const& /*first*/, std::sentinel_for<It> auto const& /*last*/, Context const& /*ctx*/, Attr& /*attr*/)
    {
        os_ << "on_success";
    }

    template<class Context>
    void on_expectation_failure(It const& /*first*/, std::sentinel_for<It> auto const& /*last*/, Context const& /*ctx*/, x4::expectation_failure<It> const& /*failure*/)
    {
        os_ << "on_expectation_failure";
    }

    template<class Context, x4::X4Attribute Attr>
    void on_trace(
        It const& /*first*/,
        std::sentinel_for<It> auto const& /*last*/,
        Context const& /*ctx*/,
        Attr const& /*attr*/,
        std::string_view const /*rule_name*/,
        x4::tracer_state const state
    )
    {
        if (state == x4::tracer_state::pre_parse) {
            os_ << "trace";
        }
    }

private:
    std::ostream& os_;
};

using enum x4::annotated_rule_kind;

using x4::eps;
using x4::expect;

IRIS_X4_DECLARE(eps_success_rule, unused_type, x4::annotated_rule<annotate_success>);
constexpr auto eps_success_rule_def = eps;
IRIS_X4_DEFINE(eps_success_rule);

IRIS_X4_DECLARE(expect_eps_false_rule, unused_type, x4::annotated_rule<annotate_expectation_failure>);
constexpr auto expect_eps_false_rule_def = expect[eps(false)];
IRIS_X4_DEFINE(expect_eps_false_rule);

IRIS_X4_DECLARE(eps_trace_rule, unused_type, x4::annotated_rule<annotate_trace>);
constexpr auto eps_trace_rule_def = eps;
IRIS_X4_DEFINE(eps_trace_rule);

TEST_CASE("error_handler")
{
    {
        using Context = x4::context<
            x4::contexts::error_handler,
            test_error_handler<char const*>,
            x4::context<x4::contexts::expectation_failure, x4::expectation_failure<char const*>>
        >;

        STATIC_CHECK(x4::has_on_success<x4::annotated_rule<annotate_success>, char const*, char const*, Context, unused_type>::value);
        STATIC_CHECK(!x4::has_on_expectation_failure<x4::annotated_rule<annotate_success>, char const*, char const*, Context>::value);
        STATIC_CHECK(!x4::has_on_trace<x4::annotated_rule<annotate_success>, char const*, char const*, Context, unused_type>::value);

        STATIC_CHECK(x4::has_on_success<x4::annotated_rule<annotate_success | annotate_expectation_failure>, char const*, char const*, Context, unused_type>::value);
        STATIC_CHECK(x4::has_on_expectation_failure<x4::annotated_rule<annotate_success | annotate_expectation_failure>, char const*, char const*, Context>::value);
        STATIC_CHECK(!x4::has_on_trace<x4::annotated_rule<annotate_success | annotate_expectation_failure>, char const*, char const*, Context, unused_type>::value);

        STATIC_CHECK(x4::has_on_success<x4::annotated_rule<annotate_success | annotate_expectation_failure | annotate_trace>, char const*, char const*, Context, unused_type>::value);
        STATIC_CHECK(x4::has_on_expectation_failure<x4::annotated_rule<annotate_success | annotate_expectation_failure | annotate_trace>, char const*, char const*, Context>::value);
        STATIC_CHECK(x4::has_on_trace<x4::annotated_rule<annotate_success | annotate_expectation_failure | annotate_trace>, char const*, char const*, Context, unused_type>::value);
    }

    {
        std::string_view const input = "foo";
        auto first = input.begin();
        auto const last = input.end();

        std::ostringstream oss;
        test_error_handler error_handler{first, last, oss};

        (void)parse(first, last, x4::with<x4::contexts::error_handler>(error_handler)[eps_success_rule], unused);
        CHECK(oss.str() == "on_success");
    }
    {
        std::string_view const input = "foo";
        auto first = input.begin();
        auto const last = input.end();

        std::ostringstream oss;
        test_error_handler error_handler{first, last, oss};

        (void)parse(first, last, x4::with<x4::contexts::error_handler>(error_handler)[expect_eps_false_rule], unused);
        CHECK(oss.str() == "on_expectation_failure");
    }
    {
        std::string_view const input = "foo";
        auto first = input.begin();
        auto const last = input.end();

        std::ostringstream oss;
        test_error_handler error_handler{first, last, oss};

        (void)parse(first, last, x4::with<x4::contexts::error_handler>(error_handler)[eps_trace_rule], unused);
        CHECK(oss.str() == "trace");
    }
}
