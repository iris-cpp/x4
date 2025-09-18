/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2013 Agustin Berge

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"

#include <boost/spirit/x4.hpp>

#include <iostream>

int main()
{
    using boost::spirit::x4::standard::space;
    using boost::spirit::x4::standard::space_type;
    using boost::spirit::x4::standard::char_;
    using boost::spirit::x4::lexeme;
    using boost::spirit::x4::no_skip;

    BOOST_SPIRIT_X4_ASSERT_CONSTEXPR_CTORS(no_skip['x']);

    // without skipping no_skip is equivalent to lexeme
    {
        std::string str;
        BOOST_TEST(parse("'  abc '", '\'' >> no_skip[+~char_('\'')] >> '\'', str));
        BOOST_TEST(str == "  abc ");
    }
    {
        std::string str;
        BOOST_TEST(parse("'  abc '", '\'' >> lexeme[+~char_('\'')] >> '\'', str));
        BOOST_TEST(str == "  abc ");
    }

    // with skipping, no_skip allows to match a leading skipper
    {
        std::string str;
        BOOST_TEST(parse("'  abc '", '\'' >> no_skip[+~char_('\'')] >> '\'', str, space));
        BOOST_TEST(str == "  abc ");
    }
    {
        std::string str;
        BOOST_TEST(parse("'  abc '", '\'' >> lexeme[+~char_('\'')] >> '\'', str, space));
        BOOST_TEST(str == "abc ");
    }

    return boost::report_errors();
}
