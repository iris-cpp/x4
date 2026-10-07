/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"
#include "iris_x4_test_parse_debug.hpp"

#include <iris/x4/debug/default_error_handler.hpp>
#include <iris/x4/debug/annotate.hpp>
#include <iris/x4/rule.hpp>
#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/directive/with.hpp>

#include <iris/alloy/tuple.hpp>

#include <iris/unicode/string.hpp>
#include <iris/colorize_format.hpp>

#ifdef _WIN32
#include <Windows.h>
#endif

#include <print>
#include <algorithm>
#include <iterator>
#include <string>
#include <string_view>
#include <sstream>
#include <type_traits>

namespace x4_test {

using enum x4::annotated_rule_kind;

// ' ' is U+1680 'OGHAM SPACE MARK'
// https://www.fileformat.info/info/unicode/char/1680/
constexpr std::string_view unicode_model_string = "aこれは𩸽だ　サロゲート\tペア 入りの魚b";

TEST_CASE("print_chars")
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // full string
    {
        std::ostringstream oss;
        x4::print_chars(oss, unicode_model_string.begin(), unicode_model_string.end(), unicode_model_string.size());
        CHECK(oss.str() == R"(aこれは𩸽だ　サロゲート\tペア\u{1680}入りの魚b)");
    }

    // partial string
    {
        std::ostringstream oss;
        x4::print_chars(oss, unicode_model_string.begin(), unicode_model_string.end(), 0);
        CHECK(oss.str() == R"()");  // NOLINT(readability-container-size-empty)
    }
    {
        std::ostringstream oss;
        x4::print_chars(oss, unicode_model_string.begin(), unicode_model_string.end(), 1);
        CHECK(oss.str() == R"(a)");
    }
    {
        std::ostringstream oss;
        x4::print_chars(oss, unicode_model_string.begin(), unicode_model_string.end(), 2);
        CHECK(oss.str() == R"(aこ)");
    }
    {
        std::ostringstream oss;
        x4::print_chars(oss, unicode_model_string.begin(), unicode_model_string.end(), 4);
        CHECK(oss.str() == R"(aこれは)");
    }
    {
        std::ostringstream oss;
        x4::print_chars(oss, unicode_model_string.begin(), unicode_model_string.end(), 5);
        CHECK(oss.str() == R"(aこれは𩸽)");
    }
    {
        std::ostringstream oss;
        x4::print_chars(oss, unicode_model_string.begin(), unicode_model_string.end(), 6);
        CHECK(oss.str() == R"(aこれは𩸽だ)");
    }
    {
        std::ostringstream oss;
        x4::print_chars(oss, unicode_model_string.begin(), unicode_model_string.end(), 15);
        CHECK(oss.str() == R"(aこれは𩸽だ　サロゲート\tペア)");
    }
    {
        std::ostringstream oss;
        x4::print_chars(oss, unicode_model_string.begin(), unicode_model_string.end(), 16);
        CHECK(oss.str() == R"(aこれは𩸽だ　サロゲート\tペア\u{1680})");
    }
    {
        std::ostringstream oss;
        x4::print_chars(oss, unicode_model_string.begin(), unicode_model_string.end(), 17);
        CHECK(oss.str() == R"(aこれは𩸽だ　サロゲート\tペア\u{1680}入)");
    }

    {
        std::ostringstream oss;
        oss.width(8);
        oss.fill('*');
        x4::print_chars(oss, U'\u1680');
        CHECK(oss.str() == R"(\u{1680})");
        CHECK(oss.width() == 8);
    }
}

TEST_CASE("print_line_highlight")
{
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    using It = std::string_view::const_iterator;
    auto first = unicode_model_string.begin();
    auto const last = unicode_model_string.end();

    {
        std::ostringstream error_out, trace_out;
        x4::default_error_handler error_handler{first, last, &error_out, &trace_out};
        using error_handler_type = decltype(error_handler);
        std::u32string const u32input = iris::unicode::transcode<char32_t>(unicode_model_string);

        for (std::size_t i = 0; i < u32input.size() + 1; ++i) {
            iris::unicode::code_point_iterator<It> it{unicode_model_string.begin(), unicode_model_string.begin(), unicode_model_string.end()};
            std::advance(it, i);
            error_handler.print_line_highlight({unicode_model_string.begin(), unicode_model_string.end()}, it.base());

            std::string left_str, right_str;
            if (i > 0) {
                auto const left_non_colored = u32input.substr(
                    0,
                    std::max(0, int(i) - error_handler_type::highlight_chars)
                );
                auto const left_colored = u32input.substr(
                    std::max(0, int(i) - error_handler_type::highlight_chars),
                    std::min(int(i), error_handler_type::highlight_chars)
                );
                left_str = iris::colorize_format(
                    error_handler_type::colorize_cfg,
                    "{}[$expect_left]{}[/$expect_left]",
                    iris::unicode::transcode<char>(left_non_colored),
                    iris::unicode::transcode<char>(left_colored)
                );
            }
            if (i < u32input.size()) {
                auto const right_colored = u32input.substr(
                    i,
                    std::min(u32input.size() - i, std::size_t(error_handler_type::highlight_chars))
                );
                auto const right_non_colored = u32input.substr(
                    std::min(u32input.size(), i + error_handler_type::highlight_chars),
                    u32input.size() - std::min(u32input.size() - i, std::size_t(error_handler_type::highlight_chars))
                );
                right_str = iris::colorize_format(
                    error_handler_type::colorize_cfg,
                    "[$expect_right]{}[/$expect_right]{}",
                    iris::unicode::transcode<char>(right_colored),
                    iris::unicode::transcode<char>(right_non_colored)
                );
            }

            //std::print("{}", error_out.str());
            CHECK(left_str + right_str + "\n" == error_out.str());

            error_out.str({});
            error_out.clear();
        }
    }
}

IRIS_X4_DECLARE(blank_rule, unused_type);
constexpr auto blank_rule_def = +x4::lit(' ');
IRIS_X4_DEFINE(blank_rule);

IRIS_X4_DECLARE(blank_in_traced_rule, unused_type);
constexpr auto blank_in_traced_rule_def = x4::lit(' ');
IRIS_X4_DEFINE(blank_in_traced_rule);

IRIS_X4_DECLARE(traced_blank_rule, unused_type, x4::annotated_rule<annotate_trace>);
constexpr auto traced_blank_rule_def = +blank_in_traced_rule;
IRIS_X4_DEFINE(traced_blank_rule);

IRIS_X4_DECLARE(number_rule, int);
constexpr auto number_rule_def = x4::int_;
IRIS_X4_DEFINE(number_rule);

TEST_CASE("trace")
{
    using It = std::string_view::const_iterator;
    using error_handler_type = x4::default_error_handler<It>;
    using Context = x4::context<x4::contexts::error_handler, error_handler_type, unused_type>;

    // Non-annotated rules are traced only when the error handler is capable of tracing
    STATIC_CHECK(std::is_empty_v<x4::detail::scoped_tracer<rules::number_rule_id, It, It, unused_type, int>>);
    STATIC_CHECK(!std::is_empty_v<x4::detail::scoped_tracer<rules::number_rule_id, It, It, Context, int>>);

    constexpr std::string_view input = "1  2";

    // Rules invoked as the skipper are excluded from the default tracing
    {
        std::ostringstream error_out, trace_out;
        error_handler_type error_handler{input.begin(), input.end(), &error_out, &trace_out};
        alloy::tuple<int, int> numbers;
        auto const res = x4::parse(
            input,
            x4::with<x4::contexts::error_handler>(error_handler)[number_rule >> number_rule],
            blank_rule,
            numbers
        );
        REQUIRE(res.completed());
        CHECK(numbers == alloy::tuple<int, int>{1, 2});
        CHECK(trace_out.str().contains("<number_rule>"));
        CHECK(!trace_out.str().contains("blank_rule"));
        CHECK(error_handler.is_default_trace_enabled());
    }

    // ... unless explicitly annotated, in which case its non-annotated subrules are still excluded
    {
        std::ostringstream error_out, trace_out;
        error_handler_type error_handler{input.begin(), input.end(), &error_out, &trace_out};
        alloy::tuple<int, int> numbers;
        auto const res = x4::parse(
            input,
            x4::with<x4::contexts::error_handler>(error_handler)[number_rule >> number_rule],
            traced_blank_rule,
            numbers
        );
        REQUIRE(res.completed());
        CHECK(trace_out.str().contains("<number_rule>"));
        CHECK(trace_out.str().contains("<traced_blank_rule>"));
        CHECK(!trace_out.str().contains("blank_in_traced_rule"));
        CHECK(error_handler.is_default_trace_enabled());
    }

    // No trace output
    {
        std::ostringstream error_out;
        error_handler_type error_handler{input.begin(), input.end(), &error_out, nullptr};
        CHECK(!error_handler.is_default_trace_enabled());
        alloy::tuple<int, int> numbers;
        auto const res = x4::parse(
            input,
            x4::with<x4::contexts::error_handler>(error_handler)[number_rule >> number_rule],
            blank_rule,
            numbers
        );
        REQUIRE(res.completed());
        CHECK(numbers == alloy::tuple<int, int>{1, 2});
    }
}

} // x4_test
