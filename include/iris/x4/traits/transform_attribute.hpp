#ifndef IRIS_X4_TRAITS_TRANSFORM_ATTRIBUTE_HPP
#define IRIS_X4_TRAITS_TRANSFORM_ATTRIBUTE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2012 Hartmut Kaiser
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/core/move_to.hpp>
#include <iris/x4/core/unused.hpp>

#include <type_traits>
#include <concepts>
#include <utility>

namespace iris::x4::traits {

template<class Transformed, class Exposed>
struct transform_attribute
{
    static_assert(!std::is_reference_v<Exposed>, "Exposed cannot be a reference type");
    static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");

    static_assert(std::assignable_from<Exposed&, Transformed&&>);

    using type = Transformed;

    [[nodiscard]] static constexpr Transformed pre(Exposed&)
        noexcept(std::is_nothrow_default_constructible_v<Transformed>)
    {
        return Transformed{};
    }

    template<class TransformedT>
    static constexpr void post(Exposed& val, TransformedT&& attr)
        noexcept(noexcept(x4::move_to(std::forward<TransformedT>(attr), val)))
    {
        x4::move_to(std::forward<TransformedT>(attr), val);
    }
};

template<class Transformed, class Exposed>
concept Transformable = requires(Exposed& val) {
    typename transform_attribute<Transformed, Exposed>::type;

    { transform_attribute<Transformed, Exposed>::pre(val) }
        -> std::same_as<typename transform_attribute<Transformed, Exposed>::type>;

    { transform_attribute<Transformed, Exposed>::post(val, std::declval<Transformed>()) };
};

// Same attribute types; no transformation needed
template<X4Attribute Attr>
    requires
        (!std::same_as<std::remove_const_t<Attr>, unused_type>) &&
        (!std::same_as<std::remove_const_t<Attr>, unused_container_type>)
struct transform_attribute<Attr, Attr>
{
    static_assert(!std::is_reference_v<Attr>, "Attribute cannot be a reference type");

    using type = Attr&;
    static constexpr Attr& pre(Attr& val) noexcept { return val; }
    static constexpr void post(Attr&, Attr const&) noexcept {}
};

template<>
struct transform_attribute<unused_type, unused_type>
{
    using type = unused_type;
    static constexpr unused_type pre(unused_type) noexcept { return unused; }
    static constexpr void post(unused_type, unused_type) noexcept {}
};

template<>
struct transform_attribute<unused_container_type, unused_container_type>
{
    using type = unused_container_type;
    static constexpr unused_container_type pre(unused_container_type) noexcept { return unused_container; }
    static constexpr void post(unused_container_type, unused_container_type) noexcept {}
};

template<class Transformed>
    requires (!std::same_as<Transformed, unused_type>)
struct transform_attribute<Transformed, unused_type>
    : transform_attribute<unused_type, unused_type>
{
    static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");
};

template<class Transformed>
struct transform_attribute<Transformed, unused_type const>
    : transform_attribute<unused_type, unused_type>
{
    static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");
};

template<class Transformed>
    requires (!std::same_as<Transformed, unused_container_type>)
struct transform_attribute<Transformed, unused_container_type>
    : transform_attribute<unused_container_type, unused_container_type>
{
    static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");
};

template<class Transformed>
struct transform_attribute<Transformed, unused_container_type const>
    : transform_attribute<unused_container_type, unused_container_type>
{
    static_assert(!std::is_reference_v<Transformed>, "Transformed cannot be a reference type");
};

} // iris::x4::traits

#endif
