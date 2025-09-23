#ifndef BOOST_SPIRIT_X4_NUMERIC_UINT_HPP
#define BOOST_SPIRIT_X4_NUMERIC_UINT_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2011 Jan Frederick Eick
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
struct uint_parser : parser<uint_parser<T, Radix, MinDigits, MaxDigits>>
{
    // check template parameter 'Radix' for validity
    static_assert((Radix >= 2 && Radix <= 36), "Unsupported Radix");

    using attribute_type = T;

    static constexpr bool has_attribute = true;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr)
        noexcept(
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(numeric::extract_uint<T, Radix, MinDigits, MaxDigits>::call(first, last, attr))
        )
    {
        x4::skip_over(first, last, ctx);
        return numeric::extract_uint<T, Radix, MinDigits, MaxDigits>::call(first, last, attr);
    }
};

inline namespace cpos {

inline constexpr uint_parser<unsigned short> ushort_{};
inline constexpr uint_parser<unsigned int> uint_{};
inline constexpr uint_parser<unsigned long> ulong_{};
inline constexpr uint_parser<unsigned long long> ulong_long{};

inline constexpr uint_parser<std::uint8_t> uint8{};
inline constexpr uint_parser<std::uint16_t> uint16{};
inline constexpr uint_parser<std::uint32_t> uint32{};
inline constexpr uint_parser<std::uint64_t> uint64{};

inline constexpr uint_parser<unsigned, 2> bin{};
inline constexpr uint_parser<unsigned, 8> oct{};
inline constexpr uint_parser<unsigned, 16> hex{};

} // cpos

} // boost::spirit::x4

#endif
