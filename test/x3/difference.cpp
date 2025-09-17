/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include "test.hpp"

#include <boost/spirit/home/x3.hpp>

#include <string>
#include <iostream>

int main()
{
    using boost::spirit::x3::standard::char_;
    using boost::spirit::x3::standard::space;
    using boost::spirit::x3::lit;
    BOOST_SPIRIT_X3_ASSERT_CONSTEXPR_CTORS(char_ - 'a');

    // Basic tests
    {
        BOOST_TEST(parse("b", char_ - 'a'));
        BOOST_TEST(!parse("a", char_ - 'a'));
        BOOST_TEST(parse("/* abcdefghijk */", "/*" >> *(char_ - "*/") >> "*/"));
        BOOST_TEST(!parse("switch", lit("switch") - "switch"));
    }

    // Test attributes
    {
        char attr;
        BOOST_TEST(parse("xg", (char_ - 'g') >> 'g', attr));
        BOOST_TEST(attr == 'x');
    }

    // Test handling of container attributes
    {
        std::string attr;
        BOOST_TEST(parse("abcdefg", *(char_ - 'g') >> 'g', attr));
        BOOST_TEST(attr == "abcdef");
    }

    {
        using boost::spirit::x3::_attr;

        std::string s;

        BOOST_TEST(parse(
            "/*abcdefghijk*/"
          , "/*" >> *(char_ - "*/")[([&](auto& ctx){ s += _attr(ctx); })] >> "*/"
        ));
        BOOST_TEST(s == "abcdefghijk");
        s.clear();

        BOOST_TEST(parse(
            "    /*abcdefghijk*/"
          , "/*" >> *(char_ - "*/")[([&](auto& ctx){ s += _attr(ctx); })] >> "*/"
          , space
        ));
        BOOST_TEST(s == "abcdefghijk");
    }

    return boost::report_errors();
}
