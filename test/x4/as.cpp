/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/attribute/as.hpp>
#include <iris/x4/attribute/value.hpp>
#include <iris/x4/primitive/eps.hpp>

#include <iris/x4/char/char.hpp>
#include <iris/x4/char/unicode_char_class.hpp>
#include <iris/x4/char_string_literal.hpp>

#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/rule.hpp>
#include <iris/x4/numeric/int.hpp>

#include <iris/alloy/tuple.hpp>

#include <iris/unicode/string.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <concepts>
#include <type_traits>

// NOLINTBEGIN(readability-container-size-empty)

using namespace std::string_view_literals;

namespace {

using x4::eps;
using x4::string;
using x4::_attr;
using x4::_rule_var;
using x4::_as_var;
using x4::fixed_value;

} // anonymous

using It = std::string_view::const_iterator;
using Se = It;

constexpr auto do_nothing = [](auto&&) {};
constexpr auto disable_attr = eps.on_match([](auto&&) {});
constexpr auto quoted_string = '\'' >> *~x4::char_('\'') >> '\'';

char const* empty_input_first = nullptr;
char const* const empty_input_last = nullptr;

TEST_CASE("as<T>(p)")
{
    // result = int or long long
    // T = int

    // -------------------------------------------
    // with semantic action
    {
        {
            constexpr auto p = x4::as<int>(fixed_value(3)).on_match([](auto&& ctx) {
                static_assert(std::same_as<std::remove_cvref_t<decltype(x4::_as_var(ctx))>, unused_type>);
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_attr(ctx))>, int>);
                _attr(ctx) += 5;
            });
            {
                int result = 42;
                REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
                CHECK(result == 8);
            }
            {
                long long result = 42ll;
                REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
                CHECK(result == 42ll); // the semantic action's content is discarded
            }
        }

        // do nothing in semantic action
        {
            constexpr auto p = fixed_value(3);
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 3);
        }
        {
            constexpr auto p = fixed_value(3).on_match(do_nothing);
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 3);
        }

        {
            constexpr auto p = x4::as<int>(
                fixed_value(3)
            );
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 3);
        }
        {
            constexpr auto p = x4::as<int>(
                fixed_value(3).on_match(do_nothing)
            );
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 42);
        }

        // The toplevel semantic action is always treated differently;
        // the intermediate value always propagates up.
        {
            constexpr auto p = x4::as<int>(
                fixed_value(3)
            ).on_match(do_nothing);
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 3);
        }
        {
            constexpr auto p = x4::as<int>(
                fixed_value(3).on_match(do_nothing)
            ).on_match(do_nothing);
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 42);
        }
    }

    // -------------------------------------------
    // without semantic action
    {
        constexpr auto p = x4::as<int>(fixed_value(3));
        int result = 42;
        REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
        CHECK(result == 3);
    }

    // `unused`
    {
        constexpr auto p = x4::as<int>(eps);
        int result = 42;
        REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
        CHECK(result == 42);
    }

    {
        constexpr auto p = x4::as<int>(x4::int_);
        constexpr std::string_view input = "7";

        long long converted = 0;
        auto first = input.begin();
        REQUIRE(p.parse(first, input.end(), unused, converted));
        CHECK(converted == 7);

        alloy::tuple<int> single{0};
        first = input.begin();
        REQUIRE(p.parse(first, input.end(), unused, single));
        CHECK(alloy::get<0>(single) == 7);
    }
}

TEST_CASE("as<T>(as<T>(p))")
{
    // result = int or long long
    // T = int
    // U = int

    // -------------------------------------------
    // with semantic action
    {
        constexpr auto p = x4::as<int>(
            x4::as<int>(fixed_value(3)).on_match([](auto&& ctx) {
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_as_var(ctx))>, int>);
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_attr(ctx))>, int>);
                CHECK(std::addressof(_as_var(ctx)) != std::addressof(_attr(ctx)));
                _as_var(ctx) = _attr(ctx) + 5;
            })
        );
        {
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 8);
        }
        {
            long long result = 42ll;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 8ll);
        }
    }

    // do nothing in semantic action
    {
        constexpr auto p = x4::as<int>(
            x4::as<int>(fixed_value(3)).on_match(do_nothing)
        );

        {
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 42);
        }
        {
            long long result = 42ll;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 0); // discarded
        }
    }

    // -------------------------------------------
    // without semantic action
    {
        constexpr auto p = x4::as<int>(
            x4::as<int>(fixed_value(3))
        );

        {
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 3);
        }
        {
            long long result = 42ll;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 3ll);
        }
    }
}

TEST_CASE("as<T>(as<U>(p))")
{
    // result = int or long long
    // T = int
    // U = short

    // -------------------------------------------
    // with semantic action
    {
        constexpr auto p = x4::as<int>(
            x4::as<short>(fixed_value(short(3))).on_match([](auto&& ctx) {
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_as_var(ctx))>, int>);
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_attr(ctx))>, short>);
                _as_var(ctx) = _attr(ctx) + 5;
            })
        );
        {
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 8);
        }
        {
            long long result = 42ll;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 8ll);
        }
    }

    // do nothing in semantic action
    {
        constexpr auto p = x4::as<int>(
            x4::as<short>(fixed_value(short(3))).on_match(do_nothing)
        );

        {
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 42);
        }
        {
            long long result = 42ll;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 0); // discarded
        }
    }

    // -------------------------------------------
    // without semantic action
    {
        constexpr auto p = x4::as<int>(
            x4::as<short>(fixed_value(short(3)))
        );

        {
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 3);
        }
        {
            long long result = 42ll;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 3ll);
        }
    }

    // --------------------------------------------------------
    // string

    {
        constexpr auto p = x4::as<std::string>(
            x4::as<std::u32string>(+x4::unicode::char_).on_match([](auto&& ctx) {
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_as_var(ctx))>, std::string>);
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_attr(ctx))>, std::u32string>);
                _as_var(ctx) = iris::unicode::transcode<char>(_attr(ctx));
            })
        );

        std::u32string_view input = U"テスト";
        std::u32string_view::const_iterator first = input.begin();
        std::u32string_view::const_iterator const last = input.end();

        std::string result;
        REQUIRE(p.parse(first, last, unused, result));
        CHECK(result == "テスト"sv);
    }
}

TEST_CASE("as (single type)")
{
    // as<unused_type>
    {
        constexpr auto p = x4::as<unused_type>(eps);
        using Underlying = std::remove_const_t<decltype(eps)>;

        static_assert(std::same_as<x4::parser_traits<Underlying>::attribute_type, unused_type>);

        std::string_view input;
        It first = input.begin();
        Se const last = input.end();
        std::string attr;
        (void)p.parse(first, last, unused, unused);
        (void)p.parse(first, last, unused, attr);
    }

    // as<int>
    {
        constexpr auto p = x4::as<int>(eps);
        using Underlying = std::remove_const_t<decltype(eps)>;

        static_assert(std::same_as<x4::parser_traits<Underlying>::attribute_type, unused_type>);

        std::string_view input;
        It first = input.begin();
        Se const last = input.end();
        long attr = 0;
        (void)p.parse(first, last, unused, unused);
        (void)p.parse(first, last, unused, attr);
    }

    // `as` only
    {
        std::string attr;
        REQUIRE(parse("'foo'", quoted_string, attr));
        CHECK(attr == "foo"sv);
    }
    {
        std::string attr;
        REQUIRE(parse("'foo'", x4::as<std::string>(quoted_string), attr));
        CHECK(attr == "foo"sv);
    }

    {
        std::string attr;
        REQUIRE(parse("'fo", quoted_string | x4::string("'fo"), attr));
        CHECK(attr == "'fo"sv);
    }
    {
        std::string attr;
        REQUIRE(parse("'fo", x4::as<std::string>(quoted_string) | x4::string("'fo"), attr));
        CHECK(attr == "'fo"sv);
    }
}

TEST_CASE("_as_var")
{
    // `_as_var(ctx)` (within `as<unused_type>(as<std::string>(...))`)
    {
        std::string result{"default"};

        constexpr auto unused_rule = x4::as<unused_type>(
            x4::as<std::string>(
                eps.on_match([]([[maybe_unused]] auto&& ctx) {
                    static_assert(std::same_as<std::remove_cvref_t<decltype(_as_var(ctx))>, unused_type>);
                })
            )
        );

        std::string_view const input;
        It first = input.begin();
        Se const last = input.end();

        REQUIRE(unused_rule.parse(first, last, unused, result));
        CHECK(result == "default"sv);
    }
    // `_as_var(ctx)` (within `as<std::string>(as<unused_type>(...))`)
    {
        std::string result;

        /*constexpr*/ auto unused_rule = x4::as<std::string>(
            fixed_value("default") >>

            eps.on_match([]([[maybe_unused]] auto&& ctx) {
                static_assert(std::same_as<std::remove_cvref_t<decltype(_as_var(ctx))>, std::string>);
            }) >>

            x4::as<unused_type>(
                eps.on_match([]([[maybe_unused]] auto&& ctx) {
                    static_assert(std::same_as<std::remove_cvref_t<decltype(_as_var(ctx))>, unused_type>);
                })
            )
        );

        std::string_view const input;
        It first = input.begin();
        Se const last = input.end();

        REQUIRE(unused_rule.parse(first, last, unused, result));
        CHECK(result == ""sv);
    }
}

IRIS_X4_DECLARE(default_foo, std::string);
IRIS_X4_DECLARE(default_foo_inhibited, std::string);

constexpr auto default_foo_def =
    x4::as<std::string>(
        eps.on_match([](auto&& ctx) {
            _rule_var(ctx) = "default";
        }) >>

        eps.on_match([](auto&& ctx) {
            _as_var(ctx) = "foo";
        })
    );

constexpr auto default_foo_inhibited_def =
    x4::as<std::string>(
        eps.on_match([](auto&& ctx) {
            _rule_var(ctx) = "default";
        }) >>

        eps.on_match([]([[maybe_unused]] auto&& ctx) {
            static_assert(std::same_as<std::remove_cvref_t<decltype(_as_var(ctx))>, unused_type>);
        })
    ) >> disable_attr; // <----------

IRIS_X4_DEFINE(default_foo);
IRIS_X4_DEFINE(default_foo_inhibited);

TEST_CASE("_as_var + rule (eps + eps)")
{
    // `_as_var(ctx)` (with attribute propagation)
    {
        std::string result;
        std::string_view const input;
        It first = input.begin();
        Se const last = input.end();
        REQUIRE(default_foo.parse(first, last, unused, result));
        CHECK(result == "foo"sv);
    }
    // `_as_var(ctx)` (with inhibited attribute)
    {
        std::string result;
        std::string_view const input;
        It first = input.begin();
        Se const last = input.end();
        REQUIRE(default_foo_inhibited.parse(first, last, unused, result));
        CHECK(result == "default"sv);
    }
}

struct StringLiteral
{
    bool is_quoted = false;
    std::string text;
};

IRIS_X4_DECLARE(quoted_string_with_push_back, StringLiteral);
IRIS_X4_DECLARE(quoted_string_without_push_back, StringLiteral);
IRIS_X4_DECLARE(quoted_string_naive, StringLiteral);

constexpr auto quoted_string_begin =
    x4::lit('"').on_match([](auto&& ctx) {
        StringLiteral& rule_var = _rule_var(ctx);
        rule_var.is_quoted = true;
    });

constexpr auto quoted_string_with_push_back_def =
    eps.on_match([](auto& ctx) { _rule_var(ctx).is_quoted = false; }) >>
    x4::as<std::string>(
        quoted_string_begin >>
        *(~x4::char_('"')).on_match([](auto&& ctx) { _as_var(ctx).push_back(_attr(ctx)); }) >>
        '"'
    ).on_match([](auto&& ctx) { _rule_var(ctx).text = std::move(_attr(ctx)); })
;

constexpr auto quoted_string_without_push_back_def =
    eps.on_match([](auto& ctx) { _rule_var(ctx).is_quoted = false; }) >>
    x4::as<std::string>(
        quoted_string_begin >>
        *~x4::char_('"') >> // <----------------- attribute ignored
        '"'
    ).on_match([](auto&& ctx) { _rule_var(ctx).text = std::move(_attr(ctx)); })
;

constexpr auto quoted_string_naive_def =
    eps.on_match([](auto& ctx) { _rule_var(ctx).is_quoted = false; }) >>
    x4::as<std::string>(
        x4::lit('"') >>     // <----------------- no semantic action
        *~x4::char_('"') >> // <----------------- attribute NOT ignored
        '"'
    ).on_match([](auto&& ctx) { _rule_var(ctx).text = std::move(_attr(ctx)); })
;

IRIS_X4_DEFINE(quoted_string_with_push_back);
IRIS_X4_DEFINE(quoted_string_without_push_back);
IRIS_X4_DEFINE(quoted_string_naive);

TEST_CASE("_as_var + rule (quoted_string)")
{
    // Use `_rule_var(ctx)` inside `as<T>(...)`
    std::string_view const input = R"("foo")";

    {
        It first = input.begin();
        Se const last = input.end();
        StringLiteral result;
        REQUIRE(quoted_string_with_push_back.parse(first, last, unused, result));
        CHECK(result.is_quoted == true);
        CHECK(result.text == "foo"sv);
    }
    {
        It first = input.begin();
        Se const last = input.end();
        StringLiteral result;
        REQUIRE(quoted_string_without_push_back.parse(first, last, unused, result));
        CHECK(result.is_quoted == true);
        CHECK(result.text == ""sv);
    }
    {
        It first = input.begin();
        Se const last = input.end();
        StringLiteral result;
        REQUIRE(quoted_string_naive.parse(first, last, unused, result));
        CHECK(result.is_quoted == false);
        CHECK(result.text == "foo"sv);
    }
}

// NOLINTEND(readability-container-size-empty)
