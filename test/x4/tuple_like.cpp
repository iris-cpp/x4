#include "iris_x4_test.hpp"

#include <iris/x4/core/traits/tuple_traits.hpp>

#include <iris/alloy/tuple.hpp>
#include <iris/alloy/adapt.hpp>

#include <concepts>
#include <utility>

namespace {

struct adapted_int
{
    int value = 0;
};

template<class Ref>
struct fixed_ref
{
    int* target = nullptr;
};

template<class Ref>
struct fixed_ref_getter
{
    template<class Self>
    constexpr Ref operator()(Self&& self) const noexcept
    {
        return static_cast<Ref>(*self.target);
    }
};

template<class T>
using unwrapped_t = decltype(x4::unwrap_single_element(std::declval<T>()));

} // anonymous

IRIS_ALLOY_ADAPT_STRUCT(adapted_int, value);

template<class Ref>
struct iris::alloy::adaptor<fixed_ref<Ref>>
{
    using getters_list = iris::constant_list<fixed_ref_getter<Ref>{}>;
};

TEST_CASE("unwrap_single_element")
{
    using alloy::tuple;

    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int>&>, int&>);
    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int> const&>, int const&>);
    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int>&&>, int&&>);
    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int> const&&>, int const&&>);

    STATIC_CHECK(std::same_as<unwrapped_t<adapted_int&>, int&>);
    STATIC_CHECK(std::same_as<unwrapped_t<adapted_int const&>, int const&>);
    STATIC_CHECK(std::same_as<unwrapped_t<adapted_int&&>, int&&>);
    STATIC_CHECK(std::same_as<unwrapped_t<adapted_int const&&>, int const&&>);

    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int&>&>, int&>);
    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int&> const&>, int&>);
    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int&>&&>, int&>);
    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int&> const&&>, int&>);

    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int const&>&>, int const&>);
    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int const&> const&>, int const&>);
    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int const&>&&>, int const&>);
    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int const&> const&&>, int const&>);

    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int&>&>, int&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int&> const&>, int&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int&>&&>, int&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int&> const&&>, int&>);

    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int const&>&>, int const&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int const&> const&>, int const&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int const&>&&>, int const&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int const&> const&&>, int const&>);

    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int&&>&>, int&&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int&&> const&>, int&&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int&&>&&>, int&&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int&&> const&&>, int&&>);

    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int const&&>&>, int const&&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int const&&> const&>, int const&&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int const&&>&&>, int const&&>);
    STATIC_CHECK(std::same_as<unwrapped_t<fixed_ref<int const&&> const&&>, int const&&>);

    STATIC_CHECK(std::same_as<unwrapped_t<int&>, int&>);
    STATIC_CHECK(std::same_as<unwrapped_t<int const&>, int const&>);
    STATIC_CHECK(std::same_as<unwrapped_t<int&&>, int&&>);
    STATIC_CHECK(std::same_as<unwrapped_t<int const&&>, int const&&>);
    STATIC_CHECK(std::same_as<unwrapped_t<tuple<int, int>&>, tuple<int, int>&>);

    STATIC_CHECK(std::same_as<x4::unwrap_single_element_t<tuple<int&> const&>, int>);
    STATIC_CHECK(std::same_as<x4::unwrap_single_element_t<adapted_int&&>, int>);
    STATIC_CHECK(std::same_as<x4::unwrap_single_element_t<fixed_ref<int const&&>>, int>);
    STATIC_CHECK(std::same_as<x4::unwrap_single_element_t<tuple<tuple<int>>>, tuple<int>>);
    STATIC_CHECK(std::same_as<x4::unwrap_single_element_t<int const&>, int>);

    {
        tuple<int> t{42};
        CHECK(&x4::unwrap_single_element(t) == &alloy::get<0>(t));

        int n = 42;
        tuple<int&> const ref_tuple{n};
        CHECK(&x4::unwrap_single_element(ref_tuple) == &n);
        CHECK(&x4::unwrap_single_element(fixed_ref<int&>{&n}) == &n);
        CHECK(&x4::unwrap_single_element(n) == &n);
    }
}
