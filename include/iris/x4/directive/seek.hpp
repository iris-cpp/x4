#ifndef IRIS_X4_DIRECTIVE_SEEK_HPP
#define IRIS_X4_DIRECTIVE_SEEK_HPP

/*=============================================================================
    Copyright (c) 2011 Jamboree
    Copyright (c) 2014 Lee Clagett
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/expectation.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject>
struct seek_directive : proxy_parser<Subject, seek_directive<Subject>>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        for (It current(first); ; ++current) {
            if (this->subject.parse(current, last, ctx, attr)) {
                first = current;
                return true;
            }

            if constexpr (has_context_v<Context, contexts::expectation_failure>) {
                if (x4::has_expectation_failure(ctx)) {
                    return false;
                }
            }

            // fail only after subject fails & no input
            if (current == last) return false;
        }
    }
};

namespace detail {

struct seek_gen
{
    template<class Subject>
    [[nodiscard]] constexpr seek_directive<as_parser_plain_t<Subject>>
    operator[](Subject&& subject) const // TODO: MSVC does not support static operator[]
        noexcept(is_parser_nothrow_constructible_v<seek_directive<as_parser_plain_t<Subject>>, Subject>)
    {
        return {as_parser(std::forward<Subject>(subject))};
    }
};

} // detail

namespace parsers::directive {

[[maybe_unused]] inline constexpr detail::seek_gen seek{};

} // parsers::directive

using parsers::directive::seek;

} // iris::x4

#endif
