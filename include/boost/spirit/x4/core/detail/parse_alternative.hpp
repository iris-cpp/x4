#ifndef BOOST_SPIRIT_X4_CORE_DETAIL_PARSE_ALTERNATIVE_HPP
#define BOOST_SPIRIT_X4_CORE_DETAIL_PARSE_ALTERNATIVE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/config.hpp>
#include <boost/spirit/x4/core/move_to.hpp>

#include <boost/spirit/x4/traits/attribute.hpp>
#include <boost/spirit/x4/traits/tuple_traits.hpp>
#include <boost/spirit/x4/traits/variant_traits.hpp>

#include <boost/spirit/x4/core/parser.hpp>
#include <boost/spirit/x4/core/detail/parse_into_container.hpp>

#include <boost/fusion/include/front.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>

namespace boost::spirit::x4 {

template<class Left, class Right>
struct alternative;

} // boost::spirit::x4

namespace boost::spirit::x4::detail {

struct pass_variant_unused
{
    using type = unused_type;

    template<class T>
    [[nodiscard]] static constexpr unused_type
    call(T&) noexcept
    {
        return unused_type{};
    }
};

template<X4Attribute Attr>
struct pass_variant_used
{
    using type = Attr&;

    [[nodiscard]] static constexpr Attr&
    call(Attr& v) noexcept
    {
        return v;
    }
};

template<>
struct pass_variant_used<unused_type> : pass_variant_unused {};

template<class Parser, X4Attribute Attr, class Context>
struct pass_parser_attribute
{
    using attribute_type = traits::attribute_of_t<Parser, Context>;
    using substitute_type = traits::variant_find_substitute_t<Attr, attribute_type>;

    using type = std::conditional_t<
        std::same_as<Attr, substitute_type>,
        Attr&,
        substitute_type
    >;

    template<X4Attribute Attr_>
        requires std::same_as<Attr_, std::remove_reference_t<type>>
    [[nodiscard]] static constexpr Attr_&
    call(Attr_& attribute) noexcept
    {
        return attribute;
    }

    template<X4Attribute Attr_>
        requires (!std::same_as<Attr_, std::remove_reference_t<type>>)
    [[nodiscard]] static type
    call(Attr_&)
        noexcept(std::is_nothrow_default_constructible_v<type>)
    {
        return type{};
    }
};

// Pass non-variant attributes as-is
template<class Parser, X4Attribute Attr, class Context>
struct pass_non_variant_attribute
{
    using type = Attr&;

    [[nodiscard]] constexpr static Attr&
    call(Attr& attribute) noexcept
    {
        return attribute;
    }
};

// Unwrap single element sequences
template<class Parser, X4Attribute Attr, class Context>
    requires traits::is_size_one_sequence_v<Attr>
struct pass_non_variant_attribute<Parser, Attr, Context>
{
    using attr_type = typename std::remove_reference_t<
        typename fusion::result_of::front<Attr>::type
    >;
    using pass = pass_parser_attribute<Parser, attr_type, Context>;
    using type = typename pass::type;

    template<X4Attribute Attr_>
    [[nodiscard]] static constexpr type
    call(Attr_& attr)
        noexcept(noexcept(pass::call(fusion::front(attr))))
    {
        return pass::call(fusion::front(attr));
    }
};

template<class Parser, X4Attribute Attr, class Context>
    requires (!traits::is_variant_v<Attr>)
struct pass_parser_attribute<Parser, Attr, Context>
    : pass_non_variant_attribute<Parser, Attr, Context>
{};

template<class Parser, class Context>
struct pass_parser_attribute<Parser, unused_type, Context>
    : pass_variant_unused
{};

template<class Parser, X4Attribute Attr, class Context>
struct pass_variant_attribute
    : std::conditional_t<
        traits::has_attribute_v<Parser, Context>,
        pass_parser_attribute<Parser, Attr, Context>,
        pass_variant_unused
    >
{};

template<class L, class R, X4Attribute Attr, class Context>
struct pass_variant_attribute<alternative<L, R>, Attr, Context>
    : std::conditional_t<
        traits::has_attribute_v<alternative<L, R>, Context>,
        pass_variant_used<Attr>,
        pass_variant_unused
    >
{};

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
using parse_alternative_pseudo_t = traits::pseudo_attribute<
    Context, typename pass_variant_attribute<Parser, Attr, Context>::type, It, Se
>;

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
constexpr bool is_reference_pseudo_type = std::is_reference_v<typename parse_alternative_pseudo_t<Parser, It, Se, Context, Attr>::type>;

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    requires is_reference_pseudo_type<Parser, It, Se, Context, Attr>
[[nodiscard]] constexpr bool
parse_alternative(
    Parser const& p, It& first, Se const& last,
    Context const& ctx, Attr& attribute
) noexcept(
    noexcept(parse_alternative_pseudo_t<Parser, It, Se, Context, Attr>::call(
        first, last, pass_variant_attribute<Parser, Attr, Context>::call(attribute)
    )) &&
    is_nothrow_parsable_v<
        Parser, It, Se, Context, std::remove_reference_t<typename parse_alternative_pseudo_t<Parser, It, Se, Context, Attr>::type>
    >
)
{
    using pass = pass_variant_attribute<Parser, Attr, Context>;
    using pseudo = traits::pseudo_attribute<Context, typename pass::type, It, Se>;
    typename pseudo::type attr_ = pseudo::call(first, last, pass::call(attribute));
    return p.parse(first, last, ctx, attr_);
}

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    requires (!is_reference_pseudo_type<Parser, It, Se, Context, Attr>)
[[nodiscard]] constexpr bool
parse_alternative(
    Parser const& p, It& first, Se const& last,
    Context const& ctx, Attr& attribute
) noexcept(
    noexcept(parse_alternative_pseudo_t<Parser, It, Se, Context, Attr>::call(
        first, last, pass_variant_attribute<Parser, Attr, Context>::call(attribute)
    )) &&
    is_nothrow_parsable_v<
        Parser, It, Se, Context, std::remove_reference_t<typename parse_alternative_pseudo_t<Parser, It, Se, Context, Attr>::type>
    > &&
    noexcept(x4::move_to(
        std::declval<typename parse_alternative_pseudo_t<Parser, It, Se, Context, Attr>::type&&>(),
        attribute
    ))
)
{
    using pass = pass_variant_attribute<Parser, Attr, Context>;
    using pseudo = traits::pseudo_attribute<Context, typename pass::type, It, Se>;
    typename pseudo::type attr_ = pseudo::call(first, last, pass::call(attribute));

    if (!p.parse(first, last, ctx, attr_)) return false;
    x4::move_to(std::move(attr_), attribute);
    return true;
}

template<class Subject>
struct alternative_helper : unary_parser<Subject, alternative_helper<Subject>>
{
    static constexpr bool is_pass_through_unary = true;

    using unary_parser<Subject, alternative_helper<Subject>>::unary_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, X4Attribute Attr>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& ctx, Attr& attr) const
        noexcept(noexcept(detail::parse_alternative(this->subject, first, last, ctx, attr)))
    {
        return detail::parse_alternative(this->subject, first, last, ctx, attr);
    }
};

template<class Left, class Right, class Context>
struct parse_into_container_impl<alternative<Left, Right>, Context>
{
    using parser_type = alternative<Left, Right>;

    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Attribute Attr>
        requires traits::is_variant_v<traits::container_value_t<Attr>>
    [[nodiscard]] static constexpr bool
    call(
        parser_type const& parser,
        It& first, Se const& last, Context const& ctx, Attr& attribute
    )
    {
        return detail::parse_into_container(alternative_helper<Left>{parser.left}, first, last, ctx, attribute)
            || detail::parse_into_container(alternative_helper<Right>{parser.right}, first, last, ctx, attribute);
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, X4Attribute Attr>
        requires (!traits::is_variant_v<traits::container_value_t<Attr>>)
    [[nodiscard]] static constexpr bool
    call(
        parser_type const& parser,
        It& first, Se const& last,
        Context const& ctx, Attr& attribute
    )
    {
        return detail::parse_into_container(parser.left, first, last, ctx, attribute)
            || detail::parse_into_container(parser.right, first, last, ctx, attribute);
    }
};

} // boost::spirit::x4::detail

#endif
