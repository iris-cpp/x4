/*=============================================================================
    Copyright (c) 2001-2012 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/primitive/eps.hpp>
#include <iris/x4/operator/delimited_list.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/operator/sequence.hpp>

#include <iris/rvariant/rvariant.hpp>
#include <iris/rvariant/recursive_wrapper.hpp>

#include <iris/alloy/adapted/std_pair.hpp>
#include <iris/alloy/adapt.hpp>

#include <vector>

#ifdef _MSC_VER
// bogus https://developercommunity.visualstudio.com/t/buggy-warning-c4709/471956
# pragma warning(disable: 4709) // comma operator within array index expression
#endif

namespace check_stationary {

IRIS_X4_DECLARE(a, x4_test::stationary);
IRIS_X4_DECLARE(b, x4_test::stationary);

constexpr auto a_def = '{' >> x4::int_ >> '}';
constexpr auto b_def = a;

IRIS_X4_DEFINE(a);
IRIS_X4_DEFINE(b);

} // check_stationary

namespace check_recursive {

struct node_array;

using node_t = iris::rvariant<
    int,
    iris::recursive_wrapper<node_array>
>;

struct node_array : std::vector<node_t>
{
    using std::vector<node_t>::vector;
};

IRIS_X4_DECLARE(grammar, node_t);
constexpr auto grammar_def = '[' >> grammar % ',' >> ']' | x4::int_;
IRIS_X4_DEFINE(grammar);

} // check_recursive

namespace check_recursive_scoped {

using check_recursive::node_t;
using check_recursive::node_array;

IRIS_X4_DECLARE(intvec, node_t);
constexpr auto intvec_def = x4::int_;
IRIS_X4_DEFINE(intvec);

constexpr auto grammar = '[' >> intvec % ',' >> ']' | x4::int_;

} // check_recursive_scoped

struct recursive_tuple
{
    int value;
    std::vector<recursive_tuple> children;
};

template<>
struct alloy::adaptor<recursive_tuple>
{
    using getters_list = iris::constant_list<&recursive_tuple::value, &recursive_tuple::children>;
};

// regression test for #461
namespace check_recursive_tuple {

using iterator_type = std::string_view::const_iterator;

IRIS_X4_DECLARE(grammar, recursive_tuple);
constexpr auto grammar_def = x4::int_ >> ('{' >> grammar % ',' >> '}' | x4::eps);
IRIS_X4_DEFINE(grammar);

IRIS_X4_INSTANTIATE(grammar, iterator_type, x4::parse_context_for<iterator_type>);

} // check_recursive_tuple

TEST_CASE("rule3")
{
    using namespace x4::standard;
    using x4::rule;
    using x4::lit;
    using x4::eps;
    using x4::_rule_var;
    using x4::_attr;

    // ensure no unneeded synthesization, copying and moving occurred
    {
        x4_test::stationary st { 0 };
        REQUIRE(parse("{42}", check_stationary::b, st));
        CHECK(st.val == 42);
    }

    {
        using namespace check_recursive;

        node_t v;
        REQUIRE(parse("[4,2]", grammar, v));
        CHECK((node_t{node_array{{4}, {2}}} == v));
    }
    {
        using namespace check_recursive_scoped;
        node_t v;
        REQUIRE(parse("[4,2]", grammar, v));
        CHECK((node_t{node_array{{4}, {2}}} == v));
    }
}
