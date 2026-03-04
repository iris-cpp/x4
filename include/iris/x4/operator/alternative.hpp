#ifndef IRIS_ZZ_X4_OPERATOR_ALTERNATIVE_HPP
#define IRIS_ZZ_X4_OPERATOR_ALTERNATIVE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/move_to.hpp>
#include <iris/x4/core/detail/parse_alternative.hpp>

#include <iris/x4/traits/attribute_of_binary.hpp>
#include <iris/x4/traits/container_traits.hpp>

#include <iris/rvariant/rvariant.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Left, class Right>
struct alternative : binary_parser<Left, Right, alternative<Left, Right>>
{
private:
    static constexpr bool is_both_same_attribute = std::is_same_v<
        typename parser_traits<Left>::attribute_type,
        typename parser_traits<Right>::attribute_type
    >;

public:
    // Canonicalize `rvariant<X, X>` to `X`
    using attribute_type = std::conditional_t<
        is_both_same_attribute,
        std::type_identity<typename parser_traits<Left>::attribute_type>,
        traits::attribute_of_alternative<Left, Right>
    >::type;

    // If canonicalized, proxy the underlying `sequence_size`. In other words:
    //
    // Let `X` be some tuple `tuple<int, int, int>`, and let `Y` be some tuple `tuple<int, int>`.
    // Let `XP` be the parser that yields the attribute type `X`. Define `YP` likewise.
    //
    // `alternative<XP, YP>` yields attribute type `rvariant<X, Y>`, and has `sequence_size` of 1.
    // `alternative<XP, XP>` yields attribute type `X`, and has `sequence_size` of 3.
    //
    // Without canonicalization, the latter case would have attribute type `rvariant<X, X>`
    // and `sequence_size` of 1. This will break automatic decomposition of tuple-like attribute.
    // See `attribute.cpp` for example.
    //
    static constexpr std::size_t sequence_size =
        is_both_same_attribute && parser_traits<Left>::sequence_size == parser_traits<Right>::sequence_size ?
        parser_traits<Left>::sequence_size : // proxy value
        static_cast<std::size_t>(!X4UnusedAttribute<attribute_type>); // unused = 0, otherwise 1

private:
    template<class Attr>
    using temp_attr_t = std::conditional_t<
        is_both_same_attribute,
        // Always the plain (non-lvalue) type
        attribute_type,
        // in case of tuple-like attribute, this might be a decomposed tuple that contains
        // lvalue reference(s), which cannot be default-constructed
        Attr
    >;

public:
    using binary_parser<Left, Right, alternative>::binary_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr const&) const
        noexcept(
            is_nothrow_parsable_v<Left, It, Se, Context, unused_type> &&
            is_nothrow_parsable_v<Right, It, Se, Context, unused_type>
        )
    {
        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            return this->left.parse(first, last, ctx, unused) ||
                (!x4::has_expectation_failure(ctx) && this->right.parse(first, last, ctx, unused));
        } else {
            return this->left.parse(first, last, ctx, unused) ||
                this->right.parse(first, last, ctx, unused);
        }
    }

    // If you're changing these logic, you should read the PR below to
    // understand the rationale:
    //   https://github.com/boostorg/spirit_x4/pull/50
    //
    // You also should add test to `attribute.cpp`.

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
        requires (!traits::X4Container<Attr>)
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(
            noexcept(detail::parse_alternative(this->left, first, last, ctx, attr)) &&
            noexcept(detail::parse_alternative(this->right, first, last, ctx, attr)) &&
            std::is_nothrow_default_constructible_v<temp_attr_t<Attr>> &&
            noexcept(x4::move_to(std::declval<temp_attr_t<Attr>>(), attr))
        )
    {
        static_assert(
            std::default_initializable<temp_attr_t<Attr>>,
            "Attribute needs to be default-initializable to support rollback on failed parse attempt."
        );

        if (temp_attr_t<Attr> temp_attr; detail::parse_alternative(this->left, first, last, ctx, temp_attr)) {
            x4::move_to(std::move(temp_attr), attr);
            return true;
        }
        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) return false;
        }

        if (temp_attr_t<Attr> temp_attr; detail::parse_alternative(this->right, first, last, ctx, temp_attr)) {
            x4::move_to(std::move(temp_attr), attr);
            return true;
        }
        return false; // `attr` is untouched
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, traits::X4Container ContainerAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, ContainerAttr& attr) const
        noexcept(
            noexcept(detail::parse_alternative(this->left, first, last, ctx, attr)) &&
            noexcept(detail::parse_alternative(this->right, first, last, ctx, attr)) &&
            noexcept(x4::move_to(std::declval<ContainerAttr>(), attr)) &&
            std::is_nothrow_default_constructible_v<ContainerAttr> &&
            noexcept(traits::clear(attr))
        )
    {
        static_assert(!std::same_as<std::remove_const_t<ContainerAttr>, unused_type>);
        static_assert(!std::same_as<std::remove_const_t<ContainerAttr>, unused_container_type>);
        static_assert(!std::is_const_v<ContainerAttr>);

        // We can (ab)use the exposed attribute as the temporary workspace
        // if and only if the modification or rollback of the exposed attribute
        // does not change the semantic state of the exposed container instance.
        //
        // The only situation we can guarantee such condition is when the container
        // is empty; assuming that the "empty" state of any user-provided container
        // class is monostate.
        if (traits::is_empty(attr)) {
            if (detail::parse_alternative(this->left, first, last, ctx, attr)) {
                return true;
            }
            traits::clear(attr); // Make sure we don't propagate observable side effect

            if constexpr (has_context_v<Context, contexts::expectation_failure>) {
                if (x4::has_expectation_failure(ctx)) return false;
            }

            if (detail::parse_alternative(this->right, first, last, ctx, attr)) {
                return true;
            }
            traits::clear(attr); // Make sure we don't propagate observable side effect
            return false;
        }

        //
        // Non-empty container
        // Since the attribute is a container, we can reuse the buffer when the `left` fails
        //
        unwrap_container_appender_t<ContainerAttr> temp_attr;

        if (detail::parse_alternative(this->left, first, last, ctx, temp_attr)) {
            x4::move_to(std::move(temp_attr), attr);
            return true;
        }

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) return false;
        }
        traits::clear(temp_attr); // Reuse the buffer

        if (detail::parse_alternative(this->right, first, last, ctx, temp_attr)) {
            x4::move_to(std::move(temp_attr), attr);
            return true;
        }
        return false; // `attr` is untouched
    }
};

template<X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr alternative<as_parser_plain_t<Left>, as_parser_plain_t<Right>>
operator|(Left&& left, Right&& right)
    noexcept(
        is_parser_nothrow_castable_v<Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<
            alternative<as_parser_plain_t<Left>, as_parser_plain_t<Right>>,
            as_parser_t<Left>,
            as_parser_t<Right>
        >
    )
{
    return alternative<as_parser_plain_t<Left>, as_parser_plain_t<Right>>{
        as_parser(std::forward<Left>(left)),
        as_parser(std::forward<Right>(right))
    };
}

} // iris::x4

#endif
