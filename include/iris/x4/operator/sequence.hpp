#ifndef IRIS_ZZ_X4_OPERATOR_SEQUENCE_HPP
#define IRIS_ZZ_X4_OPERATOR_SEQUENCE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/detail/parse_sequence.hpp>
#include <iris/x4/core/traits/attribute_category.hpp>
#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/nary_parser.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/parser_traits.hpp>

#include <iris/x4/directive/expect.hpp>

#include <iris/alloy/tuple.hpp>

#include <iris/type_list.hpp>
#include <iris/bits/specialization_of.hpp>

#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4 {

template<class... Ps>
struct sequence;

namespace detail {

template<class... Ps>
struct get_sequence_size<sequence<Ps...>>
{
    static constexpr std::size_t value = sequence_layout<Ps...>::total_sequence_size;
};

// A sequence accepts a container when every element of it can write into
// the container; the sequence as a whole then writes nothing of its own.
template<class... Ps, class Container>
struct get_accepts_container<sequence<Ps...>, Container>
{
    static constexpr bool value = ((!has_attribute_v<Ps> || writes_into_container<Ps, Container>) && ...);
};

// -------------------------------------------------------------

template<class T>
struct to_sequence_attribute_list
{
    using type = type_list<T>;
};

template<>
struct to_sequence_attribute_list<unused_type>
{
    using type = type_list<>;
};

template<class... Ts>
struct to_sequence_attribute_list<alloy::tuple<Ts...>>
{
    using type = type_list<Ts...>;
};

// -------------------------------------------------------------

template<class TypeList>
struct canonicalize_sequence_attribute;

template<>
struct canonicalize_sequence_attribute<type_list<>>
{
    using type = unused_type;
};

template<class T>
struct canonicalize_sequence_attribute<type_list<T>>
{
    using type = T;
};

template<class T0, class T1, class... Ts>
struct canonicalize_sequence_attribute<type_list<T0, T1, Ts...>>
{
    using type = alloy::tuple<T0, T1, Ts...>;
};

// -------------------------------------------------------------

template<class... Ps>
struct get_attribute_type<sequence<Ps...>>
{
    using type = canonicalize_sequence_attribute<
        typename concat_type_list<
            typename to_sequence_attribute_list<typename parser_traits<Ps>::attribute_type>::type...
        >::type
    >::type;
};

// Make this independent function to reduce lambda's type name in compilation errors
template<class Elems, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class ContainerAttr>
[[nodiscard]] constexpr auto make_container_sequence_parser(
    Elems const& elems, It& first, Se const& last, Context const& ctx, ContainerAttr& container_attr
) noexcept
{
    return [&elems, &first, &last, &ctx, &container_attr]<std::size_t... Is>(std::index_sequence<Is...>) -> bool {
        // Takes the index rather than the parser, so that diagnostics don't print the parser type here
        auto parse_elem = [&]<std::size_t I>() -> bool {
            auto const& parser = nary::get<I>(elems);
            if constexpr (parser_traits<std::remove_cvref_t<decltype(parser)>>::sequence_size > 1) {
                // Exposed attribute = container, Parser expects sequence attribute
                return parser.parse(first, last, ctx, container_attr);

            } else {
                // Exposed attribute = container, Parser expects non-sequence attribute
                return detail::parse_into_container(parser, first, last, ctx, container_attr);
            }
        };
        return (parse_elem.template operator()<Is>() && ...);
    };
}

} // detail

// -------------------------------------------------------------

template<class... Ps>
struct sequence : nary_parser<sequence<Ps...>, Ps...>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr const& unused_attr) const
    {
        It local_it = first;
        if (detail::parse_sequence_all(detail::as_sequence(*this), std::index_sequence_for<Ps...>{}, local_it, last, ctx, unused_attr)) {
            first = std::move(local_it);
            return true;
        }
        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) {
                // don't rollback iterator (mimicking exception-like behavior)
                first = std::move(local_it);
            }
        }
        return false;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        using layout = detail::sequence_layout<Ps...>;

        // Intentionally verbose branches for avoiding instantiation of erroneous grammar stem,
        // significantly reducing the amount of compilation error.

        if constexpr (layout::attributed_count < 2) {
            It local_it = first;
            if (detail::parse_sequence_all(detail::as_sequence(*this), std::index_sequence_for<Ps...>{}, local_it, last, ctx, attr)) {
                first = std::move(local_it);
                return true;
            }
            if constexpr (has_context_v<Context, contexts::expectation_failure>) {
                if (x4::has_expectation_failure(ctx)) {
                    // don't rollback iterator (mimicking exception-like behavior)
                    first = std::move(local_it);
                }
            }
            return false;

        } else if constexpr (!CategorizedAttr<Attr, tuple_tag>) {
            static_assert(false, "The attribute of a sequence with >=2 attributed elements must be tuple-like.");
            return false;

        } else if constexpr (alloy::tuple_size_v<Attr> < layout::total_sequence_size) {
            static_assert(false, "Sequence size of the passed attribute is less than expected.");
            return false;

        } else if constexpr (alloy::tuple_size_v<Attr> > layout::total_sequence_size) {
            static_assert(false, "Sequence size of the passed attribute is greater than expected.");
            return false;

        } else {
            It local_it = first;
            if (detail::parse_sequence_all(detail::as_sequence(*this), std::index_sequence_for<Ps...>{}, local_it, last, ctx, attr)) {
                first = std::move(local_it);
                return true;
            }
            if constexpr (has_context_v<Context, contexts::expectation_failure>) {
                if (x4::has_expectation_failure(ctx)) {
                    // don't rollback iterator (mimicking exception-like behavior)
                    first = std::move(local_it);
                }
            }
            return false;
        }
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute ContainerAttr>
        requires CategorizedAttr<ContainerAttr, container_tag>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, ContainerAttr& container_attr) const
    {
        It local_it = first;
        if (detail::make_container_sequence_parser(this->elems, local_it, last, ctx, container_attr)(std::index_sequence_for<Ps...>{})) {
            first = std::move(local_it);
            return true;
        }
        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) {
                // don't rollback iterator (mimicking exception-like behavior)
                first = std::move(local_it);
            }
        }
        return false;
    }

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            std::string info;
            ((info += this->template get_x4_element_info<Is>()), ...);
            return info;
        }(std::index_sequence_for<Ps...>{});
    }

private:
    template<std::size_t I>
    [[nodiscard]] constexpr std::string get_x4_element_info() const
    {
        using element_type = nary::parser_t<I, Ps...>;
        auto const& elem = nary::get<I>(this->elems);

        if constexpr (I == 0) {
            return get_info<element_type>{}(elem);

        } else if constexpr (is_ttp_specialization_of_v<element_type, expect_directive>) {
            return " > " + get_info<typename element_type::subject_type>{}(elem.subject);

        } else {
            return " >> " + get_info<element_type>{}(elem);
        }
    }
};

template<X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr auto
operator>>(Left&& left, Right&& right)
{
    return nary::concat<sequence>(as_parser(static_cast<Left&&>(left)), as_parser(static_cast<Right&&>(right)));
}

template<X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr auto
operator>(Left&& left, Right&& right)
{
    return nary::concat<sequence>(
        as_parser(static_cast<Left&&>(left)),
        expect_directive<as_parser_plain_t<Right>>(as_parser(static_cast<Right&&>(right)))
    );
}

} // iris::x4

#endif
