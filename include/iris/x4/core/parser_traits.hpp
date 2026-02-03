#ifndef IRIS_X4_CORE_PARSER_TRAITS_HPP
#define IRIS_X4_CORE_PARSER_TRAITS_HPP

#include <iris/config.hpp>

#include <type_traits>

#include <cstddef>

// If a user-provided program declares an explicit or partial
// specialization of any entity defined in this header, the
// program is ill-formed, no diagnostic required.

namespace boost::spirit::x4 {

struct unused_type;
struct unused_container_type;


namespace detail {

template<class Parser>
struct get_attribute_type
{
    using type = unused_type;
};

template<class Parser>
    requires
        requires { typename Parser::attribute_type; }
struct get_attribute_type<Parser>
{
    using type = Parser::attribute_type;
};

} // detail


template<class Parser>
struct has_attribute : std::bool_constant<
    !std::is_same_v<typename detail::get_attribute_type<Parser>::type, unused_type> &&
    !std::is_same_v<typename detail::get_attribute_type<Parser>::type, unused_container_type>
>
{};

template<class Parser>
inline constexpr bool has_attribute_v = has_attribute<Parser>::value;

template<class Parser>
    requires
        requires { Parser::has_attribute; }
struct has_attribute<Parser> : std::bool_constant<Parser::has_attribute>
{};


namespace detail {

template<class Parser>
struct get_sequence_size
{
    static constexpr std::size_t value = has_attribute<Parser>::value;
};

template<class Parser>
    requires
        requires { Parser::sequence_size; }
struct get_sequence_size<Parser>
{
    static constexpr std::size_t value = Parser::sequence_size;
};

} // detail


// A global facade exposing all traits recognized and used by the Spirit core.
//
// This class is not placed under the `traits/` directory because this is NOT
// a customization point.
//
// The purposes of this class are as follows:
//   - Provide some defaulted or automatically deduced characteristics for any
//     legitimate parser class, similar to `std::allocator_traits`.
//   - Gather all miscellaneous trait interfaces that were previously
//     scattered across the `traits/` headers.
//   - Make sure these core traits are not allowed to be specialized in user
//     applications.
template<class Parser>
struct parser_traits
{
    static_assert(!requires {
        Parser::is_pass_through_unary;
    }, "`::is_pass_through_unary` is obsolete. Derive from `x4::proxy_parser`.");

    using attribute_type = detail::get_attribute_type<Parser>::type;

    // Equivalent to `true` if and only if the deduced `attribute_type` is neither
    // `unused_type` nor `unused_container_type`.
    //
    // A user-defined parser class may define `::has_attribute` member explicitly
    // to bypass the type deduction for better compile times. If such member is
    // defined as a contradicting value, the program is ill-formed, no diagnostic
    // required.
    static constexpr bool has_attribute = x4::has_attribute<Parser>::value;

    static constexpr std::size_t sequence_size = detail::get_sequence_size<Parser>::value;

    static constexpr bool handles_container = Parser::handles_container;
    static constexpr bool has_action = Parser::has_action;
    static constexpr bool need_rcontext = Parser::need_rcontext;
};

} // boost::spirit::x4

#endif
