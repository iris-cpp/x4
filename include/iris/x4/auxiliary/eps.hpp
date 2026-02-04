#ifndef IRIS_X4_AUXILIARY_EPS_HPP
#define IRIS_X4_AUXILIARY_EPS_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/unused.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace detail {

struct semantic_predicate : parser<semantic_predicate>
{
    using attribute_type = unused_type;

    constexpr explicit semantic_predicate(bool predicate) noexcept
        : predicate_(predicate)
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr&) const
        noexcept(noexcept(x4::skip_over(first, last, ctx)))
    {
        x4::skip_over(first, last, ctx);
        return predicate_;
    }

private:
    bool predicate_;
};

template<class F>
struct lazy_semantic_predicate : parser<lazy_semantic_predicate<F>>
{
    using attribute_type = unused_type;

    template<class F_>
        requires
            (!std::is_same_v<std::remove_cvref_t<F_>, lazy_semantic_predicate>) &&
            std::is_constructible_v<F, F_>
    constexpr explicit lazy_semantic_predicate(F_&& f)
        noexcept(std::is_nothrow_constructible_v<F, F_>)
        : f_(std::forward<F_>(f))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr&) const
    {
        x4::skip_over(first, last, ctx);

        if constexpr (std::invocable<F const&, Context const&>) {
            static_assert(std::same_as<std::invoke_result_t<F const&, Context const&>, bool>);
            return f_(ctx);

        } else if constexpr (std::invocable<F const&, unused_type const&>) {
            static_assert(
                false,
                "We no longer accept a lazy semantic predicate that expects a single `unused_type`. "
                "Just make your functor take no arguments instead."
            );
            return false; // silence linter warning

        } else {
            static_assert(std::invocable<F const&>);
            static_assert(std::same_as<std::invoke_result_t<F const&>, bool>);
            return f_();
        }
    }

private:
    F f_;
};

} // detail

struct eps_parser : parser<eps_parser>
{
    using attribute_type = unused_type;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr&) const
        noexcept(noexcept(x4::skip_over(first, last, ctx)))
    {
        x4::skip_over(first, last, ctx);
        return true;
    }

    [[nodiscard]] static constexpr detail::semantic_predicate
    operator()(bool predicate) noexcept
    {
        return detail::semantic_predicate{predicate};
    }

    template<class F>
    [[nodiscard]] static constexpr detail::lazy_semantic_predicate<std::remove_cvref_t<F>>
    operator()(F&& f)
        noexcept(std::is_nothrow_constructible_v<detail::lazy_semantic_predicate<std::remove_cvref_t<F>>, F>)
    {
        return detail::lazy_semantic_predicate<std::remove_cvref_t<F>>{std::forward<F>(f)};
    }
};

namespace parsers {

[[maybe_unused]] inline constexpr eps_parser eps{};

} // parsers

using parsers::eps;

} // iris::x4

#endif
