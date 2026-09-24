#ifndef IRIS_ZZ_X4_OPERATOR_ALTERNATIVE_HPP
#define IRIS_ZZ_X4_OPERATOR_ALTERNATIVE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/multi_parser.hpp>
#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/detail/parse_alternative.hpp>

#include <iris/rvariant/rvariant.hpp>

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
struct alternative;

namespace detail {

template<class T>
struct to_alternative_attribute_list
{
    using type = type_list<T>;
};

template<>
struct to_alternative_attribute_list<unused_type>
{
    using type = type_list<>;
};

template<class... Ts>
struct to_alternative_attribute_list<rvariant<Ts...>>
{
    using type = type_list<Ts...>;
};

// -------------------------------------------------------------

template<class TypeList>
struct canonicalize_alternative_attribute;

template<>
struct canonicalize_alternative_attribute<type_list<>>
{
    using type = unused_type;
};

template<class T>
struct canonicalize_alternative_attribute<type_list<T>>
{
    using type = T;
};

template<class T0, class T1, class... Ts>
struct canonicalize_alternative_attribute<type_list<T0, T1, Ts...>>
{
    using type = rvariant<T0, T1, Ts...>;
};

// -------------------------------------------------------------

template<class... Ps>
struct alternative_layout;

template<class P0, class... PRest>
struct alternative_layout<P0, PRest...>
{
    using concated_attrs = concat_type_list<
        typename to_alternative_attribute_list<typename parser_traits<P0>::attribute_type>::type,
        typename to_alternative_attribute_list<typename parser_traits<PRest>::attribute_type>::type...
    >::type;
    using unique_attrs = unique_type_list<concated_attrs>::type;
    using attribute_type = canonicalize_alternative_attribute<unique_attrs>::type;

    // All branches share one attribute type and one slot count; the
    // alternative then occupies that many slots itself.
    static constexpr bool transparent =
        unique_attrs::size == 1 &&
        ((parser_traits<P0>::sequence_size == parser_traits<PRest>::sequence_size) && ...);

    static constexpr std::size_t sequence_size =
        transparent ? parser_traits<P0>::sequence_size : static_cast<std::size_t>(!std::same_as<attribute_type, unused_type>);
};

template<class... Ps>
struct get_attribute_type<alternative<Ps...>>
{
    using type = alternative_layout<Ps...>::attribute_type;
};

template<class... Ps>
struct get_sequence_size<alternative<Ps...>>
{
    static constexpr std::size_t value = alternative_layout<Ps...>::sequence_size;
};

} // detail

template<class... Ps>
struct alternative : multi_parser<alternative<Ps...>, Ps...>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& exposed_attr) const
    {
        return detail::parse_alternative_all<Ps...>::call(
            std::index_sequence_for<Ps...>{},
            [&]<std::size_t I>(auto&& alt_attr) {
                return x4::get_parser<I>(this->elems).parse(first, last, ctx, alt_attr);
            },
            ctx,
            exposed_attr
        );
    }

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            std::string info;
            ((info += (Is == 0 ? "" : " | ") + get_info<multi_parser_t<Is, Ps...>>{}(x4::get_parser<Is>(this->elems))), ...);
            return info;
        }(std::index_sequence_for<Ps...>{});
    }
};

namespace detail {

template<class... Ps, std::size_t... Is, class Right>
[[nodiscard]] constexpr alternative<Ps..., Right>
alternative_append_impl(std::index_sequence<Is...>, alternative<Ps...> const& left, Right right)
    noexcept(std::is_nothrow_copy_constructible_v<alternative<Ps...>> && std::is_nothrow_move_constructible_v<Right>)
{
    return {{ {}, { {x4::get_parser<Is>(left.elems)}..., {std::move(right)} } }};
}

template<class... Ps, std::size_t... Is, class Right>
[[nodiscard]] constexpr alternative<Ps..., Right>
alternative_append_impl(std::index_sequence<Is...>, alternative<Ps...>&& left, Right right)
    noexcept(std::is_nothrow_move_constructible_v<alternative<Ps...>> && std::is_nothrow_move_constructible_v<Right>)
{
    return {{ {}, { {x4::get_parser<Is>(std::move(left).elems)}..., {std::move(right)} } }};
}

} // detail

template<X4Subject Left, X4Subject Right>
    requires (!iris::is_ttp_specialization_of_v<std::remove_cvref_t<Left>, alternative>)
[[nodiscard]] constexpr alternative<as_parser_plain_t<Left>, as_parser_plain_t<Right>>
operator|(Left&& left, Right&& right)
    noexcept(
        is_parser_nothrow_castable_v<Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<as_parser_plain_t<Left>, as_parser_t<Left>> &&
        std::is_nothrow_constructible_v<as_parser_plain_t<Right>, as_parser_t<Right>>
    )
{
    return {{ {}, { {as_parser(std::forward<Left>(left))}, {as_parser(std::forward<Right>(right))} } }};
}

template<class Left, X4Subject Right>
    requires iris::is_ttp_specialization_of_v<std::remove_cvref_t<Left>, alternative>
[[nodiscard]] constexpr auto
operator|(Left&& left, Right&& right)
    noexcept(
        std::is_nothrow_constructible_v<std::remove_cvref_t<Left>, Left> &&
        is_parser_nothrow_castable_v<Right> &&
        std::is_nothrow_constructible_v<as_parser_plain_t<Right>, as_parser_t<Right>>
    )
{
    return detail::alternative_append_impl(
        std::make_index_sequence<std::remove_cvref_t<Left>::element_count>{},
        std::forward<Left>(left), as_parser(std::forward<Right>(right))
    );
}

} // iris::x4

#endif
