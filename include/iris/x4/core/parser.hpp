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

#include <iris/config.hpp>

#include <iris/x4/core/attribute.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/parser_traits.hpp>

#include <iterator>
#include <string>
#include <type_traits>
#include <concepts>
#include <utility>

#ifndef IRIS_X4_NO_RTTI
#include <typeinfo>
#endif

namespace iris::x4 {

template<class Subject, class Action>
struct action;

namespace detail {

struct parser_base {};
struct parser_id;

template<class Storage>
struct parser_storage
    : private parser_base
    , protected Storage // needs to be non-private because we want to expose static public members
{
    // `Storage` is a class type

    using Storage::Storage;

    template<class StorageT>
        requires
            (!std::same_as<StorageT, parser_storage>) &&
            std::constructible_from<Storage, StorageT>
    constexpr explicit(!std::convertible_to<StorageT, Storage>)
    parser_storage(StorageT&& storage)
        noexcept(std::is_nothrow_constructible_v<Storage, StorageT>)
        : Storage(std::forward<StorageT>(storage))
    {}

protected:
    [[nodiscard]] constexpr Storage& storage() & noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage&>(*this);
    }
    [[nodiscard]] constexpr Storage const& storage() const& noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage const&>(*this);
    }
    [[nodiscard]] constexpr Storage&& storage() && noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage&&>(*this);
    }
    [[nodiscard]] constexpr Storage const&& storage() const&& noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage const&&>(*this);
    }
};

template<class Storage>
    requires (!std::is_class_v<Storage>)
struct parser_storage<Storage>
    : private parser_base
{
    // `Storage` is some non-class type (can't inherit from it; no EBO needed)

    template<class StorageT>
        requires
            (!std::same_as<StorageT, parser_storage>) &&
            std::constructible_from<Storage, StorageT>
    constexpr explicit(!std::convertible_to<StorageT, Storage>)
    parser_storage(StorageT&& storage)
        noexcept(std::is_nothrow_constructible_v<Storage, StorageT>)
        : storage_(std::forward<StorageT>(storage))
    {}

protected:
    [[nodiscard]] constexpr Storage& storage() & noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage&>(storage_);
    }
    [[nodiscard]] constexpr Storage const& storage() const& noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage const&>(storage_);
    }
    [[nodiscard]] constexpr Storage&& storage() && noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage&&>(storage_);
    }
    [[nodiscard]] constexpr Storage const&& storage() const&& noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage const&&>(storage_);
    }

private:
    Storage storage_;
};

template<class Storage>
    requires
        (!std::is_void_v<Storage>) &&
        std::is_base_of_v<parser_base, Storage>
struct parser_storage<Storage>
    : protected Storage // needs to be non-private because we want to expose static public members
{
    // `Storage` is some X4 parser class (possibly a `Subject`)
    // (which means we must not double-inherit from `parser_base`)

    using Storage::Storage;

    template<class StorageT>
        requires
            (!std::same_as<StorageT, parser_storage>) &&
            std::constructible_from<Storage, StorageT>
    constexpr explicit(!std::convertible_to<StorageT, Storage>)
    parser_storage(StorageT&& storage)
        noexcept(std::is_nothrow_constructible_v<Storage, StorageT>)
        : Storage(std::forward<StorageT>(storage))
    {}

protected:
    [[nodiscard]] constexpr Storage& storage() & noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage&>(*this);
    }
    [[nodiscard]] constexpr Storage const& storage() const& noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage const&>(*this);
    }
    [[nodiscard]] constexpr Storage&& storage() && noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage&&>(*this);
    }
    [[nodiscard]] constexpr Storage const&& storage() const&& noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Storage const&&>(*this);
    }
};

template<>
struct parser_storage<void>
    : private parser_base
{};

} // detail


template<class Storage = void>
struct parser : detail::parser_storage<Storage>
{
    static constexpr bool has_action = false;
    static constexpr bool need_rcontext = false;
    static constexpr bool requires_exact_attribute_type = false;

    using detail::parser_storage<Storage>::parser_storage;

    template<class Self, class Action>
        requires std::constructible_from<
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
        requires std::constructible_from<
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

template<class Subject>
struct unary_parser : parser<Subject>
{
    using subject_type = Subject;

    static constexpr bool has_action = Subject::has_action;
    static constexpr bool need_rcontext = Subject::need_rcontext;

    using parser<Subject>::parser;

    [[nodiscard]] constexpr Subject& subject() & noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Subject&>(*this);
    }
    [[nodiscard]] constexpr Subject const& subject() const& noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Subject const&>(*this);
    }
    [[nodiscard]] constexpr Subject&& subject() && noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Subject&&>(*this);
    }
    [[nodiscard]] constexpr Subject const&& subject() const&& noexcept IRIS_LIFETIMEBOUND
    {
        return static_cast<Subject const&&>(*this);
    }
};

template<class Subject, class Derived_Unused = void> // TODO
struct proxy_parser : unary_parser<Subject>
{
    using proxy_backend_type = Subject;
    using attribute_type = parser_traits<Subject>::attribute_type;

    static constexpr bool has_attribute = x4::has_attribute_v<Subject>;
    static constexpr std::size_t sequence_size = parser_traits<Subject>::sequence_size;

    template<class Container>
    static constexpr bool handles_container = parser_traits<Subject>::template handles_container<Container>;

    using unary_parser<Subject>::unary_parser;
};

template<class Left, class Right, class Derived_Unused = void> // TODO
struct binary_parser : parser<>
{
    using left_type = Left;
    using right_type = Right;

    static constexpr bool has_action = left_type::has_action || right_type::has_action;
    static constexpr bool need_rcontext = left_type::need_rcontext || right_type::need_rcontext;

    constexpr binary_parser() = default;

    template<class LeftT, class RightT>
        requires std::is_constructible_v<Left, LeftT> && std::is_constructible_v<Right, RightT>
    constexpr binary_parser(LeftT&& left, RightT&& right)
        noexcept(std::is_nothrow_constructible_v<Left, LeftT> && std::is_nothrow_constructible_v<Right, RightT>)
        : left(std::forward<LeftT>(left))
        , right(std::forward<RightT>(right))
    {}

    // TODO: EBO
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

//template<class Derived>
//    requires std::is_base_of_v<detail::parser_base, std::remove_cvref_t<Derived>>
//struct as_parser<Derived>
//{
//    template<class T>
//    [[nodiscard]] static constexpr auto&& call(T&& p) noexcept
//    {
//        return std::forward<T>(p);
//    }
//};
//
//template<class Derived>
//struct as_parser<parser<Derived>>
//{
//    template<class T>
//    [[nodiscard]] static constexpr auto&& call(T&& p) noexcept
//    {
//        return std::forward<T>(p).derived();
//    }
//};

} // traits

namespace detail {

template<class T>
concept has_parser_base = std::is_base_of_v<parser_base, std::remove_cvref_t<T>>;

template<class T>
concept has_custom_as_parser = requires(T&& p) {
    { x4::traits::as_parser<std::remove_cvref_t<T>>{}(std::forward<T>(p)) } -> has_parser_base;
};

struct as_parser_fn
{
    //template<class T>
    //static void operator()(T&&) = delete; // If you reach here, your specialization of `x4::extension::as_parser` has a wrong signature, or the type is simply incompatible.

    template<class T>
        requires has_custom_as_parser<T>
    [[nodiscard]] static constexpr decltype(auto)
    operator()(T&& x) noexcept(noexcept(traits::as_parser<std::remove_cvref_t<T>>{}(std::forward<T>(x))))
    {
        static_assert(has_parser_base<decltype(traits::as_parser<std::remove_cvref_t<T>>{}(std::forward<T>(x)))>);
        return traits::as_parser<std::remove_cvref_t<T>>{}(std::forward<T>(x));
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
template<X4Subject Parser, X4Subject T>
struct is_parser_constructible : std::false_type {};

template<X4Subject Parser, X4Subject T>
    requires std::is_constructible_v<Parser, as_parser_t<T>>
struct is_parser_constructible<Parser, T> : std::true_type {};

template<X4Subject Parser, X4Subject T>
constexpr bool is_parser_constructible_v = is_parser_constructible<Parser, T>::value;

// Checks whether `Parser(as_parser(t))` is noexcept.
//
// This interface can only be used to check whether `Parser`'s single-parameter
// constructor is available. For multi-parameter construction, manually combine
// `is_parser_nothrow_castable` with `std::is_nothrow_constructible`.
template<X4Subject Parser, X4Subject T>
struct is_parser_nothrow_constructible : std::false_type {};

template<X4Subject Parser, X4Subject T>
    requires
        is_parser_nothrow_castable_v<T> &&
        std::is_nothrow_constructible_v<Parser, as_parser_t<T>>
struct is_parser_nothrow_constructible<Parser, T> : std::true_type {};

template<X4Subject Parser, X4Subject T>
constexpr bool is_parser_nothrow_constructible_v = is_parser_nothrow_constructible<Parser, T>::value;


template<class Parser, class It, class Se, class Context, class Attr>
concept Parsable = requires(Parser const& p) {
    {
        p.parse(
            std::declval<It&>(), // first
            std::declval<Se>(), // last
            std::declval<Context const&>(), // context
            std::declval<Attr&>() // attr
        )
    } -> std::same_as<bool>;
};

template<class Parser, class It, class Se, class Context, class Attr>
struct is_parsable : std::bool_constant<Parsable<Parser, It, Se, Context, Attr>>
{
    static_assert(X4ExplicitSubject<Parser>);
    static_assert(!std::is_reference_v<It>);
    static_assert(std::forward_iterator<It>);
    static_assert(std::sentinel_for<Se, It>);
    static_assert(!std::is_reference_v<Context>);
    static_assert(!std::is_reference_v<Attr>);
    static_assert(X4Attribute<Attr>);
};

template<class Parser, class It, class Se, class Context, class Attr>
constexpr bool is_parsable_v = is_parsable<Parser, It, Se, Context, Attr>::value;

template<class Parser, class It, class Se, class Context, class Attr>
struct is_nothrow_parsable
{
    static_assert(X4ExplicitSubject<Parser>);
    static_assert(!std::is_reference_v<It>);
    static_assert(std::forward_iterator<It>);
    static_assert(std::sentinel_for<Se, It>);
    static_assert(!std::is_reference_v<Context>);
    static_assert(!std::is_reference_v<Attr>);
    static_assert(X4Attribute<Attr>);

    static constexpr bool value = requires(Parser const& p) {
        {
            p.parse(
                std::declval<It&>(), // first
                std::declval<Se>(), // last
                std::declval<Context const&>(), // context
                std::declval<Attr&>() // attr
            )
        } noexcept -> std::same_as<bool>;
    };
};

template<class Parser, class It, class Se, class Context, class Attr>
constexpr bool is_nothrow_parsable_v = is_nothrow_parsable<Parser, It, Se, Context, Attr>::value;


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

// "what" is an extremely common identifier that can be defined in many user-specific
// namespaces. We should avoid ADL usage for such generic names in the first place.
// (Note: CPO inhibits ADL in general.)
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

[[maybe_unused]] inline constexpr detail::what_fn what{}; // no ADL

} // cpos

} // iris::x4

#endif
