/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2011 Bryce Lelbach
    Copyright (c) 2025 Nana Sakisaka

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "real.hpp"

#include <boost/math/concepts/real_concept.hpp>

int main()
{
    // Custom data type
    {
        using boost::math::concepts::real_concept;
        using boost::spirit::x3::real_parser;
        using boost::spirit::x3::real_policies;

        constexpr real_parser<real_concept, real_policies<real_concept> > custom_real;
        real_concept d;

        BOOST_SPIRIT_X3_ASSERT_CONSTEXPR_CTORS(custom_real);

        BOOST_TEST(parse("-1234", custom_real));
        BOOST_TEST(parse("-1234", custom_real, d) && compare(d, -1234));

        BOOST_TEST(parse("-1.2e3", custom_real));
        BOOST_TEST(parse("-1.2e3", custom_real, d) && compare(d, -1.2e3));

        BOOST_TEST(parse("+1.2e3", custom_real));
        BOOST_TEST(parse("+1.2e3", custom_real, d) && compare(d, 1.2e3));

        BOOST_TEST(parse("-0.1", custom_real));
        BOOST_TEST(parse("-0.1", custom_real, d) && compare(d, -0.1));

        BOOST_TEST(parse("-1.2e-3", custom_real));
        BOOST_TEST(parse("-1.2e-3", custom_real, d) && compare(d, -1.2e-3));

        BOOST_TEST(parse("-1.e2", custom_real));
        BOOST_TEST(parse("-1.e2", custom_real, d) && compare(d, -1.e2));

        BOOST_TEST(parse("-.2e3", custom_real));
        BOOST_TEST(parse("-.2e3", custom_real, d) && compare(d, -.2e3));

        BOOST_TEST(parse("-2e3", custom_real));
        BOOST_TEST(parse("-2e3", custom_real, d) && compare(d, -2e3));

        BOOST_TEST(!parse("-e3", custom_real));
        BOOST_TEST(!parse("-e3", custom_real, d));

        BOOST_TEST(!parse("-1.2e", custom_real));
        BOOST_TEST(!parse("-1.2e", custom_real, d));
    }

    // custom real tests
    {
        using boost::spirit::x3::double_;
        custom_real n;

        BOOST_TEST(parse("-123456e6", double_, n));
    }

    return boost::report_errors();
}
