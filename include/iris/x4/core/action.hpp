#ifndef IRIS_ZZ_X4_CORE_ACTION_HPP
#define IRIS_ZZ_X4_CORE_ACTION_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp>

#include <iris/x4/core/attribute.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/context.hpp>
#include <iris/x4/core/action_context.hpp>

#include <iterator>
#include <concepts>
#include <type_traits>
#include <utility>
#include <format>

namespace iris::x4 {

namespace detail {

template<class Context, X4Attribute Attr>
struct action_context;

template<class Context, X4NonUnusedAttribute Attr>
struct action_context<Context, Attr>
{
    using type = context<contexts::attr, Attr, canonical_context_t<Context const&>>;
};

template<class Context, X4UnusedAttribute Attr>
struct action_context<Context, Attr>
{
    using type = Context const&;
};

} // detail

// Note about the constraint on the `Action` parameter:
//
// Ideally we should have a context-agnostic concept that can be used
// like `X4ActionFunctor<F>`, but we technically can't.
//
// In order to check `std::invocable`, we need to know the actual context
// type passed to the `.parse(...)` function but it is unknown until
// runtime.
//
// Even if we make up the most trivial context type (i.e. `unused_type`),
// such concept will be useless because a user-provided functor always
// operates on user-specific precondition that assumes the context
// holds exact specific type provided to the entry point (`x4::parse`).

template<class Subject, class ActionF>
struct action : proxy_parser<Subject, action<Subject, ActionF>>
{
    static_assert(
        !std::is_reference_v<ActionF>,
        "Reference type is disallowed for semantic action functor to prevent dangling reference"
    );

    using base_type = proxy_parser<Subject, action>;

    static constexpr bool has_action = true;
    static constexpr bool need_rcontext = true;
    static constexpr bool requires_exact_attribute_type = false; // reset

    ActionF f;

    template<class SubjectT, class ActionT>
        requires std::is_constructible_v<base_type, SubjectT> && std::is_constructible_v<ActionF, ActionT>
    constexpr action(SubjectT&& subject, ActionT&& f)
        noexcept(std::is_nothrow_constructible_v<base_type, SubjectT> && std::is_nothrow_constructible_v<ActionF, ActionT>)
        : base_type(std::forward<SubjectT>(subject))
        , f(std::forward<ActionT>(f))
    {
    }

    // When the exposed attribute is `unused_type`.
    // Since we can assume that the subject unconditionally requires `_attr`,
    // we must create a temporary variable to pass it to the subject.
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr&) const
        noexcept(
            std::is_nothrow_default_constructible_v<typename base_type::attribute_type> &&
            noexcept(this->parse_main(first, last, ctx, std::declval<typename base_type::attribute_type&>()))
        )
    {
        typename base_type::attribute_type attr_temp; // default-initialize
        return this->parse_main(first, last, ctx, attr_temp);
    }

private:
    // We need to handle the general case when `Attr` and the subject's attribute
    // type are different.
    //
    // We can't unconditionally create `attr_temp` of the exact matching type
    // because there exists some cases where the temporary variable is truly
    // unnecessary.
    //
    // For instance, when the exposed attribute is `std::vector<int>` and the
    // underlying parser is `int_ >> int_` (attr is `alloy::tuple<int, int>`),
    // we must just pass the exposed vector variable directly.
    //
    // Conversely, the only reliable method to determine whether the underlying
    // parser really needs the exact matching type is by checking the dedicated
    // trait flag like below.
    template<X4NonUnusedAttribute Attr>
    static constexpr bool can_pass_exposed_attr =
        std::same_as<Attr, typename base_type::attribute_type> || !Subject::requires_exact_attribute_type;

public:
    // When the exposed attribute is NOT `unused_type`.
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
        requires can_pass_exposed_attr<Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(noexcept(this->parse_main(first, last, ctx, attr)))
    {
        return this->parse_main(first, last, ctx, attr);
    }

    // When the exposed attribute is NOT `unused_type`.
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
        requires (!can_pass_exposed_attr<Attr>)
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& /* attr is discarded */) const
        noexcept(
            std::is_nothrow_default_constructible_v<typename base_type::attribute_type> &&
            noexcept(this->parse_main(first, last, ctx, std::declval<typename base_type::attribute_type&>()))
        )
    {
        typename base_type::attribute_type attr_temp; // default-initialize
        return this->parse_main(first, last, ctx, attr_temp);
    }

    constexpr void operator[](auto const&) const = delete; // You can't add semantic action for semantic action

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return std::format("{}[f]", get_info<Subject>{}(this->subject));
    }

private:
    // Semantic action with no parameter: `p[([] { /* ... */ })]`
    template<class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    call_action(Context const&, Attr&) const
        noexcept(std::is_nothrow_invocable_v<ActionF const&>)
    {
        // Explicitly make this hard error instead of emitting "no matching overload".
        // This provides much more human-friendly errors.
        static_assert(
            std::invocable<ActionF const&>,
            "Neither `f(ctx)` nor `f()` is well-formed for your semantic action. "
            "Check your function signature. Note that some functors might need "
            "`const` qualifier to satisfy the constraints."
        );

        using action_return_type = std::invoke_result_t<ActionF const&>;
        constexpr bool action_returns_bool = std::same_as<action_return_type, bool>;
        static_assert(
            action_returns_bool || std::same_as<action_return_type, void>,
            "Semantic action should not return value other than `bool`. Check your function signature."
        );

        if constexpr (action_returns_bool) {
            return this->f();
        } else {
            this->f();
            return true;
        }
    }

    // Semantic action with parameter: `p[([](auto&& ctx) { /* ... */ })]`
    template<class Context, X4Attribute Attr>
        requires std::invocable<ActionF const&, typename detail::action_context<Context, Attr>::type>
    [[nodiscard]] constexpr bool
    call_action(Context const& ctx, Attr& attr) const
        noexcept(std::is_nothrow_invocable_v<ActionF const&, typename detail::action_context<Context, Attr>::type>)
    {
        using action_return_type = std::invoke_result_t<ActionF const&, typename detail::action_context<Context, Attr>::type>;
        constexpr bool action_returns_bool = std::same_as<action_return_type, bool>;
        static_assert(
            action_returns_bool || std::same_as<action_return_type, void>,
            "Semantic action should not return value other than `bool`. Check your function signature."
        );

        // Inject `_attr` only when `Attr` is not `unused_type`
        if constexpr (X4UnusedAttribute<Attr>) {
            if constexpr (action_returns_bool) {
                return this->f(ctx);
            } else {
                this->f(ctx);
                return true;
            }

        } else {
            if constexpr (action_returns_bool) {
                return this->f(x4::make_context<contexts::attr>(attr, ctx));
            } else {
                this->f(x4::make_context<contexts::attr>(attr, ctx));
                return true;
            }
        }
    }

    template<class Context, X4Attribute Attr>
        requires
            (!std::invocable<ActionF const&, typename detail::action_context<Context, Attr>::type>) &&
            std::invocable<ActionF const&, typename detail::action_context<Context, Attr>::type const&>
    static constexpr bool
    call_action(Context const&, Attr&)
    {
        static_assert(
            std::invocable<ActionF const&, typename detail::action_context<Context, Attr>::type>,
            "Semantic action expecting non-const lvalue reference context is obsolete. Use `auto&& ctx` and avoid using `auto& ctx`."
        );
        return false; // dummy
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse_main(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(
            std::is_copy_assignable_v<It> &&
            is_nothrow_parsable_v<Subject, It, Se, Context, Attr> &&
            noexcept(this->call_action(ctx, attr))
        )
    {
        It const saved_first = first;
        if (!this->subject.parse(first, last, ctx, attr)) return false;

        if (this->call_action(ctx, attr)) {
            return true;
        }
        // reset iterators if semantic action failed the match
        // retrospectively
        first = saved_first;
        return false;
    }
};

} // iris::x4

#endif
