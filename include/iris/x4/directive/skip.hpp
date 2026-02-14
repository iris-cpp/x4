#ifndef IRIS_X4_DIRECTIVE_SKIP_HPP
#define IRIS_X4_DIRECTIVE_SKIP_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/context.hpp>
#include <iris/x4/core/skip_over.hpp>
#include <iris/x4/core/parser.hpp>

#include <iris/x4/char/char_class_tags.hpp>

#include <format>
#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

template<class Encoding, class Tag>
struct char_class_parser;


template<class Subject, class Skipper>
struct skip_directive : proxy_parser<Subject, skip_directive<Subject, Skipper>>
{
    using base_type = proxy_parser<Subject, skip_directive>;

    template<class SubjectT, class SkipperT>
        requires std::is_constructible_v<base_type, SubjectT> && std::is_constructible_v<Skipper, SkipperT>
    constexpr skip_directive(SubjectT&& subject, SkipperT&& skipper)
        noexcept(std::is_nothrow_constructible_v<base_type, SubjectT> && std::is_nothrow_constructible_v<Skipper, SkipperT>)
        : base_type(std::forward<SubjectT>(subject))
        , skipper_(std::forward<SkipperT>(skipper))
    {}

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(is_nothrow_parsable_v<Subject, It, Se, context_t<Context>, Attr>)
    {
        return this->subject.parse(first, last, x4::replace_first_context<contexts::skipper>(ctx, skipper_), attr);
    }

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return std::format(
            "skip({})[{}]",
            get_info<Skipper>{}(this->skipper_),
            get_info<Subject>{}(this->subject)
        );
    }

private:
    template<class Context>
    using context_t = std::remove_cvref_t<decltype(
        x4::replace_first_context<contexts::skipper>(std::declval<Context const&>(), std::declval<Skipper&>())
    )>;

    Skipper skipper_;
};


template<builtin_skipper_kind Kind, class Subject>
struct builtin_skip_directive : proxy_parser<Subject, builtin_skip_directive<Kind, Subject>>
{
    using base_type = proxy_parser<Subject, builtin_skip_directive>;
    using base_type::base_type;

    // Has existing builtin skipper
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(is_nothrow_parsable_v<Subject, It, Se, Context, Attr>)
    {
        builtin_skipper_kind& skipper_kind = x4::get<contexts::skipper>(ctx);
        auto const old_skipper_kind = skipper_kind;
        skipper_kind = Kind;

        bool const ok = this->subject.parse(first, last, ctx, attr);

        skipper_kind = old_skipper_kind;
        return ok;
    }

    // No existing builtin skipper
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires
            std::same_as<get_context_plain_t<contexts::skipper, Context>, unused_type> ||
            (!std::same_as<get_context_plain_t<contexts::skipper, Context>, builtin_skipper_kind>)
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(is_nothrow_parsable_v<
            Subject, It, Se,
            std::remove_cvref_t<decltype(x4::replace_first_context<contexts::skipper>(ctx, std::declval<builtin_skipper_kind&>()))>,
            Attr
        >)
    {
        // This value could be reset by some nested parsers, so it can't be const
        /* constexpr */ builtin_skipper_kind skipper_kind = Kind;
        return this->subject.parse(first, last, x4::replace_first_context<contexts::skipper>(ctx, skipper_kind), attr);
    }

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return std::format(
            "skip({})[{}]",
            detail::builtin_skipper_traits<Kind>::name,
            get_info<Subject>{}(this->subject)
        );
    }
};


namespace detail {

template<X4Subject Skipper>
struct skip_gen_impl
{
    // Unreference rvalue reference, but hold lvalue reference as-is
    using skipper_type = std::conditional_t<
        std::is_rvalue_reference_v<Skipper>,
        std::remove_reference_t<Skipper>,
        Skipper
    >;

    template<class SkipperT>
        requires std::same_as<std::remove_cvref_t<SkipperT>, std::remove_cvref_t<Skipper>>
    constexpr skip_gen_impl(SkipperT&& skipper)
        noexcept(std::is_nothrow_constructible_v<skipper_type, SkipperT>)
        : skipper_(std::forward<SkipperT>(skipper))
    {}

    template<X4Subject Subject>
    [[nodiscard]] constexpr skip_directive<as_parser_plain_t<Subject>, std::remove_cvref_t<Skipper>>
    operator[](Subject&& subject) const
        noexcept(
            is_parser_nothrow_castable_v<Subject> &&
            std::is_nothrow_constructible_v<
                skip_directive<as_parser_plain_t<Subject>, std::remove_cvref_t<Skipper>>,
                as_parser_t<Subject>,
                skipper_type const&
            >
        )
    {
        return {as_parser(std::forward<Subject>(subject)), skipper_};
    }

private:
    skipper_type skipper_;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

template<builtin_skipper_kind Kind>
struct builtin_skip_gen_impl
{
    template<X4Subject Subject>
    [[nodiscard]] constexpr builtin_skip_directive<Kind, as_parser_plain_t<Subject>>
    operator[](Subject&& subject) const
        noexcept(
            is_parser_nothrow_castable_v<Subject> &&
            std::is_nothrow_constructible_v<
                builtin_skip_directive<Kind, as_parser_plain_t<Subject>>,
                as_parser_t<Subject>
            >
        )
    {
        return {as_parser(std::forward<Subject>(subject))};
    }
};

template<X4Subject Skipper>
struct no_builtin_t {};

struct no_builtin_fn
{
    template<X4Subject Skipper>
    [[nodiscard]] static constexpr no_builtin_t<Skipper> operator()(Skipper const&) noexcept
    {
        return {};
    }
};

} // detail

[[maybe_unused]] inline constexpr detail::no_builtin_fn no_builtin{};


namespace detail {

struct skip_gen
{
    template<class Encoding>
    [[nodiscard]]
    static constexpr builtin_skip_gen_impl<builtin_skipper_kind::blank>
    operator()(char_class_parser<Encoding, char_classes::blank_tag> const&) noexcept
    {
        return {};
    }

    template<class Encoding>
    [[nodiscard]]
    static constexpr builtin_skip_gen_impl<builtin_skipper_kind::space>
    operator()(char_class_parser<Encoding, char_classes::space_tag> const&) noexcept
    {
        return {};
    }

    template<class Encoding, class Tag>
    [[nodiscard]] static constexpr auto
    operator()(no_builtin_t<char_class_parser<Encoding, Tag>> const&)
        noexcept(noexcept(skip_gen::operator()(char_class_parser<Encoding, Tag>{})))
    {
        return skip_gen::operator()(char_class_parser<Encoding, Tag>{});
    }

    template<X4Subject Skipper>
    [[nodiscard]]
    static constexpr skip_gen_impl<as_parser_t<Skipper>>
    operator()(Skipper&& skipper)
        noexcept(
            is_parser_nothrow_castable_v<Skipper> &&
            std::is_nothrow_constructible_v<skip_gen_impl<as_parser_t<Skipper>>, as_parser_t<Skipper>>
        )
    {
        return {as_parser(std::forward<Skipper>(skipper))};
    }
};

} // detail

namespace parsers::directive {

[[maybe_unused]] inline constexpr detail::skip_gen skip{};

} // parsers::directive

using parsers::directive::skip;

} // iris::x4

#endif
