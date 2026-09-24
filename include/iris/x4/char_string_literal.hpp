#ifndef IRIS_ZZ_X4_CHAR_STRING_LITERAL_HPP
#define IRIS_ZZ_X4_CHAR_STRING_LITERAL_HPP

/*=============================================================================
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/char/literal_char.hpp>
#include <iris/x4/string/literal_string.hpp>

#include <iris/x4/core/traits/char_encoding_traits.hpp>
#include <iris/x4/core/unused.hpp>
#include <iris/x4/core/parser.hpp>

#include <iris/bits/specialization_of.hpp>

#include <algorithm>
#include <array>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

#include <cstddef> // IWYU pragma: keep

namespace iris::x4 {

namespace detail {

template<class CharT, std::size_t N>
concept is_string_parser_sso_eligible =
    CharLike<CharT> &&
    sizeof(CharT) * (N - 1) // size when directly storing null-omitted chars as an array
    < sizeof(std::basic_string_view<CharT>);

template<template<class> class AttrSelectorTT>
struct char_parser_fn
{
    template<CharLike CharT>
    [[nodiscard]] static constexpr literal_char<
        char_encoding_for<CharT>,
        typename AttrSelectorTT<CharT>::type
    >
    operator()(CharT ch) noexcept
    {
        return {ch};
    }
};

template<bool IsForceString /* false */, template<class> class AttrSelectorTT>
struct char_array_parser_fn
{
    template<CharLike CharT>
    [[nodiscard]] static constexpr literal_char<
        char_encoding_for<CharT>,
        typename AttrSelectorTT<CharT>::type
    >
    operator()(CharT const (&str)[2]) noexcept
    {
        return {str[0]};
    }

    template<CharLike CharT, std::size_t N>
    [[nodiscard]] static constexpr literal_string<
        std::array<CharT, N - 1>,
        char_encoding_for<CharT>,
        typename AttrSelectorTT<std::basic_string<CharT>>::type
    >
    operator()(CharT const (&str)[N]) noexcept
        requires (N >= 3) && x4::detail::is_string_parser_sso_eligible<CharT, N>
    {
        std::array<CharT, N - 1> arr;
        std::ranges::copy_n(str, N - 1, arr.data());
        return {arr};
    }

    template<CharLike CharT, std::size_t N>
    [[nodiscard]] static constexpr literal_string<
        std::basic_string_view<CharT>,
        char_encoding_for<CharT>,
        typename AttrSelectorTT<std::basic_string<CharT>>::type
    >
    operator()(CharT const (&str)[N]) noexcept
        requires (N >= 3) && (!x4::detail::is_string_parser_sso_eligible<CharT, N>)
    {
        static_assert(N >= 2);
        return {std::basic_string_view<CharT>(str, N - 1)};
    }
};

template<>
struct char_array_parser_fn<true, attribute_identity_switcher>
{
    template<CharLike CharT>
    [[nodiscard]] static constexpr literal_string<
        std::array<CharT, 1>,
        char_encoding_for<CharT>,
        std::basic_string<CharT>
    >
    operator()(CharT const (&str)[2]) noexcept
    {
        return literal_string<
            std::array<CharT, 1>,
            char_encoding_for<CharT>,
            std::basic_string<CharT>
        >{std::array{str[0]}};
    }

    template<CharLike CharT, std::size_t N>
    [[nodiscard]] static constexpr literal_string<
        std::array<CharT, N - 1>,
        char_encoding_for<CharT>,
        std::basic_string<CharT>
    >
    operator()(CharT const (&str)[N]) noexcept
        requires (N >= 3) && x4::detail::is_string_parser_sso_eligible<CharT, N>
    {
        std::array<CharT, N - 1> arr;
        std::ranges::copy_n(str, N - 1, arr.data());
        return {arr};
    }

    template<CharLike CharT, std::size_t N>
    [[nodiscard]] static constexpr literal_string<
        std::basic_string_view<CharT>,
        char_encoding_for<CharT>,
        std::basic_string<CharT>
    >
    operator()(CharT const (&str)[N]) noexcept
        requires (N >= 3) && (!x4::detail::is_string_parser_sso_eligible<CharT, N>)
    {
        static_assert(N >= 2);
        return {std::basic_string_view<CharT>(str, N - 1)};
    }
};

//template<template<class> class AttrSelectorTT>
//struct char_pointer_parser_fn
//{
//    template<CharLike CharT>
//    [[nodiscard]] static constexpr literal_string<
//        std::basic_string_view<CharT>,
//        char_encoding_for<CharT>,
//        typename AttrSelectorTT<std::basic_string<CharT>>::type
//    >
//    operator()(CharT const* s) noexcept
//    {
//        return {std::basic_string_view{s}};
//    }
//};

template<template<class> class AttrSelectorTT>
struct string_parser_fn
{
    template<StringLike StringLikeT>
        requires is_ttp_specialization_of_v<std::remove_cvref_t<StringLikeT>, std::basic_string>
    [[nodiscard]] static constexpr literal_string<
        std::basic_string<char_type_for<StringLikeT>>,
        char_encoding_for<char_type_for<StringLikeT>>,
        typename AttrSelectorTT<std::basic_string<char_type_for<StringLikeT>>>::type
    >
    operator()(StringLikeT&& str)
        noexcept(std::is_nothrow_constructible_v<
            literal_string<
                std::basic_string<char_type_for<StringLikeT>>,
                char_encoding_for<char_type_for<StringLikeT>>,
                typename AttrSelectorTT<std::basic_string<char_type_for<StringLikeT>>>::type
            >,
            StringLikeT
        >)
    {
        return {std::forward<StringLikeT>(str)};
    }
};

template<template<class> class AttrSelectorTT>
struct string_view_parser_fn
{
    template<StringLike StringLikeT>
        requires is_ttp_specialization_of_v<std::remove_cvref_t<StringLikeT>, std::basic_string_view>
    [[nodiscard]] static constexpr literal_string<
        std::basic_string_view<char_type_for<StringLikeT>>,
        char_encoding_for<char_type_for<StringLikeT>>,
        typename AttrSelectorTT<std::basic_string<char_type_for<StringLikeT>>>::type
    >
    operator()(StringLikeT&& str)
        noexcept(std::is_nothrow_constructible_v<
            literal_string<
                std::basic_string_view<char_type_for<StringLikeT>>,
                char_encoding_for<char_type_for<StringLikeT>>,
                typename AttrSelectorTT<std::basic_string<char_type_for<StringLikeT>>>::type
            >,
            StringLikeT
        >)
    {
        return {std::forward<StringLikeT>(str)};
    }
};

} // detail


namespace traits {

template<CharLike CharT>
struct as_parser<CharT>
    : x4::detail::char_parser_fn<x4::detail::attribute_unused_switcher>
{};

template<>
struct as_parser<x4::detail::as_parser_char_array_tag>
    : x4::detail::char_array_parser_fn<false, x4::detail::attribute_unused_switcher>
{};

//template<CharLike CharT>
//struct as_parser<CharT const*>
//    : x4::detail::char_pointer_parser_fn<x4::detail::attribute_unused_switcher>
//{};

template<class CharT>
struct as_parser<std::basic_string<CharT>>
    : x4::detail::string_parser_fn<x4::detail::attribute_unused_switcher>
{};

template<class CharT>
struct as_parser<std::basic_string_view<CharT>>
    : x4::detail::string_view_parser_fn<x4::detail::attribute_unused_switcher>
{};

} // traits


namespace detail {

template<bool NeedAttribute /* false */>
struct lit_string_fn
    : char_parser_fn<attribute_unused_switcher>
    , char_array_parser_fn<false, attribute_unused_switcher>
    //, char_pointer_parser_fn<attribute_unused_switcher>
    , string_parser_fn<attribute_unused_switcher>
    , string_view_parser_fn<attribute_unused_switcher>
{
    using char_parser_fn::operator();
    using char_array_parser_fn::operator();
    using string_parser_fn::operator();
    using string_view_parser_fn::operator();
};

template<>
struct lit_string_fn<true>
    : char_parser_fn<attribute_identity_switcher>
    , char_array_parser_fn<true, attribute_identity_switcher>
    //, char_pointer_parser_fn<attribute_identity_switcher>
    , string_parser_fn<attribute_identity_switcher>
    , string_view_parser_fn<attribute_identity_switcher>
{
    using char_parser_fn::operator();
    using char_array_parser_fn::operator();
    using string_parser_fn::operator();
    using string_view_parser_fn::operator();
};

} // detail

namespace parsers {
[[maybe_unused]] inline constexpr detail::lit_string_fn<false> lit{};
[[maybe_unused]] inline constexpr detail::lit_string_fn<true> string{};
} // parsers

using parsers::lit;
using parsers::string;


namespace standard {
[[maybe_unused]] inline constexpr auto const& lit [[deprecated("use `x4::lit`")]] = x4::lit;
[[maybe_unused]] inline constexpr auto const& string [[deprecated("use `x4::string`")]] = x4::string;
} // standard

#ifndef IRIS_X4_NO_STANDARD_WIDE
namespace standard_wide {
[[maybe_unused]] inline constexpr auto const& lit [[deprecated("use `x4::lit`")]] = x4::lit;
[[maybe_unused]] inline constexpr auto const& string [[deprecated("use `x4::string`")]] = x4::string;
} // standard_wide
#endif // IRIS_X4_NO_STANDARD_WIDE

#ifdef IRIS_X4_UNICODE
namespace unicode {
[[maybe_unused]] inline constexpr auto const& lit [[deprecated("use `x4::lit`")]] = x4::lit;
[[maybe_unused]] inline constexpr auto const& string [[deprecated("use `x4::string`")]] = x4::string;
} // unicode
#endif // IRIS_X4_UNICODE

} // iris::x4

#endif
