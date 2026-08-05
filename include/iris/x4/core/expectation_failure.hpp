#ifndef IRIS_ZZ_X4_CORE_EXPECTATION_FAILURE_HPP
#define IRIS_ZZ_X4_CORE_EXPECTATION_FAILURE_HPP

/*=============================================================================
    Copyright (c) 2017 wanghan02
    Copyright (c) 2024-2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

// This header is intended for inclusion by user-facing, non-parser headers.
// Do not add includes specific to X4's parser implementation here.

#include <iris/config.hpp>

#include <iterator>
#include <utility>
#include <type_traits>
#include <string>

#include <cassert>

namespace iris::x4 {

template<std::forward_iterator It>
struct expectation_failure
{
    constexpr expectation_failure() = default;

    template<class WhichT>
        requires std::is_constructible_v<std::string, WhichT>
    constexpr expectation_failure(It where, WhichT&& which)
        noexcept(std::is_nothrow_copy_constructible_v<It> && std::is_nothrow_constructible_v<std::string, WhichT>)
        : where_(where)
        , which_(std::forward<WhichT>(which))
    {
        if (which_.empty()) {
            which_ = "(unknown location)";
        }
    }

    [[nodiscard]]
    constexpr It const& where() const noexcept
    {
        assert(this->has_value());
        return where_;
    }

    [[nodiscard]]
    constexpr std::string const& which() const noexcept
    {
        assert(this->has_value());
        return which_;
    }

    constexpr void clear() noexcept
    {
        which_.clear();
    }

    template<class WhichT>
        requires std::is_constructible_v<std::string, WhichT>
    constexpr void emplace(It where, WhichT&& which)
        noexcept(std::is_nothrow_move_assignable_v<It> && std::is_nothrow_assignable_v<std::string&, WhichT>)
    {
        where_ = std::move(where);
        which_ = std::forward<WhichT>(which);
    }

    [[nodiscard]] constexpr explicit operator bool() const noexcept { return !which_.empty(); }
    [[nodiscard]] constexpr bool has_value() const noexcept { return !which_.empty(); }

    constexpr void swap(expectation_failure& other)
        noexcept(std::is_nothrow_swappable_v<It> && std::is_nothrow_swappable_v<std::string>)
    {
        using std::swap;
        swap(where_, other.where_);
        swap(which_, other.which_);
    }

private:
    It where_{};
    std::string which_;
};

template<std::forward_iterator It>
constexpr void swap(expectation_failure<It>& a, expectation_failure<It>& b)
    noexcept(std::is_nothrow_swappable_v<It> && std::is_nothrow_swappable_v<std::string>)
{
    a.swap(b);
}

} // iris::x4

#endif
