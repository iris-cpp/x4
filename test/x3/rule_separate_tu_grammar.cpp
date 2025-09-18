/*=============================================================================
    Copyright (c) 2019 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "rule_separate_tu_grammar.hpp"

#include <boost/spirit/x4.hpp>

namespace unused_attr {

using iterator_type = std::string_view::const_iterator;

auto const skipper_def = x3::standard::lit('*');
BOOST_SPIRIT_X3_DEFINE(skipper)
BOOST_SPIRIT_X3_INSTANTIATE(skipper_type, iterator_type, x3::parse_context_for<iterator_type>)

auto const skipper2_def = x3::standard::lit('#');
BOOST_SPIRIT_X3_DEFINE(skipper2)
BOOST_SPIRIT_X3_INSTANTIATE(skipper2_type, iterator_type, x3::parse_context_for<iterator_type>)

auto const grammar_def = *x3::standard::lit('=');
BOOST_SPIRIT_X3_DEFINE(grammar)

BOOST_SPIRIT_X3_INSTANTIATE(grammar_type, iterator_type, x3::parse_context_for<iterator_type>)
BOOST_SPIRIT_X3_INSTANTIATE(grammar_type, iterator_type, x3::phrase_parse_context_for<skipper_type, iterator_type>)
BOOST_SPIRIT_X3_INSTANTIATE(grammar_type, iterator_type, x3::phrase_parse_context_for<skipper2_type, iterator_type>)

}

namespace used_attr {

using iterator_type = std::string_view::const_iterator;

auto const skipper_def = x3::standard::space;
BOOST_SPIRIT_X3_DEFINE(skipper)
BOOST_SPIRIT_X3_INSTANTIATE(skipper_type, iterator_type, x3::parse_context_for<iterator_type>)

auto const grammar_def = x3::int_;
BOOST_SPIRIT_X3_DEFINE(grammar)
BOOST_SPIRIT_X3_INSTANTIATE(grammar_type, iterator_type, x3::parse_context_for<iterator_type>)

BOOST_SPIRIT_X3_INSTANTIATE(grammar_type, iterator_type, x3::phrase_parse_context_for<skipper_type, iterator_type>)

}
