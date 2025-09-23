/*=============================================================================
    Copyright (c) 2018-2023 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "test.hpp"

#include <boost/spirit/x4/string/utf8.hpp>

#include <string>

int main()
{
    using x4::to_utf8;
    using namespace std::string_view_literals;

    BOOST_TEST(to_utf8(0xD7FFul) == "\xED\x9F\xBF"sv);
    BOOST_TEST(to_utf8(0xE000ul) == "\xEE\x80\x80"sv);

    if constexpr (sizeof(L"\u00FF") == 2) {
        BOOST_TEST(to_utf8(L"\u00FF"[0]) == "\xC3\xBF"sv);
    }

    BOOST_TEST(to_utf8(U'\u00FF') == "\xC3\xBF"sv);

    if constexpr (sizeof(L"\uFFE1") == 2) {
        BOOST_TEST(to_utf8(L"\uFFE1"[0]) == "\xEF\xBF\xA1"sv);
    }

    BOOST_TEST(to_utf8(U'\uFFE1') == "\xEF\xBF\xA1"sv);

    if constexpr(sizeof(L"\U0001F9D0") == 2) {
        BOOST_TEST(to_utf8(L"\U0001F9D0"[0]) == "\xF0\x9F\xA7\x90"sv);
    }

    BOOST_TEST(to_utf8(U'\U0001F9D0') == "\xF0\x9F\xA7\x90"sv);
    BOOST_TEST(to_utf8(L"\U0001F9D0\U0001F9E0") == "\xF0\x9F\xA7\x90\xF0\x9F\xA7\xA0"sv);
    BOOST_TEST(to_utf8(U"\U0001F9D0\U0001F9E0") == "\xF0\x9F\xA7\x90\xF0\x9F\xA7\xA0"sv);
    BOOST_TEST(to_utf8(L"\U0001F9D0\U0001F9E0"sv) == "\xF0\x9F\xA7\x90\xF0\x9F\xA7\xA0"sv);
    BOOST_TEST(to_utf8(U"\U0001F9D0\U0001F9E0"sv) == "\xF0\x9F\xA7\x90\xF0\x9F\xA7\xA0"sv);

    return boost::report_errors();
}
