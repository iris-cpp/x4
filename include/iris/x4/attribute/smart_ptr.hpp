#ifndef IRIS_ZZ_X4_ATTRIBUTE_SMART_PTR_HPP
#define IRIS_ZZ_X4_ATTRIBUTE_SMART_PTR_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/parser_traits.hpp>

#include <concepts>
#include <string>
#include <iterator>
#include <type_traits>
#include <utility>
#include <memory>
#include <format>

namespace iris::x4 {

namespace detail {

// Since we need to do double-buffering anyway, we can additionally
// provide strong exception guarantee in the parse(...) functions
// below.
template<class Ptr>
struct [[nodiscard]] smart_ptr_rollback_guard
{
    ~smart_ptr_rollback_guard() noexcept
    {
        if (!parse_ok) {
            ptr = std::move(old_ptr);
        }
    }

    bool& parse_ok;
    Ptr& old_ptr;
    Ptr& ptr;
};

// ----------------------------------------------------

template<class Derived, class Subject, class T, class DeleterT>
struct unique_ptr_parser_base
    : proxy_parser<Subject, Derived>
{
    using base_type = proxy_parser<Subject, Derived>;

    // https://eel.is/c++draft/unique.ptr.single.ctor

    template<class SubjectT>
        requires
            (!std::is_pointer_v<DeleterT> && std::is_default_constructible_v<DeleterT>) &&
            (!std::same_as<std::remove_cvref_t<SubjectT>, Derived>) &&
            std::is_constructible_v<base_type, SubjectT>
    constexpr explicit unique_ptr_parser_base(SubjectT&& subject)
        noexcept(
            std::is_nothrow_constructible_v<base_type, SubjectT> &&
            std::is_nothrow_default_constructible_v<DeleterT>
        )
        : base_type(std::forward<SubjectT>(subject))
    {}

    template<class SubjectT>
        requires
            (!std::same_as<std::remove_cvref_t<SubjectT>, Derived>) &&
            std::is_constructible_v<base_type, SubjectT> &&
            std::is_constructible_v<DeleterT, DeleterT const&>
    constexpr unique_ptr_parser_base(SubjectT&& subject, DeleterT const& d)
        noexcept(
            std::is_nothrow_constructible_v<base_type, SubjectT> &&
            std::is_nothrow_default_constructible_v<DeleterT>
        )
        : base_type(std::forward<SubjectT>(subject))
        , deleter_(d)
    {}

    template<class SubjectT>
        requires
            (!std::same_as<std::remove_cvref_t<SubjectT>, Derived>) &&
            std::is_constructible_v<base_type, SubjectT> &&
            std::is_constructible_v<DeleterT, DeleterT&&>
    constexpr unique_ptr_parser_base(SubjectT&& subject, std::remove_reference_t<DeleterT>&& d)
        noexcept(
            std::is_nothrow_constructible_v<base_type, SubjectT> &&
            std::is_nothrow_default_constructible_v<DeleterT>
        )
        : base_type(std::forward<SubjectT>(subject))
        , deleter_(std::move(d))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& it, Se const& se, Context const& ctx, Attr& ptr) const
        noexcept(false) // never noexcept; requires dynamic memory allocation
    {
        static_assert(std::same_as<typename Attr::deleter_type, DeleterT>, "Incompatible deleter type provided for unique_ptr_parser");

        bool parse_ok = false;
        auto old_ptr = std::exchange(ptr, std::unique_ptr<T, DeleterT>(new T(), deleter_));

        smart_ptr_rollback_guard<std::unique_ptr<T, DeleterT>>
        guard{parse_ok, old_ptr, ptr};

        parse_ok = this->subject.parse(it, se, ctx, *ptr);
        return parse_ok;
    }

private:
    IRIS_NO_UNIQUE_ADDRESS DeleterT deleter_{};
};

template<class Derived, class Subject, class T>
struct unique_ptr_parser_base<Derived, Subject, T, std::default_delete<T>>
    : proxy_parser<Subject, Derived>
{
    using base_type = proxy_parser<Subject, Derived>;

    template<class SubjectT>
        requires
            (!std::same_as<std::remove_cvref_t<SubjectT>, Derived>) &&
            std::is_constructible_v<base_type, SubjectT>
    constexpr explicit unique_ptr_parser_base(SubjectT&& subject)
        noexcept(std::is_nothrow_constructible_v<base_type, SubjectT>)
        : base_type(std::forward<SubjectT>(subject))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& it, Se const& se, Context const& ctx, Attr& ptr) const
        noexcept(false) // never noexcept; requires dynamic memory allocation
    {
        static_assert(std::same_as<typename Attr::deleter_type, std::default_delete<T>>, "Incompatible deleter type provided for unique_ptr_parser");

        bool parse_ok = false;
        auto old_ptr = std::exchange(ptr, std::make_unique<T>());

        smart_ptr_rollback_guard<std::unique_ptr<T, std::default_delete<T>>>
        guard{parse_ok, old_ptr, ptr};

        parse_ok = this->subject.parse(it, se, ctx, *ptr);
        return parse_ok;
    }
};

} // detail

template<class Subject, class T = parser_traits<Subject>::attribute_type, class DeleterT = std::default_delete<T>>
struct unique_ptr_parser : detail::unique_ptr_parser_base<
    unique_ptr_parser<Subject, T, DeleterT>,
    Subject, T, DeleterT
>
{
    static_assert(X4Attribute<T>);
    static_assert(!X4UnusedAttribute<T>, "*_ptr_parser with `unused_type` is meaningless");

    using element_type = T;
    using deleter_type = DeleterT;
    using attribute_type = std::unique_ptr<T, DeleterT>;

    static constexpr bool requires_exact_attribute_type = true;

    using unique_ptr_parser::unique_ptr_parser_base::unique_ptr_parser_base;

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return std::format("unique_ptr({})", get_info<Subject>{}(this->subject));
    }
};

namespace parsers {

template<int = 0, X4Subject Subject>
[[nodiscard]] constexpr auto unique_ptr(Subject&& subject)
    noexcept(
        is_parser_nothrow_castable_v<Subject> &&
        std::is_nothrow_constructible_v<
            unique_ptr_parser<as_parser_plain_t<Subject>>,
            as_parser_t<Subject>
        >
    )
{
    return unique_ptr_parser<as_parser_plain_t<Subject>>{
        as_parser(std::forward<Subject>(subject))
    };
}

template<class T, class DeleterT = std::default_delete<T>, X4Subject Subject>
[[nodiscard]] constexpr auto unique_ptr(Subject&& subject)
    noexcept(
        is_parser_nothrow_castable_v<Subject> &&
        std::is_nothrow_constructible_v<
            unique_ptr_parser<as_parser_plain_t<Subject>, T, DeleterT>,
            as_parser_t<Subject>
        >
    )
{
    return unique_ptr_parser<as_parser_plain_t<Subject>, T, DeleterT>{
        as_parser(std::forward<Subject>(subject))
    };
}

// ^^^ These cannot be CPO because we want to overload them via template parameter

} // parsers

using parsers::unique_ptr;


// ----------------------------------------------------
// ----------------------------------------------------
// ----------------------------------------------------


namespace detail {

template<class Derived, class Subject, class T, class DeleterT>
struct shared_ptr_parser_base
    : proxy_parser<Subject, Derived>
{
    using base_type = proxy_parser<Subject, Derived>;

    // https://eel.is/c++draft/util.smartptr.shared.const

    template<class SubjectT>
        requires
            (!std::is_pointer_v<DeleterT> && std::is_default_constructible_v<DeleterT>) &&
            (!std::same_as<std::remove_cvref_t<SubjectT>, Derived>) &&
            std::is_constructible_v<base_type, SubjectT>
    constexpr explicit shared_ptr_parser_base(SubjectT&& subject)
        noexcept(
            std::is_nothrow_constructible_v<base_type, SubjectT> &&
            std::is_nothrow_default_constructible_v<DeleterT>
        )
        : base_type(std::forward<SubjectT>(subject))
    {}

    template<class SubjectT>
        requires
            (!std::same_as<std::remove_cvref_t<SubjectT>, Derived>) &&
            std::is_constructible_v<base_type, SubjectT> &&
            std::is_constructible_v<DeleterT, DeleterT const&>
    constexpr shared_ptr_parser_base(SubjectT&& subject, DeleterT const& d)
        noexcept(
            std::is_nothrow_constructible_v<base_type, SubjectT> &&
            std::is_nothrow_default_constructible_v<DeleterT>
        )
        : base_type(std::forward<SubjectT>(subject))
        , deleter_(d)
    {}

    template<class SubjectT>
        requires
            (!std::same_as<std::remove_cvref_t<SubjectT>, Derived>) &&
            std::is_constructible_v<base_type, SubjectT> &&
            std::is_constructible_v<DeleterT, DeleterT&&>
    constexpr shared_ptr_parser_base(SubjectT&& subject, std::remove_reference_t<DeleterT>&& d)
        noexcept(
            std::is_nothrow_constructible_v<base_type, SubjectT> &&
            std::is_nothrow_default_constructible_v<DeleterT>
        )
        : base_type(std::forward<SubjectT>(subject))
        , deleter_(std::move(d))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& it, Se const& se, Context const& ctx, Attr& ptr) const
        noexcept(false) // never noexcept; requires dynamic memory allocation
    {
        bool parse_ok = false;
        auto old_ptr = std::exchange(ptr, std::shared_ptr<T>(new T(), deleter_));

        smart_ptr_rollback_guard<std::shared_ptr<T>>
        guard{parse_ok, old_ptr, ptr};

        parse_ok = this->subject.parse(it, se, ctx, *ptr);
        return parse_ok;
    }

private:
    IRIS_NO_UNIQUE_ADDRESS DeleterT deleter_{};
};

template<class Derived, class Subject, class T>
struct shared_ptr_parser_base<Derived, Subject, T, std::default_delete<T>>
    : proxy_parser<Subject, Derived>
{
    using base_type = proxy_parser<Subject, Derived>;

    template<class SubjectT>
        requires
            (!std::same_as<std::remove_cvref_t<SubjectT>, Derived>) &&
            std::is_constructible_v<base_type, SubjectT>
    constexpr explicit shared_ptr_parser_base(SubjectT&& subject)
        noexcept(std::is_nothrow_constructible_v<base_type, SubjectT>)
        : base_type(std::forward<SubjectT>(subject))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& it, Se const& se, Context const& ctx, Attr& ptr) const
        noexcept(false) // never noexcept; requires dynamic memory allocation
    {
        bool parse_ok = false;
        auto old_ptr = std::exchange(ptr, std::make_shared<T>());

        smart_ptr_rollback_guard<std::shared_ptr<T>>
        guard{parse_ok, old_ptr, ptr};

        parse_ok = this->subject.parse(it, se, ctx, *ptr);
        return parse_ok;
    }
};

} // detail

template<class Subject, class T = parser_traits<Subject>::attribute_type, class DeleterT = std::default_delete<T>>
struct shared_ptr_parser : detail::shared_ptr_parser_base<
    shared_ptr_parser<Subject, T, DeleterT>,
    Subject, T, DeleterT
>
{
    static_assert(X4Attribute<T>);
    static_assert(!X4UnusedAttribute<T>, "*_ptr_parser with `unused_type` is meaningless");

    using element_type = T;
    using deleter_type = DeleterT;
    using attribute_type = std::shared_ptr<T>;

    static constexpr bool requires_exact_attribute_type = true;

    using shared_ptr_parser::shared_ptr_parser_base::shared_ptr_parser_base;

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return std::format("shared_ptr({})", get_info<Subject>{}(this->subject));
    }
};

namespace parsers {

template<int = 0, X4Subject Subject>
[[nodiscard]] constexpr auto shared_ptr(Subject&& subject)
    noexcept(
        is_parser_nothrow_castable_v<Subject> &&
        std::is_nothrow_constructible_v<
            shared_ptr_parser<as_parser_plain_t<Subject>>,
            as_parser_t<Subject>
        >
    )
{
    return shared_ptr_parser<as_parser_plain_t<Subject>>{
        as_parser(std::forward<Subject>(subject))
    };
}

template<class T, class DeleterT = std::default_delete<T>, X4Subject Subject>
[[nodiscard]] constexpr auto shared_ptr(Subject&& subject)
    noexcept(
        is_parser_nothrow_castable_v<Subject> &&
        std::is_nothrow_constructible_v<
            shared_ptr_parser<as_parser_plain_t<Subject>, T, DeleterT>,
            as_parser_t<Subject>
        >
    )
{
    return shared_ptr_parser<as_parser_plain_t<Subject>, T, DeleterT>{
        as_parser(std::forward<Subject>(subject))
    };
}

// ^^^ These cannot be CPO because we want to overload them via template parameter

} // parsers

using parsers::shared_ptr;

} // iris::x4

#endif
