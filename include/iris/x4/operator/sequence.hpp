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
#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/nary_parser.hpp>
#include <iris/x4/core/move_to.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/parser_traits.hpp>

#include <iris/x4/traits/container_traits.hpp>

#include <iris/x4/directive/expect.hpp>

#include <iris/alloy/tuple.hpp>

#include <iris/type_list.hpp>
#include <iris/bits/specialization_of.hpp>

#include <concepts>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>

#include <cstddef>

namespace iris::x4 {

template<class... Ps>
struct sequence;

namespace detail {

template<traits::X4Container Container, class Elem>
struct container_can_hold_element : std::is_same<Container, Elem>
{};

template<traits::X4Container Container, class Elem>
    requires
        (!std::same_as<Container, Elem>) &&
        (!traits::X4Container<Elem>) &&
        requires(Container& c, Elem&& elem) {
            traits::push_back(c, std::move(elem));
        }
struct container_can_hold_element<Container, Elem>
    : std::true_type
{};

template<traits::X4Container Container, class ContainerElem>
    requires
        (!std::same_as<Container, ContainerElem>) &&
        traits::X4Container<ContainerElem> &&
        requires(Container& c, ContainerElem&& container_elem) {
            x4::move_to(
                std::make_move_iterator(traits::begin(container_elem)),
                std::make_move_iterator(traits::end(container_elem)),
                c
            );
        }
struct container_can_hold_element<Container, ContainerElem>
    : std::true_type
{};

template<traits::X4Container Container, class SequenceAttr_Maybe_Unwrapped>
struct container_can_hold_sequence : container_can_hold_element<Container, SequenceAttr_Maybe_Unwrapped>
{};

template<traits::X4Container Container, class... Ts>
struct container_can_hold_sequence<Container, alloy::tuple<Ts...>>
    // this should not delegate to `container_can_hold_sequence`; we don't want recursive expansion
    : std::conjunction<container_can_hold_element<Container, Ts>...>
{};

template<class... Ps>
struct get_sequence_size<sequence<Ps...>>
{
    static constexpr std::size_t value = sequence_layout<Ps...>::total_sequence_size;
};

template<class... Ps, class Container>
struct get_handles_container<sequence<Ps...>, Container>
{
    static constexpr bool value =
        (parser_traits<Ps>::template handles_container<Container> && ...) ||
        container_can_hold_sequence<
            Container,
            typename parser_traits<sequence<Ps...>>::attribute_type
        >::value;
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

} // detail


template<class... Ps>
struct sequence : nary_parser<sequence<Ps...>, Ps...>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr const&) const
    {
        It const first_saved = first;

        bool const ok = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            return (nary::get<Is>(this->elems).parse(first, last, ctx, unused) && ...);
        }(std::index_sequence_for<Ps...>{});
        if (ok) {
            return true;
        }

        if constexpr (has_context_v<Context, contexts::expectation_failure>) {
            if (x4::has_expectation_failure(ctx)) {
                // don't rollback iterator (mimicking exception-like behavior)
                return false;
            }
        }
        first = first_saved;
        return false;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
    {
        return detail::parse_sequence(*this, first, last, ctx, attr);
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
