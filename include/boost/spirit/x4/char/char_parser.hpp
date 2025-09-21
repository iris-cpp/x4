#ifndef BOOST_SPIRIT_X4_CHAR_CHAR_PARSER_HPP
#define BOOST_SPIRIT_X4_CHAR_CHAR_PARSER_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/skip_over.hpp>
#include <boost/spirit/x4/core/move_to.hpp>

#include <iterator>

namespace boost::spirit::x4 {

template <class Derived>
struct char_parser : parser<Derived>
{
    template <std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attribute>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, Attribute& attr) const
        // TODO: noexcept
    {
        x4::skip_over(first, last, context);

        if (first != last && this->derived().test(*first, context)) {
            x4::move_to(std::iter_value_t<It>{*first}, attr);
            ++first;
            return true;
        }
        return false;
    }
};

} // boost::spirit::x4

#endif
