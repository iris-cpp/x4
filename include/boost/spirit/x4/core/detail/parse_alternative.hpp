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

template<class Attribute>
struct pass_variant_used
{
    using type = Attribute&;

    [[nodiscard]] static constexpr Attribute&
    call(Attribute& v) noexcept
    {
        return v;
    }
};

template<>
struct pass_variant_used<unused_type> : pass_variant_unused {};

template<class Parser, class Attribute, class Context>
struct pass_parser_attribute
{
    using attribute_type = traits::attribute_of_t<Parser, Context>;
    using substitute_type = traits::variant_find_substitute_t<Attribute, attribute_type>;

    using type = std::conditional_t<
        std::is_same_v<Attribute, substitute_type>,
        Attribute&,
        substitute_type
    >;

    template<class Attribute_>
        requires std::is_same_v<Attribute_, std::remove_reference_t<type>>
    [[nodiscard]] static constexpr Attribute_&
    call(Attribute_& attribute) noexcept
    {
        return attribute;
    }

    template<class Attribute_>
        requires (!std::is_same_v<Attribute_, std::remove_reference_t<type>>)
    [[nodiscard]] static type
    call(Attribute_&)
        noexcept(std::is_nothrow_default_constructible_v<type>)
    {
        return type{};
    }
};

// Pass non-variant attributes as-is
template<class Parser, class Attribute, class Context>
struct pass_non_variant_attribute
{
    using type = Attribute&;

    [[nodiscard]] constexpr static Attribute&
    call(Attribute& attribute) noexcept
    {
        return attribute;
    }
};

// Unwrap single element sequences
template<class Parser, class Attribute, class Context>
    requires traits::is_size_one_sequence_v<Attribute>
struct pass_non_variant_attribute<Parser, Attribute, Context>
{
    using attr_type = typename std::remove_reference_t<
        typename fusion::result_of::front<Attribute>::type
    >;
    using pass = pass_parser_attribute<Parser, attr_type, Context>;
    using type = typename pass::type;

    template<class Attribute_>
    [[nodiscard]] static constexpr type
    call(Attribute_& attr)
        noexcept(noexcept(pass::call(fusion::front(attr))))
    {
        return pass::call(fusion::front(attr));
    }
};

template<class Parser, class Attribute, class Context>
    requires (!traits::is_variant_v<Attribute>)
struct pass_parser_attribute<Parser, Attribute, Context>
    : pass_non_variant_attribute<Parser, Attribute, Context>
{};

template<class Parser, class Context>
struct pass_parser_attribute<Parser, unused_type, Context>
    : pass_variant_unused
{};

template<class Parser, class Attribute, class Context>
struct pass_variant_attribute
    : std::conditional_t<
        traits::has_attribute_v<Parser, Context>,
        pass_parser_attribute<Parser, Attribute, Context>,
        pass_variant_unused
    >
{};

template<class L, class R, class Attribute, class Context>
struct pass_variant_attribute<alternative<L, R>, Attribute, Context>
    : std::conditional_t<
        traits::has_attribute_v<alternative<L, R>, Context>,
        pass_variant_used<Attribute>,
        pass_variant_unused
    >
{};

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attribute>
using parse_alternative_pseudo_t = traits::pseudo_attribute<
    Context, typename pass_variant_attribute<Parser, Attribute, Context>::type, It, Se
>;

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attribute>
constexpr bool is_reference_pseudo_type = std::is_reference_v<typename parse_alternative_pseudo_t<Parser, It, Se, Context, Attribute>::type>;

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attribute>
    requires is_reference_pseudo_type<Parser, It, Se, Context, Attribute>
[[nodiscard]] constexpr bool
parse_alternative(
    Parser const& p, It& first, Se const& last,
    Context const& context, Attribute& attribute
) noexcept(
    noexcept(parse_alternative_pseudo_t<Parser, It, Se, Context, Attribute>::call(
        first, last, pass_variant_attribute<Parser, Attribute, Context>::call(attribute)
    )) &&
    is_nothrow_parsable_v<
        Parser, It, Se, Context, std::remove_reference_t<typename parse_alternative_pseudo_t<Parser, It, Se, Context, Attribute>::type>
    >
)
{
    using pass = pass_variant_attribute<Parser, Attribute, Context>;
    using pseudo = traits::pseudo_attribute<Context, typename pass::type, It, Se>;
    typename pseudo::type attr_ = pseudo::call(first, last, pass::call(attribute));
    return p.parse(first, last, context, attr_);
}

template<class Parser, std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attribute>
    requires (!is_reference_pseudo_type<Parser, It, Se, Context, Attribute>)
[[nodiscard]] constexpr bool
parse_alternative(
    Parser const& p, It& first, Se const& last,
    Context const& context, Attribute& attribute
) noexcept(
    noexcept(parse_alternative_pseudo_t<Parser, It, Se, Context, Attribute>::call(
        first, last, pass_variant_attribute<Parser, Attribute, Context>::call(attribute)
    )) &&
    is_nothrow_parsable_v<
        Parser, It, Se, Context, std::remove_reference_t<typename parse_alternative_pseudo_t<Parser, It, Se, Context, Attribute>::type>
    > &&
    noexcept(x4::move_to(
        std::declval<typename parse_alternative_pseudo_t<Parser, It, Se, Context, Attribute>::type&&>(),
        attribute
    ))
)
{
    using pass = pass_variant_attribute<Parser, Attribute, Context>;
    using pseudo = traits::pseudo_attribute<Context, typename pass::type, It, Se>;
    typename pseudo::type attr_ = pseudo::call(first, last, pass::call(attribute));

    if (!p.parse(first, last, context, attr_)) return false;
    x4::move_to(std::move(attr_), attribute);
    return true;
}

template<class Subject>
struct alternative_helper : unary_parser<Subject, alternative_helper<Subject>>
{
    static constexpr bool is_pass_through_unary = true;

    using unary_parser<Subject, alternative_helper<Subject>>::unary_parser;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, class Attribute>
    [[nodiscard]] constexpr bool
    parse(It& first, Se const& last, Context const& context, Attribute& attr) const
        noexcept(noexcept(detail::parse_alternative(this->subject, first, last, context, attr)))
    {
        return detail::parse_alternative(this->subject, first, last, context, attr);
    }
};

template<class Left, class Right, class Context>
struct parse_into_container_impl<alternative<Left, Right>, Context>
{
    using parser_type = alternative<Left, Right>;

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Attribute>
        requires traits::is_variant_v<traits::container_value_t<Attribute>>
    [[nodiscard]] static constexpr bool
    call(
        parser_type const& parser,
        It& first, Se const& last, Context const& context, Attribute& attribute
    )
    {
        return detail::parse_into_container(alternative_helper<Left>{parser.left}, first, last, context, attribute)
            || detail::parse_into_container(alternative_helper<Right>{parser.right}, first, last, context, attribute);
    }

    template<std::forward_iterator It, std::sentinel_for<It> Se, class Attribute>
        requires (!traits::is_variant_v<traits::container_value_t<Attribute>>)
    [[nodiscard]] static constexpr bool
    call(
        parser_type const& parser,
        It& first, Se const& last,
        Context const& context, Attribute& attribute
    )
    {
        return detail::parse_into_container(parser.left, first, last, context, attribute)
            || detail::parse_into_container(parser.right, first, last, context, attribute);
    }
};

} // boost::spirit::x4::detail

#endif
