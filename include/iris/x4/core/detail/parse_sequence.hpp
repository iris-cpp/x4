#ifndef IRIS_ZZ_X4_CORE_DETAIL_PARSE_SEQUENCE_HPP
#define IRIS_ZZ_X4_CORE_DETAIL_PARSE_SEQUENCE_HPP

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
#include <iris/x4/traits/can_hold.hpp>

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
struct pass_sequence_attribute_size_one_view
{
    using type = alloy::tuple_element_t<0, Attr>;

    [[nodiscard]] static constexpr type
    call(Attr& attribute)
        noexcept(noexcept(alloy::get<0>(attribute)))
    {
        return alloy::get<0>(attribute);
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
struct pass_sequence_attribute : std::conditional_t<
    traits::is_single_element_tuple_like_view<Attr>::value,
    pass_sequence_attribute_size_one_view<Attr>,
    pass_through_sequence_attribute<Attr>
>
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

template<class LParser, class RParser, class Attr>
struct partition_attribute {};

template<class LParser, class RParser, traits::CategorizedAttr<traits::tuple_attr> Attr>
    requires
        has_attribute_v<LParser> &&
        has_attribute_v<RParser>
struct partition_attribute<LParser, RParser, Attr>
{
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
    using l_part = alloy::tuple_element_t<0, splitted>;
    using r_part = alloy::tuple_element_t<1, splitted>;
    using l_pass = pass_sequence_attribute<LParser, l_part>;
    using r_pass = pass_sequence_attribute<RParser, r_part>;

    [[nodiscard]] static constexpr l_part left(Attr& s)
        // TODO: noexcept
    {
        return alloy::get<0>(alloy::tuple_split<l_size, r_size>(alloy::tuple_ref(s)));
    }

    [[nodiscard]] static constexpr r_part right(Attr& s)
        // TODO: noexcept
    {
        return alloy::get<1>(alloy::tuple_split<l_size, r_size>(alloy::tuple_ref(s)));
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

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    requires (parser_traits<Parser>::sequence_size > 1)
[[nodiscard]] constexpr bool
    parse_sequence_impl(Parser const& parser, It& first, Se const& last, Context const& ctx, Attr& attr)
    noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, Attr>)
{
    // static_assert(Parsable<Parser, It, Se, Context, Attr>);
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

// Default overload, no constraints on attribute category
template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attr>
[[nodiscard]] constexpr bool
parse_sequence(Parser const& parser, It& first, Se const& last, Context const& ctx, Attr& attr)
    // TODO: noexcept
{
    static_assert(X4Attribute<Attr>);

    if constexpr (traits::is_container_v<Attr>) {
        It local_it = first;
        if (detail::parse_sequence_impl(parser.left, local_it, last, ctx, attr) &&
            detail::parse_sequence_impl(parser.right, local_it, last, ctx, attr)
            ) {
            first = std::move(local_it);
            return true;
        }
        return false;
    } else {
        if constexpr (traits::is_single_element_tuple_like<Attr>::value && !traits::can_hold<typename parser_traits<Parser>::attribute_type, Attr>::value) {
            return detail::parse_sequence(parser, first, last, ctx, traits::unwrap_single_element(attr));
        } else {
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
    }
}

template<class Left, class Right>
struct parse_into_container_impl<sequence<Left, Right>>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    call(
        sequence<Left, Right> const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) // never noexcept (requires container insertion)
    {
        if constexpr (traits::is_container_v<Attr>) {
            constexpr bool sequence_attribute_can_directly_hold_value_type = traits::can_hold<
                typename sequence<Left, Right>::attribute_type,
                typename traits::container_value<Attr>::type
            >::value;
            if constexpr (sequence_attribute_can_directly_hold_value_type) {
                return parse_into_container_impl_default<sequence<Left, Right>>::call(parser, first, last, ctx, attr);

            } else {
                auto&& appender = x4::make_container_appender(x4::assume_container(attr));
                return detail::parse_sequence(parser, first, last, ctx, appender);
            }
        } else {
            return parse_into_container_impl_default<sequence<Left, Right>>::call(parser, first, last, ctx, attr);
        }
    }
};

} // iris::x4::detail

#endif
