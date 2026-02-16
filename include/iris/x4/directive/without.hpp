#ifndef IRIS_X4_DIRECTIVE_WITHOUT_HPP
#define IRIS_X4_DIRECTIVE_WITHOUT_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/context.hpp>

#include <format>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject, class... IDs>
struct without_directive : proxy_parser<Subject, without_directive<Subject, IDs...>>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(
            x4::is_nothrow_parsable_v<
                Subject, It, Se,
                std::remove_cvref_t<decltype(x4::remove_all_contexts<IDs...>(ctx))>,
                Attr
            >
        )
    {
        return this->subject.parse(first, last, x4::remove_all_contexts<IDs...>(ctx), attr);
    }

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return std::format("without<...>[{}]", get_info<Subject>{}(this->subject));
    }
};

namespace detail {

template<class... IDs>
struct without_gen
{
    template<class Subject>
    [[nodiscard]] constexpr without_directive<std::remove_cvref_t<Subject>, IDs...>
    operator[](Subject&& subject) const // TODO: MSVC 2022 does not properly handle static operator[]
        noexcept(std::is_nothrow_constructible_v<without_directive<std::remove_cvref_t<Subject>, IDs...>, Subject>)
    {
        return without_directive<std::remove_cvref_t<Subject>, IDs...>{
            std::forward<Subject>(subject)
        };
    }
};

} // detail

namespace parsers::directive {

template<class... IDs>
[[maybe_unused]] inline constexpr detail::without_gen<IDs...> without{};

} // parsers::directive

using parsers::directive::without;

} // iris::x4

#endif
