#ifndef IRIS_X4_DIRECTIVE_SKIP_HPP
#define IRIS_X4_DIRECTIVE_SKIP_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/context.hpp>
#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/parser.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Subject, class Skipper>
struct skip_directive : proxy_parser<Subject, skip_directive<Subject, Skipper>>
{
    using base_type = proxy_parser<Subject, skip_directive>;

    template<class SubjectT, class SkipperT>
        requires std::is_constructible_v<base_type, SubjectT> && std::is_constructible_v<Skipper, SkipperT>
    constexpr skip_directive(SubjectT&& subject, SkipperT&& skipper)
        noexcept(std::is_nothrow_constructible_v<base_type, SubjectT> && std::is_nothrow_constructible_v<Skipper, SkipperT>)
        : base_type(std::forward<SubjectT>(subject))
        , skipper_(std::forward<SkipperT>(skipper))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(is_nothrow_parsable_v<Subject, It, Se, context_t<Context>, Attr>)
    {
        return this->subject.parse(first, last, x4::replace_first_context<contexts::skipper>(ctx, skipper_), attr);
    }

private:
    template<class Context>
    using context_t = std::remove_cvref_t<decltype(
        x4::replace_first_context<contexts::skipper>(std::declval<Context const&>(), std::declval<Skipper&>())
    )>;

    Skipper skipper_;
};

namespace detail {

template<X4Subject Skipper>
struct skip_gen_impl
{
    // Unreference rvalue reference, but hold lvalue reference as-is
    using skipper_type = std::conditional_t<
        std::is_rvalue_reference_v<Skipper>,
        std::remove_reference_t<Skipper>,
        Skipper
    >;

    template<class SkipperT>
        requires std::same_as<std::remove_cvref_t<SkipperT>, std::remove_cvref_t<Skipper>>
    constexpr skip_gen_impl(SkipperT&& skipper)
        noexcept(std::is_nothrow_constructible_v<skipper_type, SkipperT>)
        : skipper_(std::forward<SkipperT>(skipper))
    {}

    template<X4Subject Subject>
    [[nodiscard]] constexpr skip_directive<as_parser_plain_t<Subject>, std::remove_cvref_t<Skipper>>
    operator[](Subject&& subject) const
        noexcept(
            is_parser_nothrow_castable_v<Subject> &&
            std::is_nothrow_constructible_v<
                skip_directive<as_parser_plain_t<Subject>, std::remove_cvref_t<Skipper>>,
                as_parser_t<Subject>,
                skipper_type const&
            >
        )
    {
        return {as_parser(std::forward<Subject>(subject)), skipper_};
    }

private:
    skipper_type skipper_;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

struct skip_gen
{
    template<X4Subject Skipper>
    [[nodiscard]]
    static constexpr skip_gen_impl<as_parser_t<Skipper>>
    operator()(Skipper&& skipper)
        noexcept(
            is_parser_nothrow_castable_v<Skipper> &&
            std::is_nothrow_constructible_v<skip_gen_impl<as_parser_t<Skipper>>, as_parser_t<Skipper>>
        )
    {
        return {as_parser(std::forward<Skipper>(skipper))};
    }
};

} // detail

namespace parsers::directive {

[[maybe_unused]] inline constexpr detail::skip_gen skip{};

} // parsers::directive

using parsers::directive::skip;

} // iris::x4

#endif
