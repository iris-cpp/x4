#ifndef IRIS_X4_CHAR_CHAR_CLASS_HPP
#define IRIS_X4_CHAR_CHAR_CLASS_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka
    Copyright (c) 2026 The Iris Project Contributors

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/core/skip_over.hpp>

#include <iris/x4/char/char_parser.hpp>
#include <iris/x4/char/char_class_tags.hpp>
#include <iris/x4/char/detail/cast_char.hpp>

#include <iris/x4/string/case_compare.hpp>

#include <iris/x4/traits/string_traits.hpp>

#include <iris/x4/char_encoding/standard.hpp>

#ifndef IRIS_X4_NO_STANDARD_WIDE
# include <iris/x4/char_encoding/standard_wide.hpp>
#endif

#include <concepts>
#include <iterator>

#include <cassert>

namespace iris::x4 {

namespace detail {

template<class Encoding>
struct char_class_base
{
    using classify_type = typename Encoding::classify_type;

#define IRIS_X4_CLASSIFY(name) \
    template<class Char> \
    [[nodiscard]] static constexpr bool \
    is(char_classes::name##_tag, Char ch) noexcept \
    { \
        static_assert(std::same_as<Char, classify_type>); \
        return (Encoding::is##name)(detail::cast_char<classify_type>(ch)); \
    }

    IRIS_X4_CLASSIFY(char)
    IRIS_X4_CLASSIFY(alnum)
    IRIS_X4_CLASSIFY(alpha)
    IRIS_X4_CLASSIFY(digit)
    IRIS_X4_CLASSIFY(xdigit)
    IRIS_X4_CLASSIFY(cntrl)
    IRIS_X4_CLASSIFY(graph)
    IRIS_X4_CLASSIFY(lower)
    IRIS_X4_CLASSIFY(print)
    IRIS_X4_CLASSIFY(punct)
    IRIS_X4_CLASSIFY(space)
    IRIS_X4_CLASSIFY(blank)
    IRIS_X4_CLASSIFY(upper)

#undef IRIS_X4_CLASSIFY
};

} // detail

template<class Encoding, class Tag>
struct char_class_parser : char_parser<Encoding, char_class_parser<Encoding, Tag>>
{
    using encoding_type = Encoding;
    using tag = Tag;
    using char_type = typename Encoding::char_type;
    using attribute_type = char_type;
    static constexpr bool has_attribute = true;

    [[nodiscard]] static constexpr bool
    test(Encoding::classify_type const classify_ch) noexcept
    {
        return encoding_type::ischar(classify_ch)
            && detail::char_class_base<Encoding>::is(
                tag{},
                static_cast<Encoding::classify_type>(classify_ch)
            );
    }

    [[nodiscard]] static constexpr bool
    test(Encoding::classify_type const classify_ch, auto const& ctx) noexcept
    {
        return encoding_type::ischar(classify_ch)
            && detail::char_class_base<Encoding>::is(
                x4::get_case_compare<Encoding>(ctx).get_char_class_tag(tag{}),
                static_cast<Encoding::classify_type>(classify_ch)
            );
    }

    static void
    test(auto const, auto const&) = delete; // Mixing incompatible char types is not allowed. Did you forget `static_cast<typename Encoding::classify_type>(ch)`?
};

#define IRIS_X4_CHAR_CLASS(encoding, name) \
    namespace encoding { \
    [[maybe_unused]] inline constexpr char_class_parser<char_encoding::encoding, char_classes::name##_tag> name{}; \
    } /* encoding */ \
    namespace parsers::encoding { \
    using x4::encoding::name; \
    } /* parsers::encoding */

#define IRIS_X4_CHAR_CLASSES(encoding) \
    IRIS_X4_CHAR_CLASS(encoding, alnum) \
    IRIS_X4_CHAR_CLASS(encoding, alpha) \
    IRIS_X4_CHAR_CLASS(encoding, digit) \
    IRIS_X4_CHAR_CLASS(encoding, xdigit) \
    IRIS_X4_CHAR_CLASS(encoding, cntrl) \
    IRIS_X4_CHAR_CLASS(encoding, graph) \
    IRIS_X4_CHAR_CLASS(encoding, lower) \
    IRIS_X4_CHAR_CLASS(encoding, print) \
    IRIS_X4_CHAR_CLASS(encoding, punct) \
    IRIS_X4_CHAR_CLASS(encoding, space) \
    IRIS_X4_CHAR_CLASS(encoding, blank) \
    IRIS_X4_CHAR_CLASS(encoding, upper)

IRIS_X4_CHAR_CLASSES(standard)

#ifndef IRIS_X4_NO_STANDARD_WIDE
IRIS_X4_CHAR_CLASSES(standard_wide)
#endif

#undef IRIS_X4_CHAR_CLASS
#undef IRIS_X4_CHAR_CLASSES

// Don't put these in namespace `parsers`, these are too much

using x4::standard::alnum;
using x4::standard::alpha;
using x4::standard::digit;
using x4::standard::xdigit;
using x4::standard::cntrl;
using x4::standard::graph;
using x4::standard::lower;
using x4::standard::print;
using x4::standard::punct;
using x4::standard::space;
using x4::standard::blank;
using x4::standard::upper;


namespace detail {

template<X4Subject Skipper>
struct to_builtin_t {};

struct to_builtin_fn
{
    template<X4Subject Skipper>
    [[nodiscard]] static constexpr decltype(auto)
    operator()(Skipper&& skipper) noexcept
    {
        return skipper;
    }

    template<class Encoding>
    [[nodiscard]] static constexpr builtin_skipper_kind
    operator()(char_class_parser<Encoding, char_classes::blank_tag> const&) noexcept
    {
        return builtin_skipper_kind::blank;
    }

    template<class Encoding>
    [[nodiscard]] static constexpr builtin_skipper_kind
    operator()(char_class_parser<Encoding, char_classes::space_tag> const&) noexcept
    {
        return builtin_skipper_kind::space;
    }
};

// ------------------------------------------------

template<class CharClassTag, std::forward_iterator It, std::sentinel_for<It> Se>
constexpr void builtin_skip_over(It& first, Se const& last) noexcept
{
    using CharT = std::remove_cvref_t<std::iter_value_t<It>>;
    static_assert(traits::CharLike<CharT>);

    using Encoding = traits::char_encoding_for<CharT>;
    using Parser = char_class_parser<Encoding, CharClassTag>;

    while (first != last && Parser::test(static_cast<Encoding::classify_type>(*first))) {
        ++first;
    }
}

} // detail

[[maybe_unused]] inline constexpr detail::to_builtin_fn to_builtin{};


// Forward declaration in "skip_over.hpp"
// Need to sync this implementation with `skip_gen` ("skip.hpp")
template<std::forward_iterator It, std::sentinel_for<It> Se, class Context>
    requires std::same_as<get_context_plain_t<contexts::skipper, Context>, builtin_skipper_kind>
constexpr void skip_over(It& first, Se const& last, Context const& ctx) noexcept
{
    switch (x4::get<contexts::skipper>(ctx)) {
    case builtin_skipper_kind::no_skip: return;
    case builtin_skipper_kind::blank: return detail::builtin_skip_over<char_classes::blank_tag>(first, last);
    case builtin_skipper_kind::space: return detail::builtin_skip_over<char_classes::space_tag>(first, last);
    default: assert(false && "unsupported builtin skipper type"); break;
    }
}

} // iris::x4

#endif
