#ifndef IRIS_ZZ_X4_DIRECTIVE_AS_HPP
#define IRIS_ZZ_X4_DIRECTIVE_AS_HPP

/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/move_to.hpp>
#include <iris/x4/core/unused.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace detail {

template<bool SubjectHasAction, class Context, X4Attribute OuterAttr>
struct as_directive_ctx_impl // false
{
    using type = Context;
};
template<class Context, X4Attribute OuterAttr>
struct as_directive_ctx_impl<true, Context, OuterAttr>
{
    using type = std::remove_cvref_t<decltype(x4::replace_first_context<contexts::as_var>(
        std::declval<Context const&>(),
        std::declval<OuterAttr&>()
    ))>;
};

} // detail

// `as_directive` forces the attribute of subject parser
// to be `T`. When `T` is `unused_type`, this is equivalent to
// `omit_directive`.
template<X4Attribute T, class Subject>
struct as_directive : unary_parser<Subject, as_directive<T, Subject>>
{
    static_assert(!std::is_const_v<T>); // Forbid const `unused_type`
    static_assert(!std::same_as<T, unused_container_type>); // Unknown use case, not supported for now

    static_assert(std::default_initializable<T>);

    using attribute_type = T;

    static constexpr bool has_attribute = !std::same_as<T, unused_type>;
    static constexpr bool has_action = false; // Explicitly re-enable attribute detection in `x4::rule`
    static constexpr bool requires_exact_attribute_type = true;

    // `as_directive` should NOT inherit underlying parser's `handles_container`
    // because `as_directive` is an atomic parser. The default implementation of
    // `parser_traits<as_directive<...>>::handles_container` must transparently
    // handle this case.

private:
    template<X4Attribute Attr>
    using exposed_attr_for_child_t = std::conditional_t<
        Subject::has_action, unused_type, Attr
    >;

public:
    // `outer_parser<T>(as<T>(subject))` forwards the outer `T&` (exposed attribute) for the subject
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute OuterAttr>
        requires std::same_as<std::remove_const_t<OuterAttr>, T>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, OuterAttr& outer_attr) const
        noexcept(is_nothrow_parsable_v<Subject, It, Se, typename detail::as_directive_ctx_impl<Subject::has_action, Context, OuterAttr>::type, exposed_attr_for_child_t<OuterAttr>>)
    {
        if constexpr (Subject::has_action) {
            return this->subject.parse(first, last, x4::replace_first_context<contexts::as_var>(ctx, outer_attr), unused);
        } else {
            return this->subject.parse(first, last, ctx, outer_attr);
        }
    }

    // `outer_parser<unused_type>(as<T>(subject))` forwards `unused` for the subject
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute OuterAttr>
        requires
            (!std::same_as<std::remove_const_t<OuterAttr>, T>)
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, OuterAttr&) const
        noexcept(is_nothrow_parsable_v<Subject, It, Se, typename detail::as_directive_ctx_impl<Subject::has_action, Context, unused_type>::type, unused_type>)
    {
        if constexpr (Subject::has_action) {
            return this->subject.parse(first, last, x4::replace_first_context<contexts::as_var>(ctx, unused), unused);
        } else {
            return this->subject.parse(first, last, ctx, unused);
        }
    }

    // `outer_parser<U>(as<T>(subject))` forwards temporary `T` local variable for the subject, then move the variable to `U&`
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute OuterAttr>
        requires
            (!std::same_as<std::remove_const_t<OuterAttr>, T>) &&
            X4Movable<T, OuterAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, OuterAttr& outer_attr) const
        noexcept(
            is_nothrow_parsable_v<Subject, It, Se, typename detail::as_directive_ctx_impl<Subject::has_action, Context, T>::type, exposed_attr_for_child_t<T>> &&
            noexcept(x4::move_to(std::declval<T>(), outer_attr))
        )
    {
        // Ideally we should default to default-initialization and avoid value-initialization.
        // However, there is currently no way to determine whether the attribute is ever touched
        // by the underlying parser (for example: semantic action).
        //
        // Note that this behavior is our implementation details. The underlying parser should
        // not rely on this behavior; they should never assume the given attribute is defaulted
        // to some arbitrary initial value.
        T attr_{}; // value-initialize

        if constexpr (Subject::has_action) {
            if (!this->subject.parse(first, last, x4::replace_first_context<contexts::as_var>(ctx, attr_), unused)) return false;
        } else {
            if (!this->subject.parse(first, last, ctx, attr_)) return false;
        }

        x4::move_to(std::move(attr_), outer_attr);
        return true;
    }
};

namespace detail {

template<X4Attribute T>
struct as_fn
{
    template<X4Subject Subject>
    [[nodiscard]] static constexpr as_directive<T, as_parser_plain_t<Subject>>
    operator()(Subject&& subject)
        noexcept(is_parser_nothrow_constructible_v<as_directive<T, as_parser_plain_t<Subject>>, Subject>)
    {
        return {std::forward<Subject>(subject)};
    }
};

} // detail

namespace parsers::directive {

template<X4Attribute T>
[[maybe_unused]] inline constexpr detail::as_fn<T> as{};

} // parsers::directive

using parsers::directive::as;

} // iris::x4

#endif
