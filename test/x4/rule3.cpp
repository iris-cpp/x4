/*=============================================================================
    Copyright (c) 2001-2012 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "iris_x4_test.hpp"

#include <iris/x4/rule.hpp>
#include <iris/x4/auxiliary/eps.hpp>
#include <iris/x4/char/char.hpp>
#include <iris/x4/char/char_class.hpp>
#include <iris/x4/numeric/int.hpp>
#include <iris/x4/operator/list.hpp>
#include <iris/x4/operator/alternative.hpp>
#include <iris/x4/operator/sequence.hpp>
#include <iris/x4/operator/plus.hpp>

#include <iris/rvariant/rvariant.hpp>
#include <iris/rvariant/recursive_wrapper.hpp>

#include <iris/alloy/adapted/std_pair.hpp>
#include <iris/alloy/adapt.hpp>

#include <string>
#include <vector>
#include <cstring>

#ifdef _MSC_VER
// bogus https://developercommunity.visualstudio.com/t/buggy-warning-c4709/471956
# pragma warning(disable: 4709) // comma operator within array index expression
#endif

namespace check_stationary {

x4::rule<class a_r, x4_test::stationary> const a;
x4::rule<class b_r, x4_test::stationary> const b;

auto const a_def = '{' >> x4::int_ >> '}';
auto const b_def = a;

IRIS_X4_DEFINE(a)
IRIS_X4_DEFINE(b)

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

x4::rule<class recursive_grammar_r, node_t> const grammar;

auto const grammar_def = '[' >> grammar % ',' >> ']' | x4::int_;

IRIS_X4_DEFINE(grammar)

} // check_recursive

namespace check_recursive_scoped {

using check_recursive::node_t;
using check_recursive::node_array;

x4::rule<class intvec_r, node_t> const intvec;
auto const grammar = intvec = '[' >> intvec % ',' >> ']' | x4::int_;

} // check_recursive_scoped

struct recursive_tuple
{
    int value;
    std::vector<recursive_tuple> children;
};

template<>
struct iris::alloy::adaptor<recursive_tuple> {
    using getters_list = make_getters_list<&recursive_tuple::value, &recursive_tuple::children>;
};

// regression test for #461
namespace check_recursive_tuple {

using iterator_type = std::string_view::const_iterator;

x4::rule<class recursive_tuple_grammar_r, recursive_tuple> const grammar;
auto const grammar_def = x4::int_ >> ('{' >> grammar % ',' >> '}' | x4::eps);
IRIS_X4_DEFINE(grammar)

IRIS_X4_INSTANTIATE(decltype(grammar), iterator_type, x4::parse_context_for<iterator_type>)

} // check_recursive_tuple

TEST_CASE("rule3")
{
    using namespace x4::standard;
    using x4::rule;
    using x4::lit;
    using x4::eps;
    using x4::_rule_var;
    using x4::_attr;

    // synth attribute value-init
    {
        std::string s;
        using rule_type = rule<class r, std::string>;

        auto rdef = rule_type{} = alpha[([](auto&& ctx) {
            x4::_rule_var(ctx) += x4::_attr(ctx);
        })];

        REQUIRE(parse("abcdef", +rdef, s));
        CHECK(s == "abcdef");
    }

    // synth attribute value-init
    {
        std::string s;
        using rule_type = rule<class r, std::string>;

        auto rdef = rule_type() =
            alpha[([](auto&& ctx) {
                _rule_var(ctx) += _attr(ctx);
            })];

        REQUIRE(parse("abcdef", +rdef, s));
        CHECK(s == "abcdef");
    }

    {
        auto r = rule<class r_id, int>{} = eps[([] ([[maybe_unused]] auto&& ctx) {
            static_assert(
                std::is_same_v<std::decay_t<decltype(_rule_var(ctx))>, unused_type>,
                "Attr must not be synthesized"
            );
        })];
        CHECK(parse("", r));
    }

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
