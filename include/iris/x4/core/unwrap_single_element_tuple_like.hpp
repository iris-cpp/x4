#ifndef IRIS_X4_CORE_UNWRAP_SINGLE_ELEMENT_TUPLE_LIKE_HPP
#define IRIS_X4_CORE_UNWRAP_SINGLE_ELEMENT_TUPLE_LIKE_HPP

#include <iris/x4/traits/tuple_traits.hpp>

#include <utility>

namespace iris::x4 {

template<class T>
constexpr T&& unwrap_single_element_tuple_like(T&& t) noexcept
{
    return std::forward<T>(t);
}

template<class T>
    requires traits::is_single_element_tuple_like_v<std::remove_cvref_t<T>>
constexpr auto&& unwrap_single_element_tuple_like(T&& t)  // TODO: add noexcept
{
    return alloy::get<0>(std::forward<T>(t));
}

}

#endif
