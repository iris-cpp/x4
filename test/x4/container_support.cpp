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
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/directive/as.hpp>
#include <iris/x4/directive/lexeme.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/list.hpp>
#include <iris/x4/operator/plus.hpp>
#include <iris/x4/operator/kleene.hpp>

#include <iris/alloy/adapted/std_pair.hpp>

#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <deque>
#include <list>
#include <string>
#include <type_traits>

namespace x4 = iris::x4;

namespace {

constexpr x4::rule<class pair_rule, std::pair<std::string, std::string>> pair_rule("pair");
constexpr auto string_parser = x4::as<std::string>(x4::lexeme[*x4::standard::alnum]);

constexpr auto pair_rule_def = string_parser >> x4::lit('=') >> string_parser;

IRIS_X4_DEFINE(pair_rule)

template<class Container>
void test_map_support()
{
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
}

template<class Container>
void test_multimap_support()
{
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
}

template<class Container>
void test_sequence_support()
{
    {
        constexpr auto rule = string_parser % x4::lit(',');
        Container actual;
        REQUIRE(parse("e1,e2,e2", rule, actual));
        CHECK(actual.size() == 3);
        CHECK(actual == Container{"e1", "e2", "e2"});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = string_parser >> ',' >> string_parser >> ',' >> string_parser;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = string_parser >> +(',' >> string_parser);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }
}

template<class Container>
void test_set_support()
{
    {
        constexpr auto rule = string_parser % x4::lit(',');
        Container actual;
        REQUIRE(parse("e1,e2,e2", rule, actual));
        CHECK(actual.size() == 2);
        CHECK(actual == Container{"e1", "e2"});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = string_parser >> ',' >> string_parser >> ',' >> string_parser;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = string_parser >> +(',' >> string_parser);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }
}

template<class Container>
void test_multiset_support()
{
    {
        constexpr auto rule = string_parser % x4::lit(',');
        Container actual;
        REQUIRE(parse("e1,e2,e2", rule, actual));
        CHECK(actual.size() == 3);
        CHECK(actual == Container{"e1", "e2", "e2"});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = string_parser >> ',' >> string_parser >> ',' >> string_parser;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = string_parser >> +(',' >> string_parser);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }
}

template<class Container>
void test_string_support()
{
    {
        constexpr auto rule = string_parser % x4::lit(',');
        Container container;
        REQUIRE(parse("e1,e2,e2", rule, container));
        CHECK(container.size() == 6);
        CHECK(container == Container{"e1e2e2"});
    }
    {
        // test sequences parsing into containers
        constexpr auto seq_rule = string_parser >> ',' >> string_parser >> ',' >> string_parser;
        Container container;
        CHECK(parse("e1,e2,e2", seq_rule, container));
    }
    {
        // test parsing container into container
        constexpr auto cic_rule = string_parser >> +(',' >> string_parser);
        Container container;
        CHECK(parse("e1,e2,e2", cic_rule, container));
    }
}

} // anonymous

TEST_CASE("container_support")
{
    using x4::traits::is_container_v;
    using x4::traits::is_associative_v;

    // ------------------------------------------------------------------

    STATIC_CHECK(is_container_v<std::string>);
    STATIC_CHECK(!is_associative_v<std::string>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::string>>);
    STATIC_CHECK(!is_associative_v<x4::container_appender<std::string>>);

    STATIC_CHECK(is_container_v<std::vector<int>>);
    STATIC_CHECK(!is_associative_v<std::vector<int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::vector<int>>>);
    STATIC_CHECK(!is_associative_v<x4::container_appender<std::vector<int>>>);

    STATIC_CHECK(is_container_v<std::deque<int>>);
    STATIC_CHECK(!is_associative_v<std::deque<int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::deque<int>>>);
    STATIC_CHECK(!is_associative_v<x4::container_appender<std::deque<int>>>);

    STATIC_CHECK(is_container_v<std::list<int>>);
    STATIC_CHECK(!is_associative_v<std::list<int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::list<int>>>);
    STATIC_CHECK(!is_associative_v<x4::container_appender<std::list<int>>>);

    // ------------------------------------------------------------------

    STATIC_CHECK(is_container_v<std::set<int>>);
    STATIC_CHECK(is_associative_v<std::set<int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::set<int>>>);
    STATIC_CHECK(is_associative_v<x4::container_appender<std::set<int>>>);

    STATIC_CHECK(is_container_v<std::unordered_set<int>>);
    STATIC_CHECK(is_associative_v<std::unordered_set<int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::unordered_set<int>>>);
    STATIC_CHECK(is_associative_v<x4::container_appender<std::unordered_set<int>>>);

    STATIC_CHECK(is_container_v<std::multiset<int>>);
    STATIC_CHECK(is_associative_v<std::multiset<int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::multiset<int>>>);
    STATIC_CHECK(is_associative_v<x4::container_appender<std::multiset<int>>>);

    STATIC_CHECK(is_container_v<std::unordered_multiset<int>>);
    STATIC_CHECK(is_associative_v<std::unordered_multiset<int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::unordered_multiset<int>>>);
    STATIC_CHECK(is_associative_v<x4::container_appender<std::unordered_multiset<int>>>);

    STATIC_CHECK(is_container_v<std::map<int,int>>);
    STATIC_CHECK(is_associative_v<std::map<int,int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::map<int,int>>>);
    STATIC_CHECK(is_associative_v<x4::container_appender<std::map<int,int>>>);

    STATIC_CHECK(is_container_v<std::unordered_map<int,int>>);
    STATIC_CHECK(is_associative_v<std::unordered_map<int,int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::unordered_map<int,int>>>);
    STATIC_CHECK(is_associative_v<x4::container_appender<std::unordered_map<int,int>>>);

    STATIC_CHECK(is_container_v<std::multimap<int,int>>);
    STATIC_CHECK(is_associative_v<std::multimap<int,int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::multimap<int,int>>>);
    STATIC_CHECK(is_associative_v<x4::container_appender<std::multimap<int,int>>>);

    STATIC_CHECK(is_container_v<std::unordered_multimap<int,int>>);
    STATIC_CHECK(is_associative_v<std::unordered_multimap<int,int>>);
    STATIC_CHECK(is_container_v<x4::container_appender<std::unordered_multimap<int,int>>>);
    STATIC_CHECK(is_associative_v<x4::container_appender<std::unordered_multimap<int,int>>>);

    // ------------------------------------------------------------------

    test_string_support<std::string>();

    test_sequence_support<std::vector<std::string>>();
    test_sequence_support<std::list<std::string>>();
    test_sequence_support<std::deque<std::string>>();

    test_set_support<std::set<std::string>>();
    test_set_support<std::unordered_set<std::string>>();

    test_multiset_support<std::multiset<std::string>>();
    test_multiset_support<std::unordered_multiset<std::string>>();

    test_map_support<std::map<std::string,std::string>>();
    test_map_support<std::unordered_map<std::string,std::string>>();

    test_multimap_support<std::multimap<std::string,std::string>>();
    test_multimap_support<std::unordered_multimap<std::string,std::string>>();
}
