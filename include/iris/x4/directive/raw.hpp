#ifndef IRIS_X4_DIRECTIVE_RAW_HPP
#define IRIS_X4_DIRECTIVE_RAW_HPP

/*=============================================================================
    Copyright (c) 2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/move_to.hpp>

#include <ranges>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace detail {

// Pseudo attribute type indicating that the parser wants the
// iterator range pointing to the [first, last) matching characters from
// the input iterators.
struct raw_attribute_t {};

} // detail

template<class Subject>
struct raw_directive : unary_parser<Subject, raw_directive<Subject>>
{
    using attribute_type = detail::raw_attribute_t;

    static constexpr bool handles_container = true;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        // never noexcept; construction of `std::ranges::subrange` is never noexcept
    {
        static_assert(Parsable<Subject, It, Se, Context, unused_type>);

        x4::skip_over(first, last, ctx);
        It local_it = first;
        if (!this->subject.parse(local_it, last, ctx, unused)) return false;

        x4::move_to(first, local_it, attr);
        first = local_it;
        return true;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, unused_type) const
        noexcept(is_nothrow_parsable_v<Subject, It, Se, Context, unused_type>)
    {
        return this->subject.parse(first, last, ctx, unused);
    }
};

namespace detail {

struct raw_gen
{
    template<X4Subject Subject>
    [[nodiscard]] constexpr raw_directive<as_parser_plain_t<Subject>>
    operator[](Subject&& subject) const
        noexcept(is_parser_nothrow_constructible_v<raw_directive<as_parser_plain_t<Subject>>, Subject>)
    {
        return {as_parser(std::forward<Subject>(subject))};
    }
};

} // detail

namespace parsers::directive {

[[maybe_unused]] inline constexpr detail::raw_gen raw{};

} // parsers::directive

using parsers::directive::raw;

} // iris::x4

namespace iris::x4::traits {

template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
struct pseudo_attribute<It, Se, Context, x4::detail::raw_attribute_t>
{
    using actual_type = std::ranges::subrange<It, Se>;

    [[nodiscard]] static constexpr actual_type
    make_actual_type(It& first, Se const& last, Context const&, x4::detail::raw_attribute_t)
    {
        return {first, last};
    }
};

} // iris::x4::traits

#endif
