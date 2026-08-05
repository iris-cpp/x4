#ifndef IRIS_ZZ_X4_NUMERIC_UINT_HPP
#define IRIS_ZZ_X4_NUMERIC_UINT_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2011 Jan Frederick Eick
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/numeric/utils/extract_int.hpp>

#include <concepts>
#include <iterator>
#include <cstdint>

// TODO: use `std::from_chars`

namespace iris::x4 {

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
            std::is_nothrow_copy_assignable_v<It> &&
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(numeric::extract_uint<T, Radix, MinDigits, MaxDigits>::call(first, last, attr))
        )
    {
        auto it = first;
        x4::skip_over(it, last, ctx);
        bool const ok = numeric::extract_uint<T, Radix, MinDigits, MaxDigits>::call(it, last, attr);
        if (ok) first = it;
        return ok;
    }

    [[nodiscard]] static std::string get_x4_info()
    {
        if constexpr (MinDigits == 1 && MaxDigits == -1) {
            if constexpr (Radix == 10) {
                if constexpr (sizeof(T) == 1) {
                    return "`uint8`";
                } else if constexpr (sizeof(T) == 2) {
                    return "`uint16`";
                } else if constexpr (sizeof(T) == 4) {
                    return "`uint32`";
                } else if constexpr (sizeof(T) == 8) {
                    return "`uint64`";
                } else {
                    static_assert(false, "sorry; unimplemented");
                    return {};
                }
            } else if constexpr (Radix == 2) {
                return "`bin`";
            } else if constexpr (Radix == 8) {
                return "`oct`";
            } else if constexpr (Radix == 16) {
                return "`hex`";
            } else {
                static_assert(false, "sorry; unimplemented");
                return {};
            }

        } else {
            static_assert(false, "sorry; unimplemented");
            return {};
        }
    }
};

namespace parsers {

[[maybe_unused]] inline constexpr uint_parser<unsigned short> ushort_{};
[[maybe_unused]] inline constexpr uint_parser<unsigned int> uint_{};
[[maybe_unused]] inline constexpr uint_parser<unsigned long> ulong_{};
[[maybe_unused]] inline constexpr uint_parser<unsigned long long> ulong_long{};

[[maybe_unused]] inline constexpr uint_parser<std::uint8_t> uint8{};
[[maybe_unused]] inline constexpr uint_parser<std::uint16_t> uint16{};
[[maybe_unused]] inline constexpr uint_parser<std::uint32_t> uint32{};
[[maybe_unused]] inline constexpr uint_parser<std::uint64_t> uint64{};

[[maybe_unused]] inline constexpr uint_parser<unsigned, 2> bin{};
[[maybe_unused]] inline constexpr uint_parser<unsigned, 8> oct{};
[[maybe_unused]] inline constexpr uint_parser<unsigned, 16> hex{};

} // parsers

using parsers::ushort_;
using parsers::uint_;
using parsers::ulong_;
using parsers::ulong_long;

using parsers::uint8;
using parsers::uint16;
using parsers::uint32;
using parsers::uint64;

using parsers::bin;
using parsers::oct;
using parsers::hex;

} // iris::x4

#endif
