#ifndef IRIS_X4_NUMERIC_UTILS_EXTRACT_INT_HPP
#define IRIS_X4_NUMERIC_UTILS_EXTRACT_INT_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2011 Jan Frederick Eick
    Copyright (c) 2011 Christopher Jefferson
    Copyright (c) 2006 Stephen Nutt
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/move_to.hpp>

#include <iris/x4/traits/numeric_traits.hpp>
#include <iris/x4/traits/char_encoding_traits.hpp>

#include <concepts>
#include <limits>
#include <iterator>
#include <type_traits>
#include <utility>

#include <cassert>

namespace iris::x4::numeric {

namespace detail {

// The maximum radix digits that can be represented without
// overflow:
//
//         template<class T, unsigned Radix>
//         struct digits_traits::value;
//
template<class T, unsigned Radix>
struct digits_traits;

template<int Digits, unsigned Radix>
struct digits2_to_n;

// lookup table for log2(x) : 2 <= x <= 36
template<int Digits> struct digits2_to_n<Digits,  2> { static constexpr int value = static_cast<int>((Digits * 1000000) / 1000000); };
template<int Digits> struct digits2_to_n<Digits,  3> { static constexpr int value = static_cast<int>((Digits * 1000000) / 1584960); };
template<int Digits> struct digits2_to_n<Digits,  4> { static constexpr int value = static_cast<int>((Digits * 1000000) / 2000000); };
template<int Digits> struct digits2_to_n<Digits,  5> { static constexpr int value = static_cast<int>((Digits * 1000000) / 2321920); };
template<int Digits> struct digits2_to_n<Digits,  6> { static constexpr int value = static_cast<int>((Digits * 1000000) / 2584960); };
template<int Digits> struct digits2_to_n<Digits,  7> { static constexpr int value = static_cast<int>((Digits * 1000000) / 2807350); };
template<int Digits> struct digits2_to_n<Digits,  8> { static constexpr int value = static_cast<int>((Digits * 1000000) / 3000000); };
template<int Digits> struct digits2_to_n<Digits,  9> { static constexpr int value = static_cast<int>((Digits * 1000000) / 3169920); };
template<int Digits> struct digits2_to_n<Digits, 10> { static constexpr int value = static_cast<int>((Digits * 1000000) / 3321920); };
template<int Digits> struct digits2_to_n<Digits, 11> { static constexpr int value = static_cast<int>((Digits * 1000000) / 3459430); };
template<int Digits> struct digits2_to_n<Digits, 12> { static constexpr int value = static_cast<int>((Digits * 1000000) / 3584960); };
template<int Digits> struct digits2_to_n<Digits, 13> { static constexpr int value = static_cast<int>((Digits * 1000000) / 3700430); };
template<int Digits> struct digits2_to_n<Digits, 14> { static constexpr int value = static_cast<int>((Digits * 1000000) / 3807350); };
template<int Digits> struct digits2_to_n<Digits, 15> { static constexpr int value = static_cast<int>((Digits * 1000000) / 3906890); };
template<int Digits> struct digits2_to_n<Digits, 16> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4000000); };
template<int Digits> struct digits2_to_n<Digits, 17> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4087460); };
template<int Digits> struct digits2_to_n<Digits, 18> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4169920); };
template<int Digits> struct digits2_to_n<Digits, 19> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4247920); };
template<int Digits> struct digits2_to_n<Digits, 20> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4321920); };
template<int Digits> struct digits2_to_n<Digits, 21> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4392310); };
template<int Digits> struct digits2_to_n<Digits, 22> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4459430); };
template<int Digits> struct digits2_to_n<Digits, 23> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4523560); };
template<int Digits> struct digits2_to_n<Digits, 24> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4584960); };
template<int Digits> struct digits2_to_n<Digits, 25> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4643850); };
template<int Digits> struct digits2_to_n<Digits, 26> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4700430); };
template<int Digits> struct digits2_to_n<Digits, 27> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4754880); };
template<int Digits> struct digits2_to_n<Digits, 28> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4807350); };
template<int Digits> struct digits2_to_n<Digits, 29> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4857980); };
template<int Digits> struct digits2_to_n<Digits, 30> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4906890); };
template<int Digits> struct digits2_to_n<Digits, 31> { static constexpr int value = static_cast<int>((Digits * 1000000) / 4954190); };
template<int Digits> struct digits2_to_n<Digits, 32> { static constexpr int value = static_cast<int>((Digits * 1000000) / 5000000); };
template<int Digits> struct digits2_to_n<Digits, 33> { static constexpr int value = static_cast<int>((Digits * 1000000) / 5044390); };
template<int Digits> struct digits2_to_n<Digits, 34> { static constexpr int value = static_cast<int>((Digits * 1000000) / 5087460); };
template<int Digits> struct digits2_to_n<Digits, 35> { static constexpr int value = static_cast<int>((Digits * 1000000) / 5129280); };
template<int Digits> struct digits2_to_n<Digits, 36> { static constexpr int value = static_cast<int>((Digits * 1000000) / 5169925); };


template<class T, unsigned Radix>
struct digits_traits : digits2_to_n<std::numeric_limits<T>::digits, Radix>
{
    static_assert(std::numeric_limits<T>::radix == 2, "Radix should be 2");
};

template<class T>
struct digits_traits<T, 10>
{
    static constexpr int value = std::numeric_limits<T>::digits10;
};

// Traits class for radix specific number conversion
template<unsigned Radix>
struct radix_traits
{
    template<class Char>
    [[nodiscard]] static constexpr bool is_valid(Char ch) noexcept
    {
        return (ch >= '0' && ch <= (Radix > 10 ? '9' : static_cast<Char>('0' + Radix -1)))
            || (Radix > 10 && ch >= 'a' && ch <= static_cast<Char>('a' + Radix -10 -1))
            || (Radix > 10 && ch >= 'A' && ch <= static_cast<Char>('A' + Radix -10 -1));
    }

    template<class Char>
    [[nodiscard]] static constexpr unsigned digit(Char ch)
        noexcept(noexcept(traits::char_encoding_traits<Char>::encoding_type::tolower(ch)))
    {
        return (Radix <= 10 || (ch >= '0' && ch <= '9'))
            ? ch - '0'
            : traits::char_encoding_traits<Char>::encoding_type::tolower(ch) - 'a' + 10;
    }
};

// Accumulator policies for extracting integer from a positive number.
template<unsigned Radix>
struct positive_accumulator
{
    template<class T, class Char>
    static constexpr void unchecked_add(T& n, Char ch)
        noexcept(noexcept(radix_traits<Radix>::digit(ch)))
    {
        n = n * T(Radix) + T(radix_traits<Radix>::digit(ch));
    }

    template<class T, class Char>
    [[nodiscard]] static constexpr bool checked_add(T& n, Char ch)
        noexcept(noexcept(radix_traits<Radix>::digit(ch)))
    {
        // Ensure n *= Radix will not overflow
        T const max = (std::numeric_limits<T>::max)();
        T const val = max / Radix;
        if (n > val) return false;

        T tmp = n * Radix;

        // Ensure n += digit will not overflow
        int const digit = radix_traits<Radix>::digit(ch);
        if (tmp > max - digit) return false;

        n = tmp + static_cast<T>(digit);
        return true;
    }
};

// Accumulator policies for extracting integer from a negative number.
template<unsigned Radix>
struct negative_accumulator
{
    template<class T, class Char>
    static constexpr void unchecked_add(T& n, Char ch)
        noexcept(noexcept(radix_traits<Radix>::digit(ch)))
    {
        n = n * T(Radix) - T(radix_traits<Radix>::digit(ch));
    }

    template<class T, class Char>
    [[nodiscard]] static constexpr bool checked_add(T& n, Char ch)
        noexcept(noexcept(radix_traits<Radix>::digit(ch)))
    {
        // Ensure n *= Radix will not underflow
        T const min = (std::numeric_limits<T>::min)();
        T const val = min / T(Radix);
        if (n < val) return false;

        T tmp = n * Radix;

        // Ensure n -= digit will not underflow
        int const digit = radix_traits<Radix>::digit(ch);
        if (tmp < min + digit) return false;

        n = tmp - static_cast<T>(digit);
        return true;
    }
};

// Common code for `extract_int::parse` specializations
template<unsigned Radix, class Accumulator, int MaxDigits>
struct int_extractor
{
    template<class T>
    static constexpr bool need_check_overflow =
        (
            MaxDigits < 0 ||
            MaxDigits > digits_traits<T, Radix>::value
        ) &&
        traits::check_overflow<T>::value;

    template<class Char, class T>
        requires need_check_overflow<T>
    [[nodiscard]] static constexpr bool
    call(Char ch, std::size_t count, T& n)
        noexcept(
            noexcept(Accumulator::unchecked_add(n, ch)) &&
            noexcept(Accumulator::checked_add(n, ch))
        )
    {
        constexpr std::size_t overflow_free = digits_traits<T, Radix>::value - 1;

        if (count < overflow_free) {
            Accumulator::unchecked_add(n, ch);
        } else {
            if (!Accumulator::checked_add(n, ch)) {
                return false; // overflow/underflow
            }
        }
        return true;
    }

    template<class Char, class T>
        requires (!need_check_overflow<T>)
    [[nodiscard]] static constexpr bool
    call(Char ch, std::size_t /*count*/, T& n)
        noexcept(noexcept(Accumulator::unchecked_add(n, ch)))
    {
        Accumulator::unchecked_add(n, ch);
        return true;
    }

    template<class Char>
    [[nodiscard]] static constexpr bool
    call(Char /*ch*/, std::size_t /*count*/, unused_type const&) noexcept
    {
        return true;
    }
};

// End of loop checking: check if the number of digits
// being parsed exceeds `MaxDigits`. Note: if `MaxDigits == -1`
// we don't do any checking.
template<int MaxDigits>
struct check_max_digits
{
    [[nodiscard]] static constexpr bool
    call(std::size_t count) noexcept
    {
        return count < static_cast<std::size_t>(MaxDigits); // bounded
    }
};

template<>
struct check_max_digits<-1>
{
    [[nodiscard]] static constexpr bool
    call(std::size_t /*count*/) noexcept
    {
        return true; // unbounded
    }
};

// extract_int: main code for extracting integers
template<
    class T, unsigned Radix, unsigned MinDigits, int MaxDigits,
    class Accumulator = positive_accumulator<Radix>,
    bool Accumulate = false
>
struct extract_int
{
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4127)   // conditional expression is constant
# pragma warning(disable: 4459)   // declaration hides global declaration
#endif
    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    parse_main(It& first, Se const& last, Attr& attr)
        // TODO: noexcept
    {
        using radix_check = radix_traits<Radix>;
        using extractor = int_extractor<Radix, Accumulator, MaxDigits>;
        using char_type = std::iter_value_t<It>;

        It it = first;
        std::size_t leading_zeros = 0;
        if constexpr (!Accumulate) {
            // skip leading zeros
            while (it != last && *it == '0' && leading_zeros < static_cast<std::size_t>(MaxDigits)) {
                ++it;
                ++leading_zeros;
            }
        }

        Attr val = Accumulate ? attr : Attr{};
        std::size_t count = 0;
        char_type ch;

        while (true) {
            if (!check_max_digits<MaxDigits>::call(count + leading_zeros) || it == last) {
                break;
            }
            ch = *it;

            if (!radix_check::is_valid(ch) || !extractor::call(ch, count, val)) {
                break;
            }
            ++it;
            ++count;
        }

        if (count + leading_zeros >= MinDigits) {
            x4::move_to(std::move(val), attr);
            first = it;
            return true;
        }
        return false;
    }
#ifdef _MSC_VER
# pragma warning(pop)
#endif

    template<std::forward_iterator It, std::sentinel_for<It> Se>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, unused_type const&)
        noexcept(
            std::is_nothrow_constructible_v<T, int> &&
            noexcept(extract_int::parse_main(first, last, std::declval<T&>()))
        )
    {
        if constexpr (std::same_as<T, unused_type>) {
            return extract_int::parse_main(first, last, unused);
        } else {
            T n(0); // must calculate value to detect over/underflow
            return extract_int::parse_main(first, last, n);
        }
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Attr& attr)
        noexcept(noexcept(extract_int::parse_main(first, last, attr)))
    {
        return extract_int::parse_main(first, last, attr);
    }
};
#undef IRIS_X4_NUMERIC_INNER_LOOP

// extract_int: main code for extracting integers
// common case where MinDigits == 1 and MaxDigits = -1

template<class T, unsigned Radix, class Accumulator, bool Accumulate>
struct extract_int<T, Radix, 1, -1, Accumulator, Accumulate>
{
#ifdef _MSC_VER
# pragma warning(push)
# pragma warning(disable: 4127)   // conditional expression is constant
# pragma warning(disable: 4459)   // declaration hides global declaration
#endif

    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    parse_main(It& first, Se const& last, Attr& attr)
        // TODO: noexcept
    {
        using radix_check = radix_traits<Radix>;
        using extractor = int_extractor<Radix, Accumulator, -1>;
        using char_type = std::iter_value_t<It>;

        It it = first;
        std::size_t count = 0;
        if constexpr (!Accumulate) {
            // skip leading zeros
            while (it != last && *it == '0') {
                ++it;
                ++count;
            }

            if (it == last) {
                if (count == 0) return false; // must have at least one digit
                if constexpr (!std::is_same_v<std::remove_const_t<Attr>, unused_type>) {
                    attr = 0;
                }
                first = it;
                return true;
            }
        }

        Attr val = Accumulate ? attr : Attr{};

        char_type ch = *it;
        if (!radix_check::is_valid(ch) || !extractor::call(ch, 0, val)) {
            if (count == 0) return false; // must have at least one digit
            x4::move_to(std::move(val), attr);
            first = it;
            return true;
        }

        count = 0;
        ++it;
        while (true) {
            if (it == last) break;
            ch = *it;
            if (!radix_check::is_valid(ch)) break;
            if (!extractor::call(ch, count, val)) return false;
            ++it;
            ++count;
        }

        x4::move_to(std::move(val), attr);
        first = it;
        return true;
    }
#ifdef _MSC_VER
# pragma warning(pop)
#endif

    template<std::forward_iterator It, std::sentinel_for<It> Se>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, unused_type)
        noexcept(
            std::is_nothrow_constructible_v<T, int> &&
            noexcept(extract_int::parse_main(first, last, std::declval<T&>()))
        )
    {
        if constexpr (std::same_as<T, unused_type>) {
            return extract_int::parse_main(first, last, unused);
        } else {
            T n(0); // must calculate value to detect over/underflow
            return extract_int::parse_main(first, last, n);
        }
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Attr& attr)
        noexcept(noexcept(extract_int::parse_main(first, last, attr)))
    {
        return extract_int::parse_main(first, last, attr);
    }
};

#undef IRIS_X4_NUMERIC_INNER_LOOP

//  Extract the prefix sign (- or +), return true if a '-' was found
template<std::forward_iterator It, std::sentinel_for<It> Se>
[[nodiscard]] constexpr bool
extract_sign(It& first, Se const& last)
    noexcept(
        noexcept(*first) &&
        noexcept(++first)
    )
{
    (void)last;
    assert(first != last); // precondition

    // Extract the sign
    bool const neg = *first == '-';
    if (neg || *first == '+') {
        ++first;
        return neg;
    }
    return false;
}

} // detail

// Low level unsigned integer parser
template<class T, unsigned Radix, unsigned MinDigits, int MaxDigits, bool Accumulate = false>
struct extract_uint
{
    // check template parameter 'Radix' for validity
    static_assert(Radix >= 2 && Radix <= 36, "Unsupported Radix");

    using extract_type = detail::extract_int<
        T, Radix, MinDigits, MaxDigits,
        detail::positive_accumulator<Radix>,
        Accumulate
    >;

    template<std::forward_iterator It, std::sentinel_for<It> Se>
    [[nodiscard]] static constexpr bool
    call(It& first, Se const& last, T& attr)
        noexcept(std::is_nothrow_copy_assignable_v<It> && noexcept(extract_type::parse(first, last, attr)))
    {
        if (first == last) return false;

        It const save = first;
        if (!extract_type::parse(first, last, attr)) {
            first = save;
            return false;
        }
        return true;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    call(It& first, Se const& last, Attr& attr)
        noexcept(
            std::is_nothrow_default_constructible_v<T> &&
            noexcept(extract_uint::call(first, last, std::declval<T&>())) &&
            noexcept(x4::move_to(std::declval<T&&>(), attr))
        )
    {
        // this case is called when Attribute is not T
        T tmp_attr; // default initialize
        if (extract_uint::call(first, last, tmp_attr)) {
            x4::move_to(std::move(tmp_attr), attr);
            return true;
        }
        return false;
    }
};

// Low level signed integer parser
template<class T, unsigned Radix, unsigned MinDigits, int MaxDigits>
struct extract_int
{
    // check template parameter 'Radix' for validity
    static_assert(
        Radix == 2 || Radix == 8 || Radix == 10 || Radix == 16,
        "Unsupported Radix"
    );

    using extract_pos_type = detail::extract_int<T, Radix, MinDigits, MaxDigits>;
    using extract_neg_type = detail::extract_int<T, Radix, MinDigits, MaxDigits, detail::negative_accumulator<Radix>>;

    template<std::forward_iterator It, std::sentinel_for<It> Se>
    [[nodiscard]] static constexpr bool
    call(It& first, Se const& last, T& attr)
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            noexcept(detail::extract_sign(first, last)) &&
            noexcept(extract_neg_type::parse(first, last, attr)) &&
            noexcept(extract_pos_type::parse(first, last, attr))
        )
    {
        if (first == last) return false;
        It const save = first;

        bool hit = detail::extract_sign(first, last);
        if (hit) {
            hit = extract_neg_type::parse(first, last, attr);
        } else {
            hit = extract_pos_type::parse(first, last, attr);
        }

        if (!hit) {
            first = save;
            return false;
        }
        return true;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    call(It& first, Se const& last, Attr& attr)
        noexcept(
            std::is_nothrow_default_constructible_v<T> &&
            noexcept(extract_int::call(first, last, std::declval<T&>())) &&
            noexcept(x4::move_to(std::declval<T&&>(), attr))
        )
    {
        // this case is called when Attribute is not T
        T tmp_attr; // default initialize
        if (extract_int::call(first, last, tmp_attr)) {
            x4::move_to(std::move(tmp_attr), attr);
            return true;
        }
        return false;
    }
};

} // iris::x4::numeric

#endif
