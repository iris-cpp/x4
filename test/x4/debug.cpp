/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/debug/error_handler.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/directive/with.hpp>
#include <iris/x4/operator/sequence.hpp>

#include <iterator>
#include <string>
#include <iostream>

struct my_error_handler
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Exception, class Context>
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

TEST_CASE("debug")
{
    using x4::int_;

    {
        // error handling

        constexpr auto int_int = '(' > int_ > ',' > int_ > ')';  // NOLINT(bugprone-chained-comparison)
        my_error_handler error_handler;

        auto parser = x4::with<x4::contexts::error_handler>(error_handler)[int_int];

        CHECK( parse("(123,456)", parser));
        CHECK(!parse("(abc,def)", parser));
        CHECK(!parse("(123,456]", parser));
        CHECK(!parse("(123;456)", parser));
        CHECK(!parse("[123,456]", parser));
    }
}
