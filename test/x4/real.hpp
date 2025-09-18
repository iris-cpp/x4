/*=============================================================================
    Copyright (c) 2001-2010 Joel de Guzman
    Copyright (c) 2001-2010 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#ifndef BOOST_SPIRIT_X4_TEST_REAL_HPP
#define BOOST_SPIRIT_X4_TEST_REAL_HPP

#include "test.hpp"

#include <boost/spirit/x4/char.hpp>
#include <boost/spirit/x4/numeric.hpp>
#include <boost/spirit/x4/operator.hpp>

#include <climits>

#include <type_traits>

// These policies can be used to parse thousand separated
// numbers with at most 2 decimal digits after the decimal
// point. e.g. 123,456,789.01
template <typename T>
struct ts_real_policies : boost::spirit::x4::ureal_policies<T>
{
    //  2 decimal places Max
    template <typename Iterator, typename Attribute>
    static bool
    parse_frac_n(Iterator& first, Iterator const& last, Attribute& attr)
    {
        namespace x4 = boost::spirit::x4;
        return boost::spirit::x4::extract_uint<T, 10, 1, 2, true>::call(first, last, attr);
    }

    //  No exponent
    template <typename Iterator>
    static bool
    parse_exp(Iterator&, Iterator const&)
    {
        return false;
    }

    //  No exponent
    template <typename Iterator, typename Attribute>
    static bool
    parse_exp_n(Iterator&, Iterator const&, Attribute&)
    {
        return false;
    }

    //  Thousands separated numbers
    template <typename Iterator, typename Accumulator>
    static bool
    parse_n(Iterator& first, Iterator const& last, Accumulator& result)
    {
        using boost::spirit::x4::uint_parser;
        namespace x4 = boost::spirit::x4;

        uint_parser<unsigned, 10, 1, 3> uint3;
        uint_parser<unsigned, 10, 3, 3> uint3_3;

        if (auto res = parse(first, last, uint3, result); res.ok)
        {
            Accumulator n;
            Iterator iter = res.remainder.begin();
            first = iter;

            while (true)
            {
                parse(res, iter, last, ',');
                if (!res.ok) break;
                iter = res.remainder.begin();

                parse(res, iter, last, uint3_3, n);
                if (!res.ok) break;
                iter = res.remainder.begin();

                result = result * 1000 + n;
                first = iter;
            }

            return true;
        }
        return false;
    }
};

template <typename T>
struct no_trailing_dot_policy : boost::spirit::x4::real_policies<T>
{
    static bool const allow_trailing_dot = false;
};

template <typename T>
struct no_leading_dot_policy : boost::spirit::x4::real_policies<T>
{
    static bool const allow_leading_dot = false;
};

template <typename T>
bool compare(T n, std::type_identity_t<T> expected)
{
    using std::abs;
    using std::log10;
    using std::pow;
    T const eps = pow(T(10), -std::numeric_limits<T>::digits10 + log10(abs(expected)));
    T delta = n - expected;
    return (delta >= -eps) && (delta <= eps);
}

// A custom real type
struct custom_real
{
    double n;
    custom_real() : n(0) {}
    custom_real(double n_) : n(n_) {}
    friend bool operator==(custom_real a, custom_real b)
        { return a.n == b.n; }
    friend custom_real operator*(custom_real a, custom_real b)
        { return custom_real(a.n * b.n); }
    friend custom_real operator+(custom_real a, custom_real b)
        { return custom_real(a.n + b.n); }
    friend custom_real operator-(custom_real a, custom_real b)
        { return custom_real(a.n - b.n); }
};

#endif
