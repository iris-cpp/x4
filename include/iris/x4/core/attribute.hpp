#ifndef BOOST_SPIRIT_X4_CORE_ATTRIBUTE_HPP
#define BOOST_SPIRIT_X4_CORE_ATTRIBUTE_HPP

/*=============================================================================
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <boost/spirit/config.hpp>

#include <concepts>
#include <iterator>
#include <type_traits>

namespace boost::spirit::x4 {

struct unused_type;
struct unused_container_type;

namespace detail {

struct parser_base;

} // detail

template<class T>
concept X4UnusedAttribute =
    std::same_as<std::remove_const_t<T>, unused_type> ||
    std::same_as<std::remove_const_t<T>, unused_container_type>;

template<class T>
concept X4NonUnusedAttribute =
    !X4UnusedAttribute<T> &&
    std::is_object_v<T> && // implies not reference
    !std::is_base_of_v<detail::parser_base, std::remove_const_t<T>> &&
    std::move_constructible<std::remove_const_t<T>>;
    // TODO: `fusion::iterator_range` does not satisfy these due to `fusion::vector`'s iterator being a reference type
    //std::default_initializable<std::remove_const_t<T>> &&
    //std::assignable_from<std::remove_const_t<T>&, std::remove_const_t<T>>;

template<class T>
concept X4Attribute = X4UnusedAttribute<T> || X4NonUnusedAttribute<T>;

} // boost::spirit::x4

namespace boost::spirit::x4::traits {

// Pseudo attribute is a parser attribute whose actual type can only be determined at
// parse time. Such attribute is dependent on at least one of It/Se/Context.
template<std::forward_iterator It, std::sentinel_for<It> Se, class Context, class AttrRef>
struct pseudo_attribute
{
    static_assert(X4Attribute<std::remove_reference_t<AttrRef>>);
    static_assert(!std::is_rvalue_reference_v<AttrRef>);

    using actual_type = AttrRef;

    [[nodiscard]] static constexpr actual_type
    make_actual_type(It&, Se const&, Context const&, AttrRef&& attr_) noexcept  // NOLINT(cppcoreguidelines-rvalue-reference-param-not-moved)
    {
        return static_cast<AttrRef&&>(attr_);
    }
};

} // boost::spirit::x4::traits

#endif
