#ifndef BOOST_SPIRIT_X4_DIRECTIVE_SKIP_HPP
#define BOOST_SPIRIT_X4_DIRECTIVE_SKIP_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2013 Agustin Berge
    Copyright (c) 2024-2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/x4/core/context.hpp>
#include <boost/spirit/x4/core/unused.hpp>
#include <boost/spirit/x4/core/expectation.hpp>
#include <boost/spirit/x4/core/skip_over.hpp>
#include <boost/spirit/x4/core/parser.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4 {

    template <typename Subject>
    struct reskip_directive : unary_parser<Subject, reskip_directive<Subject>>
    {
        using base_type = unary_parser<Subject, reskip_directive<Subject>>;
        static constexpr bool is_pass_through_unary = true;
        static constexpr bool handles_container = Subject::handles_container;

        template <typename SubjectT>
            requires
                (!std::is_same_v<std::remove_cvref_t<SubjectT>, reskip_directive>) &&
                std::is_constructible_v<base_type, SubjectT>
        constexpr reskip_directive(SubjectT&& subject)
            noexcept(std::is_nothrow_constructible_v<base_type, SubjectT>)
            : base_type(std::forward<SubjectT>(subject))
        {}

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
            requires has_skipper_v<Context>
        [[nodiscard]] constexpr bool
        parse(It& first, Se const& last, Context const& context, Attribute& attr) const
            noexcept(is_nothrow_parsable_v<Subject, It, Se, Context, Attribute>)
        {
            return this->subject.parse(first, last, context, attr);
        }

    private:
        template <typename Context>
        using context_t = context<
            skipper_tag,
            std::remove_cvref_t<decltype(detail::get_unused_skipper(
                std::declval<get_context_plain_t<skipper_tag, Context> const&>()
            ))>,
            Context
        >;

    public:
        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
            requires (!has_skipper_v<Context>)
        [[nodiscard]] constexpr bool
        parse(It& first, Se const& last, Context const& context, Attribute& attr) const
            noexcept(is_nothrow_parsable_v<Subject, It, Se, context_t<Context>, Attribute>)
        {
            // This logic is heavily related to the instantiation chain;
            // see `x4::skip_over` for details.
            auto const& skipper = detail::get_unused_skipper(x4::get<skipper_tag>(context));
            return this->subject.parse(first, last, x4::make_context<skipper_tag>(skipper, context), attr);
        }
    };

    template <typename Subject, typename Skipper>
    struct skip_directive : unary_parser<Subject, skip_directive<Subject, Skipper>>
    {
        using base_type = unary_parser<Subject, skip_directive<Subject, Skipper>>;
        static constexpr bool is_pass_through_unary = true;
        static constexpr bool handles_container = Subject::handles_container;

        template <typename SubjectT, typename SkipperT>
            requires std::is_constructible_v<base_type, SubjectT> && std::is_constructible_v<Skipper, SkipperT>
        constexpr skip_directive(SubjectT&& subject, SkipperT&& skipper)
            noexcept(std::is_nothrow_constructible_v<base_type, SubjectT> && std::is_nothrow_constructible_v<Skipper, SkipperT>)
            : base_type(std::forward<SubjectT>(subject))
            , skipper_(std::forward<SkipperT>(skipper))
        {}

        template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
        [[nodiscard]] constexpr bool
        parse(It& first, Se const& last, Context const& context, Attribute& attr) const
            noexcept(is_nothrow_parsable_v<Subject, It, Se, x4::context<skipper_tag, Skipper, Context>, Attribute>)
        {
            static_assert(
                !std::same_as<expectation_failure_t<Context>, unused_type>,
                "Context type was not specified for x4::expectation_failure_tag. "
                "You probably forgot: `x4::with<x4::expectation_failure_tag>(failure)[p]`. "
                "Note that you must also bind the context to your skipper."
            );

            // This logic is heavily related to the instantiation chain;
            // see `x4::skip_over` for details.
            return this->subject.parse(first, last, x4::make_context<skipper_tag>(skipper_, context), attr);
        }

    private:
        Skipper skipper_;
    };

    namespace detail {

        template <X4Subject Skipper>
        struct skip_gen_impl
        {
            // Unreference rvalue reference, but hold lvalue reference as-is
            using skipper_type = std::conditional_t<
                std::is_rvalue_reference_v<Skipper>,
                std::remove_reference_t<Skipper>,
                Skipper
            >;

            template <typename SkipperT>
                requires std::same_as<std::remove_cvref_t<SkipperT>, std::remove_cvref_t<Skipper>>
            constexpr skip_gen_impl(SkipperT&& skipper)
                noexcept(std::is_nothrow_constructible_v<skipper_type, SkipperT>)
                : skipper_(std::forward<SkipperT>(skipper))
            {}

            template <X4Subject Subject>
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
                return { as_parser(std::forward<Subject>(subject)), skipper_ };
            }

        private:
            skipper_type skipper_;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
        };

        struct skip_gen
        {
            template <X4Subject Skipper>
            [[nodiscard]]
            static constexpr skip_gen_impl<as_parser_t<Skipper>>
            operator()(Skipper&& skipper)
                noexcept(
                    is_parser_nothrow_castable_v<Skipper> &&
                    std::is_nothrow_constructible_v<skip_gen_impl<as_parser_t<Skipper>>, as_parser_t<Skipper>>
                )
            {
                return { as_parser(std::forward<Skipper>(skipper)) };
            }

            template <typename Subject>
            [[nodiscard, deprecated("Use `x4::reskip[p]`.")]]
            /* static */ constexpr reskip_directive<as_parser_plain_t<Subject>>
            operator[](Subject&& subject) const // MSVC 2022 bug: cannot define `static operator[]` even in C++26 mode
                noexcept(is_parser_nothrow_constructible_v<reskip_directive<as_parser_plain_t<Subject>>, Subject>)
            {
                return { as_parser(std::forward<Subject>(subject)) };
            }
        };

        struct reskip_gen
        {
            template <X4Subject Subject>
            [[nodiscard]]
            /* static */ constexpr reskip_directive<as_parser_plain_t<Subject>>
            operator[](Subject&& subject) const // MSVC 2022 bug: cannot define `static operator[]` even in C++26 mode
                noexcept(is_parser_nothrow_constructible_v<reskip_directive<as_parser_plain_t<Subject>>, Subject>)
            {
                return { as_parser(std::forward<Subject>(subject)) };
            }
        };
    } // detail

    inline namespace cpos {

        inline constexpr detail::skip_gen skip{};
        inline constexpr detail::reskip_gen reskip{};
    }
} // boost::spirit::x4

#endif
