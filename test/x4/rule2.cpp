/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"

#include <boost/spirit/x4/rule.hpp>
#include <boost/spirit/x4/char/char.hpp>
#include <boost/spirit/x4/char/char_class.hpp>
#include <boost/spirit/x4/operator/sequence.hpp>
#include <boost/spirit/x4/operator/kleene.hpp>

#include <concepts>
#include <iterator>
#include <string>
#include <cstring>
#include <type_traits>

int main()
{
    using namespace x4::standard;
    using x4::rule;
    using x4::lit;
    using x4::_attr;

    {
        // context tests

        char ch;
        auto a = rule<class a_id, char>() = alpha;

        // this semantic action requires the context
        auto f = [&](auto& ctx){ ch = _attr(ctx); };
        BOOST_TEST(parse("x", a[f]));
        BOOST_TEST(ch == 'x');

        // this semantic action requires the (unused) context
        auto f2 = [&](auto&){ ch = 'y'; };
        BOOST_TEST(parse("x", a[f2]));
        BOOST_TEST(ch == 'y');

        // the semantic action may optionally not have any arguments at all
        auto f3 = [&]{ ch = 'z'; };
        BOOST_TEST(parse("x", a[f3]));
        BOOST_TEST(ch == 'z');

        BOOST_TEST(parse("z", a, ch)); // attribute is given.
        BOOST_TEST(ch == 'z');
    }

    {
        // auto rules tests

        char ch = '\0';
        auto a = rule<class a_id, char>() = alpha;
        auto f = [&](auto& ctx){ ch = _attr(ctx); };

        BOOST_TEST(parse("x", a[f]));
        BOOST_TEST(ch == 'x');
        ch = '\0';
        BOOST_TEST(parse("z", a, ch)); // attribute is given.
        BOOST_TEST(ch == 'z');

        ch = '\0';
        BOOST_TEST(parse("x", a[f]));
        BOOST_TEST(ch == 'x');
        ch = '\0';
        BOOST_TEST(parse("z", a, ch)); // attribute is given.
        BOOST_TEST(ch == 'z');
    }

    { // auto rules tests: allow stl containers as attributes to
      // sequences (in cases where attributes of the elements
      // are convertible to the value_type of the container or if
      // the element itself is an stl container with value_type
      // that is convertible to the value_type of the attribute).

        std::string s;
        auto f = [&](auto& ctx){ s = _attr(ctx); };

        {
            auto r = rule<class r_id, std::string>()
                = char_ >> *(',' >> char_);

            BOOST_TEST(parse("a,b,c,d,e,f", r[f]));
            BOOST_TEST(s == "abcdef");
        }

        {
            auto r = rule<class r_id, std::string>()
                = char_ >> *(',' >> char_);

            s.clear();
            BOOST_TEST(parse("a,b,c,d,e,f", r[f]));
            BOOST_TEST(s == "abcdef");
        }

        {
            auto r = rule<class r_id, std::string>()
                = char_ >> char_ >> char_ >> char_ >> char_ >> char_;

            s.clear();
            BOOST_TEST(parse("abcdef", r[f]));
            BOOST_TEST(s == "abcdef");
        }
    }

    return boost::report_errors();
}
