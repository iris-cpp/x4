#ifndef IRIS_ZZ_X4_CHAR_DETAIL_CHSET_HPP
#define IRIS_ZZ_X4_CHAR_DETAIL_CHSET_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/error/throwf.hpp>

#include <algorithm>
#include <array>
#include <bitset>
#include <limits>
#include <stdexcept>
#include <type_traits>

#include <climits>
#include <cstddef> // IWYU pragma: keep

namespace iris::x4::detail {

// [first, last]
template<class ClassifyT>
struct char_range
{
    ClassifyT first;
    ClassifyT last;
};

template<class ClassifyT>
[[nodiscard]] constexpr std::size_t
insert_char_range(char_range<ClassifyT>* const ranges, std::size_t const size, ClassifyT const from, ClassifyT const to) noexcept
{
    using limits = std::numeric_limits<ClassifyT>;
    ClassifyT const lower = from == (limits::min)() ? from : static_cast<ClassifyT>(from - 1);
    ClassifyT const upper = to == (limits::max)() ? to : static_cast<ClassifyT>(to + 1);

    // [merge_first, merge_last) overlap with or are adjacent to [from, to]
    auto* const last = ranges + size;
    auto* const merge_first = std::ranges::partition_point(ranges, last, [&](char_range<ClassifyT> const& range) { return range.last < lower; });
    auto* const merge_last = std::ranges::partition_point(merge_first, last, [&](char_range<ClassifyT> const& range) { return range.first <= upper; });

    if (merge_first == merge_last) {
        std::ranges::move_backward(merge_first, last, last + 1);
        *merge_first = {from, to};
        return size + 1;
    }

    merge_first->first = std::min(merge_first->first, from);
    merge_first->last = std::max((merge_last - 1)->last, to);
    std::ranges::move(merge_last, last, merge_first + 1);
    return size - static_cast<std::size_t>(merge_last - merge_first - 1);
}

template<class ClassifyT, std::size_t Capacity>
struct ranged_chset
{
    [[nodiscard]] constexpr bool test(ClassifyT const ch) const noexcept
    {
        auto const* const last = ranges_.data() + size_;
        auto const* const it = std::ranges::partition_point(ranges_.data(), last, [&](char_range<ClassifyT> const& range) { return range.last < ch; });
        return it != last && it->first <= ch;
    }

    constexpr void set(std::same_as<ClassifyT> auto const from, std::same_as<ClassifyT> auto const to)
    {
        if (size_ == Capacity) {
            iris::throwf<std::length_error>("the character set has no room for another range");
        }
        size_ = detail::insert_char_range(ranges_.data(), size_, from, to);
    }

    template<class F>
    constexpr void for_each_range(F&& f) const
    {
        for (std::size_t i = 0; i < size_; ++i) {
            f(ranges_[i].first, ranges_[i].last);
        }
    }

private:
    std::array<char_range<ClassifyT>, Capacity> ranges_{};
    std::size_t size_ = 0;
};

struct bitset_chset
{
    [[nodiscard]] constexpr bool test(unsigned char const ch) const noexcept
    {
        return bits_[ch];
    }

    constexpr void set(unsigned char const from, unsigned char const to) noexcept
    {
        for (unsigned ch = from; ch <= to; ++ch) {
            bits_.set(ch);
        }
    }

    template<class F>
    constexpr void for_each_range(F&& f) const
    {
        for (unsigned first = 0; first <= UCHAR_MAX; ++first) {
            if (!bits_[first]) continue;

            unsigned last = first;
            while (last < UCHAR_MAX && bits_[last + 1]) {
                ++last;
            }
            f(static_cast<unsigned char>(first), static_cast<unsigned char>(last));
            first = last;
        }
    }

private:
    std::bitset<UCHAR_MAX + 1> bits_;
};

template<class ClassifyT, std::size_t Length>
using chset_for = std::conditional_t<
    std::is_same_v<ClassifyT, unsigned char>,
    bitset_chset,
    ranged_chset<ClassifyT, Length>
>;

} // iris::x4::detail

#endif
