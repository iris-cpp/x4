#ifndef IRIS_ZZ_X4_CHAR_CHAR_PARSER_HPP
#define IRIS_ZZ_X4_CHAR_CHAR_PARSER_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/move_to.hpp>
#include <iris/x4/core/char_traits.hpp>

#include <concepts>
#include <iterator>

namespace iris::x4 {

template<class Encoding, class Derived>
struct char_parser : parser<Derived>
{
    using encoding_type = Encoding;
    using char_type = typename Encoding::char_type;
    using classify_type = typename Encoding::classify_type;

private:
    template<class Context>
    static constexpr bool has_static_test = requires(classify_type classify_ch, Context const& ctx) {
        { Derived::test(classify_ch, ctx) } -> std::same_as<bool>;
    };

public:
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires has_static_test<Context>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr)
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(first != last) &&
            noexcept(Derived::test(static_cast<classify_type>(*first), ctx)) &&
            noexcept(x4::move_to(std::iter_value_t<It>{*first}, attr)) &&
            noexcept(++first)
        )
    {
        static_assert(!CharIncompatibleWith<std::iter_value_t<It>, char_type>, "Mixing incompatible char types is not allowed");
        static_assert(!CharLike<Attr> || !CharIncompatibleWith<Attr, char_type>, "Mixing incompatible char types is not allowed");

        auto it = first;
        x4::skip_over(it, last, ctx);

        if (it != last && Derived::test(static_cast<classify_type>(*it), ctx)) {
            x4::move_to(std::iter_value_t<It>{*it++}, attr);
            first = it;
            return true;
        }
        return false;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires (!has_static_test<Context>)
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(first != last) &&
            noexcept(this->derived().test(static_cast<classify_type>(*first), ctx)) &&
            noexcept(x4::move_to(std::iter_value_t<It>{*first}, attr)) &&
            noexcept(++first)
        )
    {
        static_assert(!CharIncompatibleWith<std::iter_value_t<It>, char_type>, "Mixing incompatible char types is not allowed");
        static_assert(!CharLike<Attr> || !CharIncompatibleWith<Attr, char_type>, "Mixing incompatible char types is not allowed");

        auto it = first;
        x4::skip_over(it, last, ctx);

        if (it != last && this->derived().test(static_cast<classify_type>(*it), ctx)) {
            x4::move_to(std::iter_value_t<It>{*it++}, attr);
            first = it;
            return true;
        }
        return false;
    }
};

} // iris::x4

#endif
