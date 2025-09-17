/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"

#include <boost/spirit/home/x3.hpp>

#include <iostream>

int main()
{
    using boost::spirit::x3::int_;

    BOOST_SPIRIT_X3_ASSERT_CONSTEXPR_CTORS(!int_);

    {
        BOOST_TEST(!parse("1234", !int_));
        BOOST_TEST(parse("abcd", !int_).is_partial_match());
        BOOST_TEST(!parse("abcd", !!int_));
    }

    return boost::report_errors();
}
