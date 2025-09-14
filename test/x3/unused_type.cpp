/*=============================================================================
    Copyright (c) 2019 Nikita Kniazev
    Copyright (c) 2025 Nana Sakisaka

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/home/x3/support/unused.hpp>

#include <type_traits>
#include <concepts>
#include <iostream>

void test_use(boost::spirit::x3::unused_type) {}

int main()
{
    using boost::spirit::x3::unused;
    using boost::spirit::x3::unused_type;

    static_assert(std::is_trivial_v<unused_type>);

    static_assert(std::assignable_from<unused_type&, unused_type>);
    static_assert(std::assignable_from<unused_type&, unused_type const>);
    static_assert(std::assignable_from<unused_type&, unused_type&>);
    static_assert(std::assignable_from<unused_type&, unused_type const&>);

    static_assert(std::assignable_from<unused_type const&, unused_type>);
    static_assert(std::assignable_from<unused_type const&, unused_type const>);
    static_assert(std::assignable_from<unused_type const&, unused_type&>);
    static_assert(std::assignable_from<unused_type const&, unused_type const&>);

    unused_type unused_mut;
    static_assert(std::is_same_v<decltype((unused)), unused_type const&>);
    static_assert(std::is_same_v<decltype((unused_mut)), unused_type&>);
    static_assert(std::is_same_v<decltype(unused = 123), unused_type const&>);
    static_assert(std::is_same_v<decltype(unused = unused), unused_type const&>);
    static_assert(std::is_same_v<decltype(unused = unused_mut), unused_type const&>);
    static_assert(std::is_same_v<decltype(unused_mut = 123), unused_type&>);
    static_assert(std::is_same_v<decltype(unused_mut = unused), unused_type&>);
    static_assert(std::is_same_v<decltype(unused_mut = unused_mut), unused_type&>);

    test_use(0);
    test_use(unused);
    test_use(unused_mut);

    std::cout << unused;
    std::cout << unused_mut;
    std::cin >> unused_mut;
}
