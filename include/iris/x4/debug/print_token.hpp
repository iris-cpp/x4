#ifndef IRIS_X4_DEBUG_PRINT_TOKEN_HPP
#define IRIS_X4_DEBUG_PRINT_TOKEN_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
================================================_==============================*/

#include <type_traits>
#include <ios>

#include <cctype>

namespace iris::x4::traits {

namespace detail {

// generate debug output for lookahead token (character) stream
struct token_printer_debug_for_chars
{
    template<class Char>
    static void print(std::ostream& os, Char c)
    {
        using namespace std;    // allow for ADL to find the proper iscntrl

        switch (c) {
        case '\a': os << "\\a"; break;
        case '\b': os << "\\b"; break;
        case '\f': os << "\\f"; break;
        case '\n': os << "\\n"; break;
        case '\r': os << "\\r"; break;
        case '\t': os << "\\t"; break;
        case '\v': os << "\\v"; break;
        default:
            if (c >= 0 && c < 127) {
                if (iscntrl(c)) {
                    os << "\\" << std::oct << int(c);
                } else if (isprint(c)) {
                    os << char(c);
                } else {
                    os << "\\x" << std::hex << int(c);
                }

            } else {
                os << "\\x" << std::hex << int(c);
            }
            break;
        }
    }
};

// for token types where the comparison with char constants wouldn't work
struct token_printer_debug
{
    template<class T>
    static void print(std::ostream& os, T const& val)
    {
        os << val;
    }
};

} // detail

template<class T>
struct token_printer_debug
    : std::conditional_t<
        std::is_convertible_v<T, char> && std::is_convertible_v<char, T>,
        detail::token_printer_debug_for_chars,
        detail::token_printer_debug
    >
{};

template<class T>
void print_token(std::ostream& os, T const& val)
{
    token_printer_debug<T>::print(os, val);
}

} // iris::x4::traits

#endif
