#ifndef IRIS_ZZ_X4_CORE_CONTEXT_HPP
#define IRIS_ZZ_X4_CORE_CONTEXT_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp>
#include <iris/type_list.hpp>

#include <iris/x4/core/unused.hpp>

#include <iris/bits/specialization_of.hpp>

#include <concepts>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class ID, class T, class Next>
struct context;


namespace detail {

template<class Context, class ID_To_Search>
struct has_context_impl;

template<class ID_To_Search>
struct has_context_impl<unused_type, ID_To_Search>
    : std::false_type
{};

template<class T, class Next, class ID_To_Search>
struct has_context_impl<context<ID_To_Search, T, Next>, ID_To_Search>
    : std::true_type
{
    using context_value_type = T;
};

template<class ID, class T, class Next, class ID_To_Search>
    requires (!std::same_as<ID, ID_To_Search>)
struct has_context_impl<context<ID, T, Next>, ID_To_Search>
    : has_context_impl<std::remove_cvref_t<Next>, ID_To_Search>
{};

} // detail

template<class Context, class ID_To_Search>
concept has_context = detail::has_context_impl<Context, ID_To_Search>::value;

template<class Context, class ID_To_Search, class T>
concept has_context_of =
    has_context<Context, ID_To_Search> &&
    std::same_as<std::remove_const_t<typename detail::has_context_impl<Context, ID_To_Search>::context_value_type>, T>;

template<class ID_To_Get, class ID, class T, class Next>
[[nodiscard]] constexpr decltype(auto)
get(context<ID, T, Next> const& ctx) noexcept
{
    if constexpr (has_context<context<ID, T, Next>, ID_To_Get>) {
        return ctx.get(std::type_identity<ID_To_Get>{});
    } else {
        return (unused); // return lvalue
    }
}

template<class ID_To_Get>
[[nodiscard]] constexpr unused_type const&
get(unused_type const&) noexcept
{
    return unused;
}

template<class ID_To_Get, class ID, class T, class Next>
void get(context<ID, T, Next> const&&) = delete; // dangling

template<class ID, class Context>
using get_context_plain_t = std::remove_cvref_t<decltype(x4::get<ID>(std::declval<Context const&>()))>;

namespace detail {

template<class ID>
concept UniqueContextID = !requires { ID::is_unique; } || requires { requires ID::is_unique; };

template<class ID>
concept AllowUnusedContextID = requires { requires ID::allow_unused; };

template<class ID, class Next>
concept HasNoDuplicateContext = !UniqueContextID<ID> || !has_context<Next, ID>;

template<class T>
concept ContextValueType =
    !std::same_as<std::remove_cvref_t<T>, unused_type> &&
    !std::is_reference_v<T> &&
    !is_ttp_specialization_of_v<std::remove_const_t<T>, context>;

template<class Next>
concept ContextNextType =
    !std::same_as<std::remove_cvref_t<Next>, unused_type> &&
    (
        (
            std::is_lvalue_reference_v<Next> &&
            std::is_const_v<std::remove_reference_t<Next>>
        ) ||
        (
            !std::is_lvalue_reference_v<Next> &&
            !std::is_rvalue_reference_v<Next> &&
            !std::is_const_v<Next> &&
            std::is_move_constructible_v<Next>
        )
    );

template<class ContextWithCVRef>
using canonical_context_t = std::conditional_t<
    std::same_as<std::remove_cvref_t<ContextWithCVRef>, unused_type>,
    unused_type,
    std::conditional_t<
        std::is_lvalue_reference_v<ContextWithCVRef>,
        std::remove_reference_t<ContextWithCVRef> const&, // Next& -> Next const&
        std::remove_cvref_t<ContextWithCVRef> // Next const -> Next
    >
>;

template<class ID, class T, class Next>
struct context_storage
{
    static_assert(ContextValueType<T>);
    static_assert(ContextNextType<Next>);

    // lvalue{lvalue} or non-reference{lvalue}
    constexpr context_storage(T& val IRIS_LIFETIMEBOUND, std::remove_cvref_t<Next> const& next) noexcept
        : val(val)
        , next(next)
    {}

    // non-reference{rvalue}
    constexpr context_storage(T& val IRIS_LIFETIMEBOUND, std::remove_cvref_t<Next>&& next)
        noexcept(std::is_nothrow_constructible_v<Next, std::remove_const_t<Next>>)
        requires (!std::is_lvalue_reference_v<Next>)
        : val(val)
        , next(std::move(next))
    {}

    // non-reference{const rvalue}
    constexpr context_storage(T& val IRIS_LIFETIMEBOUND, std::remove_cvref_t<Next> const&& next)
        noexcept(std::is_nothrow_constructible_v<Next, std::remove_const_t<Next> const>)
        requires (!std::is_lvalue_reference_v<Next>)
        : val(val)
        , next(std::move(next))
    {}

    // lvalue{rvalue}
    context_storage(std::remove_const_t<T> const&, std::remove_cvref_t<Next> const&&) requires std::is_lvalue_reference_v<Next> = delete;
    context_storage(std::remove_const_t<T> const&&, std::remove_cvref_t<Next> const&) = delete;

    [[nodiscard]] constexpr T& get(std::type_identity<ID> const&) const noexcept IRIS_LIFETIMEBOUND
    {
        return val;
    }

    [[nodiscard]] constexpr decltype(auto) get(auto const& id) const noexcept IRIS_LIFETIMEBOUND
    {
        return next.get(id);
    }

    T& val;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    IRIS_NO_UNIQUE_ADDRESS Next next;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

template<class ID, class T, class Next>
    requires std::same_as<std::remove_cvref_t<Next>, unused_type>
struct context_storage<ID, T, Next>
{
    static_assert(ContextValueType<T>);

    constexpr explicit context_storage(T& val IRIS_LIFETIMEBOUND) noexcept
        : val(val)
    {}

    constexpr context_storage(T& val IRIS_LIFETIMEBOUND, unused_type const&) noexcept
        : val(val)
    {}

    context_storage(std::remove_const_t<T> const&&) = delete;
    context_storage(std::remove_const_t<T> const&&, unused_type const&) = delete;

    [[nodiscard]] constexpr T& get(std::type_identity<ID> const&) const noexcept IRIS_LIFETIMEBOUND
    {
        return val;
    }

    static void get(auto const&) = delete; // ID not found

    T& val;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

#define IRIS_X4_UNUSED_CONTEXT_VALUE_TYPE_ERROR \
    "Assigning `unused` to a unique context is prohibited for maintainability because " \
    "unique context is binary: it either holds a value or is empty. Introducing " \
    "`unused` makes it tri-state. If you want to propagate `unused` as a placeholder " \
    "(e.g. for debug QoL purpose), mark the tag with `allow_unused = true`."

template<class ID, class T, class Next>
    requires std::same_as<std::remove_const_t<T>, unused_type>
struct context_storage<ID, T, Next>
{
    static_assert(ContextNextType<Next>);

    static_assert(
        !detail::UniqueContextID<ID> || detail::AllowUnusedContextID<ID>,
        IRIS_X4_UNUSED_CONTEXT_VALUE_TYPE_ERROR
    );

    // lvalue{lvalue} or non-reference{lvalue}
    constexpr context_storage(unused_type const&, std::remove_cvref_t<Next> const& next) noexcept
        : next(next)
    {}

    // non-reference{rvalue}
    constexpr context_storage(unused_type const&, std::remove_cvref_t<Next>&& next)
        noexcept(std::is_nothrow_constructible_v<Next, std::remove_cvref_t<Next>>)
        requires (!std::is_lvalue_reference_v<Next>)
        : next(std::move(next))
    {}

    // non-reference{const rvalue}
    constexpr context_storage(unused_type const&, std::remove_cvref_t<Next> const&& next)
        noexcept(std::is_nothrow_constructible_v<Next, std::remove_cvref_t<Next> const>)
        requires (!std::is_lvalue_reference_v<Next>)
        : next(std::move(next))
    {}

    // lvalue{rvalue}
    context_storage(unused_type const&, std::remove_cvref_t<Next> const&&) requires std::is_lvalue_reference_v<Next> = delete;

    [[nodiscard]] static constexpr unused_type const& get(std::type_identity<ID> const&) noexcept
    {
        return unused;
    }

    [[nodiscard]] constexpr decltype(auto) get(auto const& id) const noexcept
    {
        return next.get(id);
    }

    IRIS_NO_UNIQUE_ADDRESS Next next;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

template<class ID, class T, class Next>
    requires
        std::same_as<std::remove_const_t<T>, unused_type> &&
        std::same_as<std::remove_cvref_t<Next>, unused_type>
struct context_storage<ID, T, Next>
{
    static_assert(
        !detail::UniqueContextID<ID> || detail::AllowUnusedContextID<ID>,
        IRIS_X4_UNUSED_CONTEXT_VALUE_TYPE_ERROR
    );

    constexpr context_storage() noexcept = default;
    constexpr explicit context_storage(unused_type const&) noexcept {}
    constexpr context_storage(unused_type const&, unused_type const&) noexcept {}

    [[nodiscard]] static constexpr unused_type const& get(std::type_identity<ID> const&) noexcept
    {
        return unused;
    }

    static void get(auto const&) = delete; // ID not found
};

#undef IRIS_X4_UNUSED_CONTEXT_VALUE_TYPE_ERROR

} // detail

template<class ID, class T, class Next = unused_type>
struct context : detail::context_storage<ID, T, Next>
{
private:
    static_assert(
        !std::same_as<std::remove_cvref_t<Next>, unused_type> || std::same_as<Next, unused_type>,
        "Next cannot be a reference to `unused_type`; use plain type instead"
    );

    using storage_type = detail::context_storage<ID, T, Next>;

public:
    static_assert(detail::HasNoDuplicateContext<ID, std::remove_cvref_t<Next>>);

    using value_type = T;
    using next_type = Next;

    using storage_type::storage_type;
};

template<class ID, class T, class Next>
    requires
        (!std::same_as<std::remove_cvref_t<Next>, unused_type>)
[[nodiscard]] constexpr context<ID, T, detail::canonical_context_t<Next>>
make_context(T& val, Next&& next)
    noexcept(std::is_nothrow_constructible_v<detail::canonical_context_t<Next>, Next>)
{
    if constexpr (std::is_lvalue_reference_v<Next>) {
        // class `context` can only hold const lvalue reference
        return {val, std::as_const(next)};

    } else {
        return {val, std::forward<Next>(next)};
    }
}

template<class ID, class T>
[[nodiscard]] constexpr context<ID, T>
make_context(T& val, unused_type const&) noexcept
{
    return context<ID, T>{val};
}

template<class ID, class T>
[[nodiscard]] constexpr context<ID, T>
make_context(T& val) noexcept
{
    return context<ID, T>{val};
}

template<class ID, class T, class Next>
void make_context(T const&&, Next const&) = delete; // dangling

template<class ID, class T>
void make_context(T const&&) = delete; // dangling


// Remove the *leftmost* context having the id `ID_To_Remove`.
template<class ID_To_Remove, class ID, class T, class Next>
[[nodiscard]] constexpr decltype(auto) // may return existing reference in some cases
remove_first_context(context<ID, T, Next> const& ctx) noexcept
{
    // It does not make sense to remove the "first" context of non-unique ID,
    // because if you do so, you just can't determine the meaning of the remaining
    // (duplicate) entries.
    //
    // For example, let the context have below structure:
    //    context<
    //      tag, int,
    //      context<
    //        tag, float,
    //        context<
    //          tag, double
    //        >
    //      >
    //    >
    //
    // Applying `remove_first_context<tag>` results in this:
    //    context<
    //      tag, float,
    //      context<
    //        tag, double
    //      >
    //    >
    //
    // Although the operation is valid in type level, here's the question:
    // what do you *mean* by the remaining `float` and `double` contexts?
    //
    // As you can see, a context may have arbitrary number of duplicate
    // entries, and it just does not make sense to remove the "first" entry.
    //
    // For this reason, a "remove" operation on an any context is semantically
    // valid if and only if the ID is unique. In such case, `remove_first_context`
    // is essentially equal to `remove_*all*_context`, which is our intention.
    static_assert(detail::UniqueContextID<ID_To_Remove>);

    if constexpr (std::same_as<ID, ID_To_Remove>) { // Match
        if constexpr (std::same_as<Next, unused_type>) {
            // Existing context found; removing it will result in an
            // empty context, so create a monostate placeholder.
            return unused_type{};

        } else {
            // Existing context found; remove it and end the search.
            return (ctx.next);
        }

    } else  if constexpr (std::same_as<Next, unused_type>) {
        // No match at all. Return as-is.
        return ctx;

    } else {
        // Not match. Continue the replacement recursively.
        using NewNext = decltype(x4::remove_first_context<ID_To_Remove>(ctx.next));

        if constexpr (std::same_as<std::remove_cvref_t<NewNext>, std::remove_cvref_t<Next>>) {
            // Avoid creating copy on exact same type
            return ctx;

        } else if constexpr (std::same_as<std::remove_cvref_t<NewNext>, unused_type>) {
            // If the recursive replacement resulted in a monostate context,
            // prevent appending it; return the context without `next`.
            return context<ID, T>{ctx.val};

        } else {
            return context<ID, T, NewNext>{
                ctx.val, x4::remove_first_context<ID_To_Remove>(ctx.next)
            };
        }
    }
}

template<class ID_To_Remove>
[[nodiscard]] constexpr unused_type const&
remove_first_context(unused_type const&) noexcept
{
    return unused;
}

template<class ID_To_Remove, class ID, class T, class Next>
void remove_first_context(context<ID, T, Next> const&&) = delete; // dangling


// Remove *all* contexts having any of the ids `IDs_To_Remove...`.
template<class... IDs_To_Remove, class ID, class T, class Next>
[[nodiscard]] constexpr decltype(auto) // may return existing reference in some cases
remove_all_contexts(context<ID, T, Next> const& ctx) noexcept
{
    static_assert(sizeof...(IDs_To_Remove) > 0);

    if constexpr (iris::is_in_v<ID, IDs_To_Remove...>) { // Match
        if constexpr (std::same_as<Next, unused_type>) {
            // Existing context found; removing it will result in an
            // empty context, so create a monostate placeholder.
            return unused_type{};

        } else {
            // Existing context found; remove it and continue the search.
            return x4::remove_all_contexts<IDs_To_Remove...>(ctx.next);
        }

    } else if constexpr (std::same_as<Next, unused_type>) {
        // No match at all. Return as-is.
        return ctx;

    } else {
        // No match. Continue the replacement recursively.
        using NewNext = decltype(x4::remove_all_contexts<IDs_To_Remove...>(ctx.next));

        if constexpr (std::same_as<std::remove_cvref_t<NewNext>, std::remove_cvref_t<Next>>) {
            // Avoid creating copy on exact same type
            return ctx;

        } else if constexpr (std::same_as<std::remove_cvref_t<NewNext>, unused_type>) {
            // If the recursive replacement resulted in a monostate context,
            // prevent appending it; return the context without `next`.
            return context<ID, T>{ctx.val};

        } else {
            return context<ID, T, NewNext>{
                ctx.val, x4::remove_all_contexts<IDs_To_Remove...>(ctx.next)
            };
        }
    }
}

template<class... IDs_To_Remove>
[[nodiscard]] constexpr unused_type const&
remove_all_contexts(unused_type const&) noexcept
{
    return unused;
}

template<class... IDs_To_Remove, class ID, class T, class Next>
void remove_all_contexts(context<ID, T, Next> const&&) = delete; // dangling

namespace detail {

// Replaces the contained reference of the leftmost context
// having the id `ID_To_Replace`. If no such context exists,
// append or prepend a new one.
//
// This helper makes it possible to dynamically update the
// reference bound to the (runtime) context, while avoiding
// infinite instantiation in recursive grammars.
//
// The most notable example of a parser that requires this
// operation is `x4::with_local`. Without this helper, it would
// inevitably trigger infinite instantiation when binding
// a local variable instance to the context.
template<bool IsAppend, class ID_To_Replace, class ID, class T, class Next, class NewVal>
[[nodiscard]] constexpr decltype(auto)
replace_first_context_impl(
    context<ID, T, Next> const& ctx,
    NewVal& new_val IRIS_LIFETIMEBOUND
) noexcept
{
    static_assert(!is_ttp_specialization_of_v<std::remove_const_t<NewVal>, context>, "context's value type cannot be context");

    if constexpr (
        std::same_as<std::remove_const_t<NewVal>, unused_type> &&
        detail::UniqueContextID<ID_To_Replace> &&
        !detail::AllowUnusedContextID<ID_To_Replace>
    ) {
        (void)new_val; // == unused
        return x4::remove_first_context<ID_To_Replace>(ctx);

    } else if constexpr (!IsAppend && !has_context<context<ID, T, Next>, ID_To_Replace>) {
        return context<ID_To_Replace, NewVal, context<ID, T, Next> const&>{new_val, ctx};

    } else if constexpr (std::same_as<ID, ID_To_Replace>) { // Match
        if constexpr (std::same_as<Next, unused_type>) {
            // Existing context found; replace it and end the search.
            return context<ID, NewVal>{new_val};

        } else {
            // Existing context found; replace it and end the search.
            return context<ID, NewVal, Next const&>{new_val, ctx.next};
        }

    } else if constexpr (IsAppend && std::same_as<Next, unused_type>) {
        return context<ID, T, context<ID_To_Replace, NewVal>>{ctx.val, context<ID_To_Replace, NewVal>{new_val}};

    } else { // Not match
        // Continue the replacement recursively
        return context<ID, T, decltype(detail::replace_first_context_impl<IsAppend, ID_To_Replace>(ctx.next, new_val))>{
            ctx.val, detail::replace_first_context_impl<IsAppend, ID_To_Replace>(ctx.next, new_val)
        };
    }
}

} // detail

template<class ID_To_Replace, class ID, class T, class Next, class NewVal>
[[nodiscard]] constexpr decltype(auto)
replace_first_or_prepend_context(
    context<ID, T, Next> const& ctx,
    NewVal& new_val IRIS_LIFETIMEBOUND
) noexcept
{
    return detail::replace_first_context_impl<false, ID_To_Replace>(ctx, new_val);
}

template<class ID_To_Replace, class ID, class T, class Next, class NewVal>
[[nodiscard]] constexpr decltype(auto)
replace_first_or_append_context(
    context<ID, T, Next> const& ctx,
    NewVal& new_val IRIS_LIFETIMEBOUND
) noexcept
{
    return detail::replace_first_context_impl<true, ID_To_Replace>(ctx, new_val);
}

template<class ID_To_Replace, class NewVal>
[[nodiscard]] constexpr decltype(auto)
replace_first_or_prepend_context(
    unused_type const&,
    NewVal& new_val IRIS_LIFETIMEBOUND
) noexcept
{
    static_assert(!is_ttp_specialization_of_v<std::remove_const_t<NewVal>, context>, "context's value type cannot be context");

    if constexpr (
        detail::UniqueContextID<ID_To_Replace> &&
        !detail::AllowUnusedContextID<ID_To_Replace> &&
        std::same_as<std::remove_const_t<NewVal>, unused_type>
    ) {
        (void)new_val; // == unused
        return unused;

    } else {
        return context<ID_To_Replace, NewVal>{new_val};
    }
}

template<class ID_To_Replace, class NewVal>
[[nodiscard]] constexpr decltype(auto)
replace_first_or_append_context(
    unused_type const&,
    NewVal& new_val IRIS_LIFETIMEBOUND
) noexcept
{
    return x4::replace_first_or_prepend_context<ID_To_Replace>(unused, new_val);
}

template<class ID_To_Replace, class ID, class T, class Next, class NewVal>
void replace_first_or_prepend_context(context<ID, T, Next> const&, NewVal const&&) = delete; // dangling

template<class ID_To_Replace, class ID, class T, class Next, class NewVal>
void replace_first_or_prepend_context(context<ID, T, Next> const&&, NewVal const&) = delete; // dangling

template<class ID_To_Replace, class ID, class T, class Next, class NewVal>
void replace_first_or_append_context(context<ID, T, Next> const&, NewVal const&&) = delete; // dangling

template<class ID_To_Replace, class ID, class T, class Next, class NewVal>
void replace_first_or_append_context(context<ID, T, Next> const&&, NewVal const&) = delete; // dangling

namespace detail {

// Passes a context as `T`, the alias which a factory declares so that MSVC prints its name.
// An existing context (an lvalue) is referred to as it is; a new one (a prvalue, possibly const) is returned by value.
template<class T, class ContextT>
[[nodiscard]] constexpr decltype(auto) named_context(ContextT&& ctx) noexcept
{
    if constexpr (std::is_lvalue_reference_v<ContextT>) {
        return static_cast<T const&>(ctx);
    } else {
        return T(std::forward<ContextT>(ctx));
    }
}

} // detail

} // iris::x4

#endif
