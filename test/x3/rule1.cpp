/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"

#include <boost/spirit/x4.hpp>

#include <string>
#include <cstring>
#include <iostream>

int main()
{
    using namespace boost::spirit::x3::standard;
    using boost::spirit::x3::rule;
    using boost::spirit::x3::lit;
    using boost::spirit::x3::int_;
    using boost::spirit::x3::unused_type;
    using boost::spirit::x3::phrase_parse;
    using boost::spirit::x3::root_skipper_flag;
    using boost::spirit::x3::traits::has_attribute_v;

#ifdef BOOST_SPIRIT_X3_NO_RTTI
    BOOST_SPIRIT_X3_ASSERT_CONSTEXPR_CTORS(rule<class r>{});
#endif
    BOOST_SPIRIT_X3_ASSERT_CONSTEXPR_CTORS(rule<class r>{"r"});
    BOOST_SPIRIT_X3_ASSERT_CONSTEXPR_CTORS(rule<class r>{"r"} = 'x');

    // check attribute advertising
    static_assert( has_attribute_v<rule<class r, int>, /*Context=*/unused_type>);
    static_assert(!has_attribute_v<rule<class r     >, /*Context=*/unused_type>);
    static_assert( has_attribute_v<decltype(rule<class r, int>{} = int_), /*Context=*/unused_type>);
    static_assert(!has_attribute_v<decltype(rule<class r     >{} = int_), /*Context=*/unused_type>);

    {
        // basic tests

        auto a = lit('a');
        auto b = lit('b');
        auto c = lit('c');
        rule<class r> r;

        {
            auto start =
                r = *(a | b | c);

            BOOST_TEST(parse("abcabcacb", start));
        }

        {
            auto start =
                r = (a | b) >> (r | b);

            BOOST_TEST(parse("aaaabababaaabbb", start));
            BOOST_TEST(parse("aaaabababaaabba", start).is_partial_match());

            // ignore the skipper!
            BOOST_TEST(parse("aaaabababaaabba", start, space).is_partial_match());
        }
    }

    {
        // basic tests w/ skipper

        auto a = lit('a');
        auto b = lit('b');
        auto c = lit('c');
        rule<class r> r;

        {
            auto start =
                r = *(a | b | c);

            BOOST_TEST(parse(" a b c a b c a c b ", start, space));
        }

        {
            auto start =
                r = (a | b) >> (r | b);

            BOOST_TEST(parse(" a a a a b a b a b a a a b b b ", start, space));
            BOOST_TEST(parse(" a a a a b a b a b a a a b b a ", start, space).is_partial_match());
        }
    }

    {
        // basic tests w/ skipper but no final post-skip

        auto a = rule<class a_id>()
            = lit('a');

        auto b = rule<class b_id>()
            = lit('b');

        auto c = rule<class c_id>()
            = lit('c');

        {
            auto start = rule<class start_id>() = *(a | b) >> c;

            auto const res = parse(" a b a a b b a c ... ", start, space, root_skipper_flag::dont_post_skip);
            BOOST_TEST(res.ok && res.remainder.size() == 5);
        }

        {
            rule<class start> start;

            auto p = start = (a | b) >> (start | c);

            BOOST_TEST(parse(" a a a a b a b a b a a a b b b c ", p, space, root_skipper_flag::do_post_skip));

            {
                auto const res = parse(" a a a a b a b a b a a a b b b c ", p, space, root_skipper_flag::dont_post_skip);
                BOOST_TEST(res.ok && res.remainder.size() == 1);
            }
        }
    }

    return boost::report_errors();
}
