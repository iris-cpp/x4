/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>
#include <iris/x4/auxiliary/attr.hpp>
#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/negated_char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/directive/no_case.hpp>
#include <iris/x4/directive/omit.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/numeric/real.hpp>
#include <iris/x4/string/string.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/optional.hpp>
#include <iris/x4/operator/alternative.hpp>

#include <iris/rvariant/rvariant.hpp>

#include <iris/alloy/tuple.hpp>

#include <concepts>
#include <optional>
#include <string>

TEST_CASE("sequence")
{
    namespace traits = x4::traits;

    using x4::standard::char_;
    using x4::standard::space;
    using x4::standard::string;
    using x4::standard::lit;
    using x4::standard::alnum;
    using x4::attr;
    using x4::omit;
    using x4::unused;
    using x4::int_;
    using x4::float_;
    using x4::no_case;
    using x4::rule;
    using x4::_attr;
    using x4::eps;

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(char_ >> char_);

    CHECK(parse("aa", char_ >> char_));
    CHECK(parse("aa", char_ >> 'a'));
    CHECK(parse("aaa", char_ >> char_ >> char_('a')));
    CHECK(parse("xi", char_('x') >> char_('i')));
    CHECK(!parse("xi", char_('x') >> char_('o')));
    CHECK(parse("xin", char_('x') >> char_('i') >> char_('n')));

    CHECK(parse(" a a", char_ >> char_, space));
    CHECK(parse(" x i", char_('x') >> char_('i'), space));
    CHECK(!parse(" x i", char_('x') >> char_('o'), space));

    CHECK(parse(" Hello, World", lit("Hello") >> ',' >> "World", space));

    {
        alloy::tuple<char, char> vec;
        REQUIRE(parse("ab", char_ >> char_, vec));
        CHECK(alloy::get<0>(vec) == 'a');
        CHECK(alloy::get<1>(vec) == 'b');
    }

    {
        alloy::tuple<char, char, char> vec;
        REQUIRE(parse(" a\n  b\n  c", char_ >> char_ >> char_, space, vec));
        CHECK(alloy::get<0>(vec) == 'a');
        CHECK(alloy::get<1>(vec) == 'b');
        CHECK(alloy::get<2>(vec) == 'c');
    }

    {
        // 'b' has an unused_type. unused attributes are not part of the sequence
        alloy::tuple<char, char> vec;
        REQUIRE(parse("abc", char_ >> 'b' >> char_, vec));
        CHECK(alloy::get<0>(vec) == 'a');
        CHECK(alloy::get<1>(vec) == 'c');
    }

    {
        // 'b' has an unused_type. unused attributes are not part of the sequence
        alloy::tuple<char, char> vec;
        REQUIRE(parse("acb", char_ >> char_ >> 'b', vec));
        CHECK(alloy::get<0>(vec) == 'a');
        CHECK(alloy::get<1>(vec) == 'c');
    }

    {
        // "hello" has an unused_type. unused attributes are not part of the sequence
        alloy::tuple<char, char> vec;
        REQUIRE(parse("a hello c", char_ >> "hello" >> char_, space, vec));
        CHECK(alloy::get<0>(vec) == 'a');
        CHECK(alloy::get<1>(vec) == 'c');
    }

    {
        // a single element
        char c{};
        REQUIRE(parse("ab", char_ >> 'b', c));
        CHECK(c == 'a');
    }

    {
        // a single element tuple
        alloy::tuple<char> vec;
        REQUIRE(parse("ab", char_ >> 'b', vec));
        CHECK(alloy::get<0>(vec) == 'a');
    }

    {
        // Make sure single element tuples get passed through if the rhs
        // has a single element tuple as its attribute. Edit JDG 2014:
        // actually the issue here is that if the rhs in this case a rule
        // (r), it should get it (i.e. the sequence parser should not
        // unwrap it). It's odd that the RHS (r) does not really have a
        // single element tuple, so the original comment is not accurate.

        using attr_type = alloy::tuple<char, int>;
        attr_type tpl;

        auto r = rule<class r_id, attr_type>{} = char_ >> ',' >> int_;

        REQUIRE(parse("test:x,1", "test:" >> r, tpl));
        CHECK((tpl == attr_type('x', 1)));
    }

    {
        // make sure single element tuples get passed through if the rhs
        // has a single element tuple as its attribute. This is a correction
        // of the test above.

        using attr_type = alloy::tuple<int>;
        attr_type tpl;

        auto r = rule<class r_id, attr_type>{} = int_;

        REQUIRE(parse("test:1", "test:" >> r, tpl));
        CHECK((tpl == attr_type(1)));
    }

    // unused means we don't care about the attribute
    CHECK(parse("abc", char_ >> 'b' >> char_, unused));

    CHECK(parse("aA", no_case[char_('a') >> 'a']));
    CHECK(parse("BEGIN END", no_case[lit("begin") >> "end"], space));
    CHECK(!parse("BEGIN END", no_case[lit("begin") >> "nend"], space));

    // check attribute is passed through unary to another sequence
    {
        std::string s;
        REQUIRE(parse("ab", eps >> no_case[char_ >> char_], s));
        CHECK("ab" == s);
    }
    {
        std::string s;
        REQUIRE(parse("ab", no_case[char_ >> char_] >> eps, s));
        CHECK("ab" == s);
    }
    {
        std::string s;
        REQUIRE(parse("abc", char_ >> no_case[char_ >> char_], s));
        CHECK("abc" == s);
    }
    {
        std::string s;
        REQUIRE(parse("abc", no_case[char_ >> char_] >> char_, s));
        CHECK("abc" == s);
    }

    // Alternative forms of attributes. Allow sequences to take in
    // STL containers.
    {

        std::vector<char> v;
        REQUIRE(parse("abc", char_ >> char_ >> char_, v));
        REQUIRE(v.size() == 3);
        CHECK(v[0] == 'a');
        CHECK(v[1] == 'b');
        CHECK(v[2] == 'c');
    }
    {
        std::vector<char> v;
        REQUIRE(parse("a,b,c", char_ >> *(',' >> char_), v));
        REQUIRE(v.size() == 3);
        CHECK(v[0] == 'a');
        CHECK(v[1] == 'b');
        CHECK(v[2] == 'c');
    }
    {
        std::vector<char> v;
        REQUIRE(parse("abc", char_ >> *char_, v));
        REQUIRE(v.size() == 3);
        CHECK(v[0] == 'a');
        CHECK(v[1] == 'b');
        CHECK(v[2] == 'c');
    }
    {
        std::vector<char> v;
        REQUIRE(parse("abc", char_ >> *(char_ >> char_), v));
        REQUIRE(v.size() == 3);
        CHECK(v[0] == 'a');
        CHECK(v[1] == 'b');
        CHECK(v[2] == 'c');
    }
    {
        std::vector<char> v;
        CHECK(!parse("abcd", char_ >> *(char_ >> char_), v));
    }
    {
        std::vector<char> v;
        REQUIRE(parse("abc", char_ >> +(char_ >> char_), v));
        REQUIRE(v.size() == 3);
        CHECK(v[0] == 'a');
        CHECK(v[1] == 'b');
        CHECK(v[2] == 'c');
    }
    {
        std::vector<char> v;
        REQUIRE(parse("abc", char_ >> -(+char_), v));
        REQUIRE(v.size() == 3);
        CHECK(v[0] == 'a');
        CHECK(v[1] == 'b');
        CHECK(v[2] == 'c');
    }
    {
        std::string s;
        REQUIRE(parse("foobar", string("foo") >> string("bar"), s));
        CHECK(s == "foobar");
    }

    // Make sure get_sequence_types works for sequences of sequences.
    {
        std::vector<char> v;
        REQUIRE(parse(" a b", (' ' >> char_) >> (' ' >> char_), v));
        REQUIRE(v.size() == 2);
        CHECK(v[0] == 'a');
        CHECK(v[1] == 'b');
    }

    // alternative forms of attributes. Allow sequences to take in
    // stl containers of stl containers.
    {
        std::vector<std::string> v;
        REQUIRE(parse("abc1,abc2", *~char_(',') >> *(',' >> *~char_(',')), v));
        REQUIRE(v.size() == 2);
        CHECK(v[0] == "abc1");
        CHECK(v[1] == "abc2");
    }

    {
        std::vector<std::string> v;

        auto e = rule<class e_id, std::string>{} = *~char_(',');
        auto l = rule<class l_id, std::vector<std::string>>{} = e >> *(',' >> e);

        REQUIRE(parse("abc1,abc2,abc3", l, v));
        REQUIRE(v.size() == 3);
        CHECK(v[0] == "abc1");
        CHECK(v[1] == "abc2");
        CHECK(v[2] == "abc3");
    }

    // do the same with a plain string object
    {
        std::string s;
        REQUIRE(parse("abc1,abc2", *~char_(',') >> *(',' >> *~char_(',')), s));
        CHECK(s == "abc1abc2");
    }

    {
        std::string s;
        auto e = rule<class e_id, std::string>{} = *~char_(',');
        auto l = rule<class l_id, std::string>{} = e >> *(',' >> e);

        REQUIRE(parse("abc1,abc2,abc3", l, s));
        CHECK(s == "abc1abc2abc3");
    }

    {
        std::vector<char> v;
        REQUIRE(parse("ab", char_ >> -char_, v));
        REQUIRE(v.size() == 2);
        CHECK(v[0] == 'a');
        CHECK(v[1] == 'b');
    }
    {
        std::vector<char> v;
        REQUIRE(parse("a", char_ >> -char_, v));
        REQUIRE(v.size() == 1);
        CHECK(v[0] == 'a');
    }
    {
        // We don't think this should be allowed
        // std::vector<char> v;
        // REQUIRE(parse("a", char_, v));
        // REQUIRE(v.size() == 1);
        // CHECK(v[0] == 'a');
    }

    {
        std::vector<std::optional<char>> v;
        REQUIRE(parse("ab", char_ >> -char_, v));
        REQUIRE(v.size() == 2);
        CHECK(v[0] == 'a');
        CHECK(v[1] == 'b');
    }
    {
        std::vector<std::optional<char>> v;
        REQUIRE(parse("a", char_ >> -char_, v));
        REQUIRE(v.size() == 2);
        CHECK(v[0] == 'a');
        CHECK(!v[1].has_value());
    }
    {
        // We don't think this should be allowed
        //std::vector<std::optional<char>> v;
        // REQUIRE(parse("a", char_, v));
        // REQUIRE(v.size() == 1);
        // CHECK(v[0] == 'a');
    }

    // Test from spirit mailing list
    // "Error with container within sequence"
    {
        using attr_type = alloy::tuple<std::string>;
        attr_type vec;

        constexpr auto r = *alnum;

        REQUIRE(parse("abcdef", r, vec));
        CHECK(alloy::get<0>(vec) == "abcdef");
    }
    {
        using attr_type = alloy::tuple<std::vector<int>>;
        attr_type vec;

        constexpr auto r = *int_;

        REQUIRE(parse("123 456", r, space, vec));
        REQUIRE(alloy::get<0>(vec).size() == 2);
        CHECK(alloy::get<0>(vec)[0] == 123);
        CHECK(alloy::get<0>(vec)[1] == 456);
    }

    {
        // Non-flat optional
        alloy::tuple<int, std::optional<alloy::tuple<int, int>>> v;
        constexpr auto p = int_ >> -(':' >> int_ >> '-' >> int_);
        REQUIRE(parse("1:2-3", p, v));
        REQUIRE(alloy::get<1>(v).has_value());
        CHECK(alloy::get<0>(*alloy::get<1>(v)) == 2);
    }

    // optional with container attribute
    {
        constexpr auto p = char_ >> -(':' >> +char_);

        {
            alloy::tuple<char, std::optional<std::string>> v;
            REQUIRE(parse("x", p, v));
            CHECK(!alloy::get<1>(v).has_value());
        }
        {
            alloy::tuple<char, std::optional<std::string>> v;
            REQUIRE(parse("x:abc", p, v));
            REQUIRE(alloy::get<1>(v).has_value());
            CHECK(*alloy::get<1>(v) == "abc");
        }
    }

    {
        using Attr = iris::rvariant<int, float>;
        constexpr auto term = rule<class term_id, Attr>("term") = int_ | float_;
        constexpr auto expr = rule<class expr_id, Attr>("expr") = term | ('(' > term > ')');
        Attr var;
        CHECK(parse("(1)", expr, space, var));
    }

    // Test that failing sequence leaves attribute consistent
    {
        std::string str;
        //no need to use omit[], but lit() is buggy ATM
        REQUIRE(parse("A\nB\nC", *(char_ >> omit[lit("\n")]), str).is_partial_match());
        CHECK(str == "AB");
    }

    // Test that sequence with only one parser producing attribute makes it unwrapped
    STATIC_CHECK(std::same_as<
        x4::parser_traits<decltype(lit("abc") >> attr(long()))>::attribute_type,
        long
    >);

    {
        // test action

        char c = 0;
        int n = 0;
        auto f = [&](auto&& ctx) {
            c = alloy::get<0>(_attr(ctx));
            n = alloy::get<1>(_attr(ctx));
        };

        REQUIRE(parse("x123\"a string\"", (char_ >> int_ >> "\"a string\"")[f]));
        CHECK(c == 'x');
        CHECK(n == 123);
    }

    {
        // test action
        char c = 0;
        int n = 0;
        auto f = [&](auto&& ctx) {
            c = alloy::get<0>(_attr(ctx));
            n = alloy::get<1>(_attr(ctx));
        };

        REQUIRE(parse("x 123 \"a string\"", (char_ >> int_ >> "\"a string\"")[f], space));
        CHECK(c == 'x');
        CHECK(n == 123);
    }

    {
        using T = std::vector<x4_test::move_only>;

        STATIC_CHECK(requires(T& c) {
            typename T::value_type;
        });
        STATIC_CHECK(requires(T& c) {
            traits::begin(c);
        });
        STATIC_CHECK(requires(T& c) {
            requires std::forward_iterator<decltype(traits::begin(c))>;
        });
        STATIC_CHECK(requires(T& c) {
            traits::end(c);
        });
        STATIC_CHECK(requires(T& c) {
            requires std::sentinel_for<decltype(traits::end(c)), decltype(traits::begin(c))>;
        });
        STATIC_CHECK(requires(T& c) {
            traits::is_empty(c);
        });
        STATIC_CHECK(requires(T& c) {
            traits::push_back(c, std::declval<typename T::value_type>());
        });
        STATIC_CHECK(requires(T& c) {
            traits::append(
                c,
                std::declval<decltype(std::make_move_iterator(traits::begin(c)))>(),
                std::declval<decltype(std::make_move_iterator(traits::end(c)))>()
            );
        });
        STATIC_CHECK(requires(T& c) {
            traits::clear(c);
        });

        STATIC_CHECK(traits::is_container_v<std::vector<x4_test::move_only>>);
        STATIC_CHECK(traits::CategorizedAttr<std::vector<x4_test::move_only>, x4::traits::container_attr>);

        std::vector<x4_test::move_only> v;
        REQUIRE(parse("ssszs", *x4_test::synth_move_only >> 'z' >> x4_test::synth_move_only, v));
        CHECK(v.size() == 4);
    }
}
