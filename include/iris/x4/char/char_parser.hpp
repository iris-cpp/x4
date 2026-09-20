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
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Derived, class Encoding>
struct char_parser;

// `negated_char_parser` handles `~cp`, where `cp` is a `char_parser`
template<class Positive>
struct negated_char_parser : char_parser<negated_char_parser<Positive>, typename Positive::encoding_type>
{
    static_assert(X4ExplicitSubject<Positive>);

    using attribute_type = parser_traits<Positive>::attribute_type;
    using encoding_type = Positive::encoding_type;

    template<class PositiveT>
        requires
            (!std::is_same_v<std::remove_cvref_t<PositiveT>, negated_char_parser>) &&
            std::is_constructible_v<Positive, PositiveT>
    constexpr explicit negated_char_parser(PositiveT&& positive)
        noexcept(std::is_nothrow_constructible_v<Positive, PositiveT>)
        : positive_(std::forward<PositiveT>(positive))
    {}

    // TODO: implement static version
    template<class CharT, class Context>
    [[nodiscard]] constexpr bool
    test(CharT ch, Context const& ctx) const noexcept
    {
        static_assert(noexcept(!positive_.test(ch, ctx)));
        return !positive_.test(ch, ctx);
    }

    template<class Self>
    [[nodiscard]] constexpr auto&& operator~(this Self&& self IRIS_LIFETIMEBOUND) noexcept
    {
        return std::forward<Self>(self).positive_;
    }

private:
    Positive positive_; // TODO: EBO
};

template<class Derived, class Encoding>
struct char_parser : parser<char_parser<Derived, Encoding>>
{
    using encoding_type = Encoding;
    using char_type = Encoding::char_type;
    using classify_type = Encoding::classify_type;

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

    template<class Self, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires (!has_static_test<Context>)
    [[nodiscard]] constexpr bool
    parse(this Self const& self /* require const& */, It& first, Se const& last, Context const& ctx, Attr& attr)
        noexcept(
            std::is_nothrow_copy_assignable_v<It> &&
            noexcept(x4::skip_over(first, last, ctx)) &&
            noexcept(first != last) &&
            noexcept(self.test(static_cast<classify_type>(*first), ctx)) &&
            noexcept(x4::move_to(std::iter_value_t<It>{*first}, attr)) &&
            noexcept(++first)
        )
    {
        static_assert(!CharIncompatibleWith<std::iter_value_t<It>, char_type>, "Mixing incompatible char types is not allowed");
        static_assert(!CharLike<Attr> || !CharIncompatibleWith<Attr, char_type>, "Mixing incompatible char types is not allowed");

        auto it = first;
        x4::skip_over(it, last, ctx);

        if (it != last && self.test(static_cast<classify_type>(*it), ctx)) {
            x4::move_to(std::iter_value_t<It>{*it++}, attr);
            first = it;
            return true;
        }
        return false;
    }

    template<class Self>
        requires (!std::derived_from<std::remove_cvref_t<Self>, negated_char_parser<std::remove_cvref_t<Self>>>)
    [[nodiscard]] constexpr negated_char_parser<std::remove_cvref_t<Self>>
    operator~(this Self&& self)
        noexcept(std::is_nothrow_constructible_v<negated_char_parser<std::remove_cvref_t<Self>>, Self>)
    {
        return negated_char_parser<std::remove_cvref_t<Self>>{std::forward<Self>(self)};
    }
};

} // iris::x4

#endif
