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
#include <iris/x4/core/multi_parser.hpp>
#include <iris/x4/core/container_appender.hpp>
#include <iris/x4/core/detail/parse_into_container.hpp>

#include <iris/x4/traits/attribute_category.hpp>
#include <iris/x4/traits/container_traits.hpp>
#include <iris/x4/traits/tuple_traits.hpp>
#include <iris/x4/traits/can_hold.hpp>

#include <iris/alloy/tuple.hpp>

#include <array>
#include <iterator>
#include <type_traits>
#include <utility>

#include <cstddef>

namespace iris::x4 {

template<class... Ps>
struct sequence;

} // iris::x4

namespace iris::x4::detail {

template<class... Ps>
struct sequence_layout
{
    static constexpr std::size_t parser_count = sizeof...(Ps);

    static constexpr std::array<std::size_t, parser_count> elem_sequence_sizes{parser_traits<Ps>::sequence_size...};
    static constexpr std::size_t total_sequence_size = (std::size_t{0} + ... + parser_traits<Ps>::sequence_size);

    static constexpr std::array<std::size_t, parser_count> elem_offsets = [] {
        std::array<std::size_t, parser_count> result{};
        std::size_t offset = 0;
        for (std::size_t i = 0; i < parser_count; ++i) {
            result[i] = offset;
            offset += elem_sequence_sizes[i];
        }
        return result;
    }();

    static constexpr std::size_t attributed_count = (std::size_t{0} + ... + std::size_t{has_attribute_v<Ps>});

    static constexpr std::size_t single_attributed_index = [] {
        std::array<bool, parser_count> const is_attributed{has_attribute_v<Ps>...};
        for (std::size_t i = 0; i < parser_count; ++i) {
            if (is_attributed[i]) return i;
        }
        return parser_count;
    }();
};

template<class P>
struct sequence_passes_view : std::false_type {};

template<class... Ps>
struct sequence_passes_view<sequence<Ps...>> : std::true_type {};

template<class P>
    requires requires { typename P::proxy_backend_type; }
struct sequence_passes_view<P> : sequence_passes_view<typename P::proxy_backend_type> {};


// A helper to isolate the actual logic inside a single struct.
//
// Theoretically, this can be written directly inside a lambda in `parse_sequence`.
// However, MSVC historically fails to optimize the compilation time of this kind
// of logic when it is written directly inside a large function.
//
// MSVC has a bad behavior where it always reparses the entire tokens of large
// function body when it needs to be "reinspected" for some arbitrary reason, like
// different types of specialization, etc. This is NOT the matter of the template
// instantiation cost; it is due to the function parsing and tokenization behavior.
//
// The result is about 50-80ms reduced compilation time (in realistic code) when
// this is isolated in a struct like below.
template<class Attr, class... Ps>
struct parse_sequence_tuple
{
    using layout = sequence_layout<Ps...>;

    template<std::size_t I, std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    [[nodiscard]] static constexpr bool
    parse_element(sequence<Ps...> const& seq, It& first, Se const& last, Context const& ctx, Attr& attr)
    {
        using parser_type = multi_parser_t<I, Ps...>;
        auto const& elem = x4::get_parser<I>(seq.elems);
        constexpr std::size_t sequence_size = layout::elem_sequence_sizes[I];
        constexpr std::size_t offset = layout::elem_offsets[I];

        if constexpr (layout::attributed_count == 1) {
            if constexpr (I != layout::single_attributed_index) {
                return elem.parse(first, last, ctx, unused);

            } else if constexpr (traits::is_size_one_view_v<Attr> && !sequence_passes_view<parser_type>::value) {
                auto&& elem_attr = x4::make_container_appender(alloy::get<0>(attr));
                return elem.parse(first, last, ctx, elem_attr);

            } else {
                auto&& elem_attr = x4::make_container_appender(attr);
                return elem.parse(first, last, ctx, elem_attr);
            }

        } else {
            if constexpr (sequence_size == 0) {
                return elem.parse(first, last, ctx, unused);

            } else if constexpr (sequence_size == 1 && !sequence_passes_view<parser_type>::value) {
                auto&& elem_attr = x4::make_container_appender(alloy::get<offset>(attr));
                return elem.parse(first, last, ctx, elem_attr);

            } else {
                auto slice = [&]<std::size_t... Is>(std::index_sequence<Is...>) {
                    return alloy::tuple<alloy::tuple_element_t<offset + Is, Attr>&...>(
                        alloy::get<offset + Is>(attr)...
                    );
                }(std::make_index_sequence<sequence_size>{});
                return elem.parse(first, last, ctx, slice);
            }
        }
    }

    template<std::size_t... Is, std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    [[nodiscard]] static constexpr bool
    parse_all(std::index_sequence<Is...>, sequence<Ps...> const& seq, It& first, Se const& last, Context const& ctx, Attr& attr)
    {
        return (parse_sequence_tuple::parse_element<Is>(seq, first, last, ctx, attr) && ...);
    }
};


// Default overload; attribute is NOT a container
template<class... Ps, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attr>
[[nodiscard]] constexpr bool
parse_sequence(sequence<Ps...> const& seq, It& first, Se const& last, Context const& ctx, Attr& attr)
{
    static_assert(X4Attribute<Attr>);
    static_assert(!traits::CategorizedAttr<Attr, traits::container_attr>);

    using layout = sequence_layout<Ps...>;

    // Intentionally verbose branches for avoiding instantiation of erroneous grammar stem,
    // significantly reducing the amount of compilation error.

    if constexpr (layout::attributed_count < 2) {
        It local_it = first;
        if (parse_sequence_tuple<Attr, Ps...>::parse_all(std::index_sequence_for<Ps...>{}, seq, local_it, last, ctx, attr)) {
            first = std::move(local_it);
            return true;
        }
        return false;

    } else if constexpr (!traits::CategorizedAttr<Attr, traits::tuple_attr>) {
        static_assert(false, "The attribute of a sequence with >=2 attributed elements must be tuple-like.");
        return false;

    } else if constexpr (alloy::tuple_size_v<Attr> < layout::total_sequence_size) {
        static_assert(false, "Sequence size of the passed attribute is less than expected.");
        return false;

    } else if constexpr (alloy::tuple_size_v<Attr> > layout::total_sequence_size) {
        static_assert(false, "Sequence size of the passed attribute is greater than expected.");
        return false;

    } else {
        It local_it = first;
        if (parse_sequence_tuple<Attr, Ps...>::parse_all(std::index_sequence_for<Ps...>{}, seq, local_it, last, ctx, attr)) {
            first = std::move(local_it);
            return true;
        }
        return false;
    }
}

// Attribute is a container
template<
    class... Ps, std::forward_iterator It, std::sentinel_for<It> Se, class Context,
    traits::CategorizedAttr<traits::container_attr> ContainerAttr
>
[[nodiscard]] constexpr bool
parse_sequence(sequence<Ps...> const& seq, It& first, Se const& last, Context const& ctx, ContainerAttr& container_attr)
{
    It local_it = first;
    bool const ok = [&]<std::size_t... Is>(std::index_sequence<Is...>) -> bool {
        auto parse_sequence_impl = [&]<class P>(P const& parser) -> bool {
            if constexpr (parser_traits<P>::sequence_size > 1) {
                // Exposed attribute = container, Parser expects sequence attribute
                return parser.parse(local_it, last, ctx, container_attr);

            } else {
                // Exposed attribute = container, Parser expects non-sequence attribute
                return detail::parse_into_container(parser, local_it, last, ctx, container_attr);
            }
        };
        return (parse_sequence_impl(x4::get_parser<Is>(seq.elems)) && ...);
    }(std::index_sequence_for<Ps...>{});

    if (ok) {
        first = std::move(local_it);
        return true;
    }
    return false;
}

template<class... Ps>
struct parse_into_container_impl<sequence<Ps...>>
{
    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] static constexpr bool
    call(
        sequence<Ps...> const& seq, It& first, Se const& last,
        Context const& ctx, Attr& attr
    )
    {
        if constexpr (traits::is_container_v<Attr>) {
            constexpr bool sequence_attribute_can_directly_hold_value_type = traits::can_hold<
                typename parser_traits<sequence<Ps...>>::attribute_type,
                typename traits::container_value<Attr>::type
            >::value;

            if constexpr (sequence_attribute_can_directly_hold_value_type) {
                return parse_into_container_impl_default<sequence<Ps...>>::call(seq, first, last, ctx, attr);

            } else {
                auto&& appender = x4::make_container_appender(x4::assume_container(attr));
                return detail::parse_sequence(seq, first, last, ctx, appender);
            }

        } else {
            return parse_into_container_impl_default<sequence<Ps...>>::call(seq, first, last, ctx, attr);
        }
    }
};

} // iris::x4::detail

#endif
