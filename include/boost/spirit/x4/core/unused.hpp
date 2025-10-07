#ifndef BOOST_SPIRIT_X4_CORE_UNUSED_HPP
#define BOOST_SPIRIT_X4_CORE_UNUSED_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/spirit/config.hpp>

#include <type_traits>

namespace boost::spirit::x4 {

template<class ID, class T, class Next>
struct context;

struct unused_type
{
    constexpr explicit unused_type() noexcept = default;

    // unused_type can masquerade as an empty context (see context.hpp)

    template<class ID, class T, class Next>
    [[deprecated("If your parser don't need a `context`, just accept it by `auto const&` and discard it.")]]
    /* not explicit */ constexpr unused_type(context<ID, T, Next> const&) noexcept
    {}

    [[nodiscard]] static constexpr unused_type get(auto) noexcept
    {
        return unused_type{};
    }
};

// The attribute category type for `unused_container_type` is
// `container_attribute`, but it does not satisfy `is_container`.
struct unused_container_type
{
    constexpr explicit unused_container_type() noexcept = default;
};

inline namespace cpos {

[[maybe_unused]] inline constexpr unused_type unused{};
[[maybe_unused]] inline constexpr unused_container_type unused_container{};

} // cpos

template<class T>
    requires (!std::is_same_v<std::remove_const_t<T>, unused_type>)
[[nodiscard]] constexpr T&
assume_container(T& attr) noexcept
{
    return attr;
}

[[nodiscard]] constexpr unused_container_type const&
assume_container(unused_type const&) noexcept
{
    return unused_container;
}

} // boost::spirit::x4

#endif
