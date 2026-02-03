#ifndef BOOST_SPIRIT_X4_OPERATOR_DIFFERENCE_HPP
#define BOOST_SPIRIT_X4_OPERATOR_DIFFERENCE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/parser.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

template<class Left, class Right>
struct difference : binary_parser<Left, Right, difference<Left, Right>>
{
    using attribute_type = parser_traits<Left>::attribute_type;

    static constexpr bool handles_container = Left::handles_container;

    using binary_parser<Left, Right, difference>::binary_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        // Try Right first
        It start = first;
        if (this->right.parse(first, last, ctx, unused)) {
            // Right succeeds, we fail.
            first = start;
            return false;
        }

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            // In case of `Left - expect[r]`,
            // if Right yielded expectation error,
            // the whole difference expression (*this) should also yield error.
            // In other words, when the THROW macro was 1 (i.e. traditional behavior),
            // Right should already have thrown an exception.
            if (x4::has_expectation_failure(ctx)) {
                // don't rollback iterator (mimicking exception-like behavior)
                return false;
            }
        }

        // Right fails, now try Left
        return this->left.parse(first, last, ctx, attr);
    }
};

template<X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr difference<as_parser_plain_t<Left>, as_parser_plain_t<Right>>
operator-(Left&& left, Right&& right)
    noexcept(
        is_parser_nothrow_castable_v<Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<
            difference<as_parser_plain_t<Left>, as_parser_plain_t<Right>>,
            as_parser_t<Left>,
            as_parser_t<Right>
        >
    )
{
    return {as_parser(std::forward<Left>(left)), as_parser(std::forward<Right>(right))};
}

} // boost::spirit::x4

#endif
