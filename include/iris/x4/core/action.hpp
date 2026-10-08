#ifndef IRIS_ZZ_X4_CORE_ACTION_HPP
#define IRIS_ZZ_X4_CORE_ACTION_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/traits/attribute_traits.hpp>
#include <iris/x4/traits/container_traits.hpp>

#include <iris/x4/core/detail/action_slot.hpp>

#include <iris/x4/core/attribute.hpp>
#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/context.hpp>
#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/action_context.hpp>
#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/write_attribute.hpp>

#include <iris/rvariant/variant_helper.hpp>

#include <iris/type_traits.hpp>

#include <iterator>
#include <concepts>
#include <memory>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace detail {

struct action_attribute_access;

// Returned by `x4::_attr` when the subject of the action may succeed without writing the attribute.
// The semantic action can read it only through `x4::visit_attr`.
//
// The visitor is called with:
//   - `unused` if the match wrote nothing
//   - the candidate held if `IsMultiCand` (the attribute is a variant of the candidates of an alternative)
//   - otherwise the attribute itself
template<class A, bool IsMultiCand>
class action_attribute_view
{
    constexpr explicit action_attribute_view(A* attr) noexcept
        : attr_(attr)
    {}

    A* attr_; // null if no attribute

    friend struct action_attribute_access;
};

template<class T>
struct is_action_attribute_view : std::false_type {};

template<class A, bool IsMultiCand>
struct is_action_attribute_view<action_attribute_view<A, IsMultiCand>> : std::true_type {};

template<class T>
inline constexpr bool is_action_attribute_view_v = is_action_attribute_view<std::remove_cvref_t<T>>::value;

struct action_attribute_access
{
    template<class A, bool IsMultiCand>
    [[nodiscard]] static constexpr action_attribute_view<A, IsMultiCand> make(A* attr) noexcept
    {
        return action_attribute_view<A, IsMultiCand>(attr);
    }

    template<class A, bool IsMultiCand, class Visitor>
    static constexpr void visit(action_attribute_view<A, IsMultiCand> const& view, Visitor&& visitor)
    {
        if (!view.attr_) {
            std::forward<Visitor>(visitor)(unused);
            return;
        }
        if constexpr (IsMultiCand) {
            view.attr_->visit([&](auto& candidate) { std::forward<Visitor>(visitor)(iris::unwrap_recursive(candidate)); });

        } else {
            std::forward<Visitor>(visitor)(iris::unwrap_recursive(*view.attr_));
        }
    }
};

} // detail

// Calls `fs` with the attribute the match of a semantic action wrote, as `T&`, or with `unused_type`
// if it wrote none. Two or more functions are combined by `iris::overloaded`, which copies or moves them.
template<class Context, class... Fs>
constexpr void visit_attr(Context const& ctx, Fs&&... fs)
{
    static_assert(sizeof...(Fs) >= 1);
    auto const& view = x4::_attr(ctx);
    static_assert(
        detail::is_action_attribute_view_v<decltype(view)>,
        "`x4::visit_attr` takes the attribute of a subject which may succeed without writing it"
    );
    if constexpr (sizeof...(Fs) == 1) {
        detail::action_attribute_access::visit(view, std::forward<Fs>(fs)...);

    } else {
        detail::action_attribute_access::visit(view, iris::overloaded{std::forward<Fs>(fs)...});
    }
}

namespace detail {

template<class Context, class Attr>
[[nodiscard]] constexpr decltype(auto) make_action_context(Context const& ctx, Attr& attr) noexcept
{
    // Declare a concrete alias type; MSVC prints the alias instead of actual type,
    // which makes the compilation error significantly shorter.
    if constexpr (X4UnusedAttribute<Attr>) {
        return (ctx);
    } else {
        using T = std::remove_cvref_t<decltype(x4::make_context<contexts::attr>(attr, ctx))>;
        return T{x4::make_context<contexts::attr>(attr, ctx)};
    }
}

} // detail

// Note about the constraint on the `Action` parameter:
//
// Ideally we should have a context-agnostic concept that can be used
// like `X4ActionFunctor<F>`, but we technically can't.
//
// In order to check whether it is invocable, we need to know the actual
// context type passed to the `.parse(...)` function, but it is unknown
// until runtime.
//
// Even if we make up the most trivial context type (i.e. `unused_type`),
// such concept will be useless because a user-provided functor always
// operates on user-specific precondition that assumes the context
// holds exact specific type provided to the entry point (`x4::parse`).

template<class Subject, class ActionF>
struct action : proxy_parser<action<Subject, ActionF>, Subject>
{
    static_assert(
        !std::is_reference_v<ActionF>,
        "Reference type is disallowed for semantic action functor to prevent dangling reference"
    );

    using base_type = proxy_parser<action, Subject>;

    static constexpr bool has_action = true;
    static constexpr bool need_rcontext = true;
    static constexpr bool requires_exact_attribute_type = false; // reset

    // The subject may succeed without writing its attribute (an alternative with a branch without
    // an attribute): the action sees the attribute the match wrote, or none (see `x4::visit_attr`)
    static constexpr bool sees_written_attribute =
        has_attribute_v<Subject> && detail::may_leave_attribute_unwritten_v<Subject>;

    template<class SubjectT, class ActionT>
        requires std::is_constructible_v<base_type, SubjectT> && std::is_constructible_v<ActionF, ActionT>
    constexpr action(SubjectT&& subject, ActionT&& f)
        noexcept(std::is_nothrow_constructible_v<base_type, SubjectT> && std::is_nothrow_constructible_v<ActionF, ActionT>)
        : base_type(std::forward<SubjectT>(subject))
        , f_(std::forward<ActionT>(f))
    {
    }

    // When the exposed attribute is `unused_type`.
    // Since we can assume that the subject unconditionally requires `_attr`,
    // we must create a temporary variable to pass it to the subject.
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
        requires (!sees_written_attribute)
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr&) const
    {
        It local_it = first;
        typename base_type::attribute_type attr_temp{}; // value-initialize
        if (this->subject.parse(local_it, last, ctx, attr_temp) && this->call_action(ctx, attr_temp)) {
            first = std::move(local_it);
            return true;
        }
        if constexpr (has_context<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) {
                // don't rollback iterator (mimicking exception-like behavior)
                first = std::move(local_it);
            }
        }
        return false;
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
        requires (!sees_written_attribute) && can_pass_exposed_attr<Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        It local_it = first;
        if (this->subject.parse(local_it, last, ctx, attr) && this->call_action(ctx, attr)) {
            first = std::move(local_it);
            return true;
        }
        if constexpr (has_context<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) {
                // don't rollback iterator (mimicking exception-like behavior)
                first = std::move(local_it);
            }
        }
        return false;
    }

    // When the exposed attribute is NOT `unused_type`.
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
        requires (!sees_written_attribute) && (!can_pass_exposed_attr<Attr>)
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& /* attr is discarded */) const
    {
        typename base_type::attribute_type attr_temp{}; // value-initialize
        It local_it = first;
        if (this->subject.parse(local_it, last, ctx, attr_temp) && this->call_action(ctx, attr_temp)) {
            first = std::move(local_it);
            return true;
        }
        if constexpr (has_context<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) {
                // don't rollback iterator (mimicking exception-like behavior)
                first = std::move(local_it);
            }
        }
        return false;
    }

    // The subject may succeed without writing its attribute. The subject parses into a slot of
    // the action, which records whether the match wrote the attribute; the action sees it through
    // a view, and the attribute is written into `attr` only when the action accepts the match.
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires sees_written_attribute
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        using attribute_type = base_type::attribute_type;
        constexpr bool is_multi_cand = detail::attribute_candidates_t<Subject>::size >= 2;

        It const saved_first = first;
        detail::action_slot<attribute_type> slot;
        if (!this->subject.parse(first, last, ctx, slot)) return false;

        auto const view = detail::action_attribute_access::make<attribute_type, is_multi_cand>(
            slot.is_generated() ? std::addressof(slot.value()) : nullptr
        );
        if (!this->call_action(ctx, view)) {
            first = saved_first;
            return false;
        }
        action::commit<is_multi_cand>(attr, slot);
        return true;
    }

    constexpr void operator[](auto const&) const = delete; // You can't add semantic action for semantic action

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return get_info<Subject>{}(this->subject) + "[f]";
    }

private:
    template<bool IsMultiCand, X4Attribute Attr, class A>
    static constexpr void commit(Attr& attr, detail::action_slot<A>& slot)
    {
        if constexpr (X4UnusedAttribute<Attr>) {
            return;

        } else if constexpr (detail::is_action_slot_v<Attr>) {
            if (!slot.is_generated()) {
                attr.disengage();
                return;
            }
            auto& engaged = attr.engage();
            detail::write_slot_value<IsMultiCand>(slot, [&engaged]<class V>(V&& value) {
                x4::write_attribute(engaged, std::forward<V>(value));
            });

        } else if constexpr (traits::X4Container<planner::storage_t<Attr>>) {
            if (slot.is_generated()) {
                detail::write_slot_value<IsMultiCand>(slot, [&attr]<class V>(V&& value) {
                    x4::write_attribute(attr, std::forward<V>(value));
                });
            }

        } else {
            traits::attribute_traits<Attr>::reset(attr);
            if (slot.is_generated()) {
                detail::write_slot_value<IsMultiCand>(slot, [&attr]<class V>(V&& value) {
                    x4::write_attribute(attr, std::forward<V>(value));
                });
            }
        }
    }

    template<class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    call_action(Context const& ctx, Attr& attr) const
    {
        if constexpr (directly_invocable<ActionF const&>) {
            using action_return_type = decltype(this->f_());
            if constexpr (std::same_as<action_return_type, bool>) {
                return this->f_();
            } else {
                static_assert(std::same_as<action_return_type, void>, "Semantic action should return either `bool` or `void`");
                this->f_();
                return true;
            }

        } else {
            // Inject `_attr` only when `Attr` is not `unused_type`
            using action_return_type = decltype(this->f_(detail::make_action_context(ctx, attr)));
            if constexpr (std::same_as<action_return_type, bool>) {
                return this->f_(detail::make_action_context(ctx, attr));
            } else {
                static_assert(std::same_as<action_return_type, void>, "Semantic action should return either `bool` or `void`");
                this->f_(detail::make_action_context(ctx, attr));
                return true;
            }
        }
    }

private:
    ActionF f_;
};

} // iris::x4

#endif
