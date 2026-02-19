#ifndef IRIS_X4_CORE_DETAIL_PARSE_SEQUENCE_HPP
#define IRIS_X4_CORE_DETAIL_PARSE_SEQUENCE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp>

#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/detail/parse_into_container.hpp>

#include <iris/x4/traits/attribute_category.hpp>
#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/traits/tuple_traits.hpp>
#include <iris/x4/traits/substitution.hpp>

#include <iris/alloy/tuple.hpp>
#include <iris/alloy/utility.hpp>

#include <iterator>
#include <type_traits>
#include <concepts>
#include <utility>

namespace iris::x4 {

template<class Left, class Right>
struct sequence;

} // iris::x4

namespace iris::x4::detail {

struct pass_sequence_attribute_unused
{
    using type = unused_type;

    template<class T>
    [[nodiscard]] static constexpr unused_type
    call(T&) noexcept
    {
        return unused_type{};
    }
};

template<class Attr>
struct pass_through_sequence_attribute
{
    using type = Attr&;

    template<class Attr_>
    [[nodiscard]] static constexpr Attr_&
    call(Attr_& attribute) noexcept
    {
        return attribute;
    }
};

template<class Parser, class Attr>
struct pass_sequence_attribute
    : pass_through_sequence_attribute<Attr>
{};

template<class LParser, class RParser, class Attr>
struct pass_sequence_attribute<sequence<LParser, RParser>, Attr>
    : pass_through_sequence_attribute<Attr>
{};

template<class Parser, class Attr>
    requires requires {
        typename Parser::proxy_backend_type;
    }
struct pass_sequence_attribute<Parser, Attr>
    : pass_sequence_attribute<typename Parser::proxy_backend_type, Attr>
{};

template<class T>
struct type_unwrap_size_one_tuple
{
    using type = T;
};

template<class T>
struct type_unwrap_size_one_tuple<alloy::tuple<T>>
{
    using type = T;
};

template<class T>
using type_unwrap_size_one_tuple_t = type_unwrap_size_one_tuple<T>::type;

template<class T>
constexpr T&& unwrap_size_one_tuple(T&& t) noexcept
{
    return std::forward<T>(t);
}

template<class T>
    requires traits::is_single_element_tuple_like_v<T>
constexpr auto&& unwrap_size_one_tuple(T&& t) noexcept
{
    return alloy::get<0>(std::forward<T>(t));
}

template<class LParser, class RParser, class Attr>
struct partition_attribute
{
    using attr_category = traits::attribute_category_t<Attr>;

    static_assert(
        std::same_as<traits::tuple_attr, attr_category>,
        "The parser expects tuple-like attribute type"
    );

    static constexpr std::size_t l_size = parser_traits<LParser>::sequence_size;
    static constexpr std::size_t r_size = parser_traits<RParser>::sequence_size;

    static constexpr std::size_t actual_size = alloy::tuple_size_v<Attr>;
    static constexpr std::size_t expected_size = l_size + r_size;

    // If you got an error here, then you are trying to pass
    // a tuple-like with the wrong number of elements
    // as that expected by the (sequence) parser.
    static_assert(
        actual_size >= expected_size,
        "Sequence size of the passed attribute is less than expected."
    );
    static_assert(
        actual_size <= expected_size,
        "Sequence size of the passed attribute is greater than expected."
    );

    using view = alloy::tuple_ref_t<Attr>;
    using splitted = alloy::tuple_split_t<view, l_size, r_size>;
    using l_part = type_unwrap_size_one_tuple_t<alloy::tuple_element_t<0, splitted>>;
    using r_part = type_unwrap_size_one_tuple_t<alloy::tuple_element_t<1, splitted>>;
    using l_pass = pass_sequence_attribute<LParser, l_part>;
    using r_pass = pass_sequence_attribute<RParser, r_part>;

    [[nodiscard]] static constexpr l_part left(Attr& s)
        // TODO: noexcept
    {
        return unwrap_size_one_tuple(alloy::get<0>(alloy::tuple_split<l_size, r_size>(alloy::tuple_ref(s))));
    }

    [[nodiscard]] static constexpr r_part right(Attr& s)
        // TODO: noexcept
    {
        return unwrap_size_one_tuple(alloy::get<1>(alloy::tuple_split<l_size, r_size>(alloy::tuple_ref(s))));
    }
};

template<class LParser, class RParser, class Attr>
    requires
        (!has_attribute_v<LParser>) &&
        has_attribute_v<RParser>
struct partition_attribute<LParser, RParser, Attr>
{
    using l_pass = pass_sequence_attribute_unused;
    using r_pass = pass_sequence_attribute<RParser, Attr>;

    [[nodiscard]] static constexpr unused_type left(Attr&) noexcept
    {
        return unused;
    }

    [[nodiscard]] static constexpr Attr& right(Attr& s) noexcept
    {
        return s;
    }
};

template<class LParser, class RParser, class Attr>
    requires
        has_attribute_v<LParser> &&
        (!has_attribute_v<RParser>)
struct partition_attribute<LParser, RParser, Attr>
{
    using l_pass = pass_sequence_attribute<LParser, Attr>;
    using r_pass = pass_sequence_attribute_unused;

    [[nodiscard]] static constexpr Attr& left(Attr& s) noexcept
    {
        return s;
    }

    [[nodiscard]] static constexpr unused_type right(Attr&) noexcept
    {
        return unused;
    }
};

template<class LParser, class RParser, class Attr>
    requires
        (!has_attribute_v<LParser>) &&
        (!has_attribute_v<RParser>)
struct partition_attribute<LParser, RParser, Attr>
{
    using l_pass = pass_sequence_attribute_unused;
    using r_pass = pass_sequence_attribute_unused;

    [[nodiscard]] static constexpr unused_type left(Attr&) noexcept
    {
        return unused;
    }

    [[nodiscard]] static constexpr unused_type right(Attr&) noexcept
    {
        return unused;
    }
};

// Default overload, no constraints on attribute category
template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attr>
[[nodiscard]] constexpr bool
parse_sequence(Parser const& parser, It& first, Se const& last, Context const& ctx, Attr& attr)
    // TODO: noexcept
{
    static_assert(X4Attribute<Attr>);

    using partition = partition_attribute<
        typename Parser::left_type,
        typename Parser::right_type,
        Attr
    >;

    auto&& l_part = partition::left(attr);
    auto&& r_part = partition::right(attr);
    auto&& l_attr = partition::l_pass::call(l_part);
    auto&& r_attr = partition::r_pass::call(r_part);

    auto&& l_attr_appender = x4::make_container_appender(l_attr);
    auto&& r_attr_appender = x4::make_container_appender(r_attr);

    It local_it = first;
    if (parser.left.parse(local_it, last, ctx, l_attr_appender) &&
        parser.right.parse(local_it, last, ctx, r_attr_appender)
    ) {
        first = std::move(local_it);
        return true;
    }

    return false;
}

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    requires (parser_traits<Parser>::sequence_size > 1)
[[nodiscard]] constexpr bool
parse_sequence_impl(Parser const& parser, It& first, Se const& last, Context const& ctx, Attr& attr)
    noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, Attr>)
{
    static_assert(Parsable<Parser, It, Se, Context, Attr>);
    return parser.parse(first, last, ctx, attr);
}

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    requires (parser_traits<Parser>::sequence_size <= 1)
[[nodiscard]] constexpr bool
parse_sequence_impl(Parser const& parser, It& first, Se const& last, Context const& ctx, Attr& attr)
    noexcept(noexcept(detail::parse_into_container(parser, first, last, ctx, attr)))
{
    return detail::parse_into_container(parser, first, last, ctx, attr);
}

template<
    class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context,
    traits::CategorizedAttr<traits::container_attr> ContainerAttr
>
[[nodiscard]] constexpr bool
parse_sequence(Parser const& parser, It& first, Se const& last, Context const& ctx, ContainerAttr& container_attr)
    noexcept(
        std::is_nothrow_copy_assignable_v<It> &&
        noexcept(detail::parse_sequence_impl(parser.left, first, last, ctx, container_attr)) &&
        noexcept(detail::parse_sequence_impl(parser.right, first, last, ctx, container_attr))
    )
{
    It local_it = first;
    if (detail::parse_sequence_impl(parser.left, local_it, last, ctx, container_attr) &&
        detail::parse_sequence_impl(parser.right, local_it, last, ctx, container_attr)
    ) {
        first = std::move(local_it);
        return true;
    }
    return false;
}

template<class Left, class Right>
struct parse_into_container_impl<sequence<Left, Right>>
{
    template<X4Attribute Attr>
    static constexpr bool is_container_substitute = traits::is_substitute_v<
        typename sequence<Left, Right>::attribute_type,
        traits::container_value_t<Attr>
    >;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires is_container_substitute<Attr>
    [[nodiscard]] static constexpr bool
    call(
        sequence<Left, Right> const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) noexcept(noexcept(parse_into_container_base_impl<sequence<Left, Right>>::call(
        parser, first, last, ctx, attr
    )))
    {
        return parse_into_container_base_impl<sequence<Left, Right>>::call(
            parser, first, last, ctx, attr
        );
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires (!is_container_substitute<Attr>)
    [[nodiscard]] static constexpr bool
    call(
        sequence<Left, Right> const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) // never noexcept (requires container insertion)
    {
        static_assert(
            std::same_as<traits::attribute_category_t<Attr>, traits::container_attr> ||
            std::same_as<traits::attribute_category_t<Attr>, traits::unused_attr>
        );

        if constexpr (
            std::same_as<std::remove_const_t<Attr>, unused_type> ||
            std::same_as<std::remove_const_t<Attr>, unused_container_type>
        ) {
            return detail::parse_sequence(parser, first, last, ctx, x4::assume_container(attr));

        } else {
            auto&& appender = x4::make_container_appender(x4::assume_container(attr));
            return detail::parse_sequence(parser, first, last, ctx, appender);
        }
    }
};

} // iris::x4::detail

#endif
