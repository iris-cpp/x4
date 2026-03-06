/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/auxiliary/attr.hpp>
#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/string/string.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/directive/omit.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/alternative.hpp>

#include <iris/alloy/adapted/std_pair.hpp>

// list-like
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/list.hpp>
#include <iris/x4/directive/repeat.hpp>

#include <concepts>
#include <ostream>
#include <vector>
#include <string>
#include <utility>

// NOLINTBEGIN(readability-container-size-empty)

using namespace std::string_view_literals;

struct strong_int
{
    int value = 0;
    int assigned_count = 0;

    strong_int() = default;
    strong_int(strong_int const&) = default;
    strong_int(strong_int&&) noexcept = default;

    explicit strong_int(int value) : value(value) {}

    strong_int& operator=(strong_int const& other)
    {
        value = other.value;
        ++assigned_count;
        return *this;
    }

    strong_int& operator=(strong_int&& other) noexcept
    {
        value = other.value;
        ++assigned_count;
        return *this;
    }

    strong_int& operator=(int new_value)
    {
        value = new_value;
        ++assigned_count;
        return *this;
    }

    bool operator==(strong_int const& other) const
    {
        return value == other.value;
    }

    friend std::ostream& operator<<(std::ostream& os, strong_int const& si)
    {
        return os << si.value;
    }
};

TEST_CASE("partial success (alternative)")
{
    using x4::attr;
    using x4::eps;
    using x4::omit;
    using x4::int_;

    using x4::string;
    using x4::lit;
    using x4::standard::char_;
    using x4::standard::space;

    // Sanity checks
    {
        int i = -1;
        REQUIRE(parse("", attr(42), i));
        CHECK(i == 42);
    }
    {
        std::string str;
        REQUIRE(parse("", attr("foo"), str));
        CHECK(str == "foo");
    }
    {
        std::string str;
        REQUIRE(parse("foo", string("foo"), str));
        CHECK(str == "foo");
    }

    // Non-string container attribute
    // Related to: https://github.com/boostorg/spirit/issues/378
    {
        static_assert(x4::traits::CategorizedAttr<std::vector<int>, x4::traits::container_attr>);
        static_assert(x4::traits::X4Container<std::vector<int>>);
        static_assert(x4::traits::is_container_v<std::vector<int>>);

        {
            std::vector<int> ints;
            REQUIRE(parse("1 2", eps(false) | attr(98) >> attr(99), ints).is_partial_match());
            CHECK(ints == std::vector<int>{98, 99});
        }
        {
            std::vector<int> ints;
            REQUIRE(parse("1 2", int_ >> int_ >> eps(false) | attr(98) >> attr(99), space, ints).is_partial_match());
            // If we don't properly "hold" the value on the failed branch of
            // `x4::alternative`, we would see {1, 2, 98, 99} here.
            CHECK(ints == std::vector<int>{98, 99});
        }
        // Failed parse should not modify the exposed attribute
        {
            std::vector<int> ints;
            REQUIRE(!parse("1 2", int_ >> int_ >> eps(false) | attr(98) >> attr(99) >> eps(false), space, ints));
            // Wrong implementation yields {1, 2, 98, 99} or {98, 99}
            CHECK(ints == std::vector<int>{});
        }
        {
            std::vector<int> ints;
            REQUIRE(parse("1 2", attr(std::vector<int>{3, 4}) >> eps(false) | attr(98) >> attr(99), space, ints).is_partial_match());
            // Wrong implementation yields {3, 4, 98, 99}
            CHECK(ints == std::vector<int>{98, 99});
        }
    }

    // String container attribute
    // Intended for testing `detail::string_parse`
    {
        static_assert(x4::traits::CategorizedAttr<std::string, x4::traits::container_attr>);
        static_assert(x4::traits::X4Container<std::string>);
        static_assert(x4::traits::is_container_v<std::string>);

        {
            std::string str;
            REQUIRE(parse("foodie", "fox" | string("foodie"), str));
            CHECK(str == "foodie");
        }
        {
            constexpr auto fox = char_('f') >> char_('o') >> char_('x');

            std::string str;
            REQUIRE(parse("foodie", fox | string("foodie"), str));
            // If we don't properly "hold" the value on the failed branch of
            // `x4::alternative`, we would see "fofoodie" here.
            CHECK(str == "foodie");
        }
        {
            constexpr auto foo = char_('f') >> char_('o') >> char_('o');

            std::string str;
            REQUIRE(parse("foodie", foo >> eps(false) | string("foodie"), str));
            // Wrong implementation yields "foofoodie"
            CHECK(str == "foodie");
        }
        {
            std::string str;
            REQUIRE(parse("foodie", attr("bookworm") >> eps(false) | string("foodie"), str));
            // Wrong implementation yields "bookwormfoodie"
            CHECK(str == "foodie");
        }
        // Failed parse should not modify the exposed attribute
        {
            std::string str;
            REQUIRE(!parse("foodie", attr("bookworm") >> eps(false) | string("foodie") >> eps(false), str));
            // Wrong implementation yields "bookwormfoodie" or "foodie"
            CHECK(str == "");
        }

        {
            std::string str;
            REQUIRE(parse("foodie", string("food") >> "fan" | string("foodie"), str));
            // Wrong implementation yields "foodfoodie"
            CHECK(str == "foodie");
        }
    }

    // Plain attribute
    {
        static_assert(x4::traits::CategorizedAttr<strong_int, x4::traits::plain_attr>);
        static_assert(!x4::traits::X4Container<strong_int>);
        static_assert(!x4::traits::is_container_v<strong_int>);

        {
            strong_int si;
            REQUIRE(parse("1", int_ | attr(strong_int{9}), si));
            CHECK(si == strong_int{1});
            CHECK(si.assigned_count == 1);
        }
        {
            strong_int si;
            REQUIRE(parse("1", int_ >> eps(false) | int_, si));
            CHECK(si == strong_int{1});
            // Wrong implementation yields 2, because `x4::alternative` wrongly mutates the exposed variable
            CHECK(si.assigned_count == 1);
        }
    }

    // Tuple attribute
    {
        using pair_int = std::pair<int, int>;

        static_assert(x4::traits::CategorizedAttr<pair_int, x4::traits::tuple_attr>);
        static_assert(!x4::traits::X4Container<pair_int>);
        static_assert(!x4::traits::is_container_v<pair_int>);

        {
            pair_int pi;
            REQUIRE(parse("1 2", int_ >> int_ | attr(pair_int{98, 99}), space, pi));
            CHECK(pi == pair_int{1, 2});
        }
        {
            pair_int pi;
            REQUIRE(parse("1 2",
                int_ >> int_ >> eps(false) | attr(pair_int{98, 99}) >> omit[int_ >> int_],
                space, pi
            ));
            CHECK(pi == pair_int{98, 99});
        }
    }
}

TEST_CASE("partial success (list-like)")
{
    using x4::char_encoding::standard;

    using x4::standard::char_;
    using x4::standard::lit;
    using x4::omit;

    constexpr auto a = char_('a');
    constexpr auto b = char_('b');
    constexpr auto c = char_('c');
    [[maybe_unused]] constexpr auto abc = a >> b >> c;

    {
        using Subject = x4::sequence<
            x4::sequence<
                x4::literal_char<standard>,
                x4::literal_char<standard>
            >,
            x4::literal_char<standard>
        >;
        STATIC_CHECK(std::same_as<Subject::attribute_type, alloy::tuple<char, char, char>>);
        STATIC_CHECK(x4::detail::container_can_hold_sequence<std::string, alloy::tuple<char, char, char>>::value);

        using Parser = x4::kleene<Subject>;
        using Container = std::string;

        static_assert(std::same_as<std::remove_const_t<decltype(abc)>, Subject>);
        static_assert(std::same_as<decltype(*abc), Parser>);

        STATIC_CHECK(x4::parser_traits<Subject>::template handles_container<Container>);
        STATIC_CHECK(x4::parser_traits<Parser>::template handles_container<Container>);

        // TODO
        //std::string abcs;
        //REQUIRE(parse("abcabx", *abc >> lit("abx"), abcs));
        //CHECK(abcs == "abc"sv); // wrong implementation yields "abcab"
    }
    //{
    //    std::string abcs;
    //    REQUIRE(parse("abcabx", +abc >> "abx", abcs));
    //    CHECK(abcs == "abc"sv); // wrong implementation yields "abcab"
    //}
}

// NOLINTEND(readability-container-size-empty)
