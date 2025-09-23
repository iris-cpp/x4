#ifndef BOOST_SPIRIT_X4_NUMERIC_INT_HPP
#define BOOST_SPIRIT_X4_NUMERIC_INT_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/skip_over.hpp>
#include <boost/spirit/x4/numeric/utils/extract_int.hpp>

#include <iterator>
#include <cstdint>

// TODO: use `std::from_chars`

namespace boost::spirit::x4 {

template<
    class T,
    unsigned Radix = 10,
    unsigned MinDigits = 1,
    int MaxDigits = -1
>
struct int_parser : parser<int_parser<T, Radix, MinDigits, MaxDigits>>
{
    // check template parameter 'Radix' for validity
    static_assert(
        Radix == 2 || Radix == 8 || Radix == 10 || Radix == 16,
        "Unsupported Radix"
    );

    using attribute_type = T;
    static constexpr bool has_attribute = true;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr)
        noexcept(
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(numeric::extract_int<T, Radix, MinDigits, MaxDigits>::call(first, last, attr))
        )
    {
        x4::skip_over(first, last, ctx);
        return numeric::extract_int<T, Radix, MinDigits, MaxDigits>::call(first, last, attr);
    }
};

inline namespace cpos {

inline constexpr int_parser<short> short_{};
inline constexpr int_parser<int> int_{};
inline constexpr int_parser<long> long_{};
inline constexpr int_parser<long long> long_long{};

inline constexpr int_parser<std::int8_t> int8{};
inline constexpr int_parser<std::int16_t> int16{};
inline constexpr int_parser<std::int32_t> int32{};
inline constexpr int_parser<std::int64_t> int64{};

} // cpos

} // boost::spirit::x4

#endif
