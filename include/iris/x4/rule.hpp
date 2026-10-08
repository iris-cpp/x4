#ifndef IRIS_ZZ_X4_RULE_HPP
#define IRIS_ZZ_X4_RULE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/traits/tuple_traits.hpp>
#include <iris/x4/core/traits/write_rank.hpp>
#include <iris/x4/core/write_attribute.hpp>
#include <iris/x4/traits/container_traits.hpp>

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/context.hpp>
#include <iris/x4/core/action_context.hpp>

#include <iris/x4/debug/error_handler.hpp>

#include <iris/pp/cat.hpp>
#include <iris/pp/stringize.hpp>
#include <iris/pp/arg.hpp>

#include <string_view>
#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

namespace detail {

// A link error for a missing `IRIS_X4_INSTANTIATE` shows the parameter types of the
// undefined `parse_rule`. Passing the attribute through this wrapper shows only `RuleID`
// there, instead of the whole attribute type.
template<class RuleID>
struct rule_attr_ref
{
    RuleID::rule_attribute_type& attr;
};

// Fallback selected when no `parse_rule` for this rule is visible at the point
// where the rule's `parse` is instantiated.
//
// - For a rule declared with `IRIS_X4_DECLARE`, this means `IRIS_X4_DEFINE` is
//   missing or lives in another translation unit.
//   Note: A definition placed after the first use is ill-formed, no diagnostic
//         required.
//
// - A rule declared with `IRIS_X4_DECLARE_PUBLIC` never reaches here, since that
//   declaration is more specialized than this overload; a missing definition for
//   it is left to the linker.
template<class RuleID, class It, class Se, class Context, class RuleAttrRefT>
[[nodiscard]] constexpr bool
parse_rule(RuleID, It&, Se const&, Context const&, RuleAttrRefT)
{
    static_assert(
        std::is_void_v<RuleID>,
        "`IRIS_X4_DEFINE` is not visible for this rule. If the rule is defined in another "
        "translation unit with `IRIS_X4_INSTANTIATE`, use `IRIS_X4_DECLARE_PUBLIC` and `IRIS_X4_DEFINE_PUBLIC`."
    );
    return false; // dummy
}

// A link error for a missing `IRIS_X4_INSTANTIATE` also names the caller of `parse_rule`.
// Unless this function is inlined (it is not in Debug builds), the caller is this function
// rather than `x4::rule<...>::parse`, whose name contains the whole `RuleAttr`.
template<class RuleID, class It, class Se, class Context, class RuleAttrRefT>
[[nodiscard]] constexpr bool
call_parse_rule(It& first, Se const& last, Context const& ctx, RuleAttrRefT attr_ref)
{
    return parse_rule(RuleID{}, first, last, ctx, attr_ref); // ADL
}

template<class Context>
[[nodiscard]] constexpr decltype(auto) make_rule_agnostic_context(Context const& ctx) noexcept
{
    // Declare a concrete alias type; MSVC prints the alias instead of actual type,
    // which makes the compilation error significantly shorter.
    using T = std::remove_cvref_t<decltype(x4::remove_first_context<contexts::rule_var>(ctx))>;
    return detail::named_context<T>(x4::remove_first_context<contexts::rule_var>(ctx));
}

} // detail

template<class RuleID>
struct rule : parser<rule<RuleID>>
{
    // This type MUST be constructible with incomplete attribute types.
    // Do NOT add `static_assert`s or other constructs that cause eager
    // instantiation of `RuleID::rule_attribute_type` within class body.
    // Note: `RuleID` must be complete regardless.

    using attribute_type = RuleID::rule_attribute_type;
    static_assert(!std::is_const_v<attribute_type>);

    static constexpr bool has_attribute = !std::is_same_v<attribute_type, unused_type>;

    std::string_view name = "unnamed_rule";

    consteval rule() = default;

    consteval explicit rule(std::string_view name) noexcept
        : name(name)
    {
        // Don't place `check_invariants()` here; rule must be able to construct with incomplete type
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, class ExposedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, ExposedAttr& exposed_attr) const
    {
        check_invariants();
        static_assert(X4NonUnusedAttribute<ExposedAttr>);
        static_assert(has_attribute, "A rule must have an attribute. Check your rule definition.");

        // Remove the `_rule_var` context. This makes the actual `context` type passed to
        // the (potentially ADL-found) `parse_rule` function to be rule-agnostic.
        // If we don't do this, the specialized function signature becomes
        // nondeterministic, and we lose the opportunity to do explicit template
        // instantiation in `IRIS_X4_INSTANTIATE`.
        //
        // This removal is safe because `call_rule_definition` puts the `_rule_var` context
        // back whenever the definition or `RuleID` may use it.
        auto&& rule_agnostic_ctx = detail::make_rule_agnostic_context(ctx);

        if constexpr (std::same_as<std::remove_const_t<ExposedAttr>, attribute_type>) {
            if constexpr (traits::X4Container<attribute_type>) {
                if (!std::ranges::empty(exposed_attr)) {
                    // The container holds the preceding results, which the attribute of the rule
                    // is kept apart from; parse into a new attribute and append it on success.
                    attribute_type rule_attr{};
                    if (!detail::call_parse_rule<RuleID>(first, last, rule_agnostic_ctx, detail::rule_attr_ref<RuleID>{rule_attr})) {
                        return false;
                    }
                    planner::pass_declared_attribute(exposed_attr, std::move(rule_attr));
                    return true;
                }
            }
            return detail::call_parse_rule<RuleID>(first, last, rule_agnostic_ctx, detail::rule_attr_ref<RuleID>{exposed_attr});

        } else if constexpr (detail::holds_as_single_element<std::remove_const_t<ExposedAttr>, attribute_type>) {
            return this->parse(first, last, ctx, alloy::get<0>(exposed_attr));

        } else {
            static_assert(X4StrictlyWritable<std::remove_const_t<ExposedAttr>&, unwrap_recursive_t<attribute_type>&&>);
            static_assert(!detail::dangles<std::remove_const_t<ExposedAttr>, unwrap_recursive_t<attribute_type>&&>);

            attribute_type rule_attr{}; // value-initialize
            if (!detail::call_parse_rule<RuleID>(first, last, rule_agnostic_ctx, detail::rule_attr_ref<RuleID>{rule_attr})) {
                return false;
            }
            planner::pass_declared_attribute(exposed_attr, iris::unwrap_recursive(std::move(rule_attr)));
            return true;
        }
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] static constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr&)
    {
        check_invariants();

        // Make sure we pass exactly the rule attribute type to not break `IRIS_X4_INSTANTIATE` use case
        attribute_type unused_rule_attr{}; // value-initialize

        // See the comments on the primary overload of `rule::parse(...)`
        auto&& rule_agnostic_ctx = detail::make_rule_agnostic_context(ctx);

        return detail::call_parse_rule<RuleID>(first, last, rule_agnostic_ctx, detail::rule_attr_ref<RuleID>{unused_rule_attr});
    }

    [[nodiscard]] constexpr std::string_view get_x4_info() const noexcept
    {
        return name;
    }

private:
    static constexpr void check_invariants() noexcept
    {
        static_assert(X4Attribute<attribute_type>);
        if constexpr (X4NonUnusedAttribute<attribute_type>) {
            static_assert(X4ValueAttribute<attribute_type>);
        }
        static_assert(!std::is_same_v<attribute_type, unused_container_type>, "`rule` with `unused_container_type` is not supported");
    }
};

namespace parsers {
using x4::rule;
} // parsers

// -------------------------------------------------------------

namespace detail {

template<
    class RuleID,
    std::forward_iterator It, std::sentinel_for<It> Se,
    class Context, X4Attribute ExposedAttr
>
[[nodiscard]] constexpr bool
call_rule_definition(
    [[maybe_unused]] std::string_view const rule_name,
    It& first, Se const& last,
    Context const& ctx, ExposedAttr& exposed_attr
)
{
    auto const& rule_def_parser = get_rule_definition(RuleID{}); // ADL
    using RuleDefParserT = std::remove_cvref_t<decltype(rule_def_parser)>;
    static_assert(X4Subject<RuleDefParserT>);

    bool ok = false;

    // Debug on destructor
    [[maybe_unused]] scoped_tracer<RuleID, It, Se, Context, ExposedAttr>
    scoped_tracer{first, last, ctx, exposed_attr, rule_name, &ok};

    // The existence of semantic action inhibits attribute materialization _unless_ it is
    // explicitly requested by the user.
    //
    // Note: `x4::as<T>(...)` explicitly unsets `has_action` even if the underlying subject
    // has semantic action, so it will be dispatched to the latter branch (unless the
    // `as_type_parser` itself has semantic action).
    auto& attr = [&] noexcept -> auto& {
        if constexpr (RuleDefParserT::has_action) {
            return unused;
        } else {
            return exposed_attr;
        }
    }();
    using MaterializedAttr = std::remove_cvref_t<decltype(attr)>;

    auto const make_rcontext = [&] noexcept -> decltype(auto) {
        if constexpr (
            RuleDefParserT::need_rcontext ||
            has_on_success<RuleID, It, It /* NOT `Se` */, Context, MaterializedAttr>::value ||
            has_on_expectation_failure<RuleID, It, Se, Context>::value
        ) {
            return x4::replace_first_or_prepend_context<contexts::rule_var>(ctx, exposed_attr);
        } else {
            return (ctx);
        }
    };
    // Declare a concrete alias type; MSVC prints the alias instead of actual type,
    // which makes the compilation error significantly shorter.
    using RContext = std::remove_cvref_t<decltype(make_rcontext())>;
    RContext const& rcontext = make_rcontext();

    if constexpr (has_on_success<RuleID, It, It /* NOT `Se` */, Context, MaterializedAttr>::value) {
        It start = first; // backup

        ok = rule_def_parser.parse(first, last, rcontext, attr);
        if (ok) {
            x4::skip_over(start, first, rcontext);
            RuleID{}.on_success(std::as_const(start), std::as_const(first), rcontext, attr);

        } else if constexpr (has_on_expectation_failure<RuleID, It, Se, Context>::value) {
            if (x4::has_expectation_failure(rcontext)) {
                RuleID{}.on_expectation_failure(
                    std::as_const(first), std::as_const(last), rcontext,
                    x4::get_expectation_failure(rcontext)
                );
            }
        }

    } else { // does not have `on_success`
        if constexpr (has_on_expectation_failure<RuleID, It, Se, Context>::value) {
            ok = rule_def_parser.parse(first, last, rcontext, attr);
            if (!ok && x4::has_expectation_failure(rcontext)) {
                RuleID{}.on_expectation_failure(
                    std::as_const(first), std::as_const(last), rcontext,
                    x4::get_expectation_failure(rcontext)
                );
            }

        } else {
            ok = rule_def_parser.parse(first, last, rcontext, attr);
        }
    }
    return ok;
}

} // detail

// -------------------------------------------------------------

#define IRIS_ZZ_X4_PARSE_RULE_SIGNATURE(constexpr_, rule_name) \
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context> \
    [[nodiscard]] constexpr_ bool \
    parse_rule( \
        IRIS_PP_CAT(rule_name, _id), \
        It& first, Se const& last, \
        Context const& ctx, \
        ::iris::x4::detail::rule_attr_ref<IRIS_PP_CAT(rule_name, _id)> attr_ref \
    )

// Declares a rule whose parse function is defined as constexpr by `IRIS_X4_DEFINE`.
// Note: If `RuleAttr` contains a comma, wrap the entire type with parentheses.
// Note: If you need to use `IRIS_X4_INSTANTIATE`, use `IRIS_X4_DECLARE_PUBLIC`.
#define IRIS_X4_DECLARE(rule_name, RuleAttr, ...) \
    namespace rules { \
    struct IRIS_PP_CAT(rule_name, _id) __VA_OPT__(:) __VA_ARGS__ { using rule_attribute_type = IRIS_PP_UNPAREN_IF_PAREN(RuleAttr); }; \
    using IRIS_PP_CAT(rule_name, _rule) = ::iris::x4::rule<IRIS_PP_CAT(rule_name, _id)>; \
    } /* rules */ \
    inline constexpr rules::IRIS_PP_CAT(rule_name, _rule) rule_name{IRIS_PP_STRINGIZE(rule_name)};

// Declares a rule and its non-constexpr parse function, which `IRIS_X4_DEFINE_PUBLIC` defines.
// Can be used with `IRIS_X4_INSTANTIATE`.
// Note: If `RuleAttr` contains a comma, wrap the entire type with parentheses.
#define IRIS_X4_DECLARE_PUBLIC(rule_name, RuleAttr, ...) \
    IRIS_X4_DECLARE(rule_name, RuleAttr, __VA_ARGS__) \
    namespace rules { \
    IRIS_ZZ_X4_PARSE_RULE_SIGNATURE(, rule_name); \
    } /* rules */

// -------------------------------------------------------------

#define IRIS_ZZ_X4_DEFINE_I(constexpr_, rule_name) \
    namespace rules { \
    [[nodiscard]] inline constexpr_ auto const& get_rule_definition(IRIS_PP_CAT(rule_name, _id)) noexcept \
    { \
        return IRIS_PP_CAT(rule_name, _def); \
    } \
    \
    IRIS_ZZ_X4_PARSE_RULE_SIGNATURE(constexpr_, rule_name) \
    { \
        return ::iris::x4::detail::call_rule_definition<IRIS_PP_CAT(rule_name, _id)>( \
            rule_name.name, first, last, ctx, attr_ref.attr \
        ); \
    } \
    } /* rules */

// Defines a constexpr parse function.
// Note: If you need to use `IRIS_X4_INSTANTIATE`, use `IRIS_X4_DEFINE_PUBLIC`.
#define IRIS_X4_DEFINE(rule_name) IRIS_ZZ_X4_DEFINE_I(constexpr, rule_name)

// Defines a non-constexpr parse function.
// Can be used with `IRIS_X4_INSTANTIATE`.
#define IRIS_X4_DEFINE_PUBLIC(rule_name) IRIS_ZZ_X4_DEFINE_I(, rule_name)

// -------------------------------------------------------------

#define IRIS_ZZ_X4_FIRST(x, ...) x

#define IRIS_ZZ_X4_INSTANTIATE_I(export_macro, rule_name, It, Se, Context) \
    namespace rules { \
    template export_macro bool parse_rule<It, Se, Context>( \
        IRIS_PP_CAT(rule_name, _id), \
        It&, Se const&, Context const&, \
        ::iris::x4::detail::rule_attr_ref<IRIS_PP_CAT(rule_name, _id)> \
    ); \
    } /* rules */

// Explicitly instantiates `parse_rule` for the given iterator, sentinel, and
// context types. This allows the grammar definition to live entirely in a
// .cpp file, keeping the X4 dependency of public headers to a minimum.
//
// The rule must be declared with `IRIS_X4_DECLARE_PUBLIC` and defined with
// `IRIS_X4_DEFINE_PUBLIC`.
//
// Note: If a type contains a comma, wrap the entire type with parentheses.
//
// Usage:
//   - `IRIS_X4_INSTANTIATE(rule_name, It, Context)`
//   - `IRIS_X4_INSTANTIATE(rule_name, It, Se, Context)`
#define IRIS_X4_INSTANTIATE(rule_name, It, SeOrContext, ...) \
    IRIS_ZZ_X4_INSTANTIATE_I( \
        , /* export_macro */ \
        rule_name, \
        IRIS_PP_UNPAREN_IF_PAREN(It), \
        IRIS_PP_UNPAREN_IF_PAREN(IRIS_ZZ_X4_FIRST(__VA_OPT__(SeOrContext,) It)), \
        IRIS_PP_UNPAREN_IF_PAREN(IRIS_ZZ_X4_FIRST(__VA_ARGS__ __VA_OPT__(,) SeOrContext)) \
    )

// Similar to `IRIS_X4_INSTANTIATE`, but also exports the instantiation
// across DLL boundaries using `export_macro`.
//
// Note: If a type contains a comma, wrap the entire type with parentheses.
//
// Usage:
//   - `IRIS_X4_INSTANTIATE_EXPORT(export_macro, rule_name, It, Context)`
//   - `IRIS_X4_INSTANTIATE_EXPORT(export_macro, rule_name, It, Se, Context)`
#define IRIS_X4_INSTANTIATE_EXPORT(export_macro, rule_name, It, SeOrContext, ...) \
    IRIS_ZZ_X4_INSTANTIATE_I( \
        export_macro, \
        rule_name, \
        IRIS_PP_UNPAREN_IF_PAREN(It), \
        IRIS_PP_UNPAREN_IF_PAREN(IRIS_ZZ_X4_FIRST(__VA_OPT__(SeOrContext,) It)), \
        IRIS_PP_UNPAREN_IF_PAREN(IRIS_ZZ_X4_FIRST(__VA_ARGS__ __VA_OPT__(,) SeOrContext)) \
    )

} // iris::x4

#endif
