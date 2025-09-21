#ifndef BOOST_SPIRIT_X4_CORE_DETAIL_PARSE_INTO_CONTAINER_HPP
#define BOOST_SPIRIT_X4_CORE_DETAIL_PARSE_INTO_CONTAINER_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/config.hpp>
#include <boost/spirit/x4/core/move_to.hpp>
#include <boost/spirit/x4/core/parser.hpp>

#include <boost/spirit/x4/traits/container_traits.hpp>
#include <boost/spirit/x4/traits/attribute.hpp>
#include <boost/spirit/x4/traits/substitution.hpp>

#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/sequence/intrinsic/back.hpp>

// TODO: remove Boost.Variant usage
#include <boost/variant/variant.hpp>
#include <boost/variant/apply_visitor.hpp>
#include <boost/variant/static_visitor.hpp>

#include <ranges>
#include <iterator>
#include <type_traits>
#include <utility>

namespace boost::spirit::x4::detail {

template <typename Attribute, typename Value>
struct saver_visitor;

// save to associative fusion container where Key is simple type
template <typename Key>
struct save_to_assoc_attr
{
    template <typename Value, typename Attribute>
    static constexpr void call(Key const&, Value&& value, Attribute& attr)
        noexcept(noexcept(x4::move_to(std::forward<Value>(value), fusion::at_key<Key>(attr))))
    {
        static_assert(std::is_rvalue_reference_v<Value&&>);
        x4::move_to(std::forward<Value>(value), fusion::at_key<Key>(attr));
    }
};

// save to associative fusion container where Key
// is variant over possible keys
template <typename... Ts>
struct save_to_assoc_attr<variant<Ts...>>
{
    template <typename Value, typename Attribute>
    static constexpr void call(variant<Ts...> const& key, Value&& value, Attribute& attr)
    {
        static_assert(std::is_rvalue_reference_v<Value&&>);
        boost::apply_visitor(saver_visitor<Attribute, Value>(attr, std::forward<Value>(value)), key);
    }
};

template <typename Attribute, typename Value>
struct saver_visitor : boost::static_visitor<void>
{
    constexpr saver_visitor(Attribute& attr, Value&& value) noexcept
        : attr(attr)
        , value(std::move(value))
    {};

    Attribute& attr;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
    Value&& value;  // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)

    template <typename Key>
    constexpr void operator()(Key const& key) const
        noexcept(noexcept(save_to_assoc_attr<Key>::call(key, std::move(value), attr)))
    {
        save_to_assoc_attr<Key>::call(key, std::move(value), attr);
    }
};

template <typename Parser, typename Container, typename Context>
struct parser_accepts_container
    : traits::is_substitute<traits::attribute_of_t<Parser, Context>, Container>
{};

template <typename Parser, typename Container, typename Context>
constexpr bool parser_accepts_container_v = parser_accepts_container<Parser, Container, Context>::value;

template <typename Parser>
struct parse_into_container_base_impl
{
    // Parser has attribute (synthesize; Attribute is a container)
    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
        requires (!parser_accepts_container_v<Parser, Attribute, Context>)
    [[nodiscard]] static constexpr bool
    call_synthesize(
        Parser const& parser, It& first, Se const& last,
        Context const& context, Attribute& attr
    ) // never noexcept (requires container insertion)
    {
        static_assert(!std::same_as<std::remove_const_t<Attribute>, unused_container_type>);

        using value_type = traits::container_value_t<Attribute>;
        value_type val; // default-initialize

        static_assert(Parsable<Parser, It, Se, Context, value_type>);
        if (!parser.parse(first, last, context, val))
        {
            return false;
        }

        // push the parsed value into our attribute
        traits::push_back(attr, std::move(val));
        return true;
    }

    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context>
        requires (!parser_accepts_container_v<Parser, unused_container_type, Context>)
    [[nodiscard]] static constexpr bool
    call_synthesize(
        Parser const& parser, It& first, Se const& last,
        Context const& context, unused_container_type const&
    ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unused_type>)
    {
        static_assert(Parsable<Parser, It, Se, Context, unused_type>);
        return parser.parse(first, last, context, unused);
    }

    // Parser has attribute (synthesize; Attribute is a container)
    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
        requires parser_accepts_container_v<Parser, Attribute, Context>
    [[nodiscard]] static constexpr bool
    call_synthesize(
        Parser const& parser, It& first, Se const& last,
        Context const& context, Attribute& attr
    ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, Attribute>)
    {
        static_assert(Parsable<Parser, It, Se, Context, Attribute>);
        return parser.parse(first, last, context, attr);
    }

    // Parser has attribute && it is NOT fusion sequence
    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
        requires
            traits::has_attribute_v<Parser, Context> &&
            (!fusion::traits::is_sequence<Attribute>::value)
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& context, Attribute& attr
    )
    {
        // TODO: reduce call stack while keeping maintainability
        return parse_into_container_base_impl::call_synthesize(parser, first, last, context, attr);
    }

    // Parser has attribute && it is fusion sequence (NOT associative)
    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
        requires
            traits::has_attribute_v<Parser, Context> &&
            fusion::traits::is_sequence<Attribute>::value &&
            (!fusion::traits::is_associative<Attribute>::value)
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& context, Attribute& attr
    ) noexcept(noexcept(parse_into_container_base_impl::call_synthesize(parser, first, last, context, fusion::front(attr))))
    {
        static_assert(traits::has_size_v<Attribute, 1>, "Expecting a single element fusion sequence");
        // TODO: reduce call stack while keeping maintainability
        return parse_into_container_base_impl::call_synthesize(parser, first, last, context, fusion::front(attr));
    }

    // Parser has attribute && it is fusion sequence (associative)
    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
        requires
            traits::has_attribute_v<Parser, Context> &&
            fusion::traits::is_sequence<Attribute>::value &&
            fusion::traits::is_associative<Attribute>::value
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& context, Attribute& attr
    ) // never noexcept (requires container insertion)
    {
        using attribute_type = traits::attribute_of_t<Parser, Context>;
        static_assert(traits::has_size_v<attribute_type, 2>, "To parse directly into fusion map, parser must produce 2 element attr");

        attribute_type attr_; // default-initialize
        static_assert(Parsable<Parser, It, Se, Context, attribute_type>);
        if (!parser.parse(first, last, context, attr_))
        {
            return false;
        }

        // Use the type of first element of attribute as key
        using key = std::remove_reference_t<
            typename fusion::result_of::front<attribute_type>::type
        >;

        save_to_assoc_attr<key>::call(std::move(fusion::front(attr_)), std::move(fusion::back(attr_)), attr);
        return true;
    }

    // Parser has no attribute (pass unused)
    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Context, typename Attribute>
        requires (!traits::has_attribute_v<Parser, Context>)
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& context, Attribute& /* attr */
    ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unused_container_type>)
    {
        // static_assert(Parsable<Parser, It, Se, Context, unused_container_type>);
        return parser.parse(first, last, context, unused_container);
    }
};

template <typename Parser, typename Context>
struct parse_into_container_impl : parse_into_container_base_impl<Parser> {};


template <typename Parser, typename Context>
    requires Parser::handles_container
struct parse_into_container_impl<Parser, Context>
{
    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Attribute>
    static constexpr bool pass_attibute_as_is = std::disjunction_v<
        parser_accepts_container<Parser, Attribute, Context>,

        std::negation<traits::is_substitute< // parser attribute is substitute for container value?
            traits::pseudo_attribute_t<
                Context,
                traits::attribute_of_t<Parser, Context>,
                It, Se
            >,
            traits::container_value_t<Attribute>
        >>
    >;

    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Attribute>
        requires (!pass_attibute_as_is<It, Se, Attribute>)
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& context, Attribute& attr
    ) noexcept(noexcept(parse_into_container_base_impl<Parser>::call(
        parser, first, last, context, attr
    )))
    {
        return parse_into_container_base_impl<Parser>::call(
            parser, first, last, context, attr
        );
    }

    template <std::forward_iterator It, std::sentinel_for<It> Se>
        requires pass_attibute_as_is<It, Se, unused_container_type>
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& context, unused_container_type
    ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unused_container_type>)
    {
        static_assert(Parsable<Parser, It, Se, Context, unused_container_type>);
        return parser.parse(first, last, context, unused_container);
    }

    template <std::forward_iterator It, std::sentinel_for<It> Se, typename Attribute>
        requires pass_attibute_as_is<It, Se, Attribute>
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& context, Attribute& attr
    ) // never noexcept (requires container insertion)
    {
        static_assert(!std::is_same_v<std::remove_const_t<Attribute>, unused_type>);
        static_assert(!std::is_same_v<std::remove_const_t<Attribute>, unused_container_type>);

        static_assert(Parsable<Parser, It, Se, Context, Attribute>);

        if (traits::is_empty(attr))
        {
            return parser.parse(first, last, context, attr);
        }

        Attribute rest; // default-initialize
        if (!parser.parse(first, last, context, rest))
        {
            return false;
        }
        traits::append(
            attr,
            std::make_move_iterator(std::ranges::begin(rest)),
            std::make_move_iterator(std::ranges::end(rest))
        );
        return true;
    }
};

template <
    typename Parser, std::forward_iterator It, std::sentinel_for<It> Se,
    typename Context, typename Attribute
>
[[nodiscard]] constexpr bool
parse_into_container(
    Parser const& parser, It& first, Se const& last,
    Context const& context, Attribute& attr
) noexcept(noexcept(parse_into_container_impl<Parser, Context>::call(
    parser, first, last, context, attr
)))
{
    static_assert(
        !std::same_as<Attribute, unused_type>,
        "`unused_type` should not be passed to `parse_into_container`. Use `x4::assume_container(attr)`"
    );

    return parse_into_container_impl<Parser, Context>::call(
        parser, first, last, context, attr
    );
}

} // boost::spirit::x4::detail

#endif
