#ifndef IRIS_ZZ_X4_PRIMITIVE_EPS_HPP
#define IRIS_ZZ_X4_PRIMITIVE_EPS_HPP

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

    constexpr explicit semantic_predicate(bool cond) noexcept
        : cond_(cond)
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr&) const
        noexcept(noexcept(x4::skip_over(first, last, ctx)))
    {
        if (this->cond_) {
            x4::skip_over(first, last, ctx);
        }
        return this->cond_;
    }

private:
    bool cond_{};
};

template<class F>
struct lazy_semantic_predicate : parser<lazy_semantic_predicate<F>>
{
    using attribute_type = unused_type;

    constexpr lazy_semantic_predicate() = default;

    template<class T>
        requires
            (!std::same_as<std::remove_cvref_t<T>, lazy_semantic_predicate>) &&
            std::is_constructible_v<F, T>
    constexpr explicit lazy_semantic_predicate(T&& f)
        noexcept(std::is_nothrow_constructible_v<F, T>)
        : f_(std::forward<T>(f))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr&) const
    {
        auto it = first;
        x4::skip_over(it, last, ctx);

        if constexpr (requires { this->f_(ctx); }) {
            static_assert(std::same_as<decltype(this->f_(ctx)), bool>);
            bool const ok = this->f_(ctx);
            if (ok) first = it;
            return ok;

        } else {
            static_assert(std::same_as<decltype(this->f_()), bool>);
            bool const ok = this->f_();
            if (ok) first = it;
            return ok;
        }
    }

private:
    F f_{};
};

} // detail

struct eps_parser : parser<eps_parser>
{
    using attribute_type = unused_type;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr&)
        noexcept(noexcept(x4::skip_over(first, last, ctx)))
    {
        x4::skip_over(first, last, ctx);
        return true;
    }

    [[nodiscard]] static constexpr detail::semantic_predicate
    operator()(bool cond) noexcept
    {
        return detail::semantic_predicate{cond};
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
