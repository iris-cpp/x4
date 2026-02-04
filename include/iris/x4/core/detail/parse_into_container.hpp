#ifndef IRIS_X4_CORE_DETAIL_PARSE_INTO_CONTAINER_HPP
#define IRIS_X4_CORE_DETAIL_PARSE_INTO_CONTAINER_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp>

#include <iris/x4/core/parser.hpp>
#include <iris/x4/core/container_appender.hpp>

#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/traits/substitution.hpp>

#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/sequence/intrinsic/front.hpp>
#include <boost/fusion/sequence/intrinsic/back.hpp>

#include <boost/variant/variant.hpp> // TODO: remove Boost.Variant usage

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4::detail {

template<class Parser, class Container>
struct parser_accepts_container
    : traits::is_substitute<typename parser_traits<Parser>::attribute_type, Container>
{};

template<class Parser, class Container>
constexpr bool parser_accepts_container_v = parser_accepts_container<Parser, Container>::value;

template<class Parser>
struct parse_into_container_base_impl
{
    // Parser has attribute (synthesize; Attribute is a container)
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires (!parser_accepts_container_v<Parser, Attr>)
    [[nodiscard]] static constexpr bool
    call_synthesize(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) // never noexcept (requires container insertion)
    {
        static_assert(!std::same_as<std::remove_const_t<Attr>, unused_container_type>);

        using value_type = traits::container_value_t<Attr>;
        value_type val; // default-initialize

        static_assert(Parsable<Parser, It, Se, Context, value_type>);
        if (!parser.parse(first, last, ctx, val)) return false;

        // push the parsed value into our attribute
        traits::push_back(attr, std::move(val));
        return true;
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
        requires (!parser_accepts_container_v<Parser, unused_container_type>)
    [[nodiscard]] static constexpr bool
    call_synthesize(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, unused_container_type const&
    ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unused_type>)
    {
        static_assert(Parsable<Parser, It, Se, Context, unused_type>);
        return parser.parse(first, last, ctx, unused);
    }

    // Parser has attribute (synthesize; Attribute is a container)
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires parser_accepts_container_v<Parser, Attr>
    [[nodiscard]] static constexpr bool
    call_synthesize(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, Attr>)
    {
        static_assert(Parsable<Parser, It, Se, Context, Attr>);
        return parser.parse(first, last, ctx, attr);
    }

    // Parser has attribute && it is NOT fusion sequence
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires
            has_attribute_v<Parser> &&
            (!boost::fusion::traits::is_sequence<Attr>::value)
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    )
    {
        // TODO: reduce call stack while keeping maintainability
        return parse_into_container_base_impl::call_synthesize(parser, first, last, ctx, attr);
    }

    // Parser has attribute && it is fusion sequence
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires
            has_attribute_v<Parser> &&
            boost::fusion::traits::is_sequence<Attr>::value
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) noexcept(noexcept(parse_into_container_base_impl::call_synthesize(parser, first, last, ctx, boost::fusion::front(attr))))
    {
        static_assert(traits::has_size_v<Attr, 1>, "Expecting a single element fusion sequence");
        // TODO: reduce call stack while keeping maintainability
        return parse_into_container_base_impl::call_synthesize(parser, first, last, ctx, boost::fusion::front(attr));
    }

    // Parser has no attribute (pass unused)
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires (!has_attribute_v<Parser>)
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& /* attr */
    ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unused_container_type>)
    {
        // static_assert(Parsable<Parser, It, Se, Context, unused_container_type>);
        return parser.parse(first, last, ctx, unused_container);
    }
};

template<class Parser>
struct parse_into_container_impl : parse_into_container_base_impl<Parser> {};


template<class Parser>
    requires Parser::handles_container
struct parse_into_container_impl<Parser>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    static constexpr bool pass_attibute_as_is = std::disjunction_v<
        parser_accepts_container<Parser, Attr>,

        std::negation<traits::is_substitute< // parser attribute is substitute for container value?
            typename traits::pseudo_attribute<
                It, Se, Context,
                typename parser_traits<Parser>::attribute_type
            >::actual_type,
            traits::container_value_t<Attr>
        >>
    >;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires (!pass_attibute_as_is<It, Se, Context, Attr>)
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) noexcept(noexcept(parse_into_container_base_impl<Parser>::call(
        parser, first, last, ctx, attr
    )))
    {
        return parse_into_container_base_impl<Parser>::call(
            parser, first, last, ctx, attr
        );
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
        requires pass_attibute_as_is<It, Se, Context, unused_container_type>
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, unused_container_type
    ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unused_container_type>)
    {
        static_assert(Parsable<Parser, It, Se, Context, unused_container_type>);
        return parser.parse(first, last, ctx, unused_container);
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires pass_attibute_as_is<It, Se, Context, Attr>
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) // never noexcept (requires container insertion)
    {
        static_assert(!std::same_as<std::remove_const_t<Attr>, unused_type>);
        static_assert(!std::same_as<std::remove_const_t<Attr>, unused_container_type>);
        static_assert(Parsable<Parser, It, Se, Context, Attr>);

        auto&& appender = x4::make_container_appender(attr);
        return parser.parse(first, last, ctx, appender);
    }
};

template<
    class Parser, std::forward_iterator It, std::sentinel_for<It> Se,
    class Context, X4Attribute Attr
>
[[nodiscard]] constexpr bool
parse_into_container(
    Parser const& parser, It& first, Se const& last,
    Context const& ctx, Attr& attr
) noexcept(noexcept(parse_into_container_impl<Parser>::call(parser, first, last, ctx, attr)))
{
    static_assert(
        !std::same_as<Attr, unused_type>,
        "`unused_type` should not be passed to `parse_into_container`. Use `x4::assume_container(attr)`"
    );

    return parse_into_container_impl<Parser>::call(parser, first, last, ctx, attr);
}

} // iris::x4::detail

#endif
