/*=============================================================================
    Copyright (c) 2001-2015 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>
#include <iris/x4/attribute/as.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/directive/lexeme.hpp>
#include <iris/x4/primitive/eps.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/delimited_list.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/kleene.hpp>
#include <iris/x4/core/detail/parse_into_container.hpp>

#include <iris/alloy/adapted/std_pair.hpp>
#include <iris/alloy/tuple.hpp>
#include <iris/rvariant.hpp>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <string_view>
#include <type_traits>

namespace x4 = iris::x4;

namespace {

using char_pair = iris::alloy::tuple<char, char>;
using char_pairs_parser = std::remove_const_t<decltype(+(x4::standard::char_ >> x4::standard::char_))>;

// assignable from the whole attribute of `char_pairs_parser` and from `char`, holding neither by its shape
struct converted
{
    int from_pairs = 0;
    int from_char = 0;

    converted& operator=(std::vector<char_pair> const&) { ++from_pairs; return *this; }
    converted& operator=(char) { ++from_char; return *this; }
};

template<class Container>
constexpr x4::detail::container_parse_strategy char_pairs_parse_for = x4::detail::container_parse_strategy_for<char_pairs_parser, Container>;

// made from a `char` by a converting constructor, with or without a default constructor
struct from_char
{
    from_char() = default;
    from_char(char c) : c(c) {} // NOLINT(misc-explicit-constructor)
    char c = 0;
};

struct from_char_only
{
    from_char_only(char c) : c(c) {} // NOLINT(misc-explicit-constructor)
    char c;
};

} // anonymous

constexpr x4::rule<class pair_rule, std::pair<std::string, std::string>> pair_rule("pair");
constexpr x4::rule<class string_rule, std::string> string_rule("string");

constexpr auto string_rule_def = x4::lexeme[*x4::standard::alnum];
constexpr auto pair_rule_def = string_rule >> x4::lit('=') >> string_rule;

IRIS_X4_DEFINE(string_rule)
IRIS_X4_DEFINE(pair_rule)

constexpr auto as_string_parser = x4::as<std::string>(x4::lexeme[*x4::standard::alnum]);
constexpr auto as_pair_parser = x4::as<std::pair<std::string, std::string>>(as_string_parser >> x4::lit('=') >> as_string_parser);

template<class Container>
void test_map_support()
{
    // rule version
    {
        constexpr auto rule = pair_rule % x4::lit(',');
        Container actual;
        REQUIRE(parse("k1=v1,k2=v2,k2=v3", rule, actual));
        CHECK(actual.size() == 2);
        CHECK(actual == Container{{"k1", "v1"}, {"k2", "v2"}});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = pair_rule >> ',' >> pair_rule >> ',' >> pair_rule;
        Container container;
        CHECK(parse("k1=v1,k2=v2,k2=v3", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = pair_rule >> +(',' >> pair_rule);
        Container container;
        CHECK(parse("k1=v1,k2=v2,k2=v3", cic_rule, container));
    }

    // as version
    {
        constexpr auto rule = as_pair_parser % x4::lit(',');
        Container actual;
        REQUIRE(parse("k1=v1,k2=v2,k2=v3", rule, actual));
        CHECK(actual.size() == 2);
        CHECK(actual == Container{{"k1", "v1"}, {"k2", "v2"}});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = as_pair_parser >> ',' >> as_pair_parser >> ',' >> as_pair_parser;
        Container container;
        CHECK(parse("k1=v1,k2=v2,k2=v3", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = as_pair_parser >> +(',' >> as_pair_parser);
        Container container;
        CHECK(parse("k1=v1,k2=v2,k2=v3", cic_rule, container));
    }
}

template<class Container>
void test_multimap_support()
{
    // rule version
    {
        constexpr auto rule = pair_rule % x4::lit(',');
        Container actual;
        REQUIRE(parse("k1=v1,k2=v2,k2=v3", rule, actual));
        CHECK(actual.size() == 3);
        CHECK(actual == Container{{"k1", "v1"}, {"k2", "v2"}, {"k2", "v3"}});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = pair_rule >> ',' >> pair_rule >> ',' >> pair_rule;
        Container container;
        CHECK(parse("k1=v1,k2=v2,k2=v3", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = pair_rule >> +(',' >> pair_rule);
        Container container;
        CHECK(parse("k1=v1,k2=v2,k2=v3", cic_rule, container));
    }

    // as version
    {
        constexpr auto rule = as_pair_parser % x4::lit(',');
        Container actual;
        REQUIRE(parse("k1=v1,k2=v2,k2=v3", rule, actual));
        CHECK(actual.size() == 3);
        CHECK(actual == Container{ {"k1", "v1"}, {"k2", "v2"}, {"k2", "v3"} });
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = as_pair_parser >> ',' >> as_pair_parser >> ',' >> as_pair_parser;
        Container container;
        CHECK(parse("k1=v1,k2=v2,k2=v3", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = as_pair_parser >> +(',' >> as_pair_parser);
        Container container;
        CHECK(parse("k1=v1,k2=v2,k2=v3", cic_rule, container));
    }
}

template<class Container>
void test_sequence_support()
{
    // rule version
    {
        constexpr auto rule = string_rule % x4::lit(',');
        Container actual;
        REQUIRE(parse("e1,e2,e2", rule, actual));
        CHECK(actual.size() == 3);
        CHECK(actual == Container{"e1", "e2", "e2"});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = string_rule >> ',' >> string_rule >> ',' >> string_rule;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = string_rule >> +(',' >> string_rule);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }

    // as version
    {
        constexpr auto rule = as_string_parser % x4::lit(',');
        Container actual;
        REQUIRE(parse("e1,e2,e2", rule, actual));
        CHECK(actual.size() == 3);
        CHECK(actual == Container{ "e1", "e2", "e2" });
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = as_string_parser >> ',' >> as_string_parser >> ',' >> as_string_parser;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = as_string_parser >> +(',' >> as_string_parser);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }
}

template<class Container>
void test_set_support()
{
    // rule version
    {
        constexpr auto rule = string_rule % x4::lit(',');
        Container actual;
        REQUIRE(parse("e1,e2,e2", rule, actual));
        CHECK(actual.size() == 2);
        CHECK(actual == Container{"e1", "e2"});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = string_rule >> ',' >> string_rule >> ',' >> string_rule;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = string_rule >> +(',' >> string_rule);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }

    // as version
    {
        constexpr auto rule = as_string_parser % x4::lit(',');
        Container actual;
        REQUIRE(parse("e1,e2,e2", rule, actual));
        CHECK(actual.size() == 2);
        CHECK(actual == Container{ "e1", "e2" });
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = as_string_parser >> ',' >> as_string_parser >> ',' >> as_string_parser;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = as_string_parser >> +(',' >> as_string_parser);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }
}

template<class Container>
void test_multiset_support()
{
    // rule version
    {
        constexpr auto rule = string_rule % x4::lit(',');
        Container actual;
        REQUIRE(parse("e1,e2,e2", rule, actual));
        CHECK(actual.size() == 3);
        CHECK(actual == Container{"e1", "e2", "e2"});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = string_rule >> ',' >> string_rule >> ',' >> string_rule;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = string_rule >> +(',' >> string_rule);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }

    // as version
    {
        constexpr auto rule = as_string_parser % x4::lit(',');
        Container actual;
        REQUIRE(parse("e1,e2,e2", rule, actual));
        CHECK(actual.size() == 3);
        CHECK(actual == Container{"e1", "e2", "e2"});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = as_string_parser >> ',' >> as_string_parser >> ',' >> as_string_parser;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = as_string_parser >> +(',' >> as_string_parser);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }
}

template<class Container>
void test_string_support()
{
    // rule version
    {
        constexpr auto rule = string_rule % x4::lit(',');
        Container container;
        REQUIRE(parse("e1,e2,e2", rule, container));
        CHECK(container.size() == 6);
        CHECK(container == Container{"e1e2e2"});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = string_rule >> ',' >> string_rule >> ',' >> string_rule;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = string_rule >> +(',' >> string_rule);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }

    // as version
    {
        constexpr auto rule = as_string_parser % x4::lit(',');
        Container container;
        REQUIRE(parse("e1,e2,e2", rule, container));
        CHECK(container.size() == 6);
        CHECK(container == Container{"e1e2e2"});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = as_string_parser >> ',' >> as_string_parser >> ',' >> as_string_parser;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = as_string_parser >> +(',' >> as_string_parser);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }
}

TEST_CASE("container_support")
{
    using x4::traits::X4Container;

    // ------------------------------------------------------------------

    STATIC_CHECK(X4Container<std::string>);
    STATIC_CHECK(X4Container<std::vector<int>>);
    STATIC_CHECK(X4Container<std::deque<int>>);
    STATIC_CHECK(X4Container<std::list<int>>);

    // ------------------------------------------------------------------

    STATIC_CHECK(X4Container<std::set<int>>);
    STATIC_CHECK(X4Container<std::unordered_set<int>>);
    STATIC_CHECK(X4Container<std::multiset<int>>);
    STATIC_CHECK(X4Container<std::unordered_multiset<int>>);
    STATIC_CHECK(X4Container<std::map<int, int>>);
    STATIC_CHECK(X4Container<std::unordered_map<int, int>>);
    STATIC_CHECK(X4Container<std::multimap<int, int>>);
    STATIC_CHECK(X4Container<std::unordered_multimap<int, int>>);

    // ------------------------------------------------------------------

    test_string_support<std::string>();

    test_sequence_support<std::vector<std::string>>();
    test_sequence_support<std::list<std::string>>();
    test_sequence_support<std::deque<std::string>>();

    test_set_support<std::set<std::string>>();
    test_set_support<std::unordered_set<std::string>>();

    test_multiset_support<std::multiset<std::string>>();
    test_multiset_support<std::unordered_multiset<std::string>>();

    test_map_support<std::map<std::string, std::string>>();
    test_map_support<std::unordered_map<std::string, std::string>>();

    test_multimap_support<std::multimap<std::string, std::string>>();
    test_multimap_support<std::unordered_multimap<std::string, std::string>>();

    {
        // the container held by a variant is appended to, not replaced
        constexpr std::string_view input = "cd";
        iris::rvariant<int, std::string> v = std::string("ab");

        auto first = input.begin();
        REQUIRE(x4::detail::parse_into_container(x4::standard::char_, first, input.end(), x4::unused, v));
        REQUIRE(x4::detail::parse_into_container(x4::standard::char_, first, input.end(), x4::unused, v));
        CHECK(first == input.end());
        CHECK(v == iris::rvariant<int, std::string>{std::string("abcd")});
    }
}

TEST_CASE("container_parse_strategy_for")
{
    using x4::detail::container_parse_strategy;
    using iris::rvariant;

    STATIC_CHECK(char_pairs_parse_for<std::string> == container_parse_strategy::container_itself);
    STATIC_CHECK(char_pairs_parse_for<std::vector<std::vector<char_pair>>> == container_parse_strategy::as_part);
    STATIC_CHECK(char_pairs_parse_for<std::vector<rvariant<char_pair, std::vector<char_pair>>>> == container_parse_strategy::as_part);
    STATIC_CHECK(char_pairs_parse_for<std::vector<rvariant<char, std::vector<char_pair>>>> == container_parse_strategy::as_part);

    // A new plain element is constructed from the value, never constructed by default and assigned
    STATIC_CHECK(char_pairs_parse_for<std::vector<converted>> == container_parse_strategy::none);
    using char_parser_type = std::remove_const_t<decltype(x4::standard::char_)>;
    STATIC_CHECK(x4::detail::container_parse_strategy_for<char_parser_type, std::vector<from_char>> == container_parse_strategy::as_part);
    STATIC_CHECK(x4::detail::container_parse_strategy_for<char_parser_type, std::vector<from_char_only>> == container_parse_strategy::as_part);

    constexpr auto char_pairs = x4::eps >> +(x4::standard::char_ >> x4::standard::char_);
    {
        std::string s;
        REQUIRE(parse("abcd", char_pairs, s));
        CHECK(s == "abcd");
    }
    {
        std::vector<rvariant<char_pair, std::vector<char_pair>>> v;
        REQUIRE(parse("abcd", char_pairs, v));
        REQUIRE(v.size() == 1);
        CHECK(iris::get<std::vector<char_pair>>(v[0]).size() == 2);
    }
    {
        std::vector<rvariant<char, std::vector<char_pair>>> v;
        REQUIRE(parse("abcd", char_pairs, v));
        REQUIRE(v.size() == 1);
        CHECK(iris::get<std::vector<char_pair>>(v[0]).size() == 2);
    }
}
