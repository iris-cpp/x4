/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 6262) // Function uses 'XXXXX' bytes of stack
#endif

#include "iris_x4_test.hpp"

#include <iris/x4/char/char.hpp>
#include <iris/x4/char/negated_char.hpp>
#include <iris/x4/string/string.hpp>
#include <iris/x4/symbols.hpp>
#include <iris/x4/rule.hpp>
#include <iris/x4/auxiliary/attr.hpp>
#include <iris/x4/auxiliary/eoi.hpp>
#include <iris/x4/auxiliary/eol.hpp>
#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/directive/with.hpp>
#include <iris/x4/core/expectation.hpp>
#include <iris/x4/directive/expect.hpp>
#include <iris/x4/directive/lexeme.hpp>
#include <iris/x4/directive/matches.hpp>
#include <iris/x4/directive/no_case.hpp>
#include <iris/x4/directive/no_skip.hpp>
#include <iris/x4/directive/omit.hpp>
#include <iris/x4/directive/raw.hpp>
#include <iris/x4/directive/repeat.hpp>
#include <iris/x4/directive/seek.hpp>
#include <iris/x4/directive/skip.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/list.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/operator/and_predicate.hpp>
#include <iris/x4/operator/difference.hpp>
#include <iris/x4/operator/not_predicate.hpp>
#include <iris/x4/operator/optional.hpp>

#include <iris/alloy/tuple.hpp>

#include <optional>
#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <type_traits>

// NOLINTBEGIN(bugprone-chained-comparison)

#define X4_TEST_SUCCESS_IMPL(tester, input, parser, ...) \
    do { \
        auto const res = parse(input, parser __VA_OPT__(,) __VA_ARGS__); \
        CHECK(tester res.completed()); \
        CHECK(!res.expect_failure.has_value()); \
    } while (false)

#define X4_TEST_FAILURE_IMPL_4(tester, input, parser, catch_stmt) \
    do { \
        auto const res = parse(input, parser); \
        REQUIRE(tester res.completed()); \
        REQUIRE(res.expect_failure.has_value()); \
        auto const& x = res.expect_failure; \
        [[maybe_unused]] auto const& which = x.which(); \
        [[maybe_unused]] auto const& where = std::string_view(x.where(), res.remainder.end()); \
        catch_stmt \
    } while (false)

#define X4_TEST_FAILURE_IMPL_5(tester, input, parser, arg0, catch_stmt) \
    do { \
        auto const res = parse(input, parser, arg0); \
        REQUIRE(tester res.completed()); \
        REQUIRE(res.expect_failure.has_value()); \
        auto const& x = res.expect_failure; \
        [[maybe_unused]] auto const& which = x.which(); \
        [[maybe_unused]] auto const& where = std::string_view(x.where(), res.remainder.end()); \
        catch_stmt \
    } while (false)

#define X4_TEST_FAILURE_IMPL_6(tester, input, parser, arg0, arg1, catch_stmt) \
    do { \
        auto const res = parse(input, parser, arg0, arg1); \
        REQUIRE(tester res.completed()); \
        REQUIRE(res.expect_failure.has_value()); \
        auto const& x = res.expect_failure; \
        [[maybe_unused]] auto const& which = x.which(); \
        [[maybe_unused]] auto const& where = std::string_view(x.where(), res.remainder.end()); \
        catch_stmt \
    } while (false)

// Comments below are intentionally written verbosely
// to provide human-friendly intellisense tooltip for testers

// parser = ok, exception = none, expectation_failure = none
#define X4_TEST_SUCCESS_PASS(...)      X4_TEST_SUCCESS_IMPL(, __VA_ARGS__)

// parser = ok, exception = none, expectation_failure = none
#define X4_TEST_ATTR_SUCCESS_PASS(...) X4_TEST_SUCCESS_IMPL(, __VA_ARGS__)

// parser = fail, exception = none, expectation_failure = none
#define X4_TEST_SUCCESS_FAIL(...)      X4_TEST_SUCCESS_IMPL(!, __VA_ARGS__)

// parser = fail, exception = none, expectation_failure = none
#define X4_TEST_ATTR_SUCCESS_FAIL(...) X4_TEST_SUCCESS_IMPL(!, __VA_ARGS__)

// parser = fail, exception = none, expectation_failure = yes
#define X4_TEST_FAILURE(...)           X4_TEST_FAILURE_IMPL_4(!, __VA_ARGS__)
#define X4_TEST_FAILURE_EX(...)        X4_TEST_FAILURE_IMPL_5(!, __VA_ARGS__)

// parser = fail, exception = none, expectation_failure = yes
#define X4_TEST_ATTR_FAILURE(...)      X4_TEST_FAILURE_IMPL_6(!, __VA_ARGS__)


// For testers; development QOL purpose only.

#define DEBUG_PRINT(x) \
    do { \
        std::cout << "----------------------------------\n"; \
        std::cout << "which: " << x.which() << "\n"; \
        std::cout << "where: " << x.where() << "\n"; \
        std::cout << "----------------------------------\n"; \
        CHECK(!"remove DEBUG_PRINT before commit!"); \
    } while (false)


TEST_CASE("expectation_failure_context_uninstantiated_in_expect_less_parse")
{
    using x4::int_;
    using x4::eoi;
    using x4::eol;
    using x4::eps;
    using x4::attr;
    using x4::lit;
    using x4::string;
    using x4::char_;
    using x4::standard::space;

    using x4::expect;
    using x4::lexeme;
    using x4::matches;
    using x4::no_case;
    using x4::no_skip;
    using x4::omit;
    using x4::raw;
    using x4::repeat;
    using x4::seek;
    using x4::skip;
    using x4::with;

    using It = std::string_view::const_iterator;
    using Se = It;
    std::string_view const input;
    It first = input.begin();
    Se const last = input.end();

    int dummy_int = 0;
    std::vector<int> dummy_ints;
    std::optional<int> dummy_optional_int;
    std::optional<std::vector<int>> dummy_optional_ints;

    bool dummy_bool = false;

    (void)eps[([]{})].parse(first, last, unused, unused); // action
    (void)int_[([]{})].parse(first, last, unused, dummy_int); // action
    (void)(int_ >> int_)[([]{})].parse(first, last, unused, dummy_ints); // action

    (void)attr(42).parse(first, last, unused, unused);
    (void)eoi.parse(first, last, unused, unused);
    (void)eol.parse(first, last, unused, unused);
    (void)eps.parse(first, last, unused, unused);
    (void)eps(true).parse(first, last, unused, unused);
    (void)eps([]{ return true; }).parse(first, last, unused, unused);

    (void)char_.parse(first, last, unused, unused); // NOLINT(readability-static-accessed-through-instance)
    (void)char_('a').parse(first, last, unused, unused);
    (void)char_('a', 'z').parse(first, last, unused, unused);
    (void)char_("a-z").parse(first, last, unused, unused);
    (void)(~char_).parse(first, last, unused, unused);
    (void)lit('a').parse(first, last, unused, unused);
    (void)lit("foo").parse(first, last, unused, unused);
    (void)string("foo").parse(first, last, unused, unused);

    //(void)expect[eps].parse(first, last, unused, unused); // context required
    {
        x4::expectation_failure<It> failure;
        auto const ctx = x4::make_context<x4::contexts::expectation_failure>(failure);
        (void)expect[eps].parse(first, last, ctx, unused);
        (void)expect[int_].parse(first, last, ctx, dummy_int);
        (void)expect[int_ >> int_].parse(first, last, ctx, dummy_ints);
    }

    (void)lexeme[eps].parse(first, last, unused, unused);
    (void)lexeme[int_].parse(first, last, unused, dummy_int);
    (void)lexeme[int_ >> int_].parse(first, last, unused, dummy_ints);

    (void)matches[eps].parse(first, last, unused, unused);
    (void)matches[int_].parse(first, last, unused, dummy_bool);

    (void)no_case[eps].parse(first, last, unused, unused);
    (void)no_case[int_].parse(first, last, unused, dummy_int);
    (void)no_case[int_ >> int_].parse(first, last, unused, dummy_ints);

    (void)no_skip[eps].parse(first, last, unused, unused);
    (void)no_skip[int_].parse(first, last, unused, dummy_int);
    (void)no_skip[int_ >> int_].parse(first, last, unused, dummy_ints);

    (void)omit[eps].parse(first, last, unused, unused);
    (void)raw[eps].parse(first, last, unused, unused);
    (void)repeat(1)[eps].parse(first, last, unused, unused);
    (void)seek[eps].parse(first, last, unused, unused);
    (void)skip(space)[eps].parse(first, last, x4::make_context<x4::contexts::skipper>(space), unused);
    (void)with<struct with_id_>(input)[eps].parse(first, last, unused, unused);

    // `numeric/*` do not need to be tested, as they do not contain expectation failure related calls

    // NOLINTBEGIN(misc-redundant-expression)
    (void)(eps | eps).parse(first, last, unused, unused);
    (void)(int_ | int_).parse(first, last, unused, dummy_int);
    (void)(int_ >> int_ | int_ >> int_).parse(first, last, unused, dummy_ints);

    (void)(&eps).parse(first, last, unused, unused);
    (void)(&int_).parse(first, last, unused, unused);
    (void)(&(int_ >> int_)).parse(first, last, unused, unused);

    (void)(eps - eps).parse(first, last, unused, unused);
    (void)(int_ - eps).parse(first, last, unused, dummy_int);
    (void)((int_ >> int_) - eps).parse(first, last, unused, dummy_ints);

    (void)(*eps(false)).parse(first, last, unused, unused);
    (void)(*int_).parse(first, last, unused, dummy_ints);

    (void)(eps % eps(false)).parse(first, last, unused, unused);
    (void)(int_ % eps(false)).parse(first, last, unused, dummy_ints);

    (void)(!eps).parse(first, last, unused, unused);
    (void)(!int_).parse(first, last, unused, unused);
    (void)(!(int_ >> int_)).parse(first, last, unused, unused);

    (void)(-eps).parse(first, last, unused, unused);
    (void)(-int_).parse(first, last, unused, dummy_optional_int);
    (void)(-(int_ >> int_)).parse(first, last, unused, dummy_optional_ints);

    (void)(+eps(false)).parse(first, last, unused, unused);
    (void)(+int_).parse(first, last, unused, dummy_int);

    (void)(eps >> eps).parse(first, last, unused, unused);
    (void)(int_ >> int_).parse(first, last, unused, dummy_ints);

    // `string/*` do not need to be tested, as they do not contain expectation failure related calls

    // rule
    {
        constexpr x4::rule<struct _, unused_type> r("r");
        constexpr auto p = r = x4::lit('a') >> r;
        (void)p.parse(first, last, unused, unused);
    }
    {
        constexpr x4::rule<struct _, unused_type> r("r");
        constexpr auto p = r %= x4::lit('a') >> r;
        (void)p.parse(first, last, unused, unused);
    }

    // NOLINTEND(misc-redundant-expression)
}

TEST_CASE("expect")
{
    using namespace std::string_view_literals;

    using x4::standard::alpha;
    using x4::standard::digit;
    using x4::standard::space;
    using x4::standard::blank;
    using x4::standard::char_;
    using x4::standard::string;
    using x4::standard::lit;

    // using x4::lit;
    using x4::expect;
    using x4::lexeme;
    using x4::no_case;
    using x4::no_skip;
    using x4::omit;
    using x4::raw;
    using x4::skip;
    using x4::seek;
    using x4::repeat;
    using x4::matches;
    using x4::eps;
    using x4::eoi;
    using x4::eol;
    //using x4::attr;
    using x4::int_;
    using x4::shared_symbols;
    using x4::with;

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(expect['x']);
    IRIS_X4_ASSERT_CONSTEXPR_CTORS(char_ > char_);

    {
        X4_TEST_SUCCESS_PASS("aa", char_ >> expect[char_]);
        X4_TEST_SUCCESS_PASS("aaa", char_ >> expect[char_ >> char_('a')]);
        X4_TEST_SUCCESS_PASS("xi", char_('x') >> expect[char_('i')]);
        X4_TEST_SUCCESS_FAIL("xi", char_('y') >> expect[char_('o')]); // should not throw!
        X4_TEST_SUCCESS_PASS("xin", char_('x') >> expect[char_('i') >> char_('n')]);

        X4_TEST_FAILURE("xi", char_('x') >> expect[char_('o')], {
            CHECK(which == "'o'"sv);
            CHECK(where == "i"sv);
        });
    }

    {
        X4_TEST_SUCCESS_PASS("aa", char_ > char_);
        X4_TEST_SUCCESS_PASS("aaa", char_ > char_ > char_('a'));
        X4_TEST_SUCCESS_PASS("xi", char_('x') > char_('i'));
        X4_TEST_SUCCESS_FAIL("xi", char_('y') > char_('o')); // should not throw!
        X4_TEST_SUCCESS_PASS("xin", char_('x') > char_('i') > char_('n'));

        X4_TEST_FAILURE("xi", char_('x') > char_('o'),
        {
            CHECK(which == "'o'"sv);
            CHECK(where == "i"sv);
        });
    }

    {
    #ifndef IRIS_X4_NO_RTTI
        X4_TEST_FAILURE("ay:a", char_ > char_('x') >> ':' > 'a',
        {
            CHECK(x.which().find("sequence") != std::string::npos);
            CHECK(where == "y:a"sv);
        });
    #else
        X4_TEST_FAILURE("ay:a", char_ > char_('x') >> ':' > 'a',
        {
            CHECK(which == "undefined"sv);
            CHECK(where == "y:a"sv);
        });
    #endif
    }

#if defined(BOOST_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Woverloaded-shift-op-parentheses"
#endif

    // Test that attributes with > (sequences) work just like >> (sequences)
    {
        alloy::tuple<char, char, char> attr;
        X4_TEST_ATTR_SUCCESS_PASS(" a\n  b\n  c", char_ > char_ > char_, space, attr);
        CHECK((alloy::get<0>(attr) == 'a'));
        CHECK((alloy::get<1>(attr) == 'b'));
        CHECK((alloy::get<2>(attr) == 'c'));
    }

    {
        alloy::tuple<char, char, char> attr;
        X4_TEST_ATTR_SUCCESS_PASS(" a\n  b\n  c", char_ > char_ >> char_, space, attr);
        CHECK((alloy::get<0>(attr) == 'a'));
        CHECK((alloy::get<1>(attr) == 'b'));
        CHECK((alloy::get<2>(attr) == 'c'));
    }

    {
        alloy::tuple<char, char, char> attr;
        X4_TEST_ATTR_SUCCESS_PASS(" a, b, c", char_ >> ',' > char_ >> ',' > char_, space, attr);
        CHECK((alloy::get<0>(attr) == 'a'));
        CHECK((alloy::get<1>(attr) == 'b'));
        CHECK((alloy::get<2>(attr) == 'c'));
    }

    {
        std::string attr;
        X4_TEST_ATTR_SUCCESS_PASS("'azaaz'", "'" > *(char_("a") | char_("z")) > "'", space, attr);
        CHECK(attr == "azaaz");
    }

#if defined(BOOST_CLANG)
#pragma clang diagnostic pop
#endif

    {
        X4_TEST_SUCCESS_PASS(" a a", char_ > char_, space);
        X4_TEST_SUCCESS_PASS(" x i", char_('x') > char_('i'), space);

        X4_TEST_FAILURE_EX(" x i", char_('x') > char_('o'), space, {
            CHECK(which == "'o'"sv);
            CHECK(where == "i"sv);
        });
    }

    {
        X4_TEST_FAILURE("bar", expect[lit("foo")],
        {
            CHECK(which == "\"foo\""sv);
            CHECK(where == "bar"sv);
        });
    }


    // skipper
    {
        X4_TEST_FAILURE_EX("accb", repeat(7)[alpha], (lit('a') > 'b') | (lit('c') > 'd'), {
            CHECK(which == "'b'"sv);
            CHECK(where == "ccb"sv);
        });
    }

    //
    // ********* Developers note **********
    //
    // As of now (see `git blame`), get_info<T> is still not
    // specialized for many of the X4 parsers so that the
    // value of `expectation_failure<...>::which()` will be
    // implementation-defined demangled string.
    // Therefore, it's essentially impossible to test them
    // right now; further work must be done.
    //
    // Some specific situations are already been reported
    // (e.g. https://github.com/boostorg/spirit/issues/777)
    // but we really need to implement all specializations for
    // X4's predefined parsers, not just the one reported above.
    //


    // sanity check: test expectation_failure propagation
    // on custom skippers
    {
        X4_TEST_SUCCESS_PASS("a..b", lit('a') >> 'b', lit('.') >> '.');
        X4_TEST_SUCCESS_FAIL("a..b", lit('a') >> 'b', lit('.') >> 'z');

        X4_TEST_SUCCESS_PASS("a  b", lit('a') >> 'b', blank);
        X4_TEST_SUCCESS_PASS("a..b", lit('a') >> 'b', +lit('.'));
        X4_TEST_SUCCESS_PASS("a..b", lit('a') >> 'b', lit('.') >> '.');

        // if this test fails, there might be a problem in x4::skip_over
        X4_TEST_FAILURE_EX("a..b", lit('a') >> 'b', lit('.') >> expect[lit('z')], {
            CHECK(which == "'z'"sv);
            CHECK(where == ".b"sv);
        });

        // ---------------------------------------------------------
        // skip(...) version of the code above
        // we must test against semantically identical cases!

        X4_TEST_SUCCESS_PASS("a..b", skip(lit('.') >> '.')[lit('a') >> 'b']);
        X4_TEST_SUCCESS_FAIL("a..b", skip(lit('.') >> 'z')[lit('a') >> 'b']);

        X4_TEST_SUCCESS_PASS("a  b", skip(blank)[lit('a') >> 'b']);
        X4_TEST_SUCCESS_PASS("a..b", skip(+lit('.'))[lit('a') >> 'b']);
        X4_TEST_SUCCESS_PASS("a..b", skip(lit('.') >> '.')[lit('a') >> 'b']);

        // if this test fails, there might be a problem in x4::skip_over and/or x4::skip_directive
        X4_TEST_FAILURE     ("a..b", skip(lit('.') >> expect[lit('z')])[lit('a') >> 'b'], {
            CHECK(which == "'z'"sv);
            CHECK(where == ".b"sv);
        });
    }

    // sanity check; test `post_skip` in `x4::phrase_parse(...)`
    {
        X4_TEST_SUCCESS_PASS("a..b..", lit('a') >> 'b', lit('.') >> '.');
        X4_TEST_SUCCESS_FAIL("a..b..", lit('a') >> 'z', lit('.') >> '.');
        X4_TEST_SUCCESS_FAIL("a..b..", lit('a') >> 'b', lit('.') >> 'z');

        // should fail in `post_skip`
        X4_TEST_SUCCESS_FAIL("a..b.z", lit('a') >> 'b', lit('.') >> '.');

        // if this test fails, x4::skip_over is BUGGED when `post_skip` is run
        X4_TEST_FAILURE_EX("a..b.z", lit('a') >> 'b', lit('.') > '.', {
            CHECK(which == "'.'"sv);
            CHECK(where == "z"sv);
        });
    }

    // sequence
    {
        X4_TEST_SUCCESS_PASS("ab", lit('a') >> 'b');
        X4_TEST_SUCCESS_FAIL("ac", lit('a') >> 'b');

        X4_TEST_FAILURE("ac", lit('a') >> expect[lit('b')], {
            CHECK(which == "'b'"sv);
            CHECK(where == "c"sv);
        });
        X4_TEST_FAILURE("ac", lit('a') > lit('b'), {
            CHECK(which == "'b'"sv);
            CHECK(where == "c"sv);
        });
    }

    // auxilary parsers
    {
        X4_TEST_SUCCESS_PASS("a12", lit('a') > eps > +digit);
        X4_TEST_SUCCESS_PASS("a12", lit('a') > +digit > eoi);
        X4_TEST_SUCCESS_PASS("a12\n", lit('a') > +digit > eol);

        X4_TEST_FAILURE("a12", lit('a') > eps(false) > +digit, {
            CHECK(where == "12"sv);
        });
        X4_TEST_FAILURE("a12", lit('a') > eoi > +digit, {
            CHECK(where == "12"sv);
        });
        X4_TEST_FAILURE("a12\n", lit('a') > eol > +digit, {
            CHECK(where == "12\n"sv);
        });

        int n = 0;
        X4_TEST_ATTR_SUCCESS_PASS("abc", lit("abc") > x4::attr(12) > eoi, n);
        CHECK(n == 12);
    }

    // numeric, char, string parsers
    {
        X4_TEST_SUCCESS_PASS("abc12", +alpha > int_);
        X4_TEST_SUCCESS_PASS("12a", +digit > lit('a'));

        X4_TEST_FAILURE("abc", +alpha > int_, {
            CHECK(where == ""sv);
        });
        X4_TEST_FAILURE("12a", +digit > lit('b'), {
            CHECK(which == "'b'"sv);
            CHECK(where == "a"sv);
        });

        shared_symbols<> s;
        s.add("cat");
        X4_TEST_SUCCESS_PASS("12cat", +digit > s);
        X4_TEST_FAILURE("12dog", +digit > s, {
            CHECK(where == "dog"sv);
        });
    }

    // expect
    {
        X4_TEST_SUCCESS_PASS("abc", lit('a') >> expect[lit('b') >> 'c']);
        X4_TEST_FAILURE("abc", lit('a') >> expect[lit('b') >> 'd'], {
            CHECK(where == "bc"sv);
        });
        X4_TEST_FAILURE("abc", lit('a') >> expect[lit('b') > 'd'], {
            CHECK(which == "'d'"sv);
            CHECK(where == "c"sv);
        });
    }

    // lexeme
    {
        X4_TEST_SUCCESS_PASS("12 ab", int_ >> lexeme[lit('a') > 'b'], space);
        X4_TEST_FAILURE_EX("12 a b", int_ >> lexeme[lit('a') > 'b'], space, {
            CHECK(which == "'b'"sv);
            CHECK(where == " b"sv);
        });
    }

    // matches
    {
        X4_TEST_SUCCESS_PASS("ab", matches[lit('a') >> 'b']);
        X4_TEST_SUCCESS_PASS("ac", matches[lit('a') >> 'b'] >> "ac");
        X4_TEST_SUCCESS_PASS("ab", matches[lit('a') > 'b']);
        X4_TEST_FAILURE("ac", matches[lit('a') > 'b'] >> "ac", {
            CHECK(which == "'b'"sv);
            CHECK(where == "c"sv);
        });

        bool attr = false;
        X4_TEST_ATTR_SUCCESS_PASS("ab", matches[lit('a') > 'b'], attr);
        CHECK(attr == true);
    }

    // no_case
    {
        X4_TEST_SUCCESS_PASS("12 aB", int_ >> no_case[lit('a') > 'b'], space);
        X4_TEST_FAILURE_EX("12 aB", int_ >> no_case[lit('a') > 'c'], space, {
            CHECK(which == "'c'"sv);
            CHECK(where == "B"sv);
        });
    }

    // no_skip
    {
        X4_TEST_SUCCESS_PASS("12 3ab", int_ >> int_ >> no_skip[lit('a') > 'b'], space);
        X4_TEST_FAILURE_EX("12 3ab", int_ >> int_ >> no_skip[lit('a') > 'c'], space, {
            CHECK(which == "'c'"sv);
            CHECK(where == "b"sv);
        });
    }

    // skip
    {
        X4_TEST_SUCCESS_PASS("ab[]c[]d", skip(lit('[') > ']')[+alpha]);
        X4_TEST_FAILURE("ab[]c[5]d", skip(lit('[') > ']')[+alpha], {
            CHECK(which == "']'"sv);
            CHECK(where == "5]d"sv);
        });

        X4_TEST_SUCCESS_PASS("a1[]b2c3[]d4", skip(lit('[') > ']')[+(alpha > digit)]);
        X4_TEST_FAILURE("a1[]b2c3[]d", skip(lit('[') > ']')[+(alpha > digit)], {
            CHECK(where == ""sv);
        });

        X4_TEST_FAILURE_EX("a b c", lit('a') > 'c', space, {
            CHECK(which == "'c'"sv);
            CHECK(where == "b c"sv);
        });


        {
            std::string s;
            X4_TEST_FAILURE_EX("a b c d", skip(space)[*char_ > lit('z')], s, {
                CHECK(which == "'z'"sv);
                CHECK(where == ""sv);
            });
        }

        {
            std::string s;
            X4_TEST_ATTR_SUCCESS_PASS("a b\n c\n d", char_('a') > char_('b') > skip(space)[char_('c') > char_('d')], blank, s);
            CHECK(s == "abcd");
        }
        {
            std::string s;
            X4_TEST_ATTR_FAILURE("a b\n c\n d", char_('a') > char_('z') > skip(space)[char_('c') > char_('d')], blank, s, {
                CHECK(which == "'z'"sv);
                CHECK(where == "b\n c\n d"sv);
            });
        }
        {
            std::string s;
            X4_TEST_ATTR_FAILURE("a b\n c\n d", char_('a') > char_('b') > skip(space)[char_('z') > char_('d')], blank, s, {
                CHECK(where == "\n c\n d"sv);
            });
        }
        {
            std::string s;
            X4_TEST_ATTR_FAILURE("a b\n c\n d", char_('a') > char_('b') > skip(space)[char_('c') > char_('z')], blank, s, {
                CHECK(which == "'z'"sv);
                CHECK(where == "d"sv);
            });
        }
    }

    // omit
    {
        X4_TEST_SUCCESS_PASS("ab", omit[lit('a') > 'b']);
        X4_TEST_FAILURE("ab", omit[lit('a') > 'c'], {
            CHECK(which == "'c'"sv);
            CHECK(where == "b"sv);
        });
    }

    // raw
    {
        X4_TEST_SUCCESS_PASS("ab", raw[lit('a') > 'b']);
        X4_TEST_FAILURE("ab", raw[lit('a') > 'c'], {
            CHECK(which == "'c'"sv);
            CHECK(where == "b"sv);
        });
    }

    // repeat
    {
        X4_TEST_SUCCESS_PASS("ababac", repeat(1, 3)[lit('a') >> 'b'] >> "ac" | +alpha);

        X4_TEST_FAILURE("ababac", repeat(1, 3)[lit('a') > 'b'] | +alpha, {
            CHECK(which == "'b'"sv);
            CHECK(where == "c"sv);
        });
        X4_TEST_FAILURE("acab", repeat(2, 3)[lit('a') > 'b'] | +alpha, {
            CHECK(which == "'b'"sv);
            CHECK(where == "cab"sv);
        });

        X4_TEST_SUCCESS_PASS("bcab", repeat(2, 3)[lit('a') > 'b'] | +alpha);
    }

    // seek
    {
        X4_TEST_SUCCESS_PASS("a1b1c1", seek[lit('c') > '1']);
        X4_TEST_FAILURE("a1b1c2c1", seek[lit('c') > '1'], {
            CHECK(which == "'1'"sv);
            CHECK(where == "2c1"sv);
        });
    }

    // alternative
    {
        X4_TEST_SUCCESS_PASS("ac", lit('a') >> 'b' | "ac");
        X4_TEST_SUCCESS_PASS("ac", lit('a') >> 'b' | lit('a') >> 'd' | "ac");

        X4_TEST_FAILURE("ac", (lit('a') > 'b') | "ac", {
            CHECK(which == "'b'"sv);
            CHECK(where == "c"sv);
        });
        X4_TEST_FAILURE("ac", lit('a') >> 'b' | (lit('a') > 'd') | "ac", {
            CHECK(which == "'d'"sv);
            CHECK(where == "c"sv);
        });
    }

    // predicate
    {
        X4_TEST_SUCCESS_PASS("abc", lit('a') >> &(lit('b') > 'c') >> "bc");
        X4_TEST_FAILURE("abc", lit('a') >> &(lit('b') > 'd') >> "bc", {
            CHECK(which == "'d'"sv);
            CHECK(where == "c"sv);
        });
    }

    // difference
    {
        X4_TEST_SUCCESS_PASS("bcac", *(char_ - (lit('a') >> 'b')));
        X4_TEST_SUCCESS_PASS("bcab", *(char_ - (lit('a') > 'b')) >> "ab");
        X4_TEST_FAILURE("bcac", *(char_ - (lit('a') > 'b')) >> "ab", {
            CHECK(which == "'b'"sv);
            CHECK(where == "c"sv);
        });
    }

    // kleene
    {
        X4_TEST_SUCCESS_PASS("abac", *(lit('a') >> 'b') >> "ac");
        X4_TEST_SUCCESS_PASS("abbc", *(lit('a') > 'b') >> "bc");
        X4_TEST_FAILURE("abac", *(lit('a') > 'b') >> "ac", {
            CHECK(which == "'b'"sv);
            CHECK(where == "c"sv);
        });
    }

    // list
    {
        X4_TEST_SUCCESS_PASS("ab::ab::ac", (lit('a') >> 'b') % (lit(':') >> ':') >> "::ac");
        X4_TEST_SUCCESS_PASS("ab::ab:ac", (lit('a') > 'b') % (lit(':') >> ':') >> ":ac");

        X4_TEST_FAILURE("ab::ab::ac", (lit('a') > 'b') % (lit(':') >> ':') >> "::ac", {
            CHECK(which == "'b'"sv);
            CHECK(where == "c"sv);
        });
        X4_TEST_FAILURE("ab::ab:ab", (lit('a') >> 'b') % (lit(':') > ':') >> ":ab", {
            CHECK(which == "':'"sv);
            CHECK(where == "ab"sv);
        });
    }

    // not
    {
        X4_TEST_SUCCESS_PASS("[ac]", lit('[') >> !(lit('a') >> 'b') >> +alpha >> ']');
        X4_TEST_SUCCESS_PASS("[bc]", lit('[') >> !(lit('a') > 'b') >> +alpha >> ']');
        X4_TEST_FAILURE("[ac]", lit('[') >> !(lit('a') > 'b') >> +alpha >> ']', {
            CHECK(which == "'b'"sv);
            CHECK(where == "c]"sv);
        });
    }

    // optional
    {
        X4_TEST_SUCCESS_PASS("ac", -(lit('a') >> 'b') >> "ac");
        X4_TEST_SUCCESS_PASS("ab", -(lit('a') > 'b'));
        X4_TEST_FAILURE("ac", -(lit('a') > 'b') >> "ac", {
            CHECK(which == "'b'"sv);
            CHECK(where == "c"sv);
        });
    }

    // plus
    {
        X4_TEST_SUCCESS_PASS("abac", +(lit('a') >> 'b') >> "ac");
        X4_TEST_SUCCESS_PASS("abbc", +(lit('a') > 'b') >> "bc");
        X4_TEST_FAILURE("abac", +(lit('a') > 'b') >> "ac", {
            CHECK(which == "'b'"sv);
            CHECK(where == "c"sv);
        });
    }
}

// NOLINTEND(bugprone-chained-comparison)

#ifdef _MSC_VER
# pragma warning(pop)
#endif
