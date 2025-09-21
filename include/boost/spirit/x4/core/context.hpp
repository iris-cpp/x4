#ifndef BOOST_SPIRIT_X4_CORE_CONTEXT_HPP
#define BOOST_SPIRIT_X4_CORE_CONTEXT_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/config.hpp>
#include <boost/spirit/core/type_traits.hpp>

#include <boost/spirit/x4/core/unused.hpp>

#include <concepts>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

    template <typename ID, typename T, typename Next>
    struct context;

    namespace detail {

        struct monostate_context_tag;
        using monostate_context = context<monostate_context_tag, void, unused_type>;

    } // detail

    template <typename ContextT>
    struct owning_context; // not defined

    template <typename ID, typename T, typename Next>
    struct owning_context<context<ID, T, Next>> {};


    // TODO: Rename. `get` is too generic name.
    template <typename ID, typename Context>
    [[nodiscard]] constexpr decltype(auto)
    get(Context const& context) noexcept
    {
        return context.get(std::type_identity<ID>{});
    }

    template <typename ID, typename Context>
    void get(Context const&&) = delete; // dangling

    // TODO: check whether auto-completion is available for this current implementation.
    // If not, we should implement a partially specialized metafunction instead.
    template <typename ID, typename Context>
    using get_context_plain_t = std::remove_cvref_t<decltype(x4::get<ID>(std::declval<Context const&>()))>;


    template <typename Context, typename ID_To_Search>
    struct has_context;

    template <typename Context, typename ID_To_Search>
    constexpr bool has_context_v = has_context<Context, ID_To_Search>::value;

    template <typename ID_To_Search>
    struct has_context<unused_type, ID_To_Search>
        : std::false_type
    {};

    template <typename T, typename Next, typename ID_To_Search>
    struct has_context<context<ID_To_Search, T, Next>, ID_To_Search>
        : std::true_type
    {};

    template <typename ID, typename T, typename Next, typename ID_To_Search>
        requires (!std::same_as<ID, ID_To_Search>)
    struct has_context<context<ID, T, Next>, ID_To_Search>
        : has_context<Next, ID_To_Search>
    {};

    template <typename ContextT, typename ID_To_Search>
    struct has_context<owning_context<ContextT>, ID_To_Search>
        : has_context<ContextT, ID_To_Search>
    {};

    template <typename Context, typename ID, typename T>
    struct has_context_of
    {
        static_assert(!std::is_reference_v<T>);
        static constexpr bool value = std::same_as<get_context_plain_t<ID, Context>, T>;
    };

    template <typename Context, typename ID, typename T>
    constexpr bool has_context_of_v = has_context_of<Context, ID, T>::value;


    template <typename ID>
    concept UniqueContextID = !requires { ID::is_unique; } || requires { requires ID::is_unique; };

    namespace detail {

        template <typename ID, typename Next>
        concept HasNoDuplicateContext = !UniqueContextID<ID> || !has_context_v<Next, ID>;

    } // detail

    template <typename ID, typename T, typename Next = unused_type>
    struct context
    {
        static_assert(!std::is_reference_v<T>);
        static_assert(!is_ttp_specialization_of_v<std::remove_const_t<T>, context>, "context's value type cannot be context");

        static_assert(!std::is_reference_v<Next>);
        static_assert(!std::is_const_v<Next>);
        static_assert(detail::HasNoDuplicateContext<ID, Next>);
        static_assert(!std::same_as<Next, detail::monostate_context>);

        constexpr context(T& val BOOST_SPIRIT_LIFETIMEBOUND, Next const& next BOOST_SPIRIT_LIFETIMEBOUND) noexcept
            : val(val)
            , next(next)
        {}

        context(T&, Next const&&) = delete; // dangling
        context(std::remove_const_t<T> const&&, Next const&) = delete; // dangling
        context(std::remove_const_t<T> const&&, Next const&&) = delete; // dangling

        constexpr context(context&&) = default;
        constexpr context& operator=(context&&) = default;
        constexpr context(context const&) = delete;
        constexpr context& operator=(context const&) = delete;

        [[nodiscard]] constexpr T& get(std::type_identity<ID>) const noexcept BOOST_SPIRIT_LIFETIMEBOUND
        {
            return val;
        }

        [[nodiscard]] constexpr decltype(auto) get(auto id) const noexcept
        {
            return next.get(id);
        }

        T& val;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
        Next const& next;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    };

    // Empty context specialization. Materialized when `remove_context` removed everything.
    template <>
    struct context<detail::monostate_context_tag, void>
    {
        [[nodiscard]] static constexpr unused_type const& get(auto) noexcept
        {
            return unused;
        }
    };

    template <typename ID, typename T>
    struct context<ID, T, unused_type>
    {
        static_assert(!std::is_reference_v<T>);
        static_assert(!is_ttp_specialization_of_v<std::remove_const_t<T>, context>, "context's value type cannot be context");

        constexpr explicit context(T& val BOOST_SPIRIT_LIFETIMEBOUND) noexcept
            requires (!std::same_as<std::remove_const_t<T>, context>)
            : val(val)
        {}

        constexpr context(T& val BOOST_SPIRIT_LIFETIMEBOUND, unused_type) noexcept
            : val(val)
        {}

        context(std::remove_const_t<T> const&&) = delete; // dangling
        context(std::remove_const_t<T> const&&, unused_type) = delete; // dangling

        constexpr context(context&&) = default;
        constexpr context& operator=(context&&) = default;
        constexpr context(context const&) = delete;
        constexpr context& operator=(context const&) = delete;

        [[nodiscard]] constexpr T& get(std::type_identity<ID>) const noexcept BOOST_SPIRIT_LIFETIMEBOUND
        {
            return val;
        }

        [[nodiscard]] static constexpr unused_type const& get(auto) noexcept
        {
            return unused;
        }

        T& val;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    };

    template <typename ID, typename T, typename Next>
    struct context<ID, T, owning_context<Next>>
    {
        static_assert(!std::is_reference_v<T>);
        static_assert(!is_ttp_specialization_of_v<std::remove_const_t<T>, context>, "context's value type cannot be context");
        static_assert(!std::same_as<std::remove_const_t<T>, unused_type>, "context holding `unused_type` as `T` is not supported");

        static_assert(!std::is_reference_v<Next>);
        static_assert(detail::HasNoDuplicateContext<ID, Next>);
        static_assert(!std::same_as<Next, detail::monostate_context>);

        template <typename OwningNext>
            requires std::is_constructible_v<Next, OwningNext>
        constexpr context(T& val BOOST_SPIRIT_LIFETIMEBOUND, OwningNext&& owning_next)
            noexcept(std::is_nothrow_constructible_v<Next, OwningNext>)
            : val(val)
            , next(std::forward<OwningNext>(owning_next))
        {}

        template <typename OwningNext>
            requires std::is_constructible_v<Next, OwningNext>
        context(std::remove_const_t<T> const&&, OwningNext&&) = delete; // dangling

        constexpr context(context&&) = default;
        constexpr context& operator=(context&&) = default;
        constexpr context(context const&) = delete;
        constexpr context& operator=(context const&) = delete;

        [[nodiscard]] constexpr T& get(std::type_identity<ID>) const noexcept BOOST_SPIRIT_LIFETIMEBOUND
        {
            return val;
        }

        [[nodiscard]] constexpr decltype(auto) get(auto id) const noexcept
        {
            return next.get(id);
        }

        T& val;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
        Next next; // not reference
    };


    template <typename ID, typename T, typename Next>
    [[nodiscard]] constexpr context<ID, T, Next>
    make_context(T& val, Next const& next) noexcept
    {
        return {val, next};
    }

    template <typename ID, typename T>
    [[nodiscard]] constexpr context<ID, T>
    make_context(T& val, detail::monostate_context const&) noexcept
    {
        return context<ID, T>{val};
    }

    template <typename ID, typename T, typename Next>
    void make_context(T const&&, Next const&) = delete; // dangling

    template <typename ID, typename T, typename Next>
    void make_context(T const&&, Next const&&) = delete; // dangling

    template <typename ID, typename T, typename Next>
    void make_context(T&, Next const&&) = delete; // dangling


    template <typename ID, typename T>
    [[nodiscard]] constexpr context<ID, T>
    make_context(T& val) noexcept
    {
        return context<ID, T>{val};
    }

    template <typename ID, typename T>
    void make_context(T const&&) = delete; // dangling


    // Replaces the contained reference of the leftmost context
    // having the id `ID_To_Replace`. If no such context exists,
    // append a new one.
    //
    // This helper makes it possible to dynamically update the
    // reference bound to the (runtime) context, while avoiding
    // infinite instantiation in recursive grammars.
    //
    // The most notable example of a parser that requires this
    // operation is `x4::locals`. Without this helper, it would
    // inevitably trigger infinite instantiation when binding
    // a local variable instance to the context.
    template <typename ID_To_Replace, typename ID, typename T, typename Next, typename NewVal>
    [[nodiscard]] constexpr auto
    replace_first_context(
        context<ID, T, Next> const& ctx,
        NewVal& new_val BOOST_SPIRIT_LIFETIMEBOUND
    ) noexcept
    {
        static_assert(!is_ttp_specialization_of_v<std::remove_const_t<NewVal>, context>, "context's value type cannot be context");
        static_assert(!std::same_as<ID_To_Replace, detail::monostate_context_tag>);

        if constexpr (std::same_as<ID, ID_To_Replace>) // match
        {
            if constexpr (std::same_as<Next, unused_type>)
            {
                // Existing context found; replace it and end the search.
                return context<ID, NewVal, Next>{new_val};
            }
            else
            {
                // Existing context found; replace it and end the search.
                //
                // This implementation does not replace succeeding (duplicate) entries,
                // because it is `replace_*first*_context`.
                return context<ID, NewVal, Next>{new_val, ctx.next};
            }
        }
        else // No match
        {
            if constexpr (std::same_as<ID, detail::monostate_context_tag>)
            {
                // No match at all. Create a brand-new context.
                return context<ID_To_Replace, NewVal>{new_val};
            }
            else if constexpr (std::same_as<Next, unused_type>)
            {
                // No match at all. Append a new one and return.
                // Since we're doing the search from left to right,
                // this branch means there was no existing context
                // for `ID_To_Replace`.
                using NewContext = context<ID_To_Replace, NewVal>;
                return context<ID, T, owning_context<NewContext>>{
                    ctx.val, NewContext{new_val}
                };
            }
            else
            {
                // No match. Continue the replacement recursively.
                using NewNext = decltype(x4::replace_first_context<ID_To_Replace>(ctx.next, new_val));
                return context<ID, T, owning_context<NewNext>>{
                    ctx.val, x4::replace_first_context<ID_To_Replace>(ctx.next, new_val)
                };
            }
        }
    }

    template <typename ID_To_Replace, typename ID, typename T, typename Next, typename NewVal>
    void replace_first_context(context<ID, T, Next> const&, NewVal const&&) = delete; // dangling

    // Remove the contained reference of the leftmost context having the id `ID_To_Remove`.
    template <typename ID_To_Remove, typename ID, typename T, typename Next>
    [[nodiscard]] constexpr decltype(auto) // may return existing reference in some cases
    remove_first_context(context<ID, T, Next> const& ctx) noexcept
    {
        static_assert(!std::same_as<ID_To_Remove, detail::monostate_context_tag>);

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
        static_assert(UniqueContextID<ID_To_Remove>);

        if constexpr (std::same_as<ID, ID_To_Remove>) // match
        {
            if constexpr (std::same_as<Next, unused_type>)
            {
                // Existing context found; removing it will result in an
                // empty context, so create a monostate placeholder.
                return detail::monostate_context{};
            }
            else
            {
                // Existing context found; remove it and end the search.
                return ctx.next;
            }
        }
        else // No match
        {
            if constexpr (std::same_as<ID, detail::monostate_context_tag> || std::same_as<Next, unused_type>)
            {
                // No match at all. Return as-is.
                return ctx;
            }
            else
            {
                // No match. Continue the replacement recursively.
                using NewNext = decltype(x4::remove_first_context<ID_To_Remove>(ctx.next));

                // If the recursive replacement resulted in a monostate context,
                // prevent appending it; return the context without `next`.
                if constexpr (std::same_as<NewNext, detail::monostate_context>)
                {
                    return context<ID, T>{ctx.val};
                }
                else if constexpr (std::is_reference_v<NewNext>)
                {
                    // Assert `decltype(auto)` is working as intended; i.e., no dangling reference
                    static_assert(std::is_lvalue_reference_v<NewNext>);

                    if constexpr (std::same_as<Next, std::remove_cvref_t<NewNext>>)
                    {
                        // Avoid creating copy on exact same type
                        return ctx;
                    }
                    else
                    {
                        return context<ID, T, std::remove_cvref_t<NewNext>>{
                            ctx.val, x4::remove_first_context<ID_To_Remove>(ctx.next)
                        };
                    }
                }
                else // prvalue context
                {
                    return context<ID, T, owning_context<NewNext>>{
                        ctx.val, x4::remove_first_context<ID_To_Remove>(ctx.next)
                    };
                }
            }
        }
    }

    template <typename ID_To_Remove, typename ID, typename T, typename Next>
    void remove_first_context(context<ID, T, Next> const&&) = delete; // dangling

} // boost::spirit::x4

#endif
