#ifndef IRIS_X4_CHAR_DETAIL_CHAR_RANGE_HPP
#define IRIS_X4_CHAR_DETAIL_CHAR_RANGE_HPP

/*=============================================================================
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <type_traits>
#include <utility>

namespace boost::spirit::x4::detail {

// A closed range [first, last]
template<class CharT>
struct char_range
{
    static_assert(std::is_default_constructible_v<CharT>);
    static_assert(std::is_copy_constructible_v<CharT> && std::is_copy_assignable_v<CharT>);

    using value_type = CharT;

    constexpr char_range() noexcept(std::is_nothrow_default_constructible_v<CharT>) = default;

    template<class A, class B>
        requires std::is_constructible_v<CharT, A> && std::is_constructible_v<CharT, B>
    constexpr char_range(A&& first, B&& last)
        noexcept(std::is_nothrow_constructible_v<CharT, A> && std::is_nothrow_constructible_v<CharT, B>)
        : first(std::forward<A>(first))
        , last(std::forward<B>(last))
    {}

    CharT first{};
    CharT last{};
};

} // boost::spirit::x4::detail

#endif
