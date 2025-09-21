#ifndef BOOST_SPIRIT_X4_AUXILIARY_EOI_HPP
#define BOOST_SPIRIT_X4_AUXILIARY_EOI_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/spirit/x4/core/skip_over.hpp>
#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/unused.hpp>

#include <iterator>

namespace boost::spirit::x4 {

struct eoi_parser : parser<eoi_parser>
{
    using attribute_type = unused_type;

    static constexpr bool has_attribute = false;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, Attr&) const
        noexcept(
            noexcept(x4::skip_over(first, last, context)) &&
            noexcept(first == last)
        )
    {
        x4::skip_over(first, last, context);
        return first == last;
    }
};

template<>
struct get_info<eoi_parser>
{
    using result_type = std::string;
    [[nodiscard]] result_type operator()(eoi_parser const &) const { return "eoi"; }
};

inline namespace cpos {

inline constexpr eoi_parser eoi{};

} // cpos

} // boost::spirit::x4

#endif
