#ifndef BOOST_SPIRIT_X4_STRING_LITERAL_STRING_HPP
#define BOOST_SPIRIT_X4_STRING_LITERAL_STRING_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/spirit/x4/string/detail/string_parse.hpp>

#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/skip_over.hpp>
#include <boost/spirit/x4/core/unused.hpp>

#include <boost/spirit/x4/string/case_compare.hpp>
#include <boost/spirit/x4/string/utf8.hpp>

#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

template<class String, class Encoding, X4Attribute Attr = std::basic_string<typename Encoding::char_type>>
struct literal_string : parser<literal_string<String, Encoding, Attr>>
{
    static_assert(
        !std::is_pointer_v<std::decay_t<String>>,
        "`literal_string` for raw character pointer/array is banned; it has an undetectable risk of holding a dangling pointer."
    );
    static_assert(std::is_convertible_v<String, std::basic_string_view<typename String::value_type>>);

    using char_type = typename Encoding::char_type;
    using encoding = Encoding;
    using attribute_type = Attr;
    static constexpr bool has_attribute = !std::is_same_v<unused_type, attribute_type>;
    static constexpr bool handles_container = has_attribute;

    template<class... Args>
        requires std::is_constructible_v<String, Args...>
    constexpr literal_string(Args&&... args)
        noexcept(std::is_nothrow_constructible_v<String, Args...>)
        : str(std::forward<Args>(args)...)
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr_>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, Attr_& attr) const
        noexcept(
            noexcept(x4::skip_over(first, last, context)) &&
            noexcept(detail::string_parse(str, first, last, attr, x4::get_case_compare<encoding>(context)))
        )
    {
        x4::skip_over(first, last, context);
        return detail::string_parse(str, first, last, attr, x4::get_case_compare<encoding>(context));
    }

    String str;
};

template<class String, class Encoding, X4Attribute Attr>
struct get_info<literal_string<String, Encoding, Attr>>
{
    using result_type = std::string;
    [[nodiscard]] constexpr std::string operator()(literal_string<String, Encoding, Attr> const& p) const
    {
        return '"' + x4::to_utf8(p.str) + '"';
    }
};

} // boost::spirit::x4

#endif
