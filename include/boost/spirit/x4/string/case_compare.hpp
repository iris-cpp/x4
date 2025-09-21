#ifndef BOOST_SPIRIT_X4_STRING_CASE_COMPARE_HPP
#define BOOST_SPIRIT_X4_STRING_CASE_COMPARE_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/spirit/x4/core/unused.hpp>
#include <boost/spirit/x4/core/context.hpp>
#include <boost/spirit/x4/char/char_class_tags.hpp>

#include <boost/spirit/x4/char_encoding/standard.hpp>

#ifndef BOOST_SPIRIT_X4_NO_STANDARD_WIDE
# include <boost/spirit/x4/char_encoding/standard_wide.hpp>
#endif

#ifdef BOOST_SPIRIT_X4_UNICODE
# include <boost/spirit/x4/char_encoding/unicode.hpp>
#endif

#include <cstdint>

namespace boost::spirit::x4
{
    template <typename Encoding>
    struct case_compare
    {
        template <typename Char, typename CharSet>
        [[nodiscard]] static constexpr bool in_set(Char ch, CharSet const& set) noexcept
        {
            static_assert(noexcept(set.test(ch)));
            return set.test(ch);
        }

        template <typename Char>
        [[nodiscard]] static constexpr std::int32_t operator()(Char lc, Char rc) noexcept
        {
            return lc - rc;
        }

        template <typename CharClassTag>
        [[nodiscard]] static constexpr CharClassTag get_char_class_tag(CharClassTag tag) noexcept
        {
            return tag;
        }
    };

    template <typename Encoding>
    struct no_case_compare
    {
        template <typename Char, typename CharSet>
        [[nodiscard]] static constexpr bool in_set(Char ch_, CharSet const& set) noexcept
        {
            using classify_type = typename Encoding::classify_type;
            auto ch = classify_type(ch_);
            static_assert(noexcept(set.test(ch)));
            return set.test(ch)
                || set.test(static_cast<classify_type>(
                    Encoding::islower(ch) ? Encoding::toupper(ch) : Encoding::tolower(ch))
                );
        }

        template <typename Char>
        [[nodiscard]] static constexpr std::int32_t operator()(Char lc_, Char const rc_) noexcept
        {
            using classify_type = typename Encoding::classify_type;
            auto lc = classify_type(lc_);
            auto rc = classify_type(rc_);
            return Encoding::islower(rc)
                ? Encoding::tolower(lc) - rc : Encoding::toupper(lc) - rc;
        }

        template <typename CharClassTag>
        [[nodiscard]] static constexpr CharClassTag get_char_class_tag(CharClassTag tag) noexcept
        {
            return tag;
        }

        [[nodiscard]] static constexpr alpha_tag get_char_class_tag(lower_tag) noexcept
        {
            return {};
        }

        [[nodiscard]] static constexpr alpha_tag get_char_class_tag(upper_tag) noexcept
        {
            return {};
        }
    };

    namespace detail
    {
        struct case_compare_tag
        {
            static constexpr bool is_unique = false;
        };

        struct case_compare_no_case_t {};
        inline constexpr case_compare_no_case_t case_compare_no_case{};

    } // detail

    template <typename Encoding, typename Context>
    [[nodiscard]] constexpr auto
    get_case_compare(Context const&) noexcept
    {
        if constexpr (has_context_of_v<Context, detail::case_compare_tag, detail::case_compare_no_case_t>)
        {
            return no_case_compare<Encoding>{};
        }
        else
        {
            return case_compare<Encoding>{};
        }
    }

} // boost::spirit::x4

#endif
