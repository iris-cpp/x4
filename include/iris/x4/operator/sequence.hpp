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
#include <iris/x4/core/multi_parser.hpp>
#include <iris/x4/core/move_to.hpp>

#include <iris/x4/traits/attribute_of_binary.hpp>
#include <iris/x4/traits/container_traits.hpp>

#include <iris/x4/directive/expect.hpp>

#include <iris/alloy/tuple.hpp>

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
struct get_attribute_type<sequence<Ps...>>
{
    using type = traits::detail::attribute_of_sequence<Ps...>::type;
};

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

} // detail

template<class... Ps>
struct sequence : multi_parser<sequence<Ps...>, Ps...>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, UnusedAttr const&) const
    {
        It const first_saved = first;

        bool const ok = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            return (x4::get_parser<Is>(this->elems).parse(first, last, ctx, unused) && ...);
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
        using element_type = multi_parser_t<I, Ps...>;
        auto const& elem = x4::get_parser<I>(this->elems);

        if constexpr (I == 0) {
            return get_info<element_type>{}(elem);

        } else if constexpr (is_ttp_specialization_of_v<element_type, expect_directive>) {
            return " > " + get_info<typename element_type::subject_type>{}(elem.subject);

        } else {
            return " >> " + get_info<element_type>{}(elem);
        }
    }
};

namespace detail {

template<class... Ps, std::size_t... Is, class Right>
[[nodiscard]] constexpr sequence<Ps..., Right>
sequence_append_impl(std::index_sequence<Is...>, sequence<Ps...> const& left, Right right)
{
    return {{ {}, { {x4::get_parser<Is>(left.elems)}..., {std::move(right)} } }};
}

template<class... Ps, std::size_t... Is, class Right>
[[nodiscard]] constexpr sequence<Ps..., Right>
sequence_append_impl(std::index_sequence<Is...>, sequence<Ps...>&& left, Right right)
{
    return {{ {}, { {x4::get_parser<Is>(std::move(left).elems)}..., {std::move(right)} } }};
}

} // detail

template<X4Subject Left, X4Subject Right>
    requires (!is_ttp_specialization_of_v<std::remove_cvref_t<Left>, sequence>)
[[nodiscard]] constexpr sequence<as_parser_plain_t<Left>, as_parser_plain_t<Right>>
operator>>(Left&& left, Right&& right)
{
    return {{ {}, { {as_parser(std::forward<Left>(left))}, {as_parser(std::forward<Right>(right))} } }};
}

template<class Left, X4Subject Right>
    requires is_ttp_specialization_of_v<std::remove_cvref_t<Left>, sequence>
[[nodiscard]] constexpr auto
operator>>(Left&& left, Right&& right)
{
    return detail::sequence_append_impl(
        std::make_index_sequence<std::remove_cvref_t<Left>::element_count>{},
        std::forward<Left>(left), as_parser(std::forward<Right>(right))
    );
}

template<X4Subject Left, X4Subject Right>
    requires (!is_ttp_specialization_of_v<std::remove_cvref_t<Left>, sequence>)
[[nodiscard]] constexpr sequence<as_parser_plain_t<Left>, expect_directive<as_parser_plain_t<Right>>>
operator>(Left&& left, Right&& right)
{
    return {{
        {},
        {
            {as_parser(std::forward<Left>(left))},
            {expect_directive<as_parser_plain_t<Right>>(as_parser(std::forward<Right>(right)))}
        }
    }};
}

template<class Left, X4Subject Right>
    requires is_ttp_specialization_of_v<std::remove_cvref_t<Left>, sequence>
[[nodiscard]] constexpr auto
operator>(Left&& left, Right&& right)
{
    return detail::sequence_append_impl(
        std::make_index_sequence<std::remove_cvref_t<Left>::element_count>{},
        std::forward<Left>(left),
        expect_directive<as_parser_plain_t<Right>>(as_parser(std::forward<Right>(right)))
    );
}

} // iris::x4

#endif
