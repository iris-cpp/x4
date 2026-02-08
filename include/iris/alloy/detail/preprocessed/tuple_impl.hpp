#ifndef IRIS_ALLOY_DETAIL_PREPROCESSED_TUPLE_IMPL_HPP
#define IRIS_ALLOY_DETAIL_PREPROCESSED_TUPLE_IMPL_HPP

/*=============================================================================
    Copyright (c) 2025 Yaito Kakeyama
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/config.hpp>

#include <iris/alloy/detail/forward_like_t.hpp>
#include <iris/alloy/detail/pack_indexing.hpp>
#include <iris/alloy/detail/tuple_comparison.hpp>

#include <iris/alloy/traits.hpp>

#include <type_traits>

#include <cstddef>

namespace iris::alloy {
template<class... Ts>
class tuple;
template<class... Ts, class... Us>
    requires detail::tuple_all_elements_have_equality_operator<tuple<Ts...>, tuple<Us...>>
constexpr bool operator==(tuple<Ts...> const&, tuple<Us...> const&)
    noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Ts...>, tuple<Us...>>);
namespace detail {
template<class... Ts>
class tuple_impl;
template<>
class tuple_impl<>
{
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const& a, tuple<Vs...> const& b)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    constexpr bool equal_to(tuple_impl const&) const noexcept { return true; }

public:
    tuple_impl() = default;
    tuple_impl(tuple_impl const&) = default;
    tuple_impl(tuple_impl&&) = default;
    constexpr tuple_impl(value_initialize_t) noexcept {}
};
template<class T0>
class tuple_impl<T0>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0>
    constexpr bool equal_to(tuple_impl<U0> const& other) const noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>>)
    {
        return _0 == other._0;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(std::conjunction_v<std::is_nothrow_default_constructible<T0>>) : _0{} {}
    template<class U0>
    constexpr explicit tuple_impl(U0&& u0) noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>>) : _0(static_cast<U0>(u0))
    {
    }
    template<class U0>
    constexpr explicit tuple_impl(tuple_impl<U0>& other) noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>>) : _0(other._0)
    {
    }
    template<class U0>
    constexpr explicit tuple_impl(tuple_impl<U0> const& other) noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&>>) : _0(other._0)
    {
    }
    template<class U0>
    constexpr explicit tuple_impl(tuple_impl<U0>&& other) noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>>)
        : _0(static_cast<decltype(other)>(other)._0)
    {
    }
    template<class U0>
    constexpr explicit tuple_impl(tuple_impl<U0> const&& other) noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other) noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>>)
    {
        _0 = other._0;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other) noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        return *this;
    }
    template<class U0>
    constexpr tuple_impl& operator=(tuple_impl<U0> const& other) noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>>)
    {
        _0 = other._0;
        return *this;
    }
    template<class U0>
    constexpr tuple_impl& operator=(tuple_impl<U0>&& other) noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>>);
        using std::swap;
        swap(_0, other._0);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
    }
};
template<class T0, class T1>
class tuple_impl<T0, T1>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1>
    constexpr bool equal_to(tuple_impl<U0, U1> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>>)
    {
        return _0 == other._0 && _1 == other._1;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t)
        noexcept(std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>>)
        : _0{}, _1{}
    {
    }
    template<class U0, class U1>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1) noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1))
    {
    }
    template<class U0, class U1>
    constexpr explicit tuple_impl(tuple_impl<U0, U1>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>>)
        : _0(other._0), _1(other._1)
    {
    }
    template<class U0, class U1>
    constexpr explicit tuple_impl(tuple_impl<U0, U1> const& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>>)
        : _0(other._0), _1(other._1)
    {
    }
    template<class U0, class U1>
    constexpr explicit tuple_impl(tuple_impl<U0, U1>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1)
    {
    }
    template<class U0, class U1>
    constexpr explicit tuple_impl(tuple_impl<U0, U1> const&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>>)
    {
        _0 = other._0;
        _1 = other._1;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other) noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        return *this;
    }
    template<class U0, class U1>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1> const& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        return *this;
    }
    template<class U0, class U1>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
    }
};
template<class T0, class T1, class T2>
class tuple_impl<T0, T1, T2>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2>
    constexpr bool equal_to(tuple_impl<U0, U1, U2> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>>)
        : _0{}, _1{}, _2{}
    {
    }
    template<class U0, class U1, class U2>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2))
    {
    }
    template<class U0, class U1, class U2>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>>)
        : _0(other._0), _1(other._1), _2(other._2)
    {
    }
    template<class U0, class U1, class U2>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2> const& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>,
                                    std::is_nothrow_constructible<T2, U2 const&>>)
        : _0(other._0), _1(other._1), _2(other._2)
    {
    }
    template<class U0, class U1, class U2>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2)
    {
    }
    template<class U0, class U1, class U2>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2> const&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>,
                                    std::is_nothrow_constructible<T2, U2 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        return *this;
    }
    template<class U0, class U1, class U2>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2> const& other) noexcept(
        std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        return *this;
    }
    template<class U0, class U1, class U2>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
    }
};
template<class T0, class T1, class T2, class T3>
class tuple_impl<T0, T1, T2, T3>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t)
        noexcept(std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>,
                                    std::is_nothrow_default_constructible<T2>, std::is_nothrow_default_constructible<T3>>)
        : _0{}, _1{}, _2{}, _3{}
    {
    }
    template<class U0, class U1, class U2, class U3>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3))
    {
    }
    template<class U0, class U1, class U2, class U3>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3)
    {
    }
    template<class U0, class U1, class U2, class U3>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3> const& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>,
                                    std::is_nothrow_constructible<T2, U2 const&>, std::is_nothrow_constructible<T3, U3 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3)
    {
    }
    template<class U0, class U1, class U2, class U3>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                                    std::is_nothrow_constructible<T3, U3&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3)
    {
    }
    template<class U0, class U1, class U2, class U3>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3> const&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>,
                                    std::is_nothrow_constructible<T2, U2 const&&>, std::is_nothrow_constructible<T3, U3 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other) noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>,
                                                                                    std::is_nothrow_move_assignable<T2>, std::is_nothrow_move_assignable<T3>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        return *this;
    }
    template<class U0, class U1, class U2, class U3>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3> const& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>,
                                    std::is_nothrow_assignable<T2&, U2 const&>, std::is_nothrow_assignable<T3&, U3 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        return *this;
    }
    template<class U0, class U1, class U2, class U3>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
    }
};
template<class T0, class T1, class T2, class T3, class T4>
class tuple_impl<T0, T1, T2, T3, T4>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4> const& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>,
                                    std::is_nothrow_constructible<T2, U2 const&>, std::is_nothrow_constructible<T3, U3 const&>,
                                    std::is_nothrow_constructible<T4, U4 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                                    std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4> const&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>,
                                    std::is_nothrow_constructible<T2, U2 const&&>, std::is_nothrow_constructible<T3, U3 const&&>,
                                    std::is_nothrow_constructible<T4, U4 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4> const& other) noexcept(
        std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                           std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                                    std::is_nothrow_swappable<T4>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5>
class tuple_impl<T0, T1, T2, T3, T4, T5>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5> const& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>,
                                    std::is_nothrow_constructible<T2, U2 const&>, std::is_nothrow_constructible<T3, U3 const&>,
                                    std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                                    std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5> const&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>,
                                    std::is_nothrow_constructible<T2, U2 const&&>, std::is_nothrow_constructible<T3, U3 const&&>,
                                    std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5> const& other) noexcept(
        std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                           std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                                    std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                                    is_nothrow_equality_comparable<T6, U6>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                                    std::is_nothrow_constructible<T6, U6&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6> const& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>,
                                    std::is_nothrow_constructible<T2, U2 const&>, std::is_nothrow_constructible<T3, U3 const&>,
                                    std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                                    std::is_nothrow_constructible<T6, U6 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                                    std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                                    std::is_nothrow_constructible<T6, U6&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6> const&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>,
                                    std::is_nothrow_constructible<T2, U2 const&&>, std::is_nothrow_constructible<T3, U3 const&&>,
                                    std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                                    std::is_nothrow_constructible<T6, U6 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6> const& other) noexcept(
        std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                           std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                           std::is_nothrow_assignable<T6&, U6 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                                    std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                                    is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                                    std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7> const& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>,
                                    std::is_nothrow_constructible<T2, U2 const&>, std::is_nothrow_constructible<T3, U3 const&>,
                                    std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                                    std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                                    std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                                    std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7> const&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>,
                                    std::is_nothrow_constructible<T2, U2 const&&>, std::is_nothrow_constructible<T3, U3 const&&>,
                                    std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                                    std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7> const& other) noexcept(
        std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                           std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                           std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                                    std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                                    is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                                    std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                                    std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                                    std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8> const&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                 std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                 std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8> const& other) noexcept(
        std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                           std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                           std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                                    std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                                    std::is_nothrow_swappable<T8>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                                    is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                                    is_nothrow_equality_comparable<T9, U9>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                                    std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                                    std::is_nothrow_constructible<T9, U9&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9> const& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>,
                                    std::is_nothrow_constructible<T2, U2 const&>, std::is_nothrow_constructible<T3, U3 const&>,
                                    std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                                    std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>,
                                    std::is_nothrow_constructible<T8, U8 const&>, std::is_nothrow_constructible<T9, U9 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                                    std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                                    std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                                    std::is_nothrow_constructible<T9, U9&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9> const&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>,
                                    std::is_nothrow_constructible<T2, U2 const&&>, std::is_nothrow_constructible<T3, U3 const&&>,
                                    std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                                    std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>,
                                    std::is_nothrow_constructible<T8, U8 const&&>, std::is_nothrow_constructible<T9, U9 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9> const& other) noexcept(
        std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                           std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                           std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                           std::is_nothrow_assignable<T9&, U9 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                                    std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                                    std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                                    is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                                    is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                                    std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                                    std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                                    std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                                    std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                                    std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10> const&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                 std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                 std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                 std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10> const& other) noexcept(
        std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                           std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                           std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                           std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                                    std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                                    std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                                    is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                                    is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                                    std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                                    std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11>&& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                           std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                           std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                           std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11> const&& other) noexcept(
        std::conjunction_v<
            std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
            std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
            std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
            std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                                    is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                                    is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                                    is_nothrow_equality_comparable<T12, U12>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                                    std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                                    std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                                    std::is_nothrow_constructible<T12, U12&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                                    std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                                    std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                                    std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>,
                                    std::is_nothrow_constructible<T11, U11&&>, std::is_nothrow_constructible<T12, U12&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12> const&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                 std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                 std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                 std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>,
                 std::is_nothrow_constructible<T11, U11 const&&>, std::is_nothrow_constructible<T12, U12 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12> const& other) noexcept(
        std::conjunction_v<std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                           std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                           std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                           std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>,
                           std::is_nothrow_assignable<T11&, U11 const&>, std::is_nothrow_assignable<T12&, U12 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                                    std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                                    std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>,
                                    std::is_nothrow_swappable<T11>, std::is_nothrow_swappable<T12>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                                    is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                                    is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                                    is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                                    std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                                    std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                                    std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13>&& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                           std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                           std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                           std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                           std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13> const&& other) noexcept(
        std::conjunction_v<
            std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
            std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
            std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
            std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
            std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                                    std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                                    std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>,
                                    std::is_nothrow_swappable<T11>, std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14> const& other) const noexcept(
        std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                           is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                           is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                           is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                           is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14>& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                           std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                           std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                           std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                           std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14> const& other) noexcept(
        std::conjunction_v<
            std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
            std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
            std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
            std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
            std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14>&& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                           std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                           std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                           std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                           std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14> const&& other) noexcept(
        std::conjunction_v<
            std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
            std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
            std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
            std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
            std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>>)
    {
        static_assert(
            std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                               std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                               std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15> const& other) const
        noexcept(std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                                    is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                                    is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                                    is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                                    is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>,
                                    is_nothrow_equality_comparable<T14, U14>, is_nothrow_equality_comparable<T15, U15>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15>& other)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                                    std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                                    std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                                    std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                                    std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>,
                                    std::is_nothrow_constructible<T14, U14&>, std::is_nothrow_constructible<T15, U15&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15>&& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                           std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                           std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                           std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                           std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                           std::is_nothrow_constructible<T15, U15&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15> const&& other) noexcept(
        std::conjunction_v<
            std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
            std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
            std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
            std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
            std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
            std::is_nothrow_constructible<T15, U15 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16> const& other) const noexcept(
        std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                           is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                           is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                           is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                           is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                           is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16>& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                           std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                           std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                           std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                           std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                           std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16>&& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                           std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                           std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                           std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                           std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                           std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16> const&& other) noexcept(
        std::conjunction_v<
            std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
            std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
            std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
            std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
            std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
            std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other)
        noexcept(std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                                    std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                                    std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>,
                                    std::is_nothrow_swappable<T11>, std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>,
                                    std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>, std::is_nothrow_swappable<T16>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17> const& other) const noexcept(
        std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                           is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                           is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                           is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                           is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                           is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17>& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                           std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                           std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                           std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                           std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                           std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17> const& other) noexcept(
        std::conjunction_v<
            std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
            std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
            std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
            std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
            std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
            std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17>&& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                           std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                           std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                           std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                           std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                           std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17> const&& other) noexcept(
        std::conjunction_v<
            std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
            std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
            std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
            std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
            std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
            std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>, std::is_nothrow_constructible<T17, U17 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18> const& other) const noexcept(
        std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                           is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                           is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                           is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                           is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                           is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                           is_nothrow_equality_comparable<T18, U18>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18>& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                           std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                           std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                           std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                           std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                           std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                           std::is_nothrow_constructible<T18, U18&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                 std::is_nothrow_constructible<T18, U18 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18>&& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                           std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                           std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                           std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                           std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                           std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                           std::is_nothrow_constructible<T18, U18&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18> const&& other) noexcept(
        std::conjunction_v<
            std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
            std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
            std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
            std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
            std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
            std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>, std::is_nothrow_constructible<T17, U17 const&&>,
            std::is_nothrow_constructible<T18, U18 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19> const& other) const noexcept(
        std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                           is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                           is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                           is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                           is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                           is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                           is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19>& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                           std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                           std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                           std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                           std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                           std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                           std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                 std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19>&& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                           std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                           std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                           std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                           std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                           std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                           std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19> const&& other) noexcept(
        std::conjunction_v<
            std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
            std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
            std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
            std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
            std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
            std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>, std::is_nothrow_constructible<T17, U17 const&&>,
            std::is_nothrow_constructible<T18, U18 const&&>, std::is_nothrow_constructible<T19, U19 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>>)
    {
        static_assert(
            std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                               std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                               std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                               std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19> const& get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19> const&& get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20>& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                           std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                           std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                           std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                           std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                           std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                           std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20> const& other)
        noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20>&& other) noexcept(
        std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                           std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                           std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                           std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                           std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                           std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                           std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20> const&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                 std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                 std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                 std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>,
                 std::is_nothrow_constructible<T11, U11 const&&>, std::is_nothrow_constructible<T12, U12 const&&>,
                 std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
                 std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>,
                 std::is_nothrow_constructible<T17, U17 const&&>, std::is_nothrow_constructible<T18, U18 const&&>,
                 std::is_nothrow_constructible<T19, U19 const&&>, std::is_nothrow_constructible<T20, U20 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                                         std::is_swappable<T20>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                               is_nothrow_equality_comparable<T21, U21>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                           std::is_nothrow_default_constructible<T21>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                 std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                 std::is_nothrow_constructible<T21, U21 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21>&& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                               std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                               std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                               std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                               std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                               std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                               std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                               std::is_nothrow_constructible<T21, U21&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21> const&& other)
        noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
                std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>,
                std::is_nothrow_constructible<T14, U14 const&&>, std::is_nothrow_constructible<T15, U15 const&&>,
                std::is_nothrow_constructible<T16, U16 const&&>, std::is_nothrow_constructible<T17, U17 const&&>,
                std::is_nothrow_constructible<T18, U18 const&&>, std::is_nothrow_constructible<T19, U19 const&&>,
                std::is_nothrow_constructible<T20, U20 const&&>, std::is_nothrow_constructible<T21, U21 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>,
                 std::is_nothrow_assignable<T21&, U21 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        _21 = alloy::get<21>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                                         std::is_swappable<T20>, std::is_swappable<T21>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21>&& get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22>
    constexpr bool
    equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22> const& other) const noexcept(
        std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                           is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                           is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                           is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                           is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                           is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                           is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                           is_nothrow_equality_comparable<T21, U21>, is_nothrow_equality_comparable<T22, U22>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21 &&
               _22 == other._22;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    IRIS_NO_UNIQUE_ADDRESS T22 _22;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                           std::is_nothrow_default_constructible<T21>, std::is_nothrow_default_constructible<T22>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}, _22{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>, std::is_nothrow_constructible<T22, U22>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21)), _22(static_cast<U22>(u22))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>, std::is_nothrow_constructible<T22, U22&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                 std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                 std::is_nothrow_constructible<T21, U21 const&>, std::is_nothrow_constructible<T22, U22 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22>&& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                               std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                               std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                               std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                               std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                               std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                               std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                               std::is_nothrow_constructible<T21, U21&&>, std::is_nothrow_constructible<T22, U22&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22> const&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                 std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                 std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                 std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>,
                 std::is_nothrow_constructible<T11, U11 const&&>, std::is_nothrow_constructible<T12, U12 const&&>,
                 std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
                 std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>,
                 std::is_nothrow_constructible<T17, U17 const&&>, std::is_nothrow_constructible<T18, U18 const&&>,
                 std::is_nothrow_constructible<T19, U19 const&&>, std::is_nothrow_constructible<T20, U20 const&&>,
                 std::is_nothrow_constructible<T21, U21 const&&>, std::is_nothrow_constructible<T22, U22 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>, std::is_nothrow_copy_assignable<T22>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>, std::is_nothrow_move_assignable<T22>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22>
    constexpr tuple_impl&
    operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>,
                 std::is_nothrow_assignable<T21&, U21 const&>, std::is_nothrow_assignable<T22&, U22 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        _21 = alloy::get<21>(static_cast<decltype(other)>(other));
        _22 = alloy::get<22>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>, std::is_nothrow_swappable<T22>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                                         std::is_swappable<T20>, std::is_swappable<T21>, std::is_swappable<T22>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
        swap(_22, other._22);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22>& get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22>&&
    get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22)&&>(_22);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22) const&&>(_22);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23>
    constexpr bool
    equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                               is_nothrow_equality_comparable<T21, U21>, is_nothrow_equality_comparable<T22, U22>, is_nothrow_equality_comparable<T23, U23>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21 &&
               _22 == other._22 && _23 == other._23;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    IRIS_NO_UNIQUE_ADDRESS T22 _22;
    IRIS_NO_UNIQUE_ADDRESS T23 _23;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                           std::is_nothrow_default_constructible<T21>, std::is_nothrow_default_constructible<T22>, std::is_nothrow_default_constructible<T23>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}, _22{},
          _23{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22, U23&& u23)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>, std::is_nothrow_constructible<T22, U22>, std::is_nothrow_constructible<T23, U23>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21)), _22(static_cast<U22>(u22)), _23(static_cast<U23>(u23))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>, std::is_nothrow_constructible<T22, U22&>, std::is_nothrow_constructible<T23, U23&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23> const& other)
        noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                std::is_nothrow_constructible<T21, U21 const&>, std::is_nothrow_constructible<T22, U22 const&>, std::is_nothrow_constructible<T23, U23 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23>&& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                               std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                               std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                               std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                               std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                               std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                               std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                               std::is_nothrow_constructible<T21, U21&&>, std::is_nothrow_constructible<T22, U22&&>, std::is_nothrow_constructible<T23, U23&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23> const&& other)
        noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
                std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>,
                std::is_nothrow_constructible<T14, U14 const&&>, std::is_nothrow_constructible<T15, U15 const&&>,
                std::is_nothrow_constructible<T16, U16 const&&>, std::is_nothrow_constructible<T17, U17 const&&>,
                std::is_nothrow_constructible<T18, U18 const&&>, std::is_nothrow_constructible<T19, U19 const&&>,
                std::is_nothrow_constructible<T20, U20 const&&>, std::is_nothrow_constructible<T21, U21 const&&>,
                std::is_nothrow_constructible<T22, U22 const&&>, std::is_nothrow_constructible<T23, U23 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>, std::is_nothrow_copy_assignable<T22>, std::is_nothrow_copy_assignable<T23>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>, std::is_nothrow_move_assignable<T22>, std::is_nothrow_move_assignable<T23>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23>
    constexpr tuple_impl&
    operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>,
                 std::is_nothrow_assignable<T21&, U21 const&>, std::is_nothrow_assignable<T22&, U22 const&>, std::is_nothrow_assignable<T23&, U23 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23>
    constexpr tuple_impl&
    operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>, std::is_nothrow_assignable<T23&, U23&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        _21 = alloy::get<21>(static_cast<decltype(other)>(other));
        _22 = alloy::get<22>(static_cast<decltype(other)>(other));
        _23 = alloy::get<23>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>, std::is_nothrow_swappable<T22>, std::is_nothrow_swappable<T23>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                                         std::is_swappable<T20>, std::is_swappable<T21>, std::is_swappable<T22>, std::is_swappable<T23>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
        swap(_22, other._22);
        swap(_23, other._23);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23>&
    get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23>&&
    get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22)&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23)&&>(_23);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22) const&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23) const&&>(_23);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24>
    constexpr bool
    equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                               is_nothrow_equality_comparable<T21, U21>, is_nothrow_equality_comparable<T22, U22>, is_nothrow_equality_comparable<T23, U23>,
                               is_nothrow_equality_comparable<T24, U24>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21 &&
               _22 == other._22 && _23 == other._23 && _24 == other._24;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    IRIS_NO_UNIQUE_ADDRESS T22 _22;
    IRIS_NO_UNIQUE_ADDRESS T23 _23;
    IRIS_NO_UNIQUE_ADDRESS T24 _24;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                           std::is_nothrow_default_constructible<T21>, std::is_nothrow_default_constructible<T22>, std::is_nothrow_default_constructible<T23>,
                           std::is_nothrow_default_constructible<T24>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}, _22{},
          _23{}, _24{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22, U23&& u23,
                                  U24&& u24)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>, std::is_nothrow_constructible<T22, U22>, std::is_nothrow_constructible<T23, U23>,
                                    std::is_nothrow_constructible<T24, U24>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21)), _22(static_cast<U22>(u22)), _23(static_cast<U23>(u23)), _24(static_cast<U24>(u24))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>, std::is_nothrow_constructible<T22, U22&>, std::is_nothrow_constructible<T23, U23&>,
                               std::is_nothrow_constructible<T24, U24&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                 std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                 std::is_nothrow_constructible<T21, U21 const&>, std::is_nothrow_constructible<T22, U22 const&>, std::is_nothrow_constructible<T23, U23 const&>,
                 std::is_nothrow_constructible<T24, U24 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24>&& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                               std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                               std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                               std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                               std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                               std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                               std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                               std::is_nothrow_constructible<T21, U21&&>, std::is_nothrow_constructible<T22, U22&&>, std::is_nothrow_constructible<T23, U23&&>,
                               std::is_nothrow_constructible<T24, U24&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24> const&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                 std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                 std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                 std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>,
                 std::is_nothrow_constructible<T11, U11 const&&>, std::is_nothrow_constructible<T12, U12 const&&>,
                 std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
                 std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>,
                 std::is_nothrow_constructible<T17, U17 const&&>, std::is_nothrow_constructible<T18, U18 const&&>,
                 std::is_nothrow_constructible<T19, U19 const&&>, std::is_nothrow_constructible<T20, U20 const&&>,
                 std::is_nothrow_constructible<T21, U21 const&&>, std::is_nothrow_constructible<T22, U22 const&&>,
                 std::is_nothrow_constructible<T23, U23 const&&>, std::is_nothrow_constructible<T24, U24 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>, std::is_nothrow_copy_assignable<T22>, std::is_nothrow_copy_assignable<T23>,
                                    std::is_nothrow_copy_assignable<T24>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>, std::is_nothrow_move_assignable<T22>, std::is_nothrow_move_assignable<T23>,
                                    std::is_nothrow_move_assignable<T24>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24>
    constexpr tuple_impl&
    operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>,
                 std::is_nothrow_assignable<T21&, U21 const&>, std::is_nothrow_assignable<T22&, U22 const&>, std::is_nothrow_assignable<T23&, U23 const&>,
                 std::is_nothrow_assignable<T24&, U24 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24>
    constexpr tuple_impl&
    operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>, std::is_nothrow_assignable<T23&, U23&&>,
                                    std::is_nothrow_assignable<T24&, U24&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        _21 = alloy::get<21>(static_cast<decltype(other)>(other));
        _22 = alloy::get<22>(static_cast<decltype(other)>(other));
        _23 = alloy::get<23>(static_cast<decltype(other)>(other));
        _24 = alloy::get<24>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>, std::is_nothrow_swappable<T22>, std::is_nothrow_swappable<T23>,
                           std::is_nothrow_swappable<T24>>)
    {
        static_assert(
            std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                               std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                               std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                               std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                               std::is_swappable<T20>, std::is_swappable<T21>, std::is_swappable<T22>, std::is_swappable<T23>, std::is_swappable<T24>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
        swap(_22, other._22);
        swap(_23, other._23);
        swap(_24, other._24);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24>&
    get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24>&&
    get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22)&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23)&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24)&&>(_24);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22) const&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23) const&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24) const&&>(_24);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25>
    constexpr bool equal_to(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                               is_nothrow_equality_comparable<T21, U21>, is_nothrow_equality_comparable<T22, U22>, is_nothrow_equality_comparable<T23, U23>,
                               is_nothrow_equality_comparable<T24, U24>, is_nothrow_equality_comparable<T25, U25>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21 &&
               _22 == other._22 && _23 == other._23 && _24 == other._24 && _25 == other._25;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    IRIS_NO_UNIQUE_ADDRESS T22 _22;
    IRIS_NO_UNIQUE_ADDRESS T23 _23;
    IRIS_NO_UNIQUE_ADDRESS T24 _24;
    IRIS_NO_UNIQUE_ADDRESS T25 _25;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                           std::is_nothrow_default_constructible<T21>, std::is_nothrow_default_constructible<T22>, std::is_nothrow_default_constructible<T23>,
                           std::is_nothrow_default_constructible<T24>, std::is_nothrow_default_constructible<T25>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}, _22{},
          _23{}, _24{}, _25{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22, U23&& u23,
                                  U24&& u24, U25&& u25)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>, std::is_nothrow_constructible<T22, U22>, std::is_nothrow_constructible<T23, U23>,
                                    std::is_nothrow_constructible<T24, U24>, std::is_nothrow_constructible<T25, U25>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21)), _22(static_cast<U22>(u22)), _23(static_cast<U23>(u23)), _24(static_cast<U24>(u24)), _25(static_cast<U25>(u25))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>, std::is_nothrow_constructible<T22, U22&>, std::is_nothrow_constructible<T23, U23&>,
                               std::is_nothrow_constructible<T24, U24&>, std::is_nothrow_constructible<T25, U25&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                 std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                 std::is_nothrow_constructible<T21, U21 const&>, std::is_nothrow_constructible<T22, U22 const&>, std::is_nothrow_constructible<T23, U23 const&>,
                 std::is_nothrow_constructible<T24, U24 const&>, std::is_nothrow_constructible<T25, U25 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25>&& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                               std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                               std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                               std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                               std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                               std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                               std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                               std::is_nothrow_constructible<T21, U21&&>, std::is_nothrow_constructible<T22, U22&&>, std::is_nothrow_constructible<T23, U23&&>,
                               std::is_nothrow_constructible<T24, U24&&>, std::is_nothrow_constructible<T25, U25&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25> const&& other)
        noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
                std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>,
                std::is_nothrow_constructible<T14, U14 const&&>, std::is_nothrow_constructible<T15, U15 const&&>,
                std::is_nothrow_constructible<T16, U16 const&&>, std::is_nothrow_constructible<T17, U17 const&&>,
                std::is_nothrow_constructible<T18, U18 const&&>, std::is_nothrow_constructible<T19, U19 const&&>,
                std::is_nothrow_constructible<T20, U20 const&&>, std::is_nothrow_constructible<T21, U21 const&&>,
                std::is_nothrow_constructible<T22, U22 const&&>, std::is_nothrow_constructible<T23, U23 const&&>,
                std::is_nothrow_constructible<T24, U24 const&&>, std::is_nothrow_constructible<T25, U25 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>, std::is_nothrow_copy_assignable<T22>, std::is_nothrow_copy_assignable<T23>,
                                    std::is_nothrow_copy_assignable<T24>, std::is_nothrow_copy_assignable<T25>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>, std::is_nothrow_move_assignable<T22>, std::is_nothrow_move_assignable<T23>,
                                    std::is_nothrow_move_assignable<T24>, std::is_nothrow_move_assignable<T25>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25>
    constexpr tuple_impl&
    operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>,
                 std::is_nothrow_assignable<T21&, U21 const&>, std::is_nothrow_assignable<T22&, U22 const&>, std::is_nothrow_assignable<T23&, U23 const&>,
                 std::is_nothrow_assignable<T24&, U24 const&>, std::is_nothrow_assignable<T25&, U25 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25>
    constexpr tuple_impl&
    operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>, std::is_nothrow_assignable<T23&, U23&&>,
                                    std::is_nothrow_assignable<T24&, U24&&>, std::is_nothrow_assignable<T25&, U25&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        _21 = alloy::get<21>(static_cast<decltype(other)>(other));
        _22 = alloy::get<22>(static_cast<decltype(other)>(other));
        _23 = alloy::get<23>(static_cast<decltype(other)>(other));
        _24 = alloy::get<24>(static_cast<decltype(other)>(other));
        _25 = alloy::get<25>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>, std::is_nothrow_swappable<T22>, std::is_nothrow_swappable<T23>,
                           std::is_nothrow_swappable<T24>, std::is_nothrow_swappable<T25>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                                         std::is_swappable<T20>, std::is_swappable<T21>, std::is_swappable<T22>, std::is_swappable<T23>, std::is_swappable<T24>,
                                         std::is_swappable<T25>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
        swap(_22, other._22);
        swap(_23, other._23);
        swap(_24, other._24);
        swap(_25, other._25);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25>&
    get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24,
                                   T25> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25>&&
    get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22)&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23)&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24)&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25)&&>(_25);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24,
                                   T25> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22) const&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23) const&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24) const&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25) const&&>(_25);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25,
                                       U26> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                               is_nothrow_equality_comparable<T21, U21>, is_nothrow_equality_comparable<T22, U22>, is_nothrow_equality_comparable<T23, U23>,
                               is_nothrow_equality_comparable<T24, U24>, is_nothrow_equality_comparable<T25, U25>, is_nothrow_equality_comparable<T26, U26>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21 &&
               _22 == other._22 && _23 == other._23 && _24 == other._24 && _25 == other._25 && _26 == other._26;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    IRIS_NO_UNIQUE_ADDRESS T22 _22;
    IRIS_NO_UNIQUE_ADDRESS T23 _23;
    IRIS_NO_UNIQUE_ADDRESS T24 _24;
    IRIS_NO_UNIQUE_ADDRESS T25 _25;
    IRIS_NO_UNIQUE_ADDRESS T26 _26;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                           std::is_nothrow_default_constructible<T21>, std::is_nothrow_default_constructible<T22>, std::is_nothrow_default_constructible<T23>,
                           std::is_nothrow_default_constructible<T24>, std::is_nothrow_default_constructible<T25>, std::is_nothrow_default_constructible<T26>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}, _22{},
          _23{}, _24{}, _25{}, _26{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22, U23&& u23,
                                  U24&& u24, U25&& u25, U26&& u26)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>, std::is_nothrow_constructible<T22, U22>, std::is_nothrow_constructible<T23, U23>,
                                    std::is_nothrow_constructible<T24, U24>, std::is_nothrow_constructible<T25, U25>, std::is_nothrow_constructible<T26, U26>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21)), _22(static_cast<U22>(u22)), _23(static_cast<U23>(u23)), _24(static_cast<U24>(u24)), _25(static_cast<U25>(u25)),
          _26(static_cast<U26>(u26))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>, std::is_nothrow_constructible<T22, U22&>, std::is_nothrow_constructible<T23, U23&>,
                               std::is_nothrow_constructible<T24, U24&>, std::is_nothrow_constructible<T25, U25&>, std::is_nothrow_constructible<T26, U26&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26> const& other)
        noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                std::is_nothrow_constructible<T21, U21 const&>, std::is_nothrow_constructible<T22, U22 const&>, std::is_nothrow_constructible<T23, U23 const&>,
                std::is_nothrow_constructible<T24, U24 const&>, std::is_nothrow_constructible<T25, U25 const&>, std::is_nothrow_constructible<T26, U26 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26>&& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                               std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                               std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                               std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                               std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                               std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                               std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                               std::is_nothrow_constructible<T21, U21&&>, std::is_nothrow_constructible<T22, U22&&>, std::is_nothrow_constructible<T23, U23&&>,
                               std::is_nothrow_constructible<T24, U24&&>, std::is_nothrow_constructible<T25, U25&&>, std::is_nothrow_constructible<T26, U26&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26> const&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                 std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                 std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                 std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>,
                 std::is_nothrow_constructible<T11, U11 const&&>, std::is_nothrow_constructible<T12, U12 const&&>,
                 std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
                 std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>,
                 std::is_nothrow_constructible<T17, U17 const&&>, std::is_nothrow_constructible<T18, U18 const&&>,
                 std::is_nothrow_constructible<T19, U19 const&&>, std::is_nothrow_constructible<T20, U20 const&&>,
                 std::is_nothrow_constructible<T21, U21 const&&>, std::is_nothrow_constructible<T22, U22 const&&>,
                 std::is_nothrow_constructible<T23, U23 const&&>, std::is_nothrow_constructible<T24, U24 const&&>,
                 std::is_nothrow_constructible<T25, U25 const&&>, std::is_nothrow_constructible<T26, U26 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>, std::is_nothrow_copy_assignable<T22>, std::is_nothrow_copy_assignable<T23>,
                                    std::is_nothrow_copy_assignable<T24>, std::is_nothrow_copy_assignable<T25>, std::is_nothrow_copy_assignable<T26>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>, std::is_nothrow_move_assignable<T22>, std::is_nothrow_move_assignable<T23>,
                                    std::is_nothrow_move_assignable<T24>, std::is_nothrow_move_assignable<T25>, std::is_nothrow_move_assignable<T26>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26>
    constexpr tuple_impl& operator=(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>,
                 std::is_nothrow_assignable<T21&, U21 const&>, std::is_nothrow_assignable<T22&, U22 const&>, std::is_nothrow_assignable<T23&, U23 const&>,
                 std::is_nothrow_assignable<T24&, U24 const&>, std::is_nothrow_assignable<T25&, U25 const&>, std::is_nothrow_assignable<T26&, U26 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26>
    constexpr tuple_impl&
    operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>, std::is_nothrow_assignable<T23&, U23&&>,
                                    std::is_nothrow_assignable<T24&, U24&&>, std::is_nothrow_assignable<T25&, U25&&>, std::is_nothrow_assignable<T26&, U26&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        _21 = alloy::get<21>(static_cast<decltype(other)>(other));
        _22 = alloy::get<22>(static_cast<decltype(other)>(other));
        _23 = alloy::get<23>(static_cast<decltype(other)>(other));
        _24 = alloy::get<24>(static_cast<decltype(other)>(other));
        _25 = alloy::get<25>(static_cast<decltype(other)>(other));
        _26 = alloy::get<26>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>, std::is_nothrow_swappable<T22>, std::is_nothrow_swappable<T23>,
                           std::is_nothrow_swappable<T24>, std::is_nothrow_swappable<T25>, std::is_nothrow_swappable<T26>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                                         std::is_swappable<T20>, std::is_swappable<T21>, std::is_swappable<T22>, std::is_swappable<T23>, std::is_swappable<T24>,
                                         std::is_swappable<T25>, std::is_swappable<T26>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
        swap(_22, other._22);
        swap(_23, other._23);
        swap(_24, other._24);
        swap(_25, other._25);
        swap(_26, other._26);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26>&
    get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26>&&
    get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22)&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23)&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24)&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25)&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26)&&>(_26);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22) const&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23) const&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24) const&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25) const&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26) const&&>(_26);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26,
         class T27>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25,
                                       U26, U27> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                               is_nothrow_equality_comparable<T21, U21>, is_nothrow_equality_comparable<T22, U22>, is_nothrow_equality_comparable<T23, U23>,
                               is_nothrow_equality_comparable<T24, U24>, is_nothrow_equality_comparable<T25, U25>, is_nothrow_equality_comparable<T26, U26>,
                               is_nothrow_equality_comparable<T27, U27>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21 &&
               _22 == other._22 && _23 == other._23 && _24 == other._24 && _25 == other._25 && _26 == other._26 && _27 == other._27;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    IRIS_NO_UNIQUE_ADDRESS T22 _22;
    IRIS_NO_UNIQUE_ADDRESS T23 _23;
    IRIS_NO_UNIQUE_ADDRESS T24 _24;
    IRIS_NO_UNIQUE_ADDRESS T25 _25;
    IRIS_NO_UNIQUE_ADDRESS T26 _26;
    IRIS_NO_UNIQUE_ADDRESS T27 _27;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                           std::is_nothrow_default_constructible<T21>, std::is_nothrow_default_constructible<T22>, std::is_nothrow_default_constructible<T23>,
                           std::is_nothrow_default_constructible<T24>, std::is_nothrow_default_constructible<T25>, std::is_nothrow_default_constructible<T26>,
                           std::is_nothrow_default_constructible<T27>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}, _22{},
          _23{}, _24{}, _25{}, _26{}, _27{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22, U23&& u23,
                                  U24&& u24, U25&& u25, U26&& u26, U27&& u27)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>, std::is_nothrow_constructible<T22, U22>, std::is_nothrow_constructible<T23, U23>,
                                    std::is_nothrow_constructible<T24, U24>, std::is_nothrow_constructible<T25, U25>, std::is_nothrow_constructible<T26, U26>,
                                    std::is_nothrow_constructible<T27, U27>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21)), _22(static_cast<U22>(u22)), _23(static_cast<U23>(u23)), _24(static_cast<U24>(u24)), _25(static_cast<U25>(u25)),
          _26(static_cast<U26>(u26)), _27(static_cast<U27>(u27))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26, U27>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>, std::is_nothrow_constructible<T22, U22&>, std::is_nothrow_constructible<T23, U23&>,
                               std::is_nothrow_constructible<T24, U24&>, std::is_nothrow_constructible<T25, U25&>, std::is_nothrow_constructible<T26, U26&>,
                               std::is_nothrow_constructible<T27, U27&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26), _27(other._27)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                 std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                 std::is_nothrow_constructible<T21, U21 const&>, std::is_nothrow_constructible<T22, U22 const&>, std::is_nothrow_constructible<T23, U23 const&>,
                 std::is_nothrow_constructible<T24, U24 const&>, std::is_nothrow_constructible<T25, U25 const&>, std::is_nothrow_constructible<T26, U26 const&>,
                 std::is_nothrow_constructible<T27, U27 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26), _27(other._27)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27>
    constexpr explicit tuple_impl(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26, U27>&& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                               std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                               std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                               std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                               std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                               std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                               std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                               std::is_nothrow_constructible<T21, U21&&>, std::is_nothrow_constructible<T22, U22&&>, std::is_nothrow_constructible<T23, U23&&>,
                               std::is_nothrow_constructible<T24, U24&&>, std::is_nothrow_constructible<T25, U25&&>, std::is_nothrow_constructible<T26, U26&&>,
                               std::is_nothrow_constructible<T27, U27&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26),
          _27(static_cast<decltype(other)>(other)._27)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27> const&& other)
        noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
                std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>,
                std::is_nothrow_constructible<T14, U14 const&&>, std::is_nothrow_constructible<T15, U15 const&&>,
                std::is_nothrow_constructible<T16, U16 const&&>, std::is_nothrow_constructible<T17, U17 const&&>,
                std::is_nothrow_constructible<T18, U18 const&&>, std::is_nothrow_constructible<T19, U19 const&&>,
                std::is_nothrow_constructible<T20, U20 const&&>, std::is_nothrow_constructible<T21, U21 const&&>,
                std::is_nothrow_constructible<T22, U22 const&&>, std::is_nothrow_constructible<T23, U23 const&&>,
                std::is_nothrow_constructible<T24, U24 const&&>, std::is_nothrow_constructible<T25, U25 const&&>,
                std::is_nothrow_constructible<T26, U26 const&&>, std::is_nothrow_constructible<T27, U27 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26),
          _27(static_cast<decltype(other)>(other)._27)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>, std::is_nothrow_copy_assignable<T22>, std::is_nothrow_copy_assignable<T23>,
                                    std::is_nothrow_copy_assignable<T24>, std::is_nothrow_copy_assignable<T25>, std::is_nothrow_copy_assignable<T26>,
                                    std::is_nothrow_copy_assignable<T27>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        _27 = other._27;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>, std::is_nothrow_move_assignable<T22>, std::is_nothrow_move_assignable<T23>,
                                    std::is_nothrow_move_assignable<T24>, std::is_nothrow_move_assignable<T25>, std::is_nothrow_move_assignable<T26>,
                                    std::is_nothrow_move_assignable<T27>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        _27 = static_cast<decltype(other)>(other)._27;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23,
                                               U24, U25, U26, U27> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>,
                 std::is_nothrow_assignable<T21&, U21 const&>, std::is_nothrow_assignable<T22&, U22 const&>, std::is_nothrow_assignable<T23&, U23 const&>,
                 std::is_nothrow_assignable<T24&, U24 const&>, std::is_nothrow_assignable<T25&, U25 const&>, std::is_nothrow_assignable<T26&, U26 const&>,
                 std::is_nothrow_assignable<T27&, U27 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        _27 = other._27;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27>
    constexpr tuple_impl& operator=(
        tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25, U26, U27>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>, std::is_nothrow_assignable<T23&, U23&&>,
                                    std::is_nothrow_assignable<T24&, U24&&>, std::is_nothrow_assignable<T25&, U25&&>, std::is_nothrow_assignable<T26&, U26&&>,
                                    std::is_nothrow_assignable<T27&, U27&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        _27 = static_cast<decltype(other)>(other)._27;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        _21 = alloy::get<21>(static_cast<decltype(other)>(other));
        _22 = alloy::get<22>(static_cast<decltype(other)>(other));
        _23 = alloy::get<23>(static_cast<decltype(other)>(other));
        _24 = alloy::get<24>(static_cast<decltype(other)>(other));
        _25 = alloy::get<25>(static_cast<decltype(other)>(other));
        _26 = alloy::get<26>(static_cast<decltype(other)>(other));
        _27 = alloy::get<27>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>, std::is_nothrow_swappable<T22>, std::is_nothrow_swappable<T23>,
                           std::is_nothrow_swappable<T24>, std::is_nothrow_swappable<T25>, std::is_nothrow_swappable<T26>, std::is_nothrow_swappable<T27>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                                         std::is_swappable<T20>, std::is_swappable<T21>, std::is_swappable<T22>, std::is_swappable<T23>, std::is_swappable<T24>,
                                         std::is_swappable<T25>, std::is_swappable<T26>, std::is_swappable<T27>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
        swap(_22, other._22);
        swap(_23, other._23);
        swap(_24, other._24);
        swap(_25, other._25);
        swap(_26, other._26);
        swap(_27, other._27);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27>&
    get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
        else if constexpr (I == 27)
            return _27;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
        else if constexpr (I == 27)
            return _27;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27>&&
    get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22)&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23)&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24)&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25)&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26)&&>(_26);
        else if constexpr (I == 27)
            return static_cast<decltype(_27)&&>(_27);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22) const&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23) const&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24) const&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25) const&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26) const&&>(_26);
        else if constexpr (I == 27)
            return static_cast<decltype(_27) const&&>(_27);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26,
         class T27, class T28>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25,
                                       U26, U27, U28> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                               is_nothrow_equality_comparable<T21, U21>, is_nothrow_equality_comparable<T22, U22>, is_nothrow_equality_comparable<T23, U23>,
                               is_nothrow_equality_comparable<T24, U24>, is_nothrow_equality_comparable<T25, U25>, is_nothrow_equality_comparable<T26, U26>,
                               is_nothrow_equality_comparable<T27, U27>, is_nothrow_equality_comparable<T28, U28>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21 &&
               _22 == other._22 && _23 == other._23 && _24 == other._24 && _25 == other._25 && _26 == other._26 && _27 == other._27 && _28 == other._28;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    IRIS_NO_UNIQUE_ADDRESS T22 _22;
    IRIS_NO_UNIQUE_ADDRESS T23 _23;
    IRIS_NO_UNIQUE_ADDRESS T24 _24;
    IRIS_NO_UNIQUE_ADDRESS T25 _25;
    IRIS_NO_UNIQUE_ADDRESS T26 _26;
    IRIS_NO_UNIQUE_ADDRESS T27 _27;
    IRIS_NO_UNIQUE_ADDRESS T28 _28;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                           std::is_nothrow_default_constructible<T21>, std::is_nothrow_default_constructible<T22>, std::is_nothrow_default_constructible<T23>,
                           std::is_nothrow_default_constructible<T24>, std::is_nothrow_default_constructible<T25>, std::is_nothrow_default_constructible<T26>,
                           std::is_nothrow_default_constructible<T27>, std::is_nothrow_default_constructible<T28>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}, _22{},
          _23{}, _24{}, _25{}, _26{}, _27{}, _28{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22, U23&& u23,
                                  U24&& u24, U25&& u25, U26&& u26, U27&& u27, U28&& u28)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>, std::is_nothrow_constructible<T22, U22>, std::is_nothrow_constructible<T23, U23>,
                                    std::is_nothrow_constructible<T24, U24>, std::is_nothrow_constructible<T25, U25>, std::is_nothrow_constructible<T26, U26>,
                                    std::is_nothrow_constructible<T27, U27>, std::is_nothrow_constructible<T28, U28>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21)), _22(static_cast<U22>(u22)), _23(static_cast<U23>(u23)), _24(static_cast<U24>(u24)), _25(static_cast<U25>(u25)),
          _26(static_cast<U26>(u26)), _27(static_cast<U27>(u27)), _28(static_cast<U28>(u28))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>, std::is_nothrow_constructible<T22, U22&>, std::is_nothrow_constructible<T23, U23&>,
                               std::is_nothrow_constructible<T24, U24&>, std::is_nothrow_constructible<T25, U25&>, std::is_nothrow_constructible<T26, U26&>,
                               std::is_nothrow_constructible<T27, U27&>, std::is_nothrow_constructible<T28, U28&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26), _27(other._27),
          _28(other._28)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                 std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                 std::is_nothrow_constructible<T21, U21 const&>, std::is_nothrow_constructible<T22, U22 const&>, std::is_nothrow_constructible<T23, U23 const&>,
                 std::is_nothrow_constructible<T24, U24 const&>, std::is_nothrow_constructible<T25, U25 const&>, std::is_nothrow_constructible<T26, U26 const&>,
                 std::is_nothrow_constructible<T27, U27 const&>, std::is_nothrow_constructible<T28, U28 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26), _27(other._27),
          _28(other._28)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28>&& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                               std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                               std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                               std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                               std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                               std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                               std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                               std::is_nothrow_constructible<T21, U21&&>, std::is_nothrow_constructible<T22, U22&&>, std::is_nothrow_constructible<T23, U23&&>,
                               std::is_nothrow_constructible<T24, U24&&>, std::is_nothrow_constructible<T25, U25&&>, std::is_nothrow_constructible<T26, U26&&>,
                               std::is_nothrow_constructible<T27, U27&&>, std::is_nothrow_constructible<T28, U28&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26),
          _27(static_cast<decltype(other)>(other)._27), _28(static_cast<decltype(other)>(other)._28)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28> const&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                 std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                 std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                 std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>,
                 std::is_nothrow_constructible<T11, U11 const&&>, std::is_nothrow_constructible<T12, U12 const&&>,
                 std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
                 std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>,
                 std::is_nothrow_constructible<T17, U17 const&&>, std::is_nothrow_constructible<T18, U18 const&&>,
                 std::is_nothrow_constructible<T19, U19 const&&>, std::is_nothrow_constructible<T20, U20 const&&>,
                 std::is_nothrow_constructible<T21, U21 const&&>, std::is_nothrow_constructible<T22, U22 const&&>,
                 std::is_nothrow_constructible<T23, U23 const&&>, std::is_nothrow_constructible<T24, U24 const&&>,
                 std::is_nothrow_constructible<T25, U25 const&&>, std::is_nothrow_constructible<T26, U26 const&&>,
                 std::is_nothrow_constructible<T27, U27 const&&>, std::is_nothrow_constructible<T28, U28 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26),
          _27(static_cast<decltype(other)>(other)._27), _28(static_cast<decltype(other)>(other)._28)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>, std::is_nothrow_copy_assignable<T22>, std::is_nothrow_copy_assignable<T23>,
                                    std::is_nothrow_copy_assignable<T24>, std::is_nothrow_copy_assignable<T25>, std::is_nothrow_copy_assignable<T26>,
                                    std::is_nothrow_copy_assignable<T27>, std::is_nothrow_copy_assignable<T28>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        _27 = other._27;
        _28 = other._28;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>, std::is_nothrow_move_assignable<T22>, std::is_nothrow_move_assignable<T23>,
                                    std::is_nothrow_move_assignable<T24>, std::is_nothrow_move_assignable<T25>, std::is_nothrow_move_assignable<T26>,
                                    std::is_nothrow_move_assignable<T27>, std::is_nothrow_move_assignable<T28>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        _27 = static_cast<decltype(other)>(other)._27;
        _28 = static_cast<decltype(other)>(other)._28;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23,
                                               U24, U25, U26, U27, U28> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>,
                 std::is_nothrow_assignable<T21&, U21 const&>, std::is_nothrow_assignable<T22&, U22 const&>, std::is_nothrow_assignable<T23&, U23 const&>,
                 std::is_nothrow_assignable<T24&, U24 const&>, std::is_nothrow_assignable<T25&, U25 const&>, std::is_nothrow_assignable<T26&, U26 const&>,
                 std::is_nothrow_assignable<T27&, U27 const&>, std::is_nothrow_assignable<T28&, U28 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        _27 = other._27;
        _28 = other._28;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23,
                                               U24, U25, U26, U27, U28>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>, std::is_nothrow_assignable<T23&, U23&&>,
                                    std::is_nothrow_assignable<T24&, U24&&>, std::is_nothrow_assignable<T25&, U25&&>, std::is_nothrow_assignable<T26&, U26&&>,
                                    std::is_nothrow_assignable<T27&, U27&&>, std::is_nothrow_assignable<T28&, U28&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        _27 = static_cast<decltype(other)>(other)._27;
        _28 = static_cast<decltype(other)>(other)._28;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        _21 = alloy::get<21>(static_cast<decltype(other)>(other));
        _22 = alloy::get<22>(static_cast<decltype(other)>(other));
        _23 = alloy::get<23>(static_cast<decltype(other)>(other));
        _24 = alloy::get<24>(static_cast<decltype(other)>(other));
        _25 = alloy::get<25>(static_cast<decltype(other)>(other));
        _26 = alloy::get<26>(static_cast<decltype(other)>(other));
        _27 = alloy::get<27>(static_cast<decltype(other)>(other));
        _28 = alloy::get<28>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>, std::is_nothrow_swappable<T22>, std::is_nothrow_swappable<T23>,
                           std::is_nothrow_swappable<T24>, std::is_nothrow_swappable<T25>, std::is_nothrow_swappable<T26>, std::is_nothrow_swappable<T27>,
                           std::is_nothrow_swappable<T28>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                                         std::is_swappable<T20>, std::is_swappable<T21>, std::is_swappable<T22>, std::is_swappable<T23>, std::is_swappable<T24>,
                                         std::is_swappable<T25>, std::is_swappable<T26>, std::is_swappable<T27>, std::is_swappable<T28>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
        swap(_22, other._22);
        swap(_23, other._23);
        swap(_24, other._24);
        swap(_25, other._25);
        swap(_26, other._26);
        swap(_27, other._27);
        swap(_28, other._28);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28>&
    get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
        else if constexpr (I == 27)
            return _27;
        else if constexpr (I == 28)
            return _28;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
        else if constexpr (I == 27)
            return _27;
        else if constexpr (I == 28)
            return _28;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28>&&
    get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22)&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23)&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24)&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25)&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26)&&>(_26);
        else if constexpr (I == 27)
            return static_cast<decltype(_27)&&>(_27);
        else if constexpr (I == 28)
            return static_cast<decltype(_28)&&>(_28);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22) const&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23) const&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24) const&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25) const&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26) const&&>(_26);
        else if constexpr (I == 27)
            return static_cast<decltype(_27) const&&>(_27);
        else if constexpr (I == 28)
            return static_cast<decltype(_28) const&&>(_28);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26,
         class T27, class T28, class T29>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25,
                                       U26, U27, U28, U29> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                               is_nothrow_equality_comparable<T21, U21>, is_nothrow_equality_comparable<T22, U22>, is_nothrow_equality_comparable<T23, U23>,
                               is_nothrow_equality_comparable<T24, U24>, is_nothrow_equality_comparable<T25, U25>, is_nothrow_equality_comparable<T26, U26>,
                               is_nothrow_equality_comparable<T27, U27>, is_nothrow_equality_comparable<T28, U28>, is_nothrow_equality_comparable<T29, U29>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21 &&
               _22 == other._22 && _23 == other._23 && _24 == other._24 && _25 == other._25 && _26 == other._26 && _27 == other._27 && _28 == other._28 &&
               _29 == other._29;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    IRIS_NO_UNIQUE_ADDRESS T22 _22;
    IRIS_NO_UNIQUE_ADDRESS T23 _23;
    IRIS_NO_UNIQUE_ADDRESS T24 _24;
    IRIS_NO_UNIQUE_ADDRESS T25 _25;
    IRIS_NO_UNIQUE_ADDRESS T26 _26;
    IRIS_NO_UNIQUE_ADDRESS T27 _27;
    IRIS_NO_UNIQUE_ADDRESS T28 _28;
    IRIS_NO_UNIQUE_ADDRESS T29 _29;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                           std::is_nothrow_default_constructible<T21>, std::is_nothrow_default_constructible<T22>, std::is_nothrow_default_constructible<T23>,
                           std::is_nothrow_default_constructible<T24>, std::is_nothrow_default_constructible<T25>, std::is_nothrow_default_constructible<T26>,
                           std::is_nothrow_default_constructible<T27>, std::is_nothrow_default_constructible<T28>, std::is_nothrow_default_constructible<T29>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}, _22{},
          _23{}, _24{}, _25{}, _26{}, _27{}, _28{}, _29{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22, U23&& u23,
                                  U24&& u24, U25&& u25, U26&& u26, U27&& u27, U28&& u28, U29&& u29)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>, std::is_nothrow_constructible<T22, U22>, std::is_nothrow_constructible<T23, U23>,
                                    std::is_nothrow_constructible<T24, U24>, std::is_nothrow_constructible<T25, U25>, std::is_nothrow_constructible<T26, U26>,
                                    std::is_nothrow_constructible<T27, U27>, std::is_nothrow_constructible<T28, U28>, std::is_nothrow_constructible<T29, U29>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21)), _22(static_cast<U22>(u22)), _23(static_cast<U23>(u23)), _24(static_cast<U24>(u24)), _25(static_cast<U25>(u25)),
          _26(static_cast<U26>(u26)), _27(static_cast<U27>(u27)), _28(static_cast<U28>(u28)), _29(static_cast<U29>(u29))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>, std::is_nothrow_constructible<T22, U22&>, std::is_nothrow_constructible<T23, U23&>,
                               std::is_nothrow_constructible<T24, U24&>, std::is_nothrow_constructible<T25, U25&>, std::is_nothrow_constructible<T26, U26&>,
                               std::is_nothrow_constructible<T27, U27&>, std::is_nothrow_constructible<T28, U28&>, std::is_nothrow_constructible<T29, U29&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26), _27(other._27),
          _28(other._28), _29(other._29)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29> const& other)
        noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                std::is_nothrow_constructible<T21, U21 const&>, std::is_nothrow_constructible<T22, U22 const&>, std::is_nothrow_constructible<T23, U23 const&>,
                std::is_nothrow_constructible<T24, U24 const&>, std::is_nothrow_constructible<T25, U25 const&>, std::is_nothrow_constructible<T26, U26 const&>,
                std::is_nothrow_constructible<T27, U27 const&>, std::is_nothrow_constructible<T28, U28 const&>, std::is_nothrow_constructible<T29, U29 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26), _27(other._27),
          _28(other._28), _29(other._29)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29>&& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                               std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                               std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                               std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                               std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                               std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                               std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                               std::is_nothrow_constructible<T21, U21&&>, std::is_nothrow_constructible<T22, U22&&>, std::is_nothrow_constructible<T23, U23&&>,
                               std::is_nothrow_constructible<T24, U24&&>, std::is_nothrow_constructible<T25, U25&&>, std::is_nothrow_constructible<T26, U26&&>,
                               std::is_nothrow_constructible<T27, U27&&>, std::is_nothrow_constructible<T28, U28&&>, std::is_nothrow_constructible<T29, U29&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26),
          _27(static_cast<decltype(other)>(other)._27), _28(static_cast<decltype(other)>(other)._28), _29(static_cast<decltype(other)>(other)._29)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29> const&& other)
        noexcept(
            std::conjunction_v<
                std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>, std::is_nothrow_constructible<T11, U11 const&&>,
                std::is_nothrow_constructible<T12, U12 const&&>, std::is_nothrow_constructible<T13, U13 const&&>,
                std::is_nothrow_constructible<T14, U14 const&&>, std::is_nothrow_constructible<T15, U15 const&&>,
                std::is_nothrow_constructible<T16, U16 const&&>, std::is_nothrow_constructible<T17, U17 const&&>,
                std::is_nothrow_constructible<T18, U18 const&&>, std::is_nothrow_constructible<T19, U19 const&&>,
                std::is_nothrow_constructible<T20, U20 const&&>, std::is_nothrow_constructible<T21, U21 const&&>,
                std::is_nothrow_constructible<T22, U22 const&&>, std::is_nothrow_constructible<T23, U23 const&&>,
                std::is_nothrow_constructible<T24, U24 const&&>, std::is_nothrow_constructible<T25, U25 const&&>,
                std::is_nothrow_constructible<T26, U26 const&&>, std::is_nothrow_constructible<T27, U27 const&&>,
                std::is_nothrow_constructible<T28, U28 const&&>, std::is_nothrow_constructible<T29, U29 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26),
          _27(static_cast<decltype(other)>(other)._27), _28(static_cast<decltype(other)>(other)._28), _29(static_cast<decltype(other)>(other)._29)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>, std::is_nothrow_copy_assignable<T22>, std::is_nothrow_copy_assignable<T23>,
                                    std::is_nothrow_copy_assignable<T24>, std::is_nothrow_copy_assignable<T25>, std::is_nothrow_copy_assignable<T26>,
                                    std::is_nothrow_copy_assignable<T27>, std::is_nothrow_copy_assignable<T28>, std::is_nothrow_copy_assignable<T29>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        _27 = other._27;
        _28 = other._28;
        _29 = other._29;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>, std::is_nothrow_move_assignable<T22>, std::is_nothrow_move_assignable<T23>,
                                    std::is_nothrow_move_assignable<T24>, std::is_nothrow_move_assignable<T25>, std::is_nothrow_move_assignable<T26>,
                                    std::is_nothrow_move_assignable<T27>, std::is_nothrow_move_assignable<T28>, std::is_nothrow_move_assignable<T29>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        _27 = static_cast<decltype(other)>(other)._27;
        _28 = static_cast<decltype(other)>(other)._28;
        _29 = static_cast<decltype(other)>(other)._29;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23,
                                               U24, U25, U26, U27, U28, U29> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>,
                 std::is_nothrow_assignable<T21&, U21 const&>, std::is_nothrow_assignable<T22&, U22 const&>, std::is_nothrow_assignable<T23&, U23 const&>,
                 std::is_nothrow_assignable<T24&, U24 const&>, std::is_nothrow_assignable<T25&, U25 const&>, std::is_nothrow_assignable<T26&, U26 const&>,
                 std::is_nothrow_assignable<T27&, U27 const&>, std::is_nothrow_assignable<T28&, U28 const&>, std::is_nothrow_assignable<T29&, U29 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        _27 = other._27;
        _28 = other._28;
        _29 = other._29;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23,
                                               U24, U25, U26, U27, U28, U29>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>, std::is_nothrow_assignable<T23&, U23&&>,
                                    std::is_nothrow_assignable<T24&, U24&&>, std::is_nothrow_assignable<T25&, U25&&>, std::is_nothrow_assignable<T26&, U26&&>,
                                    std::is_nothrow_assignable<T27&, U27&&>, std::is_nothrow_assignable<T28&, U28&&>, std::is_nothrow_assignable<T29&, U29&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        _27 = static_cast<decltype(other)>(other)._27;
        _28 = static_cast<decltype(other)>(other)._28;
        _29 = static_cast<decltype(other)>(other)._29;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        _21 = alloy::get<21>(static_cast<decltype(other)>(other));
        _22 = alloy::get<22>(static_cast<decltype(other)>(other));
        _23 = alloy::get<23>(static_cast<decltype(other)>(other));
        _24 = alloy::get<24>(static_cast<decltype(other)>(other));
        _25 = alloy::get<25>(static_cast<decltype(other)>(other));
        _26 = alloy::get<26>(static_cast<decltype(other)>(other));
        _27 = alloy::get<27>(static_cast<decltype(other)>(other));
        _28 = alloy::get<28>(static_cast<decltype(other)>(other));
        _29 = alloy::get<29>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>, std::is_nothrow_swappable<T22>, std::is_nothrow_swappable<T23>,
                           std::is_nothrow_swappable<T24>, std::is_nothrow_swappable<T25>, std::is_nothrow_swappable<T26>, std::is_nothrow_swappable<T27>,
                           std::is_nothrow_swappable<T28>, std::is_nothrow_swappable<T29>>)
    {
        static_assert(
            std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                               std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                               std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                               std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                               std::is_swappable<T20>, std::is_swappable<T21>, std::is_swappable<T22>, std::is_swappable<T23>, std::is_swappable<T24>,
                               std::is_swappable<T25>, std::is_swappable<T26>, std::is_swappable<T27>, std::is_swappable<T28>, std::is_swappable<T29>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
        swap(_22, other._22);
        swap(_23, other._23);
        swap(_24, other._24);
        swap(_25, other._25);
        swap(_26, other._26);
        swap(_27, other._27);
        swap(_28, other._28);
        swap(_29, other._29);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29>&
    get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
        else if constexpr (I == 27)
            return _27;
        else if constexpr (I == 28)
            return _28;
        else if constexpr (I == 29)
            return _29;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
        else if constexpr (I == 27)
            return _27;
        else if constexpr (I == 28)
            return _28;
        else if constexpr (I == 29)
            return _29;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29>&&
    get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22)&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23)&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24)&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25)&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26)&&>(_26);
        else if constexpr (I == 27)
            return static_cast<decltype(_27)&&>(_27);
        else if constexpr (I == 28)
            return static_cast<decltype(_28)&&>(_28);
        else if constexpr (I == 29)
            return static_cast<decltype(_29)&&>(_29);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22) const&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23) const&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24) const&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25) const&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26) const&&>(_26);
        else if constexpr (I == 27)
            return static_cast<decltype(_27) const&&>(_27);
        else if constexpr (I == 28)
            return static_cast<decltype(_28) const&&>(_28);
        else if constexpr (I == 29)
            return static_cast<decltype(_29) const&&>(_29);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26,
         class T27, class T28, class T29, class T30>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
                 T30>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const&, tuple<Vs...> const&)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25,
                                       U26, U27, U28, U29, U30> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                               is_nothrow_equality_comparable<T21, U21>, is_nothrow_equality_comparable<T22, U22>, is_nothrow_equality_comparable<T23, U23>,
                               is_nothrow_equality_comparable<T24, U24>, is_nothrow_equality_comparable<T25, U25>, is_nothrow_equality_comparable<T26, U26>,
                               is_nothrow_equality_comparable<T27, U27>, is_nothrow_equality_comparable<T28, U28>, is_nothrow_equality_comparable<T29, U29>,
                               is_nothrow_equality_comparable<T30, U30>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21 &&
               _22 == other._22 && _23 == other._23 && _24 == other._24 && _25 == other._25 && _26 == other._26 && _27 == other._27 && _28 == other._28 &&
               _29 == other._29 && _30 == other._30;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    IRIS_NO_UNIQUE_ADDRESS T22 _22;
    IRIS_NO_UNIQUE_ADDRESS T23 _23;
    IRIS_NO_UNIQUE_ADDRESS T24 _24;
    IRIS_NO_UNIQUE_ADDRESS T25 _25;
    IRIS_NO_UNIQUE_ADDRESS T26 _26;
    IRIS_NO_UNIQUE_ADDRESS T27 _27;
    IRIS_NO_UNIQUE_ADDRESS T28 _28;
    IRIS_NO_UNIQUE_ADDRESS T29 _29;
    IRIS_NO_UNIQUE_ADDRESS T30 _30;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t) noexcept(
        std::conjunction_v<std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                           std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                           std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                           std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                           std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                           std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                           std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                           std::is_nothrow_default_constructible<T21>, std::is_nothrow_default_constructible<T22>, std::is_nothrow_default_constructible<T23>,
                           std::is_nothrow_default_constructible<T24>, std::is_nothrow_default_constructible<T25>, std::is_nothrow_default_constructible<T26>,
                           std::is_nothrow_default_constructible<T27>, std::is_nothrow_default_constructible<T28>, std::is_nothrow_default_constructible<T29>,
                           std::is_nothrow_default_constructible<T30>>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}, _22{},
          _23{}, _24{}, _25{}, _26{}, _27{}, _28{}, _29{}, _30{}
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30>
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22, U23&& u23,
                                  U24&& u24, U25&& u25, U26&& u26, U27&& u27, U28&& u28, U29&& u29, U30&& u30)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>, std::is_nothrow_constructible<T22, U22>, std::is_nothrow_constructible<T23, U23>,
                                    std::is_nothrow_constructible<T24, U24>, std::is_nothrow_constructible<T25, U25>, std::is_nothrow_constructible<T26, U26>,
                                    std::is_nothrow_constructible<T27, U27>, std::is_nothrow_constructible<T28, U28>, std::is_nothrow_constructible<T29, U29>,
                                    std::is_nothrow_constructible<T30, U30>>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21)), _22(static_cast<U22>(u22)), _23(static_cast<U23>(u23)), _24(static_cast<U24>(u24)), _25(static_cast<U25>(u25)),
          _26(static_cast<U26>(u26)), _27(static_cast<U27>(u27)), _28(static_cast<U28>(u28)), _29(static_cast<U29>(u29)), _30(static_cast<U30>(u30))
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29, U30>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>, std::is_nothrow_constructible<T22, U22&>, std::is_nothrow_constructible<T23, U23&>,
                               std::is_nothrow_constructible<T24, U24&>, std::is_nothrow_constructible<T25, U25&>, std::is_nothrow_constructible<T26, U26&>,
                               std::is_nothrow_constructible<T27, U27&>, std::is_nothrow_constructible<T28, U28&>, std::is_nothrow_constructible<T29, U29&>,
                               std::is_nothrow_constructible<T30, U30&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26), _27(other._27),
          _28(other._28), _29(other._29), _30(other._30)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29, U30> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                 std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                 std::is_nothrow_constructible<T21, U21 const&>, std::is_nothrow_constructible<T22, U22 const&>, std::is_nothrow_constructible<T23, U23 const&>,
                 std::is_nothrow_constructible<T24, U24 const&>, std::is_nothrow_constructible<T25, U25 const&>, std::is_nothrow_constructible<T26, U26 const&>,
                 std::is_nothrow_constructible<T27, U27 const&>, std::is_nothrow_constructible<T28, U28 const&>, std::is_nothrow_constructible<T29, U29 const&>,
                 std::is_nothrow_constructible<T30, U30 const&>>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26), _27(other._27),
          _28(other._28), _29(other._29), _30(other._30)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29, U30>&& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                               std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                               std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                               std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                               std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                               std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                               std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                               std::is_nothrow_constructible<T21, U21&&>, std::is_nothrow_constructible<T22, U22&&>, std::is_nothrow_constructible<T23, U23&&>,
                               std::is_nothrow_constructible<T24, U24&&>, std::is_nothrow_constructible<T25, U25&&>, std::is_nothrow_constructible<T26, U26&&>,
                               std::is_nothrow_constructible<T27, U27&&>, std::is_nothrow_constructible<T28, U28&&>, std::is_nothrow_constructible<T29, U29&&>,
                               std::is_nothrow_constructible<T30, U30&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26),
          _27(static_cast<decltype(other)>(other)._27), _28(static_cast<decltype(other)>(other)._28), _29(static_cast<decltype(other)>(other)._29),
          _30(static_cast<decltype(other)>(other)._30)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29, U30> const&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                 std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                 std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                 std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>,
                 std::is_nothrow_constructible<T11, U11 const&&>, std::is_nothrow_constructible<T12, U12 const&&>,
                 std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
                 std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>,
                 std::is_nothrow_constructible<T17, U17 const&&>, std::is_nothrow_constructible<T18, U18 const&&>,
                 std::is_nothrow_constructible<T19, U19 const&&>, std::is_nothrow_constructible<T20, U20 const&&>,
                 std::is_nothrow_constructible<T21, U21 const&&>, std::is_nothrow_constructible<T22, U22 const&&>,
                 std::is_nothrow_constructible<T23, U23 const&&>, std::is_nothrow_constructible<T24, U24 const&&>,
                 std::is_nothrow_constructible<T25, U25 const&&>, std::is_nothrow_constructible<T26, U26 const&&>,
                 std::is_nothrow_constructible<T27, U27 const&&>, std::is_nothrow_constructible<T28, U28 const&&>,
                 std::is_nothrow_constructible<T29, U29 const&&>, std::is_nothrow_constructible<T30, U30 const&&>>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26),
          _27(static_cast<decltype(other)>(other)._27), _28(static_cast<decltype(other)>(other)._28), _29(static_cast<decltype(other)>(other)._29),
          _30(static_cast<decltype(other)>(other)._30)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>, std::is_nothrow_copy_assignable<T22>, std::is_nothrow_copy_assignable<T23>,
                                    std::is_nothrow_copy_assignable<T24>, std::is_nothrow_copy_assignable<T25>, std::is_nothrow_copy_assignable<T26>,
                                    std::is_nothrow_copy_assignable<T27>, std::is_nothrow_copy_assignable<T28>, std::is_nothrow_copy_assignable<T29>,
                                    std::is_nothrow_copy_assignable<T30>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        _27 = other._27;
        _28 = other._28;
        _29 = other._29;
        _30 = other._30;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>, std::is_nothrow_move_assignable<T22>, std::is_nothrow_move_assignable<T23>,
                                    std::is_nothrow_move_assignable<T24>, std::is_nothrow_move_assignable<T25>, std::is_nothrow_move_assignable<T26>,
                                    std::is_nothrow_move_assignable<T27>, std::is_nothrow_move_assignable<T28>, std::is_nothrow_move_assignable<T29>,
                                    std::is_nothrow_move_assignable<T30>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        _27 = static_cast<decltype(other)>(other)._27;
        _28 = static_cast<decltype(other)>(other)._28;
        _29 = static_cast<decltype(other)>(other)._29;
        _30 = static_cast<decltype(other)>(other)._30;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23,
                                               U24, U25, U26, U27, U28, U29, U30> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_assignable<T0&, U0 const&>, std::is_nothrow_assignable<T1&, U1 const&>, std::is_nothrow_assignable<T2&, U2 const&>,
                 std::is_nothrow_assignable<T3&, U3 const&>, std::is_nothrow_assignable<T4&, U4 const&>, std::is_nothrow_assignable<T5&, U5 const&>,
                 std::is_nothrow_assignable<T6&, U6 const&>, std::is_nothrow_assignable<T7&, U7 const&>, std::is_nothrow_assignable<T8&, U8 const&>,
                 std::is_nothrow_assignable<T9&, U9 const&>, std::is_nothrow_assignable<T10&, U10 const&>, std::is_nothrow_assignable<T11&, U11 const&>,
                 std::is_nothrow_assignable<T12&, U12 const&>, std::is_nothrow_assignable<T13&, U13 const&>, std::is_nothrow_assignable<T14&, U14 const&>,
                 std::is_nothrow_assignable<T15&, U15 const&>, std::is_nothrow_assignable<T16&, U16 const&>, std::is_nothrow_assignable<T17&, U17 const&>,
                 std::is_nothrow_assignable<T18&, U18 const&>, std::is_nothrow_assignable<T19&, U19 const&>, std::is_nothrow_assignable<T20&, U20 const&>,
                 std::is_nothrow_assignable<T21&, U21 const&>, std::is_nothrow_assignable<T22&, U22 const&>, std::is_nothrow_assignable<T23&, U23 const&>,
                 std::is_nothrow_assignable<T24&, U24 const&>, std::is_nothrow_assignable<T25&, U25 const&>, std::is_nothrow_assignable<T26&, U26 const&>,
                 std::is_nothrow_assignable<T27&, U27 const&>, std::is_nothrow_assignable<T28&, U28 const&>, std::is_nothrow_assignable<T29&, U29 const&>,
                 std::is_nothrow_assignable<T30&, U30 const&>>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        _27 = other._27;
        _28 = other._28;
        _29 = other._29;
        _30 = other._30;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23,
                                               U24, U25, U26, U27, U28, U29, U30>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>, std::is_nothrow_assignable<T23&, U23&&>,
                                    std::is_nothrow_assignable<T24&, U24&&>, std::is_nothrow_assignable<T25&, U25&&>, std::is_nothrow_assignable<T26&, U26&&>,
                                    std::is_nothrow_assignable<T27&, U27&&>, std::is_nothrow_assignable<T28&, U28&&>, std::is_nothrow_assignable<T29&, U29&&>,
                                    std::is_nothrow_assignable<T30&, U30&&>>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        _27 = static_cast<decltype(other)>(other)._27;
        _28 = static_cast<decltype(other)>(other)._28;
        _29 = static_cast<decltype(other)>(other)._29;
        _30 = static_cast<decltype(other)>(other)._30;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        _0 = alloy::get<0>(static_cast<decltype(other)>(other));
        _1 = alloy::get<1>(static_cast<decltype(other)>(other));
        _2 = alloy::get<2>(static_cast<decltype(other)>(other));
        _3 = alloy::get<3>(static_cast<decltype(other)>(other));
        _4 = alloy::get<4>(static_cast<decltype(other)>(other));
        _5 = alloy::get<5>(static_cast<decltype(other)>(other));
        _6 = alloy::get<6>(static_cast<decltype(other)>(other));
        _7 = alloy::get<7>(static_cast<decltype(other)>(other));
        _8 = alloy::get<8>(static_cast<decltype(other)>(other));
        _9 = alloy::get<9>(static_cast<decltype(other)>(other));
        _10 = alloy::get<10>(static_cast<decltype(other)>(other));
        _11 = alloy::get<11>(static_cast<decltype(other)>(other));
        _12 = alloy::get<12>(static_cast<decltype(other)>(other));
        _13 = alloy::get<13>(static_cast<decltype(other)>(other));
        _14 = alloy::get<14>(static_cast<decltype(other)>(other));
        _15 = alloy::get<15>(static_cast<decltype(other)>(other));
        _16 = alloy::get<16>(static_cast<decltype(other)>(other));
        _17 = alloy::get<17>(static_cast<decltype(other)>(other));
        _18 = alloy::get<18>(static_cast<decltype(other)>(other));
        _19 = alloy::get<19>(static_cast<decltype(other)>(other));
        _20 = alloy::get<20>(static_cast<decltype(other)>(other));
        _21 = alloy::get<21>(static_cast<decltype(other)>(other));
        _22 = alloy::get<22>(static_cast<decltype(other)>(other));
        _23 = alloy::get<23>(static_cast<decltype(other)>(other));
        _24 = alloy::get<24>(static_cast<decltype(other)>(other));
        _25 = alloy::get<25>(static_cast<decltype(other)>(other));
        _26 = alloy::get<26>(static_cast<decltype(other)>(other));
        _27 = alloy::get<27>(static_cast<decltype(other)>(other));
        _28 = alloy::get<28>(static_cast<decltype(other)>(other));
        _29 = alloy::get<29>(static_cast<decltype(other)>(other));
        _30 = alloy::get<30>(static_cast<decltype(other)>(other));
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>, std::is_nothrow_swappable<T22>, std::is_nothrow_swappable<T23>,
                           std::is_nothrow_swappable<T24>, std::is_nothrow_swappable<T25>, std::is_nothrow_swappable<T26>, std::is_nothrow_swappable<T27>,
                           std::is_nothrow_swappable<T28>, std::is_nothrow_swappable<T29>, std::is_nothrow_swappable<T30>>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                                         std::is_swappable<T20>, std::is_swappable<T21>, std::is_swappable<T22>, std::is_swappable<T23>, std::is_swappable<T24>,
                                         std::is_swappable<T25>, std::is_swappable<T26>, std::is_swappable<T27>, std::is_swappable<T28>, std::is_swappable<T29>,
                                         std::is_swappable<T30>>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
        swap(_22, other._22);
        swap(_23, other._23);
        swap(_24, other._24);
        swap(_25, other._25);
        swap(_26, other._26);
        swap(_27, other._27);
        swap(_28, other._28);
        swap(_29, other._29);
        swap(_30, other._30);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29, T30>&
    get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
        else if constexpr (I == 27)
            return _27;
        else if constexpr (I == 28)
            return _28;
        else if constexpr (I == 29)
            return _29;
        else if constexpr (I == 30)
            return _30;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29, T30> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
        else if constexpr (I == 27)
            return _27;
        else if constexpr (I == 28)
            return _28;
        else if constexpr (I == 29)
            return _29;
        else if constexpr (I == 30)
            return _30;
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29, T30>&&
    get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22)&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23)&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24)&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25)&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26)&&>(_26);
        else if constexpr (I == 27)
            return static_cast<decltype(_27)&&>(_27);
        else if constexpr (I == 28)
            return static_cast<decltype(_28)&&>(_28);
        else if constexpr (I == 29)
            return static_cast<decltype(_29)&&>(_29);
        else if constexpr (I == 30)
            return static_cast<decltype(_30)&&>(_30);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29, T30> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22) const&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23) const&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24) const&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25) const&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26) const&&>(_26);
        else if constexpr (I == 27)
            return static_cast<decltype(_27) const&&>(_27);
        else if constexpr (I == 28)
            return static_cast<decltype(_28) const&&>(_28);
        else if constexpr (I == 29)
            return static_cast<decltype(_29) const&&>(_29);
        else if constexpr (I == 30)
            return static_cast<decltype(_30) const&&>(_30);
    }
};
template<class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9, class T10, class T11, class T12, class T13,
         class T14, class T15, class T16, class T17, class T18, class T19, class T20, class T21, class T22, class T23, class T24, class T25, class T26,
         class T27, class T28, class T29, class T30, class T31, class... Ts>
class tuple_impl<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25, T26, T27, T28, T29,
                 T30, T31, Ts...>
{
    template<class... Us>
    friend class tuple_impl;
    template<class... Us, class... Vs>
        requires tuple_all_elements_have_equality_operator<tuple<Us...>, tuple<Vs...>>
    friend constexpr bool alloy::operator==(tuple<Us...> const& a, tuple<Vs...> const& b)
        noexcept(detail::are_tuple_all_elements_nothrow_equality_comparable_v<tuple<Us...>, tuple<Vs...>>);

private:
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30, class U31, class... Us>
    constexpr void assign(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12, U13&& u13,
                          U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22, U23&& u23, U24&& u24, U25&& u25,
                          U26&& u26, U27&& u27, U28&& u28, U29&& u29, U30&& u30, U31&& u31, Us&&... us)
    {
        _0 = static_cast<U0>(u0);
        _1 = static_cast<U1>(u1);
        _2 = static_cast<U2>(u2);
        _3 = static_cast<U3>(u3);
        _4 = static_cast<U4>(u4);
        _5 = static_cast<U5>(u5);
        _6 = static_cast<U6>(u6);
        _7 = static_cast<U7>(u7);
        _8 = static_cast<U8>(u8);
        _9 = static_cast<U9>(u9);
        _10 = static_cast<U10>(u10);
        _11 = static_cast<U11>(u11);
        _12 = static_cast<U12>(u12);
        _13 = static_cast<U13>(u13);
        _14 = static_cast<U14>(u14);
        _15 = static_cast<U15>(u15);
        _16 = static_cast<U16>(u16);
        _17 = static_cast<U17>(u17);
        _18 = static_cast<U18>(u18);
        _19 = static_cast<U19>(u19);
        _20 = static_cast<U20>(u20);
        _21 = static_cast<U21>(u21);
        _22 = static_cast<U22>(u22);
        _23 = static_cast<U23>(u23);
        _24 = static_cast<U24>(u24);
        _25 = static_cast<U25>(u25);
        _26 = static_cast<U26>(u26);
        _27 = static_cast<U27>(u27);
        _28 = static_cast<U28>(u28);
        _29 = static_cast<U29>(u29);
        _30 = static_cast<U30>(u30);
        _31 = static_cast<U31>(u31);
        rest.assign(static_cast<Us>(us)...);
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30, class U31>
    constexpr bool equal_to(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24, U25,
                                       U26, U27, U28, U29, U30, U31> const& other) const
        noexcept(
            std::conjunction_v<is_nothrow_equality_comparable<T0, U0>, is_nothrow_equality_comparable<T1, U1>, is_nothrow_equality_comparable<T2, U2>,
                               is_nothrow_equality_comparable<T3, U3>, is_nothrow_equality_comparable<T4, U4>, is_nothrow_equality_comparable<T5, U5>,
                               is_nothrow_equality_comparable<T6, U6>, is_nothrow_equality_comparable<T7, U7>, is_nothrow_equality_comparable<T8, U8>,
                               is_nothrow_equality_comparable<T9, U9>, is_nothrow_equality_comparable<T10, U10>, is_nothrow_equality_comparable<T11, U11>,
                               is_nothrow_equality_comparable<T12, U12>, is_nothrow_equality_comparable<T13, U13>, is_nothrow_equality_comparable<T14, U14>,
                               is_nothrow_equality_comparable<T15, U15>, is_nothrow_equality_comparable<T16, U16>, is_nothrow_equality_comparable<T17, U17>,
                               is_nothrow_equality_comparable<T18, U18>, is_nothrow_equality_comparable<T19, U19>, is_nothrow_equality_comparable<T20, U20>,
                               is_nothrow_equality_comparable<T21, U21>, is_nothrow_equality_comparable<T22, U22>, is_nothrow_equality_comparable<T23, U23>,
                               is_nothrow_equality_comparable<T24, U24>, is_nothrow_equality_comparable<T25, U25>, is_nothrow_equality_comparable<T26, U26>,
                               is_nothrow_equality_comparable<T27, U27>, is_nothrow_equality_comparable<T28, U28>, is_nothrow_equality_comparable<T29, U29>,
                               is_nothrow_equality_comparable<T30, U30>, is_nothrow_equality_comparable<T31, U31>>)
    {
        return _0 == other._0 && _1 == other._1 && _2 == other._2 && _3 == other._3 && _4 == other._4 && _5 == other._5 && _6 == other._6 && _7 == other._7 &&
               _8 == other._8 && _9 == other._9 && _10 == other._10 && _11 == other._11 && _12 == other._12 && _13 == other._13 && _14 == other._14 &&
               _15 == other._15 && _16 == other._16 && _17 == other._17 && _18 == other._18 && _19 == other._19 && _20 == other._20 && _21 == other._21 &&
               _22 == other._22 && _23 == other._23 && _24 == other._24 && _25 == other._25 && _26 == other._26 && _27 == other._27 && _28 == other._28 &&
               _29 == other._29 && _30 == other._30 && _31 == other._31 && rest == other.rest;
    }

public:
    IRIS_NO_UNIQUE_ADDRESS T0 _0;
    IRIS_NO_UNIQUE_ADDRESS T1 _1;
    IRIS_NO_UNIQUE_ADDRESS T2 _2;
    IRIS_NO_UNIQUE_ADDRESS T3 _3;
    IRIS_NO_UNIQUE_ADDRESS T4 _4;
    IRIS_NO_UNIQUE_ADDRESS T5 _5;
    IRIS_NO_UNIQUE_ADDRESS T6 _6;
    IRIS_NO_UNIQUE_ADDRESS T7 _7;
    IRIS_NO_UNIQUE_ADDRESS T8 _8;
    IRIS_NO_UNIQUE_ADDRESS T9 _9;
    IRIS_NO_UNIQUE_ADDRESS T10 _10;
    IRIS_NO_UNIQUE_ADDRESS T11 _11;
    IRIS_NO_UNIQUE_ADDRESS T12 _12;
    IRIS_NO_UNIQUE_ADDRESS T13 _13;
    IRIS_NO_UNIQUE_ADDRESS T14 _14;
    IRIS_NO_UNIQUE_ADDRESS T15 _15;
    IRIS_NO_UNIQUE_ADDRESS T16 _16;
    IRIS_NO_UNIQUE_ADDRESS T17 _17;
    IRIS_NO_UNIQUE_ADDRESS T18 _18;
    IRIS_NO_UNIQUE_ADDRESS T19 _19;
    IRIS_NO_UNIQUE_ADDRESS T20 _20;
    IRIS_NO_UNIQUE_ADDRESS T21 _21;
    IRIS_NO_UNIQUE_ADDRESS T22 _22;
    IRIS_NO_UNIQUE_ADDRESS T23 _23;
    IRIS_NO_UNIQUE_ADDRESS T24 _24;
    IRIS_NO_UNIQUE_ADDRESS T25 _25;
    IRIS_NO_UNIQUE_ADDRESS T26 _26;
    IRIS_NO_UNIQUE_ADDRESS T27 _27;
    IRIS_NO_UNIQUE_ADDRESS T28 _28;
    IRIS_NO_UNIQUE_ADDRESS T29 _29;
    IRIS_NO_UNIQUE_ADDRESS T30 _30;
    IRIS_NO_UNIQUE_ADDRESS T31 _31;
    IRIS_NO_UNIQUE_ADDRESS tuple_impl<Ts...> rest;
    explicit tuple_impl() = default;
    explicit tuple_impl(tuple_impl const&) = default;
    explicit tuple_impl(tuple_impl&&) = default;
    constexpr explicit tuple_impl(value_initialize_t vi)
        noexcept(std::conjunction_v<
                 std::is_nothrow_default_constructible<T0>, std::is_nothrow_default_constructible<T1>, std::is_nothrow_default_constructible<T2>,
                 std::is_nothrow_default_constructible<T3>, std::is_nothrow_default_constructible<T4>, std::is_nothrow_default_constructible<T5>,
                 std::is_nothrow_default_constructible<T6>, std::is_nothrow_default_constructible<T7>, std::is_nothrow_default_constructible<T8>,
                 std::is_nothrow_default_constructible<T9>, std::is_nothrow_default_constructible<T10>, std::is_nothrow_default_constructible<T11>,
                 std::is_nothrow_default_constructible<T12>, std::is_nothrow_default_constructible<T13>, std::is_nothrow_default_constructible<T14>,
                 std::is_nothrow_default_constructible<T15>, std::is_nothrow_default_constructible<T16>, std::is_nothrow_default_constructible<T17>,
                 std::is_nothrow_default_constructible<T18>, std::is_nothrow_default_constructible<T19>, std::is_nothrow_default_constructible<T20>,
                 std::is_nothrow_default_constructible<T21>, std::is_nothrow_default_constructible<T22>, std::is_nothrow_default_constructible<T23>,
                 std::is_nothrow_default_constructible<T24>, std::is_nothrow_default_constructible<T25>, std::is_nothrow_default_constructible<T26>,
                 std::is_nothrow_default_constructible<T27>, std::is_nothrow_default_constructible<T28>, std::is_nothrow_default_constructible<T29>,
                 std::is_nothrow_default_constructible<T30>, std::is_nothrow_default_constructible<T31>, std::is_nothrow_default_constructible<Ts>...>)
        : _0{}, _1{}, _2{}, _3{}, _4{}, _5{}, _6{}, _7{}, _8{}, _9{}, _10{}, _11{}, _12{}, _13{}, _14{}, _15{}, _16{}, _17{}, _18{}, _19{}, _20{}, _21{}, _22{},
          _23{}, _24{}, _25{}, _26{}, _27{}, _28{}, _29{}, _30{}, _31{}, rest(vi)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30, class U31, class... Us>
        requires (sizeof...(Ts) == sizeof...(Us))
    constexpr explicit tuple_impl(U0&& u0, U1&& u1, U2&& u2, U3&& u3, U4&& u4, U5&& u5, U6&& u6, U7&& u7, U8&& u8, U9&& u9, U10&& u10, U11&& u11, U12&& u12,
                                  U13&& u13, U14&& u14, U15&& u15, U16&& u16, U17&& u17, U18&& u18, U19&& u19, U20&& u20, U21&& u21, U22&& u22, U23&& u23,
                                  U24&& u24, U25&& u25, U26&& u26, U27&& u27, U28&& u28, U29&& u29, U30&& u30, U31&& u31, Us&&... us)
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T0, U0>, std::is_nothrow_constructible<T1, U1>, std::is_nothrow_constructible<T2, U2>,
                                    std::is_nothrow_constructible<T3, U3>, std::is_nothrow_constructible<T4, U4>, std::is_nothrow_constructible<T5, U5>,
                                    std::is_nothrow_constructible<T6, U6>, std::is_nothrow_constructible<T7, U7>, std::is_nothrow_constructible<T8, U8>,
                                    std::is_nothrow_constructible<T9, U9>, std::is_nothrow_constructible<T10, U10>, std::is_nothrow_constructible<T11, U11>,
                                    std::is_nothrow_constructible<T12, U12>, std::is_nothrow_constructible<T13, U13>, std::is_nothrow_constructible<T14, U14>,
                                    std::is_nothrow_constructible<T15, U15>, std::is_nothrow_constructible<T16, U16>, std::is_nothrow_constructible<T17, U17>,
                                    std::is_nothrow_constructible<T18, U18>, std::is_nothrow_constructible<T19, U19>, std::is_nothrow_constructible<T20, U20>,
                                    std::is_nothrow_constructible<T21, U21>, std::is_nothrow_constructible<T22, U22>, std::is_nothrow_constructible<T23, U23>,
                                    std::is_nothrow_constructible<T24, U24>, std::is_nothrow_constructible<T25, U25>, std::is_nothrow_constructible<T26, U26>,
                                    std::is_nothrow_constructible<T27, U27>, std::is_nothrow_constructible<T28, U28>, std::is_nothrow_constructible<T29, U29>,
                                    std::is_nothrow_constructible<T30, U30>, std::is_nothrow_constructible<T31, U31>, std::is_nothrow_constructible<Ts, Us>...>)
        : _0(static_cast<U0>(u0)), _1(static_cast<U1>(u1)), _2(static_cast<U2>(u2)), _3(static_cast<U3>(u3)), _4(static_cast<U4>(u4)), _5(static_cast<U5>(u5)),
          _6(static_cast<U6>(u6)), _7(static_cast<U7>(u7)), _8(static_cast<U8>(u8)), _9(static_cast<U9>(u9)), _10(static_cast<U10>(u10)),
          _11(static_cast<U11>(u11)), _12(static_cast<U12>(u12)), _13(static_cast<U13>(u13)), _14(static_cast<U14>(u14)), _15(static_cast<U15>(u15)),
          _16(static_cast<U16>(u16)), _17(static_cast<U17>(u17)), _18(static_cast<U18>(u18)), _19(static_cast<U19>(u19)), _20(static_cast<U20>(u20)),
          _21(static_cast<U21>(u21)), _22(static_cast<U22>(u22)), _23(static_cast<U23>(u23)), _24(static_cast<U24>(u24)), _25(static_cast<U25>(u25)),
          _26(static_cast<U26>(u26)), _27(static_cast<U27>(u27)), _28(static_cast<U28>(u28)), _29(static_cast<U29>(u29)), _30(static_cast<U30>(u30)),
          _31(static_cast<U31>(u31)), rest(static_cast<Us&&>(us)...)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30, class U31, class... Us>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29, U30, U31, Us...>& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_constructible<T0, U0&>, std::is_nothrow_constructible<T1, U1&>, std::is_nothrow_constructible<T2, U2&>,
                               std::is_nothrow_constructible<T3, U3&>, std::is_nothrow_constructible<T4, U4&>, std::is_nothrow_constructible<T5, U5&>,
                               std::is_nothrow_constructible<T6, U6&>, std::is_nothrow_constructible<T7, U7&>, std::is_nothrow_constructible<T8, U8&>,
                               std::is_nothrow_constructible<T9, U9&>, std::is_nothrow_constructible<T10, U10&>, std::is_nothrow_constructible<T11, U11&>,
                               std::is_nothrow_constructible<T12, U12&>, std::is_nothrow_constructible<T13, U13&>, std::is_nothrow_constructible<T14, U14&>,
                               std::is_nothrow_constructible<T15, U15&>, std::is_nothrow_constructible<T16, U16&>, std::is_nothrow_constructible<T17, U17&>,
                               std::is_nothrow_constructible<T18, U18&>, std::is_nothrow_constructible<T19, U19&>, std::is_nothrow_constructible<T20, U20&>,
                               std::is_nothrow_constructible<T21, U21&>, std::is_nothrow_constructible<T22, U22&>, std::is_nothrow_constructible<T23, U23&>,
                               std::is_nothrow_constructible<T24, U24&>, std::is_nothrow_constructible<T25, U25&>, std::is_nothrow_constructible<T26, U26&>,
                               std::is_nothrow_constructible<T27, U27&>, std::is_nothrow_constructible<T28, U28&>, std::is_nothrow_constructible<T29, U29&>,
                               std::is_nothrow_constructible<T30, U30&>, std::is_nothrow_constructible<T31, U31&>, std::is_nothrow_constructible<Ts, Us&>...>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26), _27(other._27),
          _28(other._28), _29(other._29), _30(other._30), _31(other._31), rest(other.rest)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30, class U31, class... Us>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29, U30, U31, Us...> const& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&>, std::is_nothrow_constructible<T1, U1 const&>, std::is_nothrow_constructible<T2, U2 const&>,
                 std::is_nothrow_constructible<T3, U3 const&>, std::is_nothrow_constructible<T4, U4 const&>, std::is_nothrow_constructible<T5, U5 const&>,
                 std::is_nothrow_constructible<T6, U6 const&>, std::is_nothrow_constructible<T7, U7 const&>, std::is_nothrow_constructible<T8, U8 const&>,
                 std::is_nothrow_constructible<T9, U9 const&>, std::is_nothrow_constructible<T10, U10 const&>, std::is_nothrow_constructible<T11, U11 const&>,
                 std::is_nothrow_constructible<T12, U12 const&>, std::is_nothrow_constructible<T13, U13 const&>, std::is_nothrow_constructible<T14, U14 const&>,
                 std::is_nothrow_constructible<T15, U15 const&>, std::is_nothrow_constructible<T16, U16 const&>, std::is_nothrow_constructible<T17, U17 const&>,
                 std::is_nothrow_constructible<T18, U18 const&>, std::is_nothrow_constructible<T19, U19 const&>, std::is_nothrow_constructible<T20, U20 const&>,
                 std::is_nothrow_constructible<T21, U21 const&>, std::is_nothrow_constructible<T22, U22 const&>, std::is_nothrow_constructible<T23, U23 const&>,
                 std::is_nothrow_constructible<T24, U24 const&>, std::is_nothrow_constructible<T25, U25 const&>, std::is_nothrow_constructible<T26, U26 const&>,
                 std::is_nothrow_constructible<T27, U27 const&>, std::is_nothrow_constructible<T28, U28 const&>, std::is_nothrow_constructible<T29, U29 const&>,
                 std::is_nothrow_constructible<T30, U30 const&>, std::is_nothrow_constructible<T31, U31 const&>,
                 std::is_nothrow_constructible<Ts, Us const&>...>)
        : _0(other._0), _1(other._1), _2(other._2), _3(other._3), _4(other._4), _5(other._5), _6(other._6), _7(other._7), _8(other._8), _9(other._9),
          _10(other._10), _11(other._11), _12(other._12), _13(other._13), _14(other._14), _15(other._15), _16(other._16), _17(other._17), _18(other._18),
          _19(other._19), _20(other._20), _21(other._21), _22(other._22), _23(other._23), _24(other._24), _25(other._25), _26(other._26), _27(other._27),
          _28(other._28), _29(other._29), _30(other._30), _31(other._31), rest(other.rest)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30, class U31, class... Us>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29, U30, U31, Us...>&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0&&>, std::is_nothrow_constructible<T1, U1&&>, std::is_nothrow_constructible<T2, U2&&>,
                 std::is_nothrow_constructible<T3, U3&&>, std::is_nothrow_constructible<T4, U4&&>, std::is_nothrow_constructible<T5, U5&&>,
                 std::is_nothrow_constructible<T6, U6&&>, std::is_nothrow_constructible<T7, U7&&>, std::is_nothrow_constructible<T8, U8&&>,
                 std::is_nothrow_constructible<T9, U9&&>, std::is_nothrow_constructible<T10, U10&&>, std::is_nothrow_constructible<T11, U11&&>,
                 std::is_nothrow_constructible<T12, U12&&>, std::is_nothrow_constructible<T13, U13&&>, std::is_nothrow_constructible<T14, U14&&>,
                 std::is_nothrow_constructible<T15, U15&&>, std::is_nothrow_constructible<T16, U16&&>, std::is_nothrow_constructible<T17, U17&&>,
                 std::is_nothrow_constructible<T18, U18&&>, std::is_nothrow_constructible<T19, U19&&>, std::is_nothrow_constructible<T20, U20&&>,
                 std::is_nothrow_constructible<T21, U21&&>, std::is_nothrow_constructible<T22, U22&&>, std::is_nothrow_constructible<T23, U23&&>,
                 std::is_nothrow_constructible<T24, U24&&>, std::is_nothrow_constructible<T25, U25&&>, std::is_nothrow_constructible<T26, U26&&>,
                 std::is_nothrow_constructible<T27, U27&&>, std::is_nothrow_constructible<T28, U28&&>, std::is_nothrow_constructible<T29, U29&&>,
                 std::is_nothrow_constructible<T30, U30&&>, std::is_nothrow_constructible<T31, U31&&>, std::is_nothrow_constructible<Ts, Us&&>...>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26),
          _27(static_cast<decltype(other)>(other)._27), _28(static_cast<decltype(other)>(other)._28), _29(static_cast<decltype(other)>(other)._29),
          _30(static_cast<decltype(other)>(other)._30), _31(static_cast<decltype(other)>(other)._31), rest(static_cast<decltype(other)>(other).rest)
    {
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30, class U31, class... Us>
    constexpr explicit tuple_impl(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23, U24,
                                             U25, U26, U27, U28, U29, U30, U31, Us...> const&& other)
        noexcept(std::conjunction_v<
                 std::is_nothrow_constructible<T0, U0 const&&>, std::is_nothrow_constructible<T1, U1 const&&>, std::is_nothrow_constructible<T2, U2 const&&>,
                 std::is_nothrow_constructible<T3, U3 const&&>, std::is_nothrow_constructible<T4, U4 const&&>, std::is_nothrow_constructible<T5, U5 const&&>,
                 std::is_nothrow_constructible<T6, U6 const&&>, std::is_nothrow_constructible<T7, U7 const&&>, std::is_nothrow_constructible<T8, U8 const&&>,
                 std::is_nothrow_constructible<T9, U9 const&&>, std::is_nothrow_constructible<T10, U10 const&&>,
                 std::is_nothrow_constructible<T11, U11 const&&>, std::is_nothrow_constructible<T12, U12 const&&>,
                 std::is_nothrow_constructible<T13, U13 const&&>, std::is_nothrow_constructible<T14, U14 const&&>,
                 std::is_nothrow_constructible<T15, U15 const&&>, std::is_nothrow_constructible<T16, U16 const&&>,
                 std::is_nothrow_constructible<T17, U17 const&&>, std::is_nothrow_constructible<T18, U18 const&&>,
                 std::is_nothrow_constructible<T19, U19 const&&>, std::is_nothrow_constructible<T20, U20 const&&>,
                 std::is_nothrow_constructible<T21, U21 const&&>, std::is_nothrow_constructible<T22, U22 const&&>,
                 std::is_nothrow_constructible<T23, U23 const&&>, std::is_nothrow_constructible<T24, U24 const&&>,
                 std::is_nothrow_constructible<T25, U25 const&&>, std::is_nothrow_constructible<T26, U26 const&&>,
                 std::is_nothrow_constructible<T27, U27 const&&>, std::is_nothrow_constructible<T28, U28 const&&>,
                 std::is_nothrow_constructible<T29, U29 const&&>, std::is_nothrow_constructible<T30, U30 const&&>,
                 std::is_nothrow_constructible<T31, U31 const&&>, std::is_nothrow_constructible<Ts, Us const&&>...>)
        : _0(static_cast<decltype(other)>(other)._0), _1(static_cast<decltype(other)>(other)._1), _2(static_cast<decltype(other)>(other)._2),
          _3(static_cast<decltype(other)>(other)._3), _4(static_cast<decltype(other)>(other)._4), _5(static_cast<decltype(other)>(other)._5),
          _6(static_cast<decltype(other)>(other)._6), _7(static_cast<decltype(other)>(other)._7), _8(static_cast<decltype(other)>(other)._8),
          _9(static_cast<decltype(other)>(other)._9), _10(static_cast<decltype(other)>(other)._10), _11(static_cast<decltype(other)>(other)._11),
          _12(static_cast<decltype(other)>(other)._12), _13(static_cast<decltype(other)>(other)._13), _14(static_cast<decltype(other)>(other)._14),
          _15(static_cast<decltype(other)>(other)._15), _16(static_cast<decltype(other)>(other)._16), _17(static_cast<decltype(other)>(other)._17),
          _18(static_cast<decltype(other)>(other)._18), _19(static_cast<decltype(other)>(other)._19), _20(static_cast<decltype(other)>(other)._20),
          _21(static_cast<decltype(other)>(other)._21), _22(static_cast<decltype(other)>(other)._22), _23(static_cast<decltype(other)>(other)._23),
          _24(static_cast<decltype(other)>(other)._24), _25(static_cast<decltype(other)>(other)._25), _26(static_cast<decltype(other)>(other)._26),
          _27(static_cast<decltype(other)>(other)._27), _28(static_cast<decltype(other)>(other)._28), _29(static_cast<decltype(other)>(other)._29),
          _30(static_cast<decltype(other)>(other)._30), _31(static_cast<decltype(other)>(other)._31), rest(static_cast<decltype(other)>(other).rest)
    {
    }
    constexpr tuple_impl& operator=(tuple_impl const& other)
        noexcept(std::conjunction_v<std::is_nothrow_copy_assignable<T0>, std::is_nothrow_copy_assignable<T1>, std::is_nothrow_copy_assignable<T2>,
                                    std::is_nothrow_copy_assignable<T3>, std::is_nothrow_copy_assignable<T4>, std::is_nothrow_copy_assignable<T5>,
                                    std::is_nothrow_copy_assignable<T6>, std::is_nothrow_copy_assignable<T7>, std::is_nothrow_copy_assignable<T8>,
                                    std::is_nothrow_copy_assignable<T9>, std::is_nothrow_copy_assignable<T10>, std::is_nothrow_copy_assignable<T11>,
                                    std::is_nothrow_copy_assignable<T12>, std::is_nothrow_copy_assignable<T13>, std::is_nothrow_copy_assignable<T14>,
                                    std::is_nothrow_copy_assignable<T15>, std::is_nothrow_copy_assignable<T16>, std::is_nothrow_copy_assignable<T17>,
                                    std::is_nothrow_copy_assignable<T18>, std::is_nothrow_copy_assignable<T19>, std::is_nothrow_copy_assignable<T20>,
                                    std::is_nothrow_copy_assignable<T21>, std::is_nothrow_copy_assignable<T22>, std::is_nothrow_copy_assignable<T23>,
                                    std::is_nothrow_copy_assignable<T24>, std::is_nothrow_copy_assignable<T25>, std::is_nothrow_copy_assignable<T26>,
                                    std::is_nothrow_copy_assignable<T27>, std::is_nothrow_copy_assignable<T28>, std::is_nothrow_copy_assignable<T29>,
                                    std::is_nothrow_copy_assignable<T30>, std::is_nothrow_copy_assignable<T31>, std::is_nothrow_copy_assignable<Ts>...>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        _27 = other._27;
        _28 = other._28;
        _29 = other._29;
        _30 = other._30;
        _31 = other._31;
        rest = other.rest;
        return *this;
    }
    constexpr tuple_impl& operator=(tuple_impl&& other)
        noexcept(std::conjunction_v<std::is_nothrow_move_assignable<T0>, std::is_nothrow_move_assignable<T1>, std::is_nothrow_move_assignable<T2>,
                                    std::is_nothrow_move_assignable<T3>, std::is_nothrow_move_assignable<T4>, std::is_nothrow_move_assignable<T5>,
                                    std::is_nothrow_move_assignable<T6>, std::is_nothrow_move_assignable<T7>, std::is_nothrow_move_assignable<T8>,
                                    std::is_nothrow_move_assignable<T9>, std::is_nothrow_move_assignable<T10>, std::is_nothrow_move_assignable<T11>,
                                    std::is_nothrow_move_assignable<T12>, std::is_nothrow_move_assignable<T13>, std::is_nothrow_move_assignable<T14>,
                                    std::is_nothrow_move_assignable<T15>, std::is_nothrow_move_assignable<T16>, std::is_nothrow_move_assignable<T17>,
                                    std::is_nothrow_move_assignable<T18>, std::is_nothrow_move_assignable<T19>, std::is_nothrow_move_assignable<T20>,
                                    std::is_nothrow_move_assignable<T21>, std::is_nothrow_move_assignable<T22>, std::is_nothrow_move_assignable<T23>,
                                    std::is_nothrow_move_assignable<T24>, std::is_nothrow_move_assignable<T25>, std::is_nothrow_move_assignable<T26>,
                                    std::is_nothrow_move_assignable<T27>, std::is_nothrow_move_assignable<T28>, std::is_nothrow_move_assignable<T29>,
                                    std::is_nothrow_move_assignable<T30>, std::is_nothrow_move_assignable<T31>, std::is_nothrow_move_assignable<Ts>...>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        _27 = static_cast<decltype(other)>(other)._27;
        _28 = static_cast<decltype(other)>(other)._28;
        _29 = static_cast<decltype(other)>(other)._29;
        _30 = static_cast<decltype(other)>(other)._30;
        _31 = static_cast<decltype(other)>(other)._31;
        rest = static_cast<decltype(other)>(other).rest;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30, class U31, class... Us>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23,
                                               U24, U25, U26, U27, U28, U29, U30, U31> const& other)
        noexcept(
            std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                               std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                               std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                               std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                               std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                               std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                               std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                               std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>, std::is_nothrow_assignable<T23&, U23&&>,
                               std::is_nothrow_assignable<T24&, U24&&>, std::is_nothrow_assignable<T25&, U25&&>, std::is_nothrow_assignable<T26&, U26&&>,
                               std::is_nothrow_assignable<T27&, U27&&>, std::is_nothrow_assignable<T28&, U28&&>, std::is_nothrow_assignable<T29&, U29&&>,
                               std::is_nothrow_assignable<T30&, U30&&>, std::is_nothrow_assignable<T31&, U31&&>, std::is_nothrow_assignable<Ts&, Us const&>...>)
    {
        _0 = other._0;
        _1 = other._1;
        _2 = other._2;
        _3 = other._3;
        _4 = other._4;
        _5 = other._5;
        _6 = other._6;
        _7 = other._7;
        _8 = other._8;
        _9 = other._9;
        _10 = other._10;
        _11 = other._11;
        _12 = other._12;
        _13 = other._13;
        _14 = other._14;
        _15 = other._15;
        _16 = other._16;
        _17 = other._17;
        _18 = other._18;
        _19 = other._19;
        _20 = other._20;
        _21 = other._21;
        _22 = other._22;
        _23 = other._23;
        _24 = other._24;
        _25 = other._25;
        _26 = other._26;
        _27 = other._27;
        _28 = other._28;
        _29 = other._29;
        _30 = other._30;
        _31 = other._31;
        rest = other.rest;
        return *this;
    }
    template<class U0, class U1, class U2, class U3, class U4, class U5, class U6, class U7, class U8, class U9, class U10, class U11, class U12, class U13,
             class U14, class U15, class U16, class U17, class U18, class U19, class U20, class U21, class U22, class U23, class U24, class U25, class U26,
             class U27, class U28, class U29, class U30, class U31, class... Us>
    constexpr tuple_impl& operator=(tuple_impl<U0, U1, U2, U3, U4, U5, U6, U7, U8, U9, U10, U11, U12, U13, U14, U15, U16, U17, U18, U19, U20, U21, U22, U23,
                                               U24, U25, U26, U27, U28, U29, U30, U31>&& other)
        noexcept(std::conjunction_v<std::is_nothrow_assignable<T0&, U0&&>, std::is_nothrow_assignable<T1&, U1&&>, std::is_nothrow_assignable<T2&, U2&&>,
                                    std::is_nothrow_assignable<T3&, U3&&>, std::is_nothrow_assignable<T4&, U4&&>, std::is_nothrow_assignable<T5&, U5&&>,
                                    std::is_nothrow_assignable<T6&, U6&&>, std::is_nothrow_assignable<T7&, U7&&>, std::is_nothrow_assignable<T8&, U8&&>,
                                    std::is_nothrow_assignable<T9&, U9&&>, std::is_nothrow_assignable<T10&, U10&&>, std::is_nothrow_assignable<T11&, U11&&>,
                                    std::is_nothrow_assignable<T12&, U12&&>, std::is_nothrow_assignable<T13&, U13&&>, std::is_nothrow_assignable<T14&, U14&&>,
                                    std::is_nothrow_assignable<T15&, U15&&>, std::is_nothrow_assignable<T16&, U16&&>, std::is_nothrow_assignable<T17&, U17&&>,
                                    std::is_nothrow_assignable<T18&, U18&&>, std::is_nothrow_assignable<T19&, U19&&>, std::is_nothrow_assignable<T20&, U20&&>,
                                    std::is_nothrow_assignable<T21&, U21&&>, std::is_nothrow_assignable<T22&, U22&&>, std::is_nothrow_assignable<T23&, U23&&>,
                                    std::is_nothrow_assignable<T24&, U24&&>, std::is_nothrow_assignable<T25&, U25&&>, std::is_nothrow_assignable<T26&, U26&&>,
                                    std::is_nothrow_assignable<T27&, U27&&>, std::is_nothrow_assignable<T28&, U28&&>, std::is_nothrow_assignable<T29&, U29&&>,
                                    std::is_nothrow_assignable<T30&, U30&&>, std::is_nothrow_assignable<T31&, U31&&>, std::is_nothrow_assignable<Ts&, Us&&>...>)
    {
        _0 = static_cast<decltype(other)>(other)._0;
        _1 = static_cast<decltype(other)>(other)._1;
        _2 = static_cast<decltype(other)>(other)._2;
        _3 = static_cast<decltype(other)>(other)._3;
        _4 = static_cast<decltype(other)>(other)._4;
        _5 = static_cast<decltype(other)>(other)._5;
        _6 = static_cast<decltype(other)>(other)._6;
        _7 = static_cast<decltype(other)>(other)._7;
        _8 = static_cast<decltype(other)>(other)._8;
        _9 = static_cast<decltype(other)>(other)._9;
        _10 = static_cast<decltype(other)>(other)._10;
        _11 = static_cast<decltype(other)>(other)._11;
        _12 = static_cast<decltype(other)>(other)._12;
        _13 = static_cast<decltype(other)>(other)._13;
        _14 = static_cast<decltype(other)>(other)._14;
        _15 = static_cast<decltype(other)>(other)._15;
        _16 = static_cast<decltype(other)>(other)._16;
        _17 = static_cast<decltype(other)>(other)._17;
        _18 = static_cast<decltype(other)>(other)._18;
        _19 = static_cast<decltype(other)>(other)._19;
        _20 = static_cast<decltype(other)>(other)._20;
        _21 = static_cast<decltype(other)>(other)._21;
        _22 = static_cast<decltype(other)>(other)._22;
        _23 = static_cast<decltype(other)>(other)._23;
        _24 = static_cast<decltype(other)>(other)._24;
        _25 = static_cast<decltype(other)>(other)._25;
        _26 = static_cast<decltype(other)>(other)._26;
        _27 = static_cast<decltype(other)>(other)._27;
        _28 = static_cast<decltype(other)>(other)._28;
        _29 = static_cast<decltype(other)>(other)._29;
        _30 = static_cast<decltype(other)>(other)._30;
        _31 = static_cast<decltype(other)>(other)._31;
        rest = static_cast<decltype(other)>(other).rest;
        return *this;
    }
    template<class UTuple>
    constexpr tuple_impl& operator=(UTuple&& other)
    {
        [&, this]<std::size_t... Is>(std::index_sequence<Is...>) { assign(alloy::get<Is>(static_cast<UTuple>(other))...); }(std::index_sequence_for<Ts...>{});
        return *this;
    }
    constexpr void swap(tuple_impl& other) noexcept(
        std::conjunction_v<std::is_nothrow_swappable<T0>, std::is_nothrow_swappable<T1>, std::is_nothrow_swappable<T2>, std::is_nothrow_swappable<T3>,
                           std::is_nothrow_swappable<T4>, std::is_nothrow_swappable<T5>, std::is_nothrow_swappable<T6>, std::is_nothrow_swappable<T7>,
                           std::is_nothrow_swappable<T8>, std::is_nothrow_swappable<T9>, std::is_nothrow_swappable<T10>, std::is_nothrow_swappable<T11>,
                           std::is_nothrow_swappable<T12>, std::is_nothrow_swappable<T13>, std::is_nothrow_swappable<T14>, std::is_nothrow_swappable<T15>,
                           std::is_nothrow_swappable<T16>, std::is_nothrow_swappable<T17>, std::is_nothrow_swappable<T18>, std::is_nothrow_swappable<T19>,
                           std::is_nothrow_swappable<T20>, std::is_nothrow_swappable<T21>, std::is_nothrow_swappable<T22>, std::is_nothrow_swappable<T23>,
                           std::is_nothrow_swappable<T24>, std::is_nothrow_swappable<T25>, std::is_nothrow_swappable<T26>, std::is_nothrow_swappable<T27>,
                           std::is_nothrow_swappable<T28>, std::is_nothrow_swappable<T29>, std::is_nothrow_swappable<T30>, std::is_nothrow_swappable<T31>,
                           std::is_nothrow_swappable<Ts>...>)
    {
        static_assert(std::conjunction_v<std::is_swappable<T0>, std::is_swappable<T1>, std::is_swappable<T2>, std::is_swappable<T3>, std::is_swappable<T4>,
                                         std::is_swappable<T5>, std::is_swappable<T6>, std::is_swappable<T7>, std::is_swappable<T8>, std::is_swappable<T9>,
                                         std::is_swappable<T10>, std::is_swappable<T11>, std::is_swappable<T12>, std::is_swappable<T13>, std::is_swappable<T14>,
                                         std::is_swappable<T15>, std::is_swappable<T16>, std::is_swappable<T17>, std::is_swappable<T18>, std::is_swappable<T19>,
                                         std::is_swappable<T20>, std::is_swappable<T21>, std::is_swappable<T22>, std::is_swappable<T23>, std::is_swappable<T24>,
                                         std::is_swappable<T25>, std::is_swappable<T26>, std::is_swappable<T27>, std::is_swappable<T28>, std::is_swappable<T29>,
                                         std::is_swappable<T30>, std::is_swappable<T31>, std::is_swappable<Ts>...>);
        using std::swap;
        swap(_0, other._0);
        swap(_1, other._1);
        swap(_2, other._2);
        swap(_3, other._3);
        swap(_4, other._4);
        swap(_5, other._5);
        swap(_6, other._6);
        swap(_7, other._7);
        swap(_8, other._8);
        swap(_9, other._9);
        swap(_10, other._10);
        swap(_11, other._11);
        swap(_12, other._12);
        swap(_13, other._13);
        swap(_14, other._14);
        swap(_15, other._15);
        swap(_16, other._16);
        swap(_17, other._17);
        swap(_18, other._18);
        swap(_19, other._19);
        swap(_20, other._20);
        swap(_21, other._21);
        swap(_22, other._22);
        swap(_23, other._23);
        swap(_24, other._24);
        swap(_25, other._25);
        swap(_26, other._26);
        swap(_27, other._27);
        swap(_28, other._28);
        swap(_29, other._29);
        swap(_30, other._30);
        swap(_31, other._31);
        rest.swap(other.rest);
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29, T30, T31, Ts...>&
    get() & noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
        else if constexpr (I == 27)
            return _27;
        else if constexpr (I == 28)
            return _28;
        else if constexpr (I == 29)
            return _29;
        else if constexpr (I == 30)
            return _30;
        else if constexpr (I == 31)
            return _31;
        else
            return rest.template get<I - 32>();
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29, T30, T31, Ts...> const&
    get() const& noexcept
    {
        if constexpr (I == 0)
            return _0;
        else if constexpr (I == 1)
            return _1;
        else if constexpr (I == 2)
            return _2;
        else if constexpr (I == 3)
            return _3;
        else if constexpr (I == 4)
            return _4;
        else if constexpr (I == 5)
            return _5;
        else if constexpr (I == 6)
            return _6;
        else if constexpr (I == 7)
            return _7;
        else if constexpr (I == 8)
            return _8;
        else if constexpr (I == 9)
            return _9;
        else if constexpr (I == 10)
            return _10;
        else if constexpr (I == 11)
            return _11;
        else if constexpr (I == 12)
            return _12;
        else if constexpr (I == 13)
            return _13;
        else if constexpr (I == 14)
            return _14;
        else if constexpr (I == 15)
            return _15;
        else if constexpr (I == 16)
            return _16;
        else if constexpr (I == 17)
            return _17;
        else if constexpr (I == 18)
            return _18;
        else if constexpr (I == 19)
            return _19;
        else if constexpr (I == 20)
            return _20;
        else if constexpr (I == 21)
            return _21;
        else if constexpr (I == 22)
            return _22;
        else if constexpr (I == 23)
            return _23;
        else if constexpr (I == 24)
            return _24;
        else if constexpr (I == 25)
            return _25;
        else if constexpr (I == 26)
            return _26;
        else if constexpr (I == 27)
            return _27;
        else if constexpr (I == 28)
            return _28;
        else if constexpr (I == 29)
            return _29;
        else if constexpr (I == 30)
            return _30;
        else if constexpr (I == 31)
            return _31;
        else
            return rest.template get<I - 32>();
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29, T30, T31, Ts...>&&
    get() && noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0)&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1)&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2)&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3)&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4)&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5)&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6)&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7)&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8)&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9)&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10)&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11)&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12)&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13)&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14)&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15)&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16)&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17)&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18)&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19)&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20)&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21)&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22)&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23)&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24)&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25)&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26)&&>(_26);
        else if constexpr (I == 27)
            return static_cast<decltype(_27)&&>(_27);
        else if constexpr (I == 28)
            return static_cast<decltype(_28)&&>(_28);
        else if constexpr (I == 29)
            return static_cast<decltype(_29)&&>(_29);
        else if constexpr (I == 30)
            return static_cast<decltype(_30)&&>(_30);
        else if constexpr (I == 31)
            return static_cast<decltype(_31)&&>(_31);
        else
            return std::move(rest).template get<I - 32>();
    }
    template<std::size_t I>
    constexpr type_pack_indexing_t<I, T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25,
                                   T26, T27, T28, T29, T30, T31, Ts...> const&&
    get() const&& noexcept
    {
        if constexpr (I == 0)
            return static_cast<decltype(_0) const&&>(_0);
        else if constexpr (I == 1)
            return static_cast<decltype(_1) const&&>(_1);
        else if constexpr (I == 2)
            return static_cast<decltype(_2) const&&>(_2);
        else if constexpr (I == 3)
            return static_cast<decltype(_3) const&&>(_3);
        else if constexpr (I == 4)
            return static_cast<decltype(_4) const&&>(_4);
        else if constexpr (I == 5)
            return static_cast<decltype(_5) const&&>(_5);
        else if constexpr (I == 6)
            return static_cast<decltype(_6) const&&>(_6);
        else if constexpr (I == 7)
            return static_cast<decltype(_7) const&&>(_7);
        else if constexpr (I == 8)
            return static_cast<decltype(_8) const&&>(_8);
        else if constexpr (I == 9)
            return static_cast<decltype(_9) const&&>(_9);
        else if constexpr (I == 10)
            return static_cast<decltype(_10) const&&>(_10);
        else if constexpr (I == 11)
            return static_cast<decltype(_11) const&&>(_11);
        else if constexpr (I == 12)
            return static_cast<decltype(_12) const&&>(_12);
        else if constexpr (I == 13)
            return static_cast<decltype(_13) const&&>(_13);
        else if constexpr (I == 14)
            return static_cast<decltype(_14) const&&>(_14);
        else if constexpr (I == 15)
            return static_cast<decltype(_15) const&&>(_15);
        else if constexpr (I == 16)
            return static_cast<decltype(_16) const&&>(_16);
        else if constexpr (I == 17)
            return static_cast<decltype(_17) const&&>(_17);
        else if constexpr (I == 18)
            return static_cast<decltype(_18) const&&>(_18);
        else if constexpr (I == 19)
            return static_cast<decltype(_19) const&&>(_19);
        else if constexpr (I == 20)
            return static_cast<decltype(_20) const&&>(_20);
        else if constexpr (I == 21)
            return static_cast<decltype(_21) const&&>(_21);
        else if constexpr (I == 22)
            return static_cast<decltype(_22) const&&>(_22);
        else if constexpr (I == 23)
            return static_cast<decltype(_23) const&&>(_23);
        else if constexpr (I == 24)
            return static_cast<decltype(_24) const&&>(_24);
        else if constexpr (I == 25)
            return static_cast<decltype(_25) const&&>(_25);
        else if constexpr (I == 26)
            return static_cast<decltype(_26) const&&>(_26);
        else if constexpr (I == 27)
            return static_cast<decltype(_27) const&&>(_27);
        else if constexpr (I == 28)
            return static_cast<decltype(_28) const&&>(_28);
        else if constexpr (I == 29)
            return static_cast<decltype(_29) const&&>(_29);
        else if constexpr (I == 30)
            return static_cast<decltype(_30) const&&>(_30);
        else if constexpr (I == 31)
            return static_cast<decltype(_31) const&&>(_31);
        else
            return std::move(rest).template get<I - 32>();
    }
};
}
}

#endif
