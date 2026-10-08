#ifndef IRIS_ZZ_X4_OPERATOR_LIST_HPP
#define IRIS_ZZ_X4_OPERATOR_LIST_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/list_like_parser.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/expectation.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4 {

// a % b
template<class Subject, class Separator>
struct delimited_list : parser<delimited_list<Subject, Separator>>
{
    // Not a `binary_parser` due to MSVC QoL issue.
    // The type of the member is spelled `Subject` as in `unary_parser`, so that MSVC prints it once in the
    // frame of `parse_into_container`, whose parameter is also named `Subject`.

    using subject_type = Subject;
    using separator_type = Separator;
    using attribute_type = traits::default_container<typename parser_traits<Subject>::attribute_type>::type;

    static constexpr bool has_action = Subject::has_action || Separator::has_action;
    static constexpr bool need_rcontext = Subject::need_rcontext || Separator::need_rcontext;

    template<class Container>
    static constexpr bool accepts_container = writes_into_container<Subject, Container>;

    constexpr delimited_list() = default;

    template<class SubjectT, class SeparatorT>
        requires std::same_as<std::remove_cvref_t<SubjectT>, Subject> && std::same_as<std::remove_cvref_t<SeparatorT>, Separator>
    constexpr delimited_list(SubjectT&& subject, SeparatorT&& separator)
        noexcept(std::is_nothrow_constructible_v<Subject, SubjectT> && std::is_nothrow_constructible_v<Separator, SeparatorT>)
        : subject(std::forward<SubjectT>(subject))
        , separator(std::forward<SeparatorT>(separator))
    {}

    // Empty instance elimination technique: please read the comment on `unary_parser`.
    IRIS_NO_UNIQUE_ADDRESS Subject subject;
    IRIS_NO_UNIQUE_ADDRESS Separator separator;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        if constexpr (list_like_parser::writes_as_one_element<attribute_type, Attr>) {
            return list_like_parser::parse_as_one_element(*this, first, last, ctx, attr);

        } else {
            list_like_parser::chunk_buffer<Subject, attribute_type, Attr> chunk_buf(detail::ref_or_init_attribute_for<attribute_type>(attr));

            // In order to succeed, we need to match at least one element
            if (detail::parse_into_container(this->subject, first, last, ctx, chunk_buf.container())) {
                chunk_buf.merge();
            } else {
                return false;
            }

            It last_parse_it = first;
            while (
                this->separator.parse(last_parse_it, last, ctx, unused) &&
                detail::parse_into_container(this->subject, last_parse_it, last, ctx, chunk_buf.container())
            ) {
                chunk_buf.merge();
                first = last_parse_it;
            }

            if constexpr (has_context<Context, contexts::expectation_failure>) {
                if (x4::has_expectation_failure(ctx)) {
                    // don't rollback iterator (mimicking exception-like behavior)
                    first = std::move(last_parse_it);
                    return false;
                }
            }
            return true;
        }
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr& unused_attr) const
    {
        // In order to succeed we need to match at least one element
        if (!detail::parse_into_container(this->subject, first, last, ctx, x4::assume_container(unused_attr))) {
            return false;
        }

        It last_parse_it = first;
        while (
            this->separator.parse(last_parse_it, last, ctx, unused) &&
            detail::parse_into_container(this->subject, last_parse_it, last, ctx, x4::assume_container(unused_attr))
        ) {
            // TODO: can we reduce this copy assignment?
            first = last_parse_it;
        }

        if constexpr (has_context<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) {
                // don't rollback iterator (mimicking exception-like behavior)
                first = std::move(last_parse_it);
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return '(' + get_info<Subject>{}(this->subject) + " % " + get_info<Separator>{}(this->separator);
    }
};

template<X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr delimited_list<as_parser_plain_t<Left>, as_parser_plain_t<Right>>
operator%(Left&& left, Right&& right)
    noexcept(
        is_parser_nothrow_castable_v<Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<
            delimited_list<as_parser_plain_t<Left>, as_parser_plain_t<Right>>,
            as_parser_t<Left>,
            as_parser_t<Right>
        >
    )
{
    return {as_parser(std::forward<Left>(left)), as_parser(std::forward<Right>(right))};
}

} // iris::x4

#endif
