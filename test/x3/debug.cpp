/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#define BOOST_SPIRIT_X3_DEBUG

#include "test.hpp"

#include <boost/spirit/x4.hpp>
#include <boost/spirit/x4/debug/error_reporting.hpp>

#include <boost/fusion/include/std_pair.hpp>
#include <boost/fusion/include/vector.hpp>

#include <optional>
#include <iterator>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

namespace x3 = boost::spirit::x3;

struct my_error_handler
{
    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Exception, typename Context>
    void operator()(It const&, Se const& last, Exception const& x, Context const&) const
    {
        std::cout
            << "Error! Expecting: "
            << x.which()
            << ", got: \""
            << std::string(x.where(), last)
            << "\""
            << std::endl;
    }
};

struct my_attribute
{
    bool alive = true;

    void access() const
    {
        BOOST_TEST(alive);
    }
    ~my_attribute()
    {
        alive = false;
    }

    friend std::ostream& operator<<(std::ostream & os, my_attribute const & attr)
    {
        attr.access();
        return os << "my_attribute";
    }
};

int main()
{
    using namespace boost::spirit::x3::standard;
    using x3::rule;
    using x3::int_;

    { // basic tests

        auto a = rule<class a_id>("a") = 'a';
        auto b = rule<class b_id>("b") = 'b';
        auto c = rule<class c_id>("c") = 'c';

        {
            auto start = *(a | b | c);
            BOOST_TEST(parse("abcabcacb", start));
        }

        {
            rule<class start> start("start");
            auto start_def =
                start = (a | b) >> (start | b);

            BOOST_TEST(parse("aaaabababaaabbb", start_def));
            BOOST_TEST(parse("aaaabababaaabba", start_def).is_partial_match());
        }
    }

    { // basic tests w/ skipper

        auto a = rule<class a_id>("a") = 'a';
        auto b = rule<class b_id>("b") = 'b';
        auto c = rule<class c_id>("c") = 'c';

        {
            auto start = *(a | b | c);
            BOOST_TEST(parse(" a b c a b c a c b ", start, space));
        }

        {
            rule<class start> start("start");
            auto start_def =
                start = (a | b) >> (start | b);

            BOOST_TEST(parse(" a a a a b a b a b a a a b b b ", start_def, space));
            BOOST_TEST(parse(" a a a a b a b a b a a a b b a ", start_def, space).is_partial_match());
        }
    }

    {
        // std::container attributes

        typedef boost::fusion::vector<int, char> fs;
        rule<class start, std::vector<fs>> start("start");
        auto start_def =
            start = *(int_ >> alpha);

        BOOST_TEST(parse("1 a 2 b 3 c", start_def, space));
    }

    {
         // error handling

        auto r_def = '(' > int_ > ',' > int_ > ')';
        my_error_handler error_handler;

        auto parser = x3::with<x3::error_handler_tag>(error_handler)[r_def];

        BOOST_TEST( parse("(123,456)", parser));
        BOOST_TEST(!parse("(abc,def)", parser));
        BOOST_TEST(!parse("(123,456]", parser));
        BOOST_TEST(!parse("(123;456)", parser));
        BOOST_TEST(!parse("[123,456]", parser));
    }

    {
        x3::shared_symbols<my_attribute> a{{{ "a", my_attribute{} }}};

        auto b = rule<struct b_id, my_attribute>("b") = a;

        my_attribute attr;

        BOOST_TEST(parse("a", b, attr));
    }

    return boost::report_errors();
}
