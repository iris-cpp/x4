/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"

#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/directive/lexeme.hpp>
#include <iris/x4/directive/skip.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/sequence.hpp>

TEST_CASE("skip")
{
    using x4::standard::space;
    using x4::standard::char_;
    using x4::standard::alpha;
    using x4::lexeme;
    using x4::skip;
    using x4::reskip;
    using x4::lit;

    IRIS_X4_ASSERT_CONSTEXPR_CTORS(skip('x')['y']);

    CHECK(parse("a b c d", skip(space)[*char_]));

    {
        // test attribute
        std::string s;
        REQUIRE(parse("a b c d", skip(space)[*char_], s));
        CHECK(s == "abcd");
    }

    // reskip
    CHECK(parse("ab c d", lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']], space));
    CHECK(parse("abcd", lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']], space));
    CHECK(!(parse("a bcd", lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']], space)));

    CHECK(parse("ab c d", lexeme[lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']]], space));
    CHECK(parse("abcd", lexeme[lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']]], space));
    CHECK(!(parse("a bcd", lexeme[lexeme[lit('a') >> 'b' >> reskip[lit('c') >> 'd']]], space)));
}
