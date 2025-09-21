#ifndef BOOST_SPIRIT_X4_STRING_DETAIL_STRING_PARSE_HPP
#define BOOST_SPIRIT_X4_STRING_DETAIL_STRING_PARSE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/spirit/x4/core/move_to.hpp>

#include <string_view>
#include <iterator>

namespace boost::spirit::x4::detail {

template <class CharT, class CharTraitsT, std::forward_iterator It, std::sentinel_for<It> Se, class Attribute, class CaseCompareFunc>
[[nodiscard]] constexpr bool
string_parse(
    std::basic_string_view<CharT, CharTraitsT> const str,
    It& first, Se const& last,
    Attribute& attr, CaseCompareFunc const& compare
) noexcept(std::is_same_v<std::remove_const_t<Attribute>, unused_type>)
{
    It i = first;
    auto stri = str.begin();
    auto str_last = str.end();

    for (; stri != str_last; ++stri, ++i) {
        if (i == last || compare(*stri, *i) != 0) {
            return false;
        }
    }

    x4::move_to(first, i, x4::assume_container(attr));
    first = i;
    return true;
}

template <class CharT, class CharTraitsT, std::forward_iterator It, std::sentinel_for<It> Se, class Attribute, class CaseCompareFunc>
[[nodiscard]] constexpr bool
string_parse(
    std::basic_string<CharT, CharTraitsT> const& str,
    It& first, Se const& last,
    Attribute& attr, CaseCompareFunc const& compare
) noexcept(std::is_same_v<std::remove_const_t<Attribute>, unused_type>)
{
    return detail::string_parse(std::basic_string_view{str}, first, last, attr, compare);
}

template <class CharT, class CharTraitsT, std::forward_iterator It, std::sentinel_for<It> Se, class Attribute>
[[nodiscard]] constexpr bool
string_parse(
    std::basic_string_view<CharT, CharTraitsT> const ucstr,
    std::basic_string_view<CharT, CharTraitsT> const lcstr,
    It& first, Se const& last, Attribute& attr
) noexcept(std::is_same_v<std::remove_const_t<Attribute>, unused_type>)
{
    auto uc_i = ucstr.begin();
    auto uc_last = ucstr.end();
    auto lc_i = lcstr.begin();
    It i = first;

    for (; uc_i != uc_last; ++uc_i, ++lc_i, ++i) {
        if (i == last || (*uc_i != *i && *lc_i != *i)) {
            return false;
        }
    }
    x4::move_to(first, i, x4::assume_container(attr));
    first = i;
    return true;
}

template <class CharT, class CharTraitsT, std::forward_iterator It, std::sentinel_for<It> Se, class Attribute>
[[nodiscard]] constexpr bool
string_parse(
    std::basic_string<CharT, CharTraitsT> const& ucstr,
    std::basic_string<CharT, CharTraitsT> const& lcstr,
    It& first, Se const& last, Attribute& attr
) noexcept(std::is_same_v<std::remove_const_t<Attribute>, unused_type>)
{
    return detail::string_parse(std::basic_string_view{ucstr}, std::basic_string_view{lcstr}, first, last, attr);
}

} // boost::spirit::x4::detail

#endif
