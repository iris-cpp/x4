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

#include <iris/alloy/tuple.hpp>

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
        requires (!parser_accepts_container_v<Parser, unwrap_recursive_type<Attr>>)
    [[nodiscard]] static constexpr bool
    call_synthesize(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) // never noexcept (requires container insertion)
    {
        static_assert(!std::same_as<std::remove_const_t<Attr>, unused_container_type>);

        using value_type = traits::container_value_t<unwrap_recursive_type<Attr>>;
        value_type val; // default-initialize

        //static_assert(Parsable<Parser, It, Se, Context, value_type>);
        if (!parser.parse(first, last, ctx, val)) return false;

        // push the parsed value into our attribute
        traits::push_back(unwrap_recursive(attr), std::move(val));
        return true;
    }

    // unused_container_type
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
        requires (!parser_accepts_container_v<Parser, unused_container_type>)
    [[nodiscard]] static constexpr bool
    call_synthesize(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, unused_container_type const&
    ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unused_type>)
    {
        //static_assert(Parsable<Parser, It, Se, Context, unused_type>);
        return parser.parse(first, last, ctx, unused);
    }

    // Parser has attribute (synthesize; Attribute is a container)
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires parser_accepts_container_v<Parser, unwrap_recursive_type<Attr>>
    [[nodiscard]] static constexpr bool
    call_synthesize(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unwrap_recursive_type<Attr>>)
    {
        //static_assert(Parsable<Parser, It, Se, Context, unwrap_recursive_type<Attr>>);
        return parser.parse(first, last, ctx, attr);
    }

    // ------------------------------------------------------

    // Parser has attribute && it is NOT tuple-like
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires
            has_attribute_v<Parser> &&
            (!alloy::is_tuple_like_v<Attr>)
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    )
    {
        // TODO: reduce call stack while keeping maintainability
        return parse_into_container_base_impl::call_synthesize(parser, first, last, ctx, attr);
    }

    // Parser has attribute && it is tuple-like
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
        requires
            has_attribute_v<Parser> &&
            alloy::is_tuple_like_v<Attr>
    [[nodiscard]] static constexpr bool
    call(
        Parser const& parser, It& first, Se const& last,
        Context const& ctx, Attr& attr
    ) noexcept(noexcept(parse_into_container_base_impl::call_synthesize(parser, first, last, ctx, alloy::get<0>(attr))))
    {
        static_assert(traits::is_single_element_tuple_like_v<Attr>, "Expecting a single element tuple-like");
        // TODO: reduce call stack while keeping maintainability
        return parse_into_container_base_impl::call_synthesize(parser, first, last, ctx, alloy::get<0>(attr));
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

    if constexpr (traits::is_variant_v<Attr>) {
        // e.g. `char` when the caller is `+char_`
        using attribute_type = parser_traits<Parser>::attribute_type;
     
        // e.g. `std::string` when the attribute_type is `char`
        using substitute_type = traits::variant_find_substitute_t<Attr, traits::build_container_t<attribute_type>>;

        // instead of creating a temporary `substitute_type`, append directly into the emplaced alternative
        auto& variant_alt = attr.template emplace<substitute_type>();
        return parse_into_container_impl<Parser>::call(parser, first, last, ctx, variant_alt);
    } else {
        return parse_into_container_impl<Parser>::call(parser, first, last, ctx, attr);
    }
}

} // iris::x4::detail

#endif
