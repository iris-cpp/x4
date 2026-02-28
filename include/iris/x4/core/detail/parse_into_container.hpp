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
#include <iris/x4/traits/tuple_traits.hpp>
#include <iris/x4/traits/can_hold.hpp>

#include <iris/alloy/tuple.hpp>

#include <iterator>
#include <type_traits>
#include <utility>

namespace iris::x4::detail {

// template<class Parser>
// struct parse_into_container_base_impl
// {
//     // Parser has attribute (synthesize; Attribute is a container)
//     template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
//         requires (!parser_accepts_container_v<Parser, unwrap_recursive_type<Attr>>)
//     [[nodiscard]] static constexpr bool
//     call_synthesize(
//         Parser const& parser, It& first, Se const& last,
//         Context const& ctx, Attr& attr
//     ) // never noexcept (requires container insertion)
//     {
//         static_assert(!std::same_as<std::remove_const_t<Attr>, unused_container_type>);

//         using value_type = traits::container_value_t<unwrap_recursive_type<Attr>>;
//         value_type val; // default-initialize

//         //static_assert(Parsable<Parser, It, Se, Context, value_type>);
//         if (!parser.parse(first, last, ctx, val)) return false;

//         // push the parsed value into our attribute
//         traits::push_back(unwrap_recursive(attr), std::move(val));
//         return true;
//     }

//     // unused_container_type
//     template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
//         requires (!parser_accepts_container_v<Parser, unused_container_type>)
//     [[nodiscard]] static constexpr bool
//     call_synthesize(
//         Parser const& parser, It& first, Se const& last,
//         Context const& ctx, unused_container_type const&
//     ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unused_type>)
//     {
//         //static_assert(Parsable<Parser, It, Se, Context, unused_type>);
//         return parser.parse(first, last, ctx, unused);
//     }

//     // Parser has attribute (synthesize; Attribute is a container)
//     template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
//         requires parser_accepts_container_v<Parser, unwrap_recursive_type<Attr>>
//     [[nodiscard]] static constexpr bool
//     call_synthesize(
//         Parser const& parser, It& first, Se const& last,
//         Context const& ctx, Attr& attr
//     ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unwrap_recursive_type<Attr>>)
//     {
//         //static_assert(Parsable<Parser, It, Se, Context, unwrap_recursive_type<Attr>>);
//         return parser.parse(first, last, ctx, attr);
//     }

//     // ------------------------------------------------------

//     // Parser has attribute && it is NOT tuple-like
//     template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
//         requires
//             has_attribute_v<Parser> &&
//             (!alloy::is_tuple_like_v<Attr>)
//     [[nodiscard]] static constexpr bool
//     call(
//         Parser const& parser, It& first, Se const& last,
//         Context const& ctx, Attr& attr
//     )
//     {
//         // TODO: reduce call stack while keeping maintainability
//         return parse_into_container_base_impl::call_synthesize(parser, first, last, ctx, attr);
//     }

//     // Parser has attribute && it is tuple-like
//     template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
//         requires
//             has_attribute_v<Parser> &&
//             alloy::is_tuple_like_v<Attr>
//     [[nodiscard]] static constexpr bool
//     call(
//         Parser const& parser, It& first, Se const& last,
//         Context const& ctx, Attr& attr
//     ) noexcept(noexcept(parse_into_container_base_impl::call_synthesize(parser, first, last, ctx, alloy::get<0>(attr))))
//     {
//         static_assert(traits::has_size_v<Attr, 1>, "Expecting a single element tuple-like");
//         // TODO: reduce call stack while keeping maintainability
//         return parse_into_container_base_impl::call_synthesize(parser, first, last, ctx, alloy::get<0>(attr));
//     }

//     // Parser has no attribute (pass unused)
//     template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
//         requires (!has_attribute_v<Parser>)
//     [[nodiscard]] static constexpr bool
//     call(
//         Parser const& parser, It& first, Se const& last,
//         Context const& ctx, Attr& /* attr */
//     ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unused_container_type>)
//     {
//         // static_assert(Parsable<Parser, It, Se, Context, unused_container_type>);
//         return parser.parse(first, last, ctx, unused_container);
//     }
// };

// template<class Parser>
// struct parse_into_container_impl : parse_into_container_base_impl<Parser> {};


// template<class Parser>
//     requires Parser::handles_container
// struct parse_into_container_impl<Parser>
// {
//     // TODO: decompose pass_attribute_as_is to make more logic clear
//     template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
//     static constexpr bool pass_attibute_as_is = std::disjunction_v<
//         parser_accepts_container<Parser, Attr>,

//         std::negation<traits::can_hold<
//             typename traits::pseudo_attribute<
//                 It, Se, Context,
//                 typename parser_traits<Parser>::attribute_type
//             >::actual_type,
//             traits::container_value_t<Attr>
//         >>
//     >;

//     template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
//         requires (!pass_attibute_as_is<It, Se, Context, Attr>)
//     [[nodiscard]] static constexpr bool
//     call(
//         Parser const& parser, It& first, Se const& last,
//         Context const& ctx, Attr& attr
//     ) noexcept(noexcept(parse_into_container_base_impl<Parser>::call(
//         parser, first, last, ctx, attr
//     )))
//     {
//         return parse_into_container_base_impl<Parser>::call(
//             parser, first, last, ctx, attr
//         );
//     }

//     template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
//         requires pass_attibute_as_is<It, Se, Context, unused_container_type>
//     [[nodiscard]] static constexpr bool
//     call(
//         Parser const& parser, It& first, Se const& last,
//         Context const& ctx, unused_container_type
//     ) noexcept(is_nothrow_parsable_v<Parser, It, Se, Context, unused_container_type>)
//     {
//         static_assert(Parsable<Parser, It, Se, Context, unused_container_type>);
//         return parser.parse(first, last, ctx, unused_container);
//     }

//     template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
//         requires pass_attibute_as_is<It, Se, Context, Attr>
//     [[nodiscard]] static constexpr bool
//     call(
//         Parser const& parser, It& first, Se const& last,
//         Context const& ctx, Attr& attr
//     ) // never noexcept (requires container insertion)
//     {
//         static_assert(!std::same_as<std::remove_const_t<Attr>, unused_type>);
//         static_assert(!std::same_as<std::remove_const_t<Attr>, unused_container_type>);
//         static_assert(Parsable<Parser, It, Se, Context, Attr>);

//         auto&& appender = x4::make_container_appender(attr);
//         return parser.parse(first, last, ctx, appender);
//     }
// };

template<class Parser, class Container>
struct parser_accepts_container {};

template<class Parser, class Container>
    requires traits::is_container_v<std::remove_const_t<Container>>
struct parser_accepts_container<Parser, Container>
    : std::disjunction<
        traits::can_hold<typename parser_traits<Parser>::attribute_type, Container>,
        std::bool_constant<parser_traits<Parser>::handles_container>
    >
{};

template<class Parser, class Container>
inline constexpr bool parser_accepts_container_v = parser_accepts_container<Parser, Container>::value;

template<class Parser>
struct parse_into_container_impl_default {
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
    static constexpr bool call(Parser const& parser, It& first, Se const& last, Context& ctx, Attr& attr) // TODO: add noexcept
    {
        using unwrapped_attribute_type = iris::unwrap_recursive_type<Attr>;
        auto& unwrapped_attr = iris::unwrap_recursive(attr);

        if constexpr (traits::is_container_v<unwrapped_attribute_type>) { // Attr is a container
            if constexpr (parser_accepts_container_v<Parser, unwrapped_attribute_type>) { // parser accepts the container; make parser append directly
                auto&& appender = x4::make_container_appender(unwrapped_attr);
                return parser.parse(first, last, ctx, appender);
            } else { // parser DOES NOT accept the container; parse into value type and append it
                using value_type = traits::container_value_t<unwrapped_attribute_type>;
                value_type value;
                if (!parser.parse(first, last, ctx, value)) return false;
                traits::push_back(unwrapped_attr, std::move(value));
                return true;
            }
        } else {
            if constexpr (traits::is_size_one_sequence_v<unwrapped_attribute_type>) { // attribute is single element tuple-like; unwrap and try again
                return parse_into_container_impl_default<Parser>::call(parser, first, last, ctx, alloy::get<0>(unwrapped_attr));
            } else {
                static_assert(false, "parse_into_container accepts a container, a variant of container or a single element tuple-like of container");
                return false;
            }
        }
    }
};

// internal customization point
template<class Parser>
struct parse_into_container_impl
    : parse_into_container_impl_default<Parser>
{};

//template<class Parser>
//    requires parser_traits<Parser>::handles_container
//struct parse_into_container_impl<Parser>
//{
//    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4NonUnusedAttribute Attr>
//    static constexpr bool call(Parser const& parser, It& first, Se const& last, Context& ctx, Attr& attr) // TODO: add noexcept
//    {
//        // TODO: make below English
//        // - attribute_type が container のとき
//        // - Parser::handles_container が true のとき
//        //   現在の例は
//        //   - raw
//        //   - optional
//
//        using unwrapped_attribute_type = iris::unwrap_recursive_type<Attr>;
//        auto& unwrapped_attr = iris::unwrap_recursive(attr);
//
//        if constexpr (traits::is_container_v<unwrapped_attribute_type>) {
//            if constexpr (parser_accepts_container_v<Parser, unwrapped_attribute_type>) {
//                auto&& appender = x4::make_container_appender(unwrapped_attr);
//                return parser.parse(first, last, ctx, appender);
//            } else {
//                return false;  // ?
//            }
//        } else {
//            // TODO: make is_pseudo_attribute
//            // static_assert(is_pseudo_attribute<unwrapped_attibute_type>);
//            static_assert(false);
//            return false;
//        }
//    }
//};

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
[[nodiscard]] constexpr bool
parse_into_container(
    Parser const& parser, It& first, Se const& last,
    Context const& ctx, Attr& attr
) // TODO: add noexcept
{
    if constexpr (X4UnusedAttribute<Attr> || !has_attribute_v<Parser>) { // handle unused types first
        return parser.parse(first, last, ctx, unused);
    } else {
        if constexpr (traits::is_variant_v<Attr>) {
             // e.g. `char` when the caller is `+char_`
            using attribute_type = parser_traits<Parser>::attribute_type;

            // e.g. `std::string` when the attribute_type is `char`
            using substitute_type = traits::variant_find_holdable_type_t<Attr, traits::build_container_t<attribute_type>>;

            // instead of creating a temporary `substitute_type`, append directly into the emplaced alternative
            auto& variant_alt = attr.template emplace<substitute_type>();
            return parse_into_container_impl<Parser>::call(parser, first, last, ctx, variant_alt);
        } else {
            return parse_into_container_impl<Parser>::call(parser, first, last, ctx, attr);
        }
    }
}

} // iris::x4::detail

#endif
