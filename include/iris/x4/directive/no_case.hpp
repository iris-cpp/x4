#ifndef IRIS_ZZ_X4_DIRECTIVE_NO_CASE_HPP
#define IRIS_ZZ_X4_DIRECTIVE_NO_CASE_HPP

/*=============================================================================
    Copyright (c) 2014 Thomas Bernard
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/char/case_compare.hpp>

#include <iris/x4/core/context.hpp>
#include <iris/x4/core/parser.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace detail {

template<class Context>
[[nodiscard]] constexpr decltype(auto) make_no_case_context(Context const& ctx) noexcept
{
    // Declare a concrete alias type; MSVC prints the alias instead of actual type,
    // which makes the compilation error significantly shorter.
    using T = std::remove_cvref_t<decltype(x4::replace_first_or_prepend_context<case_compare_tag>(ctx, case_compare_no_case))>;
    return detail::named_context<T>(x4::replace_first_or_prepend_context<case_compare_tag>(ctx, case_compare_no_case));
}

} // detail

// propagate no_case information through the context
template<class Subject>
struct no_case_directive : proxy_parser<no_case_directive<Subject>, Subject>
{
    using proxy_parser<no_case_directive, Subject>::proxy_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        return this->subject.parse(first, last, detail::make_no_case_context(ctx), attr);
    }
};

namespace detail {

struct no_case_gen
{
    template<X4Subject Subject>
    [[nodiscard]] constexpr no_case_directive<as_parser_plain_t<Subject>>
    operator[](Subject&& subject) const
        noexcept(is_parser_nothrow_constructible_v<no_case_directive<as_parser_plain_t<Subject>>, Subject>)
    {
        return no_case_directive<as_parser_plain_t<Subject>>{as_parser(std::forward<Subject>(subject))};
    }
};

} // detail

namespace parsers::directive {

[[maybe_unused]] inline constexpr detail::no_case_gen no_case{};

} // parsers::directive

using parsers::directive::no_case;

} // iris::x4

#endif
