/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#define IRIS_X4_UNICODE

#include "iris_x4_test.hpp"

#include <iris/x4/char_string_literal.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>

#include <concepts>
#include <stdexcept>
#include <string_view>
#include <type_traits>

TEST_CASE("char")
{
    namespace standard = x4::standard;
    namespace standard_wide = x4::standard_wide;
    namespace unicode = x4::unicode;

    {
        using namespace x4::standard;

        IRIS_X4_ASSERT_CONSTEXPR_CTORS(char_);
        IRIS_X4_ASSERT_CONSTEXPR_CTORS(char_('x'));
        IRIS_X4_ASSERT_CONSTEXPR_CTORS(char_('a', 'z'));
        IRIS_X4_ASSERT_CONSTEXPR_CTORS(char_("a-z"));
        IRIS_X4_ASSERT_CONSTEXPR_CTORS(~char_('x'));

        CHECK(parse("x", char_));
        CHECK(parse("x", 'x'));
        CHECK(!parse("y", 'x'));
        CHECK(parse("x", char_('x')));
        CHECK(!parse("y", char_('x')));

        CHECK(parse("0", char_('0', '9')));
        CHECK(parse("9", char_('0', '9')));
        CHECK(!parse("/", char_('0', '9')));
        CHECK(!parse(":", char_('0', '9')));

        CHECK(!parse("x", ~char_));
        CHECK(!parse("x", ~char_('x')));
        CHECK(parse("y", ~char_('x')));
        CHECK(!parse("0", ~char_('0', '9')));
        CHECK(parse("/", ~char_('0', '9')));

        CHECK(parse("   x", char_('x'), space));
    }

    STATIC_CHECK(std::same_as<decltype(standard::char_("x")), decltype(standard::char_('x'))>);
    STATIC_CHECK(std::same_as<decltype(x4::lit("x")), decltype(x4::lit('x'))>);
    STATIC_CHECK(std::same_as<std::remove_cvref_t<decltype(~~standard::char_('x'))>, decltype(standard::char_('x'))>);

    STATIC_CHECK(requires(std::string_view::iterator it, char ch) {
        std::remove_const_t<decltype(standard::char_)>::parse(it, it, unused, ch);
        std::remove_const_t<decltype(standard::alnum)>::parse(it, it, unused, ch);
    });

    {
        constexpr auto set = unicode::char_(U"-a-c-e0-9");
        STATIC_CHECK(set.test(U'a', unused) && set.test(U'c', unused) && !set.test(U'd', unused));
        STATIC_CHECK(set.test(U'0', unused) && set.test(U'9', unused) && !set.test(U'/', unused) && !set.test(U':', unused));
        STATIC_CHECK(set.test(U'-', unused) && set.test(U'e', unused) && unicode::char_(U"a-").test(U'-', unused));

        STATIC_CHECK(unicode::char_(U"a\0b").test(U'\0', unused) && unicode::char_(U"a\0b").test(U'b', unused));
        STATIC_CHECK(!unicode::char_(U"ab").test(U'\0', unused));

        STATIC_CHECK(standard::char_("\x7f-\x80").test('\x80', unused));
        STATIC_CHECK(standard::char_('\x7f', '\x80').test('\x80', unused));

        CHECK(x4::what(unicode::char_(U"x-zda-cb")) == R"(char_("a-dx-z"))");
        CHECK(x4::what(unicode::char_(U"acegb-f")) == R"(char_("a-g"))");
        CHECK(x4::what(standard::char_("acegb-f")) == R"(char_("a-g"))");
        CHECK(x4::what(standard::char_("+/-")) == R"(char_("+/-"))");

        char32_t ch{};
        CHECK(x4::parse(std::u32string_view(U"q"), unicode::char_(U"a-z"), ch));
        CHECK(ch == U'q');
    }

    {
        auto const is_rejected = [](auto const make_parser) {
            try {
                (void)make_parser();
            }
            catch (std::invalid_argument const&) {
                return true;
            }
            return false;
        };
        CHECK(is_rejected([] { return standard_wide::char_(L"a-cz-x"); }));
        CHECK(is_rejected([] { return standard_wide::char_(L'z', L'x'); }));
        CHECK(is_rejected([] { return unicode::char_(U"a\x110000"); }));
        CHECK(is_rejected([] { return unicode::char_(U'a', U'\x110000'); }));
        CHECK(is_rejected([] { return unicode::char_(U'\x110000'); }));
        CHECK(is_rejected([] { return x4::lit(U'\x110000'); }));
        CHECK(is_rejected([] { return x4::lit(U"\x110000"); }));
        CHECK(is_rejected([] { return x4::string(U"\x110000"); }));
        CHECK(is_rejected([] { return x4::lit(U"a\x110000"); }));
        CHECK(is_rejected([] { return x4::lit(U"abcdefg\x110000"); }));
        CHECK(!is_rejected([] { return unicode::char_(U'\0', U'\x10FFFF'); }));
    }
}
