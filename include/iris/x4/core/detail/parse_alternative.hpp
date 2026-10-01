#ifndef IRIS_ZZ_X4_CORE_DETAIL_PARSE_ALTERNATIVE_HPP
#define IRIS_ZZ_X4_CORE_DETAIL_PARSE_ALTERNATIVE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/x4/traits/attribute_traits.hpp>
#include <iris/x4/traits/container_traits.hpp>

#include <iris/x4/core/detail/parse_into_container.hpp>
#include <iris/x4/core/expectation.hpp>
#include <iris/x4/core/write_attribute.hpp>
#include <iris/x4/core/nary_parser.hpp>
#include <iris/x4/core/parser_traits.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/parser.hpp>

#include <iris/alloy/tuple.hpp> // IWYU pragma: keep

#include <concepts>
#include <iterator>
#include <type_traits>

namespace iris::x4 {

template<class... Ps>
struct alternative;

} // iris::x4

namespace iris::x4::detail {

template<class Context>
[[nodiscard]] constexpr bool alternative_should_stop(Context const& ctx) noexcept
{
    if constexpr (has_context_v<Context, contexts::expectation_failure>) {
        return x4::has_expectation_failure(ctx);
    } else {
        return false;
    }
}

// This must be an independent struct to reduce compilation time.
// For details, see notes on `parse_sequence_tuple`.
template<class... Ps>
struct parse_alternative_all_impl
{
    template<std::size_t I, class Try, X4NonUnusedAttribute ExposedAttr>
    [[nodiscard]] static constexpr bool parse_branch(Try&& try_branch, ExposedAttr& exposed_attr)
    {
        using branch_attr = parser_traits<nary::parser_t<I, Ps...>>::attribute_type;
        if constexpr (X4UnusedAttribute<branch_attr> || traits::detail::clearable_for<ExposedAttr, branch_attr>) {
            auto&& alt_attr = detail::prepare_attribute<branch_attr>(exposed_attr);
            return try_branch.template operator()<I>(alt_attr);

        } else {
            static_assert(
                requires(branch_attr&& value) { x4::write_attribute(exposed_attr, std::move(value)); },
                "The attribute of this branch cannot be converted into the attribute of the alternative."
            );
            // The branch yields a whole value of an unrelated shape (e.g. a narrower variant);
            // parse it into a temporary and convert on success.
            branch_attr temp{};
            if (!try_branch.template operator()<I>(temp)) return false;

            // As in the other branches, the value is written into the default state. An earlier
            // branch may have failed after writing, and `write_attribute` writes into an existing
            // content: it appends to a container, and writes nothing for a disengaged optional.
            traits::attribute_traits<ExposedAttr>::reset(exposed_attr);
            x4::write_attribute(exposed_attr, std::move(temp));
            return true;
        }
    }
};

// Tries the branches in order; stops at the first match, or at an expectation
// failure raised inside a branch.
template<class... Ps>
struct parse_alternative_all
{
    template<std::size_t... Is, class Try, class Context, X4UnusedAttribute UnusedAttr>
    [[nodiscard]] static constexpr bool
    call(std::index_sequence<Is...>, Try&& try_branch, Context const& ctx, UnusedAttr const& unused_attr)
    {
        bool matched = false;
        (void)((((matched = try_branch.template operator()<Is>(unused_attr))) || detail::alternative_should_stop(ctx)) || ...);
        return matched;
    }

    template<std::size_t... Is, class Try, class Context, X4NonUnusedAttribute ExposedAttr>
        requires (!traits::X4Container<ExposedAttr>)
    [[nodiscard]] static constexpr bool
    call(std::index_sequence<Is...>, Try&& try_branch, Context const& ctx, ExposedAttr& exposed_attr)
    {
        static_assert(!std::is_const_v<ExposedAttr>);
        bool matched = false;
        (void)((((matched = parse_alternative_all_impl<Ps...>::template parse_branch<Is>(try_branch, exposed_attr))) || detail::alternative_should_stop(ctx)) || ...);
        return matched;
    }

    template<std::size_t... Is, class Try, class Context, X4NonUnusedAttribute ContainerAttr>
        requires traits::X4Container<ContainerAttr>
    [[nodiscard]] static constexpr bool
    call(std::index_sequence<Is...>, Try&& try_branch, Context const& ctx, ContainerAttr& container_attr)
    {
        static_assert(!std::same_as<std::remove_const_t<ContainerAttr>, unused_type>);
        static_assert(!std::same_as<std::remove_const_t<ContainerAttr>, unused_container_type>);
        static_assert(!std::is_const_v<ContainerAttr>);

        // Same logic as in `x4::optional`

        // We can (ab)use the exposed attribute as the temporary workspace
        // if and only if the modification or rollback of the exposed attribute
        // does not change the semantic state of the exposed container instance.
        //
        // The only situation we can guarantee such condition is when the container
        // is empty; assuming that the "empty" state of any user-provided container
        // class is monostate.
        if (std::ranges::empty(container_attr)) {
            auto parse_branch = [&]<std::size_t I>() -> bool {
                if constexpr (!has_attribute_v<nary::parser_t<I, Ps...>>) {
                    return try_branch.template operator()<I>(unused);

                } else {
                    if (try_branch.template operator()<I>(container_attr)) return true;
                    iris::container::clear(container_attr);
                    return false;
                }
            };
            bool matched = false;
            (void)((((matched = parse_branch.template operator()<Is>())) || detail::alternative_should_stop(ctx)) || ...);
            return matched;
        }

        // The container already holds elements: a failed branch must not touch
        // them, and there is no general way to undo appends, so each branch parses
        // into a buffer that is appended only on success.
        ContainerAttr buffer;
        auto parse_branch = [&]<std::size_t I>() -> bool {
            if constexpr (!has_attribute_v<nary::parser_t<I, Ps...>>) {
                return try_branch.template operator()<I>(unused);

            } else {
                if (try_branch.template operator()<I>(buffer)) {
                    iris::container::append_range(container_attr, buffer | std::views::as_rvalue);
                    return true;
                }
                iris::container::clear(buffer);
                return false;
            }
        };
        bool matched = false;
        (void)((((matched = parse_branch.template operator()<Is>())) || detail::alternative_should_stop(ctx)) || ...);
        return matched;
    }
};

template<class... Ps>
struct parse_into_container_impl<alternative<Ps...>>
{
    using parser_type = alternative<Ps...>;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute ExposedAttr>
    [[nodiscard]] static constexpr bool
    call(
        parser_type const& parser,
        It& first, Se const& last, Context const& ctx, ExposedAttr& exposed_attr
    )
    {
        static_assert(traits::X4Container<ExposedAttr>);

        return parse_alternative_all<Ps...>::call(
            std::index_sequence_for<Ps...>{},
            [&]<std::size_t I>(auto& container_attr) {
                return detail::parse_into_container(nary::get<I>(parser.elems), first, last, ctx, container_attr);
            },
            ctx,
            exposed_attr
        );
    }
};

} // iris::x4::detail

#endif
