/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef BOOST_SPIRIT_X3_UNUSED_APRIL_16_2006_0616PM
#define BOOST_SPIRIT_X3_UNUSED_APRIL_16_2006_0616PM

#include <iosfwd> // TODO: remove this, iosfwd is heavier than most people might think
#include <type_traits>
#include <utility>

namespace boost::spirit::x3
{
    struct unused_type
    {
        constexpr explicit unused_type() noexcept = default;

        template <typename T>
        constexpr unused_type(T&&) noexcept
        {
            // consume arbitrary type
        }

        template <typename T>
        constexpr unused_type&
        operator=(T&&) noexcept
        {
            // consume arbitrary type
            return *this;
        }

        template <typename T>
        constexpr unused_type const&
        operator=(T&&) const noexcept
        {
            // consume arbitrary type
            return *this;
        }

        // unused_type can also masquerade as an empty context (see context.hpp)

        template <typename ID>
        [[nodiscard]] constexpr unused_type get(ID&&) const noexcept
        {
            return unused_type{};
        }

        friend std::ostream& operator<<(std::ostream& os, unused_type const&) noexcept
        {
            return os;
        }

        friend std::istream& operator>>(std::istream& is, unused_type&) noexcept
        {
            return is;
        }
    };

    struct unused_container_type : unused_type
    {
        // constexpr explicit unused_container_type() noexcept = default;
        using unused_type::unused_type;
    };

    inline namespace cpos
    {
        inline constexpr unused_type unused{};
        inline constexpr unused_container_type unused_container{};
    } // cpos

} // boost::spirit::x3

#endif
