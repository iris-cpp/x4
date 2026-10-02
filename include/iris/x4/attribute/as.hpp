#ifndef IRIS_ZZ_X4_ATTRIBUTE_AS_HPP
#define IRIS_ZZ_X4_ATTRIBUTE_AS_HPP

/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/traits/tuple_traits.hpp>
#include <iris/x4/core/traits/write_rank.hpp>
#include <iris/x4/core/write_attribute.hpp>
#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/core/context.hpp>
#include <iris/x4/core/action_context.hpp>

#include <typeinfo>
#include <concepts>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace detail {

template<bool SubjectHasAction, class Context, X4Attribute OuterAttr>
struct as_type_parser_ctx_impl // false
{
    using type = Context;
};
template<class Context, X4Attribute OuterAttr>
struct as_type_parser_ctx_impl<true, Context, OuterAttr>
{
    using type = std::remove_cvref_t<decltype(x4::replace_first_context<contexts::as_var>(
        std::declval<Context const&>(),
        std::declval<OuterAttr&>()
    ))>;
};

} // detail

// `as_type_parser` forces the attribute of subject parser
// to be `T`. When `T` is `unused_type`, this is equivalent to
// `omit_directive`.
template<X4Attribute T, class Subject>
struct as_type_parser : unary_parser<as_type_parser<T, Subject>, Subject>
{
    static_assert(!std::is_const_v<T>); // Forbid const `unused_type`
    static_assert(!std::same_as<T, unused_container_type>); // Unknown use case, not supported for now

    static_assert(std::default_initializable<T>);

    using attribute_type = T;

    static constexpr bool has_attribute = !std::same_as<T, unused_type>;
    static constexpr bool has_action = false; // Explicitly re-enable attribute detection in `x4::rule`
    static constexpr bool requires_exact_attribute_type = true;

    // `as_type_parser` should NOT inherit underlying parser's `accepts_container`
    // because `as_type_parser` is an atomic parser. The default implementation of
    // `parser_traits<as_type_parser<...>>::accepts_container` must transparently
    // handle this case.

    using unary_parser<as_type_parser, Subject>::unary_parser;

private:
    template<X4Attribute Attr>
    using exposed_attr_for_child_t = std::conditional_t<
        Subject::has_action, unused_type, Attr
    >;

public:
    // `outer_parser<T>(as<T>(subject))` forwards the outer `T&` (exposed attribute) for the subject, unless
    // it is a container which already holds the preceding results
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute OuterAttr>
        requires std::same_as<std::remove_const_t<OuterAttr>, T>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, OuterAttr& outer_attr) const
    {
        if constexpr (traits::X4Container<T>) {
            if (!std::ranges::empty(outer_attr)) {
                // The container holds the preceding results, which the attribute of `as<T>`
                // is kept apart from: parse into a new attribute and append it on success
                T attr_{};
                if (!this->parse_subject(first, last, ctx, attr_)) return false;
                planner::pass_declared_attribute(outer_attr, std::move(attr_));
                return true;
            }
        }
        return this->parse_subject(first, last, ctx, outer_attr);
    }

    // `outer_parser<unused_type>(as<T>(subject))` forwards `unused` for the subject
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute OuterAttr>
        requires
            (!std::same_as<std::remove_const_t<OuterAttr>, T>)
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, OuterAttr&) const
    {
        if constexpr (Subject::has_action) {
            return this->subject.parse(first, last, x4::replace_first_context<contexts::as_var>(ctx, unused), unused);
        } else {
            return this->subject.parse(first, last, ctx, unused);
        }
    }

    // `outer_parser<U>(as<T>(subject))` parses into the element of `U` if `U` holds exactly `T` as its single
    // element; otherwise into a temporary `T`, then passes it to `U&` by the ordinary conversion and assignment,
    // never reinterpreting it into another structure (the assigned value is appended to a container which holds
    // the preceding results)
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute OuterAttr>
        requires
            (!std::same_as<std::remove_const_t<OuterAttr>, T>)
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, OuterAttr& outer_attr) const
    {
        if constexpr (!has_attribute) {
            return this->parse(first, last, ctx, unused); // equivalent to `omit[subject]`

        } else if constexpr (detail::holds_as_single_element<std::remove_const_t<OuterAttr>, T>) {
            return this->parse(first, last, ctx, alloy::get<0>(outer_attr));

        } else {
            static_assert(X4StrictlyWritable<std::remove_const_t<OuterAttr>&, unwrap_recursive_t<T>&&>);
            static_assert(!detail::dangles<std::remove_const_t<OuterAttr>, unwrap_recursive_t<T>&&>);

            T attr_{}; // value-initialize

            if (!this->parse_subject(first, last, ctx, attr_)) return false;
            planner::pass_declared_attribute(outer_attr, iris::unwrap_recursive(std::move(attr_)));
            return true;
        }
    }

    [[nodiscard]] /*constexpr*/ std::string get_x4_info() const
    {
        return std::string("as<") + typeid(T).name() + ">("
            + get_info<Subject>{}(this->subject) + ')';
    }

private:
    // Parses the subject into `attr`, the attribute of `as<T>`; an action in the subject refers to it as `_as_var`
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse_subject(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        if constexpr (Subject::has_action) {
            return this->subject.parse(first, last, x4::replace_first_context<contexts::as_var>(ctx, attr), unused);
        } else {
            return this->subject.parse(first, last, ctx, attr);
        }
    }
};

namespace detail {

template<X4Attribute T>
struct as_fn
{
    template<X4Subject Subject>
    [[nodiscard]] static constexpr as_type_parser<T, as_parser_plain_t<Subject>>
    operator()(Subject&& subject)
        noexcept(is_parser_nothrow_constructible_v<as_type_parser<T, as_parser_plain_t<Subject>>, Subject>)
    {
        return as_type_parser<T, as_parser_plain_t<Subject>>{std::forward<Subject>(subject)};
    }
};

} // detail

namespace parsers {

template<X4Attribute T>
[[maybe_unused]] inline constexpr detail::as_fn<T> as{};

} // parsers

using parsers::as;

} // iris::x4

#endif
