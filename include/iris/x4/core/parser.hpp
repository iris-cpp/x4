#ifndef IRIS_ZZ_X4_CORE_PARSER_HPP
#define IRIS_ZZ_X4_CORE_PARSER_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/core/attribute.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/parser_traits.hpp>

#include <iris/string.hpp>

#include <ranges>
#include <string>
#include <type_traits>
#include <concepts>
#include <utility>

#ifndef IRIS_X4_NO_RTTI
#include <typeinfo>
#endif

#include <cstddef> // IWYU pragma: keep

namespace iris::x4 {

template<class Subject, class Action>
struct action;

template<
    // Although this parameter is largely unnecessary now that C++23 provides explicit object
    // parameters ("deducing this"), omitting it would cause virtually all parser classes to
    // inherit from the same empty base type, significantly increasing their size.
    //
    // In practice, shared-empty-base design would make a composite parser instance nearly
    // 3 to 8 times as large as one using the optimized layout. While the increased size would
    // have virtually no runtime impact (as the access to redundant empty instances would be
    // optimized-away anyway), it would still affect the compilation time.
    //
    // In other words, we currently require this parameter solely for making the base type
    // `parser<...>` distinct for each derived class.
    //
    // Note: The above describes the current design. Future changes may introduce an actual
    // dependency on `Derived`, regardless of the emptiness of the actual type.
    //
    // Caveat: Directly referring to `Derived` inside this base class (without using "deducing
    // this") is almost always wrong, as it is not guaranteed to point to the *most* derived
    // type when a class further derives from `Derived`.
    class Derived
>
struct parser
{
    using x4_parser_base_type = detail::parser_base;

    static constexpr bool has_action = false;
    static constexpr bool need_rcontext = false;
    static constexpr bool requires_exact_attribute_type = false;

    template<class Self, class Action>
        requires std::is_constructible_v<
            action<std::remove_cvref_t<Self>, std::remove_cvref_t<Action>>,
            Self, Action
        >
    [[nodiscard]]
    constexpr action<std::remove_cvref_t<Self>, std::remove_cvref_t<Action>>
    on_match(this Self&& self, Action&& f)
        noexcept(std::is_nothrow_constructible_v<
            action<std::remove_cvref_t<Self>, std::remove_cvref_t<Action>>,
            Self, Action
        >)
    {
        return {std::forward<Self>(self), std::forward<Action>(f)};
    }

    template<class Self, class Action>
        requires std::is_constructible_v<
            action<std::remove_cvref_t<Self>, std::remove_cvref_t<Action>>,
            Self, Action
        >
    [[nodiscard, deprecated("Use `p.on_match(...)` instead. The legacy `operator[]` syntax will be removed because it frequently conflicts with lambda syntax.")]]
    constexpr action<std::remove_cvref_t<Self>, std::remove_cvref_t<Action>>
    operator[](this Self&& self, Action&& f)
        noexcept(std::is_nothrow_constructible_v<
            action<std::remove_cvref_t<Self>, std::remove_cvref_t<Action>>,
            Self, Action
        >)
    {
        return std::forward<Self>(self).on_match(std::forward<Action>(f));
    }
};

template<class Derived, class Subject>
struct unary_parser : parser<Derived>
{
    using subject_type = Subject;

    static constexpr bool has_action = Subject::has_action;
    static constexpr bool need_rcontext = Subject::need_rcontext;

    constexpr unary_parser() = default;

    template<class SubjectT>
        requires std::same_as<std::remove_cvref_t<SubjectT>, Subject>
    constexpr explicit unary_parser(SubjectT&& subject)
        noexcept(std::is_nothrow_constructible_v<Subject, SubjectT>)
        : subject(std::forward<SubjectT>(subject))
    {}

    // Empty instance elimination technique:
    //
    // For the background, please read the comment on the `Derived` parameter of the base
    // `parser<...>` class first.
    //
    // In theory, we could go further and eliminate all empty parser instances, including
    // those stored in `unary_parser` and `binary_parser`, by omitting member variables
    // whose types are empty classes. However, doing so would require one of the following:
    //
    //   (a) Return a value-initialized instance on access:
    //       e.g. `binary_p.left() -> Empty`
    //
    //   (b) Return a reference to a static const instance:
    //       e.g. `binary_p.left() -> Empty const&`
    //
    // We benchmarked both approaches and found that each *increased* compilation time.
    IRIS_NO_UNIQUE_ADDRESS Subject subject;
};

template<class Derived, class Subject>
struct proxy_parser : unary_parser<Derived, Subject>
{
    using proxy_backend_type = Subject;
    using attribute_type = parser_traits<Subject>::attribute_type;

    static constexpr bool has_attribute = x4::has_attribute_v<Subject>;
    static constexpr std::size_t sequence_size = parser_traits<Subject>::sequence_size;

    template<class Container>
    static constexpr bool handles_container = parser_traits<Subject>::template handles_container<Container>;

    using unary_parser<Derived, Subject>::unary_parser;
};

template<class Derived, class Left, class Right>
struct binary_parser : parser<Derived>
{
    using left_type = Left;
    using right_type = Right;

    static constexpr bool has_action = Left::has_action || Right::has_action;
    static constexpr bool need_rcontext = Left::need_rcontext || Right::need_rcontext;

    constexpr binary_parser() = default;

    template<class LeftT, class RightT>
        requires std::same_as<std::remove_cvref_t<LeftT>, Left> && std::same_as<std::remove_cvref_t<RightT>, Right>
    constexpr binary_parser(LeftT&& left, RightT&& right)
        noexcept(std::is_nothrow_constructible_v<Left, LeftT> && std::is_nothrow_constructible_v<Right, RightT>)
        : left(std::forward<LeftT>(left))
        , right(std::forward<RightT>(right))
    {}

    // Empty instance elimination technique: please read the comment on `unary_parser`.
    IRIS_NO_UNIQUE_ADDRESS Left left;
    IRIS_NO_UNIQUE_ADDRESS Right right;
};

namespace traits {

template<class T>
struct as_parser; // not defined

template<>
struct as_parser<unused_type>
{
    template<class T>
    [[nodiscard]] static constexpr auto&& operator()(T&& unused_ IRIS_LIFETIMEBOUND) noexcept
    {
        return static_cast<T&&>(unused_);
    }
};

} // traits

namespace detail {

struct as_parser_char_array_tag {};

template<class T>
struct as_parser_plain_type
{
    using type = std::remove_cvref_t<T>;
};
template<CharLike CharT, std::size_t N>
struct as_parser_plain_type<CharT const (&)[N]>
{
    using type = as_parser_char_array_tag;
};

template<class T>
concept has_custom_as_parser = requires(T&& p) {
    { x4::traits::as_parser<typename as_parser_plain_type<T>::type>{}(std::forward<T>(p)) } -> has_parser_base;
};

struct as_parser_fn
{
    template<class T>
        requires has_custom_as_parser<T>
    [[nodiscard]] static constexpr decltype(auto)
    operator()(T&& x) noexcept(noexcept(traits::as_parser<typename as_parser_plain_type<T>::type>{}(std::forward<T>(x))))
    {
        static_assert(has_parser_base<decltype(traits::as_parser<typename as_parser_plain_type<T>::type>{}(std::forward<T>(x)))>);
        return traits::as_parser<typename as_parser_plain_type<T>::type>{}(std::forward<T>(x));
    }

    template<class P>
        requires
            (!has_custom_as_parser<P>) &&
            has_parser_base<P>
    [[nodiscard]] static constexpr auto&&
    operator()(P&& p) noexcept
    {
        return static_cast<P&&>(p);
    }

}; // as_parser_fn

} // detail

inline namespace cpos {

[[maybe_unused]] inline constexpr detail::as_parser_fn as_parser{};

} // cpos

template<class T>
using as_parser_t = decltype(as_parser(std::declval<T>())); // If you see an error here, your `T` is not castable to X4's parser type.

template<class T>
using as_parser_plain_t = std::remove_cvref_t<as_parser_t<T>>;


// This is a very low level API provided for consistency with
// `is_parser_nothrow_castable`. Most users should use `X4Subject`
// instead.
template<class T>
struct is_parser_castable
{
    static constexpr bool value = requires {
        { x4::as_parser(std::declval<T>()) };
    };
};

template<class T>
constexpr bool is_parser_castable_v = is_parser_castable<T>::value;

// This trait can be used primarily for multi-parameter constructors.
// For example, `a op b` normally requires the below condition to
// become fully noexcept:
//    is_parser_nothrow_castable_v<A> &&
//    is_parser_nothrow_castable_v<B> &&
//    std::is_nothrow_constructible_v<op_parser, as_parser_t<A>, as_parser_t<B>>
template<class T>
struct is_parser_nothrow_castable
{
    static constexpr bool value = requires {
        { x4::as_parser(std::declval<T>()) } noexcept;
    };
};

template<class T>
constexpr bool is_parser_nothrow_castable_v = is_parser_nothrow_castable<T>::value;


template<class T>
concept X4ExplicitSubject =
    detail::has_parser_base<T> &&
    std::move_constructible<std::remove_cvref_t<T>>;
    // Note: a lambda with a capture has a deleted move assignment operator,
    // thus requiring move assignable here would make such `x4::action` to
    // not satisfy this trait; we consider it too strict for now.

template<class T>
concept X4ImplicitSubject =
    !detail::has_parser_base<T> &&
    is_parser_castable_v<T> && // `as_parser(t)` is valid?
    X4ExplicitSubject<as_parser_t<T>>;

// A type that models `X4Subject` can be used in generic directives
// and operators. Note that this concept is iterator-agnostic.
//
// For example, let `p` denote an object of `T`. Then, `!p` is a
// well-formed NOT predicate in X4's domain (with "NOT predicate"
// referring to that of the PEG semantics) if and only if `T`
// models `X4Subject`.
template<class T>
concept X4Subject = X4ExplicitSubject<T> || X4ImplicitSubject<T>;

// ------------------------------------------------------------

template<X4Subject ParserT>
using as_parser_traits = parser_traits<as_parser_plain_t<ParserT>>;

template<X4Subject ParserT>
using as_parser_attr_t = parser_traits<as_parser_plain_t<ParserT>>::attribute_type;

// ------------------------------------------------------------


// Checks whether `Parser(as_parser(t))` is valid.
//
// This trait can be used for checking whether a "Parser" is constructible
// with some arbitrary argument `T`. In our (X4 core) use cases, the `Parser`
// is usually a concrete type (e.g. `some_parser<as_parser_plain_t<Subject>>`)
// whereas the `T` is some user-provided arbitrary type (e.g. `X4Subject Subject`).
//
// This interface can only be used to check whether `Parser`'s single-parameter
// constructor is available. For multi-parameter construction, manually combine
// `is_parser_castable` with `std::is_constructible`.
template<class Parser, class T>
struct is_parser_constructible : std::false_type {};

template<X4Subject Parser, X4Subject T>
    requires std::is_constructible_v<Parser, as_parser_t<T>>
struct is_parser_constructible<Parser, T> : std::true_type {};

template<class Parser, class T>
constexpr bool is_parser_constructible_v = is_parser_constructible<Parser, T>::value;

// Checks whether `Parser(as_parser(t))` is noexcept.
//
// This interface can only be used to check whether `Parser`'s single-parameter
// constructor is available. For multi-parameter construction, manually combine
// `is_parser_nothrow_castable` with `std::is_nothrow_constructible`.
template<class Parser, class T>
struct is_parser_nothrow_constructible : std::false_type {};

template<X4Subject Parser, X4Subject T>
    requires
        is_parser_nothrow_castable_v<T> &&
        std::is_nothrow_constructible_v<Parser, as_parser_t<T>>
struct is_parser_nothrow_constructible<Parser, T> : std::true_type {};

template<class Parser, class T>
constexpr bool is_parser_nothrow_constructible_v = is_parser_nothrow_constructible<Parser, T>::value;


template<class Parser, class It, class Se, class Context, class Attr>
concept Parsable = requires(Parser const& p, It& first, Se last, Context const& ctx, Attr& attr) {
    { p.parse(first, last, ctx, attr) } -> std::same_as<bool>;
};

template<class Parser, class It, class Se, class Context, class Attr>
struct is_parsable : std::bool_constant<Parsable<Parser, It, Se, Context, Attr>>
{};

template<class Parser, class It, class Se, class Context, class Attr>
constexpr bool is_parsable_v = is_parsable<Parser, It, Se, Context, Attr>::value;


template<class Parser, class It, class Se>
concept X4ExplicitParser =
    X4ExplicitSubject<Parser> &&
    Parsable<std::remove_cvref_t<Parser>, It, Se, unused_type, unused_type>;

template<class Parser, class It, class Se>
concept X4ImplicitParser =
    X4ImplicitSubject<Parser> &&
    Parsable<as_parser_plain_t<Parser>, It, Se, unused_type, unused_type>;

// The primary "parser" concept of X4, applicable in iterator-aware contexts.
//
// Let `it` denote an lvalue reference of `It`, and let `se` denote a prvalue of `Se`.
// Let `p` denote an lvalue reference of `Parser`.
//
// For `Parser` to model `X4Parser`, the following conditions must be satisfied:
//   -- the expression `x4::as_parser(p)` is well-formed in unevaluated context, and
//   -- the expression `cp.parse(it, se, x4::unused, x4::unused)`
//      is well-formed and the return type is same as `bool`, where `cp` denotes a
//      const lvalue reference to the result of the expression `x4::as_parser(p)`.
//
// Although some exotic user-defined parser could be designed to operate on the very
// specific context type and/or attribute type, we require the parser to at least
// accept `unused_type` for `Context` and `Attribute`. This is because
// core parsers of Spirit have historically been assuming natural use of `unused_type`
// in many locations.
template<class Parser, class It, class Se>
concept X4Parser = X4ExplicitParser<Parser, It, Se> || X4ImplicitParser<Parser, It, Se>;


// The runtime type info that can be obtained via `x4::what(p)`.
// Note: X4's builtin parsers must NOT use `std::format` for implementing this.
//       (It has been confirmed that it would cause significant compilation time bloat)
template<class Subject>
struct get_info
{
    static_assert(X4Subject<Subject>);

    [[nodiscard]] static constexpr std::string operator()(Subject const& subject)
    {
        if constexpr (requires {
            { subject.get_x4_info() } -> std::convertible_to<std::string>;
        }) {
            return subject.get_x4_info();

        } else {
            (void)subject;
    #ifndef IRIS_X4_NO_RTTI
            return typeid(Subject).name();
    #else
            return "(get_info undefined)";
    #endif
        }
    }
};

namespace detail {

struct what_fn
{
    template<X4Subject Subject>
    [[nodiscard]] static constexpr std::string operator()(Subject const& p)
    {
        return get_info<Subject>{}(p);
    }
};

} // detail

inline namespace cpos {

[[maybe_unused]] inline constexpr detail::what_fn what{};

} // cpos

} // iris::x4

#endif
