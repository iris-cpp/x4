/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_SPIRIT_X4_SIMPLE_TRACE_DECEMBER_06_2008_1102AM
#define BOOST_SPIRIT_X4_SIMPLE_TRACE_DECEMBER_06_2008_1102AM

#include <boost/spirit/x4/core/unused.hpp>

#include <boost/spirit/x4/debug/print_token.hpp>
#include <boost/spirit/x4/debug/print_attribute.hpp>
#include <boost/spirit/x4/debug/debug_handler_state.hpp>

#include <iostream>
#include <type_traits>

//  The stream to use for debug output
#ifndef BOOST_SPIRIT_X4_DEBUG_OUT
#define BOOST_SPIRIT_X4_DEBUG_OUT std::cerr
#endif

//  number of tokens to print while debugging
#ifndef BOOST_SPIRIT_X4_DEBUG_PRINT_SOME
#define BOOST_SPIRIT_X4_DEBUG_PRINT_SOME 20
#endif

//  number of spaces to indent
#ifndef BOOST_SPIRIT_X4_DEBUG_INDENT
#define BOOST_SPIRIT_X4_DEBUG_INDENT 2
#endif

namespace boost::spirit::x4
{
    namespace detail
    {
        template <typename Char>
        void token_printer(std::ostream& o, Char c)
        {
            // allow customization of the token printer routine
            traits::print_token(o, c);
        }
    }

    template <int IndentSpaces = 2, int CharsToPrint = 20>
    struct simple_trace
    {
        explicit simple_trace(std::ostream& out) noexcept
            : out(out)
        {}

        void print_indent(int n) const
        {
            n *= IndentSpaces;
            for (int i = 0; i != n; ++i)
            {
                out << ' ';
            }
        }

        template <std::forward_iterator It, std::sentinel_for<It> Se>
        void print_some(
            char const* tag,
            It first, Se last
        ) const
        {
            simple_trace::print_indent(indent);

            out << '<' << tag << '>';

            for (int i = 0; first != last && i != CharsToPrint && *first; ++i, ++first)
            {
                detail::token_printer(out, *first);
            }
            out << "</" << tag << '>' << std::endl;

            // $$$ FIXME convert invalid xml characters (e.g. '<') to valid
            // character entities. $$$
        }

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Attribute>
        void operator()(
            It first,
            Se last,
            Attribute const& attr,
            debug_handler_state const state,
            std::string const& rule_name
        ) const
        {
            using enum debug_handler_state;
            switch (state)
            {
                case pre_parse:
                    print_indent(indent++);
                    out
                        << '<' << rule_name << '>'
                        << std::endl;
                    print_some("try", first, last);
                    break;

                case successful_parse:
                    print_some("success", first, last);
                    if constexpr (!std::is_same_v<Attribute, unused_type>)
                    {
                        print_indent(indent);
                        out
                            << "<attributes>";
                        traits::print_attribute(out, attr);
                        out
                            << "</attributes>";
                        out << std::endl;
                    }
                    print_indent(--indent);
                    out
                        << "</" << rule_name << '>'
                        << std::endl;
                    break;

                case failed_parse:
                    print_indent(indent);
                    out << "<fail/>" << std::endl;
                    print_indent(--indent);
                    out
                        << "</" << rule_name << '>'
                        << std::endl;
                    break;
            }
        }

        std::ostream& out;
        mutable int indent = 0;
    };

    namespace detail
    {
        using simple_trace_type = simple_trace<
            BOOST_SPIRIT_X4_DEBUG_INDENT,
            BOOST_SPIRIT_X4_DEBUG_PRINT_SOME
        >;

        [[nodiscard]] inline simple_trace_type&
        get_simple_trace()
        {
            static simple_trace_type tracer(BOOST_SPIRIT_X4_DEBUG_OUT);
            return tracer;
        }

    } // detail

} // boost::spirit::x4

#endif
