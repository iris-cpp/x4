/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>
#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/debug/annotate.hpp>
#include <iris/x4/debug/error_handler.hpp>
#include <iris/x4/debug/default_error_handler.hpp>

#include <iris/x4/char/char.hpp>
#include <iris/x4/string/string.hpp>

#include <iris/x4/directive/with.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <print>
#include <iterator>
#include <string>
#include <sstream>

namespace x4_test {

using enum x4::annotated_rule_kind;

struct RuleID : x4::annotated_rule<annotate_trace> {};
using Rule = x4::rule<RuleID, unused_type>;
constexpr Rule rule;

IRIS_X4_DECLARE(Rule);

constexpr auto rule_def = x4::string("foo");

IRIS_X4_DEFINE(rule);


TEST_CASE("default_error_handler")
{
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif

    {
        // ' ' is U+1680 'OGHAM SPACE MARK'
        // https://www.fileformat.info/info/unicode/char/1680/
        constexpr std::string_view input = "aこれは𩸽だ　サロゲート\nペア 入りの魚b";

        // full string
        {
            std::ostringstream oss;
            x4::print_chars(oss, input.begin(), input.end(), input.size());
            CHECK(oss.str() == R"(aこれは𩸽だ　サロゲート\nペア\u{1680}入りの魚b)");
        }

        // partial string
        {
            std::ostringstream oss;
            x4::print_chars(oss, input.begin(), input.end(), 0);
            CHECK(oss.str() == R"()");  // NOLINT(readability-container-size-empty)
        }
        {
            std::ostringstream oss;
            x4::print_chars(oss, input.begin(), input.end(), 1);
            CHECK(oss.str() == R"(a)");
        }
        {
            std::ostringstream oss;
            x4::print_chars(oss, input.begin(), input.end(), 2);
            CHECK(oss.str() == R"(aこ)");
        }
        {
            std::ostringstream oss;
            x4::print_chars(oss, input.begin(), input.end(), 4);
            CHECK(oss.str() == R"(aこれは)");
        }
        {
            std::ostringstream oss;
            x4::print_chars(oss, input.begin(), input.end(), 5);
            CHECK(oss.str() == R"(aこれは𩸽)");
        }
        {
            std::ostringstream oss;
            x4::print_chars(oss, input.begin(), input.end(), 6);
            CHECK(oss.str() == R"(aこれは𩸽だ)");
        }
        {
            std::ostringstream oss;
            x4::print_chars(oss, input.begin(), input.end(), 15);
            CHECK(oss.str() == R"(aこれは𩸽だ　サロゲート\nペア)");
        }
        {
            std::ostringstream oss;
            x4::print_chars(oss, input.begin(), input.end(), 16);
            CHECK(oss.str() == R"(aこれは𩸽だ　サロゲート\nペア\u{1680})");
        }
        {
            std::ostringstream oss;
            x4::print_chars(oss, input.begin(), input.end(), 17);
            CHECK(oss.str() == R"(aこれは𩸽だ　サロゲート\nペア\u{1680}入)");
        }

        auto first = input.begin();
        auto const last = input.end();

        std::ostringstream error_out, trace_out;
        x4::default_error_handler error_handler{first, last, &error_out, &trace_out};

        (void)parse(first, last, x4::with<x4::contexts::error_handler>(error_handler)[rule], unused);

        std::println("{}", trace_out.str());
    }
}

} // x4_test
