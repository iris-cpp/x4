/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define IRIS_X4_UNICODE

#include "iris_x4_test.hpp"

#include <iris/x4/attribute/value.hpp>
#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/directive/as.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/unicode_char_class.hpp>
#include <iris/x4/char/negated_char.hpp>
#include <iris/x4/string/string.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/rule.hpp>

#include <iris/unicode/string.hpp>

#include <memory>
#include <string>
#include <string_view>
#include <concepts>
#include <type_traits>

// NOLINTBEGIN(readability-container-size-empty)

using namespace std::string_view_literals;

using x4::eps;
using x4::string;

using It = std::string_view::const_iterator;
using Se = It;
using Context = unused_type;

constexpr auto do_nothing = [](auto&&) {};
constexpr auto disable_attr = eps[([](auto&&) {})];
constexpr auto quoted_string = '\'' >> *~x4::char_('\'') >> '\'';

char const* empty_input_first = nullptr;
char const* const empty_input_last = nullptr;

TEST_CASE("as<T>(p)")
{
    using x4::_as_var;
    using x4::_attr;
    using x4::fixed_value;

    // result = int or long long
    // T = int

    // -------------------------------------------
    // with semantic action
    {
        {
            constexpr auto p = x4::as<int>(fixed_value(3))[([](auto&& ctx) {
                static_assert(std::same_as<std::remove_cvref_t<decltype(x4::_as_var(ctx))>, unused_type>);
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_attr(ctx))>, int>);
                _attr(ctx) += 5;
            })];
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
            constexpr auto p = fixed_value(3)[do_nothing];
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
                fixed_value(3)[do_nothing]
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
            )[do_nothing];
            int result = 42;
            REQUIRE(p.parse(empty_input_first, empty_input_last, unused, result));
            CHECK(result == 3);
        }
        {
            constexpr auto p = x4::as<int>(
                fixed_value(3)[do_nothing]
            )[do_nothing];
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
}

TEST_CASE("as<T>(as<T>(p))")
{
    using x4::_as_var;
    using x4::_attr;
    using x4::fixed_value;

    // result = int or long long
    // T = int
    // U = int

    // -------------------------------------------
    // with semantic action
    {
        constexpr auto p = x4::as<int>(
            x4::as<int>(fixed_value(3))[([](auto&& ctx) {
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_as_var(ctx))>, int>);
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_attr(ctx))>, int>);
                CHECK(std::addressof(_as_var(ctx)) != std::addressof(_attr(ctx)));
                _as_var(ctx) = _attr(ctx) + 5;
            })]
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
            x4::as<int>(fixed_value(3))[do_nothing]
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
    using x4::_as_var;
    using x4::_attr;
    using x4::fixed_value;

    // result = int or long long
    // T = int
    // U = short

    // -------------------------------------------
    // with semantic action
    {
        constexpr auto p = x4::as<int>(
            x4::as<short>(fixed_value(short(3)))[([](auto&& ctx) {
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_as_var(ctx))>, int>);
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_attr(ctx))>, short>);
                _as_var(ctx) = _attr(ctx) + 5;
            })]
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
            x4::as<short>(fixed_value(short(3)))[do_nothing]
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
            x4::as<std::u32string>(+x4::unicode::char_)[([](auto&& ctx) {
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_as_var(ctx))>, std::string>);
                static_assert(std::same_as<std::remove_reference_t<decltype(x4::_attr(ctx))>, std::u32string>);
                _as_var(ctx) = iris::unicode::transcode<char>(_attr(ctx));
            })]
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
    using x4::_attr;
    using x4::_rule_var;
    using x4::_as_var;

    // as<unused_type>
    {
        constexpr auto p = x4::as<unused_type>(eps);
        using Underlying = std::remove_const_t<decltype(eps)>;
        using AsParser = std::remove_const_t<decltype(p)>;

        static_assert(std::same_as<x4::parser_traits<Underlying>::attribute_type, unused_type>);

        static_assert(x4::is_nothrow_parsable_v<Underlying, It, Se, Context, unused_type>);
        static_assert(x4::is_nothrow_parsable_v<Underlying, It, Se, Context, std::string>); // Arbitrary exposed attribute

        static_assert(x4::is_nothrow_parsable_v<AsParser, It, Se, Context, unused_type>);
        static_assert(x4::is_nothrow_parsable_v<AsParser, It, Se, Context, std::string>); // Arbitrary exposed attribute

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
        using AsParser = std::remove_const_t<decltype(p)>;

        static_assert(std::same_as<x4::parser_traits<Underlying>::attribute_type, unused_type>);

        static_assert(x4::is_nothrow_parsable_v<Underlying, It, Se, Context, unused_type>);
        static_assert(x4::is_nothrow_parsable_v<Underlying, It, Se, Context, std::string>); // Arbitrary exposed attribute

        static_assert(x4::is_nothrow_parsable_v<AsParser, It, Se, Context, unused_type>);
        static_assert(x4::is_nothrow_parsable_v<AsParser, It, Se, Context, long>); // Arbitrary exposed attribute

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

TEST_CASE("as + rule")
{
    using x4::_attr;
    using x4::_rule_var;
    using x4::_as_var;

    {
        constexpr x4::rule<struct _, std::string> rule_maker{"rule_maker"};

        // Non-forced attribute, `operator=`
        {
            // Attribute is disabled because the sub parser has semantic action and the operator is `=`
            constexpr auto rule_without_attr = rule_maker = quoted_string >> disable_attr;
            std::string str;
            REQUIRE(parse("'foo'", rule_without_attr, str));
            CHECK(str == ""sv); // Disabled attribute should yield default-constructed attribute
        }
        {
            // Attribute is disabled because the sub parser has semantic action and the operator is `=`
            constexpr auto rule_without_attr = rule_maker = x4::as<std::string>(quoted_string) >> disable_attr;
            std::string str;
            REQUIRE(parse("'foo'", rule_without_attr, str));
            CHECK(str == ""sv); // Disabled attribute should yield default-constructed attribute
        }
        {
            // Attribute is disabled because the sub parser has semantic action
            constexpr auto rule_without_attr = rule_maker = x4::as<std::string>(quoted_string >> disable_attr);
            std::string str;
            REQUIRE(parse("'foo'", rule_without_attr, str));
            CHECK(str == ""sv); // Disabled attribute should yield default-constructed attribute
        }

        // Forced attribute, `operator%=`
        {
            constexpr auto rule_with_forced_attr = rule_maker %= quoted_string >> disable_attr;
            std::string str;
            REQUIRE(parse("'foo'", rule_with_forced_attr, str));
            CHECK(str == "foo"sv); // Forced attribute should hold the parsed value
        }
        {
            constexpr auto rule_with_forced_attr = rule_maker %= x4::as<std::string>(quoted_string) >> disable_attr;
            std::string str;
            REQUIRE(parse("'foo'", rule_with_forced_attr, str));
            CHECK(str == "foo"sv); // `as` should not create a temporary; it should directly parse into the exposed variable
        }
        {
            constexpr auto rule_with_forced_attr = rule_maker %= x4::as<std::string>(quoted_string >> disable_attr);
            std::string str;
            REQUIRE(parse("'foo'", rule_with_forced_attr, str));
            CHECK(str == ""sv); // Disabled attribute should yield default-constructed attribute
        }
    }
}

TEST_CASE("_as_var")
{
    using x4::_attr;
    using x4::_rule_var;
    using x4::_as_var;
    using x4::fixed_value;

    // `_as_var(ctx)` (with auto attribute propagation)
    {
        std::string result;

        constexpr auto string_rule = x4::rule<struct _, decltype(result)>{""} =
            x4::as<std::string>(
                eps[([](auto&& ctx) {
                    _rule_var(ctx) = "default";
                })] >>

                eps[([](auto&& ctx) {
                    _as_var(ctx) = "foo";
                })]
            );

        std::string_view const input;
        It first = input.begin();
        Se const last = input.end();

        REQUIRE(string_rule.parse(first, last, unused, result));
        CHECK(result == "foo"sv);
    }
    // `_as_var(ctx)` (with disabled attribute)
    {
        std::string result;

        constexpr auto string_rule = x4::rule<struct _, decltype(result)>{""} =
            x4::as<std::string>(
                eps[([](auto&& ctx) {
                    _rule_var(ctx) = "default";
                })] >>

                eps[([]([[maybe_unused]] auto&& ctx) {
                    static_assert(std::same_as<std::remove_cvref_t<decltype(_as_var(ctx))>, unused_type>);
                })]
            ) >> disable_attr; // <----------

        std::string_view const input;
        It first = input.begin();
        Se const last = input.end();

        REQUIRE(string_rule.parse(first, last, unused, result));
        CHECK(result == "default"sv);
    }
    // `_as_var(ctx)` (within `as<unused_type>(as<std::string>(...))`)
    {
        std::string result{"default"};

        constexpr auto unused_rule = x4::as<unused_type>(
            x4::as<std::string>(
                eps[([]([[maybe_unused]] auto&& ctx) {
                    static_assert(std::same_as<std::remove_cvref_t<decltype(_as_var(ctx))>, unused_type>);
                })]
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

            eps[([]([[maybe_unused]] auto&& ctx) {
                static_assert(std::same_as<std::remove_cvref_t<decltype(_as_var(ctx))>, std::string>);
            })] >>

            x4::as<unused_type>(
                eps[([]([[maybe_unused]] auto&& ctx) {
                    static_assert(std::same_as<std::remove_cvref_t<decltype(_as_var(ctx))>, unused_type>);
                })]
            )
        );

        std::string_view const input;
        It first = input.begin();
        Se const last = input.end();

        REQUIRE(unused_rule.parse(first, last, unused, result));
        CHECK(result == ""sv);
    }

    // Use `_rule_var(ctx)` inside `as<T>(...)`
    {
        struct StringLiteral
        {
            bool is_quoted = false;
            std::string text;
        };

        std::string_view const input = R"("foo")";

        {
            constexpr auto string_literal = x4::rule<struct _, StringLiteral>{"StringLiteral"} =
                eps[([](auto& ctx) { _rule_var(ctx).is_quoted = false; })] >>
                x4::as<std::string>(
                    x4::lit('"')[([](auto&& ctx) {
                        StringLiteral& rule_var = _rule_var(ctx);
                        rule_var.is_quoted = true;
                    })] >>
                    *(~x4::char_('"'))[([](auto&& ctx) { _as_var(ctx).push_back(_attr(ctx)); })] >>
                    '"'
                )[([](auto&& ctx) { _rule_var(ctx).text = std::move(_attr(ctx)); })];

            It first = input.begin();
            Se const last = input.end();

            StringLiteral result;
            REQUIRE(string_literal.parse(first, last, unused, result));
            CHECK(result.is_quoted == true);
            CHECK(result.text == "foo"sv);
        }
        {
            constexpr auto string_literal = x4::rule<struct _, StringLiteral>{"StringLiteral"} =
                eps[([](auto& ctx) { _rule_var(ctx).is_quoted = false; })] >>
                x4::as<std::string>(
                    x4::lit('"')[([](auto&& ctx) {
                        StringLiteral& rule_var = _rule_var(ctx);
                        rule_var.is_quoted = true;
                    })] >>
                    *~x4::char_('"') >> // <----------------- attribute ignored
                    '"'
                )[([](auto&& ctx) { _rule_var(ctx).text = std::move(_attr(ctx)); })];

            It first = input.begin();
            Se const last = input.end();

            StringLiteral result;
            REQUIRE(string_literal.parse(first, last, unused, result));
            CHECK(result.is_quoted == true);
            CHECK(result.text == ""sv);
        }
        {
            constexpr auto string_literal = x4::rule<struct _, StringLiteral>{"StringLiteral"} =
                eps[([](auto& ctx) { _rule_var(ctx).is_quoted = false; })] >>
                x4::as<std::string>(
                    x4::lit('"') >>     // <----------------- no semantic action
                    *~x4::char_('"') >> // <----------------- attribute NOT ignored
                    '"'
                )[([](auto&& ctx) { _rule_var(ctx).text = std::move(_attr(ctx)); })];

            It first = input.begin();
            Se const last = input.end();

            StringLiteral result;
            REQUIRE(string_literal.parse(first, last, unused, result));
            CHECK(result.is_quoted == false);
            CHECK(result.text == "foo"sv);
        }
    }
}

// NOLINTEND(readability-container-size-empty)
