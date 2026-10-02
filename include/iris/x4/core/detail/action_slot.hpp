#ifndef IRIS_ZZ_X4_CORE_DETAIL_ACTION_SLOT_HPP
#define IRIS_ZZ_X4_CORE_DETAIL_ACTION_SLOT_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/config.hpp> // IWYU pragma: keep

#include <iris/bits/specialization_of.hpp>
#include <iris/rvariant/recursive_wrapper.hpp>

#include <optional>
#include <type_traits>
#include <utility>

namespace iris::x4::detail {

// The attribute of the subject of a semantic action which may succeed without writing it, and
// whether the match wrote it.
//
// The action owns the slot and passes it to the subject as its attribute; it goes through the
// parsers which pass the attribute through, down to the alternative whose selected branch decides
// whether the attribute is written.
template<class A>
class action_slot
{
public:
    using attribute_type = A;

    // A new attribute in its default state, into which the branch writes
    [[nodiscard]] constexpr A& engage()
    {
        return attr_.emplace();
    }

    // No attribute, as a branch without an attribute succeeds (or a branch failed)
    constexpr void disengage() noexcept
    {
        attr_.reset();
    }

    [[nodiscard]] constexpr bool is_generated() const noexcept
    {
        return attr_.has_value();
    }

    [[nodiscard]] constexpr A& value() noexcept
    {
        return *attr_;
    }

private:
    std::optional<A> attr_;
};

template<class T>
inline constexpr bool is_action_slot_v = is_ttp_specialization_of_v<std::remove_const_t<T>, action_slot>;

// Calls `writer` with the value in `slot`, moved.
//   - `IsMultiCand`: the value is a variant of the attributes of the branches of `a | b | ...`.
//     `write` is called with the one it holds, i.e. the attribute of the branch that matched.
//   - Otherwise: `writer` is called with the value itself.
// A variant declared by `rule` or `as<T>` is the attribute of one branch, so it is passed whole.
template<bool IsMultiCand, class A, class WriterF>
constexpr void write_slot_value(action_slot<A>& slot, WriterF const& writer)
{
    if constexpr (IsMultiCand) {
        slot.value().visit([&writer](auto& candidate) { writer(std::move(iris::unwrap_recursive(candidate))); });

    } else {
        writer(std::move(slot.value()));
    }
}

} // iris::x4::detail

#endif
