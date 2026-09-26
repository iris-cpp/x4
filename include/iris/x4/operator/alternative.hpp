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

#include <iris/x4/core/nary_parser.hpp>
#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/detail/parse_alternative.hpp>

#include <iris/rvariant/rvariant.hpp>

#include <iris/type_list.hpp>

#include <concepts>
#include <iterator>
#include <string>
#include <utility>

#include <cstddef> // IWYU pragma: keep

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

template<class T, class Wrapped>
struct alternative_wrapped_entry {};

template<class T>
struct alternative_entry {};

template<class T>
    requires is_recursive_wrapper_v<T>
struct alternative_entry<T>
    : alternative_wrapped_entry<unwrap_recursive_t<T>, T>
{};

template<class... Ts>
struct alternative_entries
    : alternative_entry<Ts>...
{};

template<class T, class Wrapped>
Wrapped alternative_wrapped_form(alternative_wrapped_entry<unwrap_recursive_t<T>, Wrapped>*);

template<class T>
T alternative_wrapped_form(...);

template<class TypeList>
struct unique_alternative_list;

template<class... Ts>
struct unique_alternative_list<type_list<Ts...>>
    : unique_type_list<type_list<decltype(detail::alternative_wrapped_form<Ts>(static_cast<alternative_entries<Ts...>*>(nullptr)))...>>
{};

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
    using unique_attrs = unique_alternative_list<typename unique_type_list<concated_attrs>::type>::type;
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
struct alternative : nary_parser<alternative<Ps...>, Ps...>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& exposed_attr) const
    {
        return detail::parse_alternative_all<Ps...>::call(
            std::index_sequence_for<Ps...>{},
            [&]<std::size_t I>(auto&& alt_attr) {
                return nary::get<I>(this->elems).parse(first, last, ctx, alt_attr);
            },
            ctx,
            exposed_attr
        );
    }

    [[nodiscard]] constexpr std::string get_x4_info() const
    {
        return [&]<std::size_t... Is>(std::index_sequence<Is...>) {
            std::string info;
            ((info += (Is == 0 ? "" : " | ") + get_info<nary::parser_t<Is, Ps...>>{}(nary::get<Is>(this->elems))), ...);
            return info;
        }(std::index_sequence_for<Ps...>{});
    }
};

template<X4Subject Left, X4Subject Right>
[[nodiscard]] constexpr auto
operator|(Left&& left, Right&& right)
{
    return nary::concat<alternative>(as_parser(static_cast<Left&&>(left)), as_parser(static_cast<Right&&>(right)));
}

} // iris::x4

#endif
