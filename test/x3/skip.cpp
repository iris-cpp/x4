/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"

#include <boost/spirit/x4.hpp>

#include <iostream>

int main()
{
    using boost::spirit::x3::standard::space;
    using boost::spirit::x3::standard::space_type;
    using boost::spirit::x3::standard::char_;
    using boost::spirit::x3::standard::alpha;
    using boost::spirit::x3::lexeme;
    using boost::spirit::x3::skip;
    using boost::spirit::x3::reskip;
    using boost::spirit::x3::lit;

    BOOST_SPIRIT_X3_ASSERT_CONSTEXPR_CTORS(skip('x')['y']);

    {
        BOOST_TEST(parse("a b c d", skip(space)[*char_]));
    }

    {
        // test attribute
        std::string s;
        BOOST_TEST(parse("a b c d", skip(space)[*char_], s));
        BOOST_TEST(s == "abcd");
    }

    {
        // reskip
        BOOST_TEST(parse("ab c d", lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']], space));
        BOOST_TEST(parse("abcd", lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']], space));
        BOOST_TEST(!(parse("a bcd", lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']], space)));

        BOOST_TEST(parse("ab c d", lexeme[lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']]], space));
        BOOST_TEST(parse("abcd", lexeme[lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']]], space));
        BOOST_TEST(!(parse("a bcd", lexeme[lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']]], space)));
    }

    return boost::report_errors();
}
