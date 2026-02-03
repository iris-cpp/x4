#ifndef IRIS_X4_CHAR_ENCODING_UNICODE_HPP
#define IRIS_X4_CHAR_ENCODING_UNICODE_HPP

/*=============================================================================
    Copyright (c) 2001-2011 Hartmut Kaiser
    Copyright (c) 2001-2011 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include <iris/x4/char_encoding/unicode/classification.hpp>

#include <cstdint>

namespace boost::spirit::x4::char_encoding {

struct unicode
{
    using char_type = char32_t;
    using classify_type = x4::unicode::classify_type;

    [[nodiscard]] static constexpr bool
    isascii_(classify_type ch) noexcept
    {
        return 0 == (ch & ~0x7f);
    }

    [[nodiscard]] static constexpr bool
    ischar(classify_type ch) noexcept
    {
        // unicode code points in the range 0x00 to 0x10FFFF
        return ch <= 0x10FFFF;
    }

    [[nodiscard]] static constexpr bool
    isalnum(classify_type ch) noexcept
    {
        return x4::unicode::is_alphanumeric(ch);
    }

    [[nodiscard]] static constexpr bool
    isalpha(classify_type ch) noexcept
    {
        return x4::unicode::is_alphabetic(ch);
    }

    [[nodiscard]] static constexpr bool
    isdigit(classify_type ch) noexcept
    {
        return x4::unicode::is_decimal_number(ch);
    }

    [[nodiscard]] static constexpr bool
    isxdigit(classify_type ch) noexcept
    {
        return x4::unicode::is_hex_digit(ch);
    }

    [[nodiscard]] static constexpr bool
    iscntrl(classify_type ch) noexcept
    {
        return x4::unicode::is_control(ch);
    }

    [[nodiscard]] static constexpr bool
    isgraph(classify_type ch) noexcept
    {
        return x4::unicode::is_graph(ch);
    }

    [[nodiscard]] static constexpr bool
    islower(classify_type ch) noexcept
    {
        return x4::unicode::is_lowercase(ch);
    }

    [[nodiscard]] static constexpr bool
    isprint(classify_type ch) noexcept
    {
        return x4::unicode::is_print(ch);
    }

    [[nodiscard]] static constexpr bool
    ispunct(classify_type ch) noexcept
    {
        return x4::unicode::is_punctuation(ch);
    }

    [[nodiscard]] static constexpr bool
    isspace(classify_type ch) noexcept
    {
        return x4::unicode::is_white_space(ch);
    }

    [[nodiscard]] static constexpr bool
    (isblank)(classify_type ch) noexcept
    {
        return x4::unicode::is_blank(ch);
    }

    [[nodiscard]] static constexpr bool
    isupper(classify_type ch) noexcept
    {
        return x4::unicode::is_uppercase(ch);
    }

    // Mixing character encodings is semantically wrong
    static constexpr void isascii_(char) = delete;
    static constexpr void isascii_(wchar_t) = delete;
    static constexpr void ischar(char) = delete;
    static constexpr void ischar(wchar_t) = delete;
    static constexpr void isalnum(char) = delete;
    static constexpr void isalnum(wchar_t) = delete;
    static constexpr void isalpha(char) = delete;
    static constexpr void isalpha(wchar_t) = delete;
    static constexpr void isdigit(char) = delete;
    static constexpr void isdigit(wchar_t) = delete;
    static constexpr void isxdigit(char) = delete;
    static constexpr void isxdigit(wchar_t) = delete;
    static constexpr void iscntrl(char) = delete;
    static constexpr void iscntrl(wchar_t) = delete;
    static constexpr void isgraph(char) = delete;
    static constexpr void isgraph(wchar_t) = delete;
    static constexpr void islower(char) = delete;
    static constexpr void islower(wchar_t) = delete;
    static constexpr void isprint(char) = delete;
    static constexpr void isprint(wchar_t) = delete;
    static constexpr void ispunct(char) = delete;
    static constexpr void ispunct(wchar_t) = delete;
    static constexpr void isspace(char) = delete;
    static constexpr void isspace(wchar_t) = delete;
    static constexpr void isblank(char) = delete;
    static constexpr void isblank(wchar_t) = delete;
    static constexpr void isupper(char) = delete;
    static constexpr void isupper(wchar_t) = delete;

    // Simple character conversions

    [[nodiscard]] static constexpr classify_type
    tolower(classify_type ch) noexcept
    {
        return x4::unicode::to_lowercase(ch);
    }

    [[nodiscard]] static constexpr classify_type
    toupper(classify_type ch) noexcept
    {
        return x4::unicode::to_uppercase(ch);
    }

    [[nodiscard]] static constexpr std::uint32_t
    toucs4(classify_type ch) noexcept
    {
        return ch;
    }

    static constexpr void tolower(char) = delete;
    static constexpr void tolower(wchar_t) = delete;
    static constexpr void toupper(char) = delete;
    static constexpr void toupper(wchar_t) = delete;
    static constexpr void toucs4(char) = delete;
    static constexpr void toucs4(wchar_t) = delete;

    // Major Categories
#define IRIS_X4_MAJOR_CATEGORY(name) \
    [[nodiscard]] static constexpr bool \
    is_##name(classify_type ch) noexcept \
    { \
        return x4::unicode::get_major_category(ch) == x4::unicode::properties::name; \
    } \
    static constexpr void is_##name(char) = delete; \
    static constexpr void is_##name(wchar_t) = delete;

    IRIS_X4_MAJOR_CATEGORY(letter)
    IRIS_X4_MAJOR_CATEGORY(mark)
    IRIS_X4_MAJOR_CATEGORY(number)
    IRIS_X4_MAJOR_CATEGORY(separator)
    IRIS_X4_MAJOR_CATEGORY(other)
    IRIS_X4_MAJOR_CATEGORY(punctuation)
    IRIS_X4_MAJOR_CATEGORY(symbol)

#undef IRIS_X4_MAJOR_CATEGORY

    // General Categories
#define IRIS_X4_CATEGORY(name) \
    [[nodiscard]] static constexpr bool \
    is_##name(classify_type ch) noexcept \
    { \
        return x4::unicode::get_category(ch) == x4::unicode::properties::name; \
    } \
    static constexpr void is_##name(char) = delete; \
    static constexpr void is_##name(wchar_t) = delete;

    IRIS_X4_CATEGORY(uppercase_letter)
    IRIS_X4_CATEGORY(lowercase_letter)
    IRIS_X4_CATEGORY(titlecase_letter)
    IRIS_X4_CATEGORY(modifier_letter)
    IRIS_X4_CATEGORY(other_letter)

    IRIS_X4_CATEGORY(nonspacing_mark)
    IRIS_X4_CATEGORY(enclosing_mark)
    IRIS_X4_CATEGORY(spacing_mark)

    IRIS_X4_CATEGORY(decimal_number)
    IRIS_X4_CATEGORY(letter_number)
    IRIS_X4_CATEGORY(other_number)

    IRIS_X4_CATEGORY(space_separator)
    IRIS_X4_CATEGORY(line_separator)
    IRIS_X4_CATEGORY(paragraph_separator)

    IRIS_X4_CATEGORY(control)
    IRIS_X4_CATEGORY(format)
    IRIS_X4_CATEGORY(private_use)
    IRIS_X4_CATEGORY(surrogate)
    IRIS_X4_CATEGORY(unassigned)

    IRIS_X4_CATEGORY(dash_punctuation)
    IRIS_X4_CATEGORY(open_punctuation)
    IRIS_X4_CATEGORY(close_punctuation)
    IRIS_X4_CATEGORY(connector_punctuation)
    IRIS_X4_CATEGORY(other_punctuation)
    IRIS_X4_CATEGORY(initial_punctuation)
    IRIS_X4_CATEGORY(final_punctuation)

    IRIS_X4_CATEGORY(math_symbol)
    IRIS_X4_CATEGORY(currency_symbol)
    IRIS_X4_CATEGORY(modifier_symbol)
    IRIS_X4_CATEGORY(other_symbol)

#undef IRIS_X4_CATEGORY

    // Derived Categories
#define IRIS_X4_DERIVED_CATEGORY(name) \
    [[nodiscard]] static constexpr bool \
    is_##name(classify_type ch) noexcept \
    { \
        return x4::unicode::is_##name(ch); \
    } \
    static constexpr void is_##name(char) = delete; \
    static constexpr void is_##name(wchar_t) = delete;

    IRIS_X4_DERIVED_CATEGORY(alphabetic)
    IRIS_X4_DERIVED_CATEGORY(uppercase)
    IRIS_X4_DERIVED_CATEGORY(lowercase)
    IRIS_X4_DERIVED_CATEGORY(white_space)
    IRIS_X4_DERIVED_CATEGORY(hex_digit)
    IRIS_X4_DERIVED_CATEGORY(noncharacter_code_point)
    IRIS_X4_DERIVED_CATEGORY(default_ignorable_code_point)

#undef IRIS_X4_DERIVED_CATEGORY

    // Scripts
#define IRIS_X4_SCRIPT(name) \
    [[nodiscard]] static constexpr bool \
    is_##name(classify_type ch) noexcept \
    { \
        return x4::unicode::get_script(ch) == x4::unicode::properties::name; \
    } \
    static constexpr void is_##name(char) = delete; \
    static constexpr void is_##name(wchar_t) = delete;

    IRIS_X4_SCRIPT(adlam)
    IRIS_X4_SCRIPT(caucasian_albanian)
    IRIS_X4_SCRIPT(ahom)
    IRIS_X4_SCRIPT(arabic)
    IRIS_X4_SCRIPT(imperial_aramaic)
    IRIS_X4_SCRIPT(armenian)
    IRIS_X4_SCRIPT(avestan)
    IRIS_X4_SCRIPT(balinese)
    IRIS_X4_SCRIPT(bamum)
    IRIS_X4_SCRIPT(bassa_vah)
    IRIS_X4_SCRIPT(batak)
    IRIS_X4_SCRIPT(bengali)
    IRIS_X4_SCRIPT(bhaiksuki)
    IRIS_X4_SCRIPT(bopomofo)
    IRIS_X4_SCRIPT(brahmi)
    IRIS_X4_SCRIPT(braille)
    IRIS_X4_SCRIPT(buginese)
    IRIS_X4_SCRIPT(buhid)
    IRIS_X4_SCRIPT(chakma)
    IRIS_X4_SCRIPT(canadian_aboriginal)
    IRIS_X4_SCRIPT(carian)
    IRIS_X4_SCRIPT(cham)
    IRIS_X4_SCRIPT(cherokee)
    IRIS_X4_SCRIPT(chorasmian)
    IRIS_X4_SCRIPT(coptic)
    IRIS_X4_SCRIPT(cypro_minoan)
    IRIS_X4_SCRIPT(cypriot)
    IRIS_X4_SCRIPT(cyrillic)
    IRIS_X4_SCRIPT(devanagari)
    IRIS_X4_SCRIPT(dives_akuru)
    IRIS_X4_SCRIPT(dogra)
    IRIS_X4_SCRIPT(deseret)
    IRIS_X4_SCRIPT(duployan)
    IRIS_X4_SCRIPT(egyptian_hieroglyphs)
    IRIS_X4_SCRIPT(elbasan)
    IRIS_X4_SCRIPT(elymaic)
    IRIS_X4_SCRIPT(ethiopic)
    IRIS_X4_SCRIPT(georgian)
    IRIS_X4_SCRIPT(glagolitic)
    IRIS_X4_SCRIPT(gunjala_gondi)
    IRIS_X4_SCRIPT(masaram_gondi)
    IRIS_X4_SCRIPT(gothic)
    IRIS_X4_SCRIPT(grantha)
    IRIS_X4_SCRIPT(greek)
    IRIS_X4_SCRIPT(gujarati)
    IRIS_X4_SCRIPT(gurmukhi)
    IRIS_X4_SCRIPT(hangul)
    IRIS_X4_SCRIPT(han)
    IRIS_X4_SCRIPT(hanunoo)
    IRIS_X4_SCRIPT(hatran)
    IRIS_X4_SCRIPT(hebrew)
    IRIS_X4_SCRIPT(hiragana)
    IRIS_X4_SCRIPT(anatolian_hieroglyphs)
    IRIS_X4_SCRIPT(pahawh_hmong)
    IRIS_X4_SCRIPT(nyiakeng_puachue_hmong)
    IRIS_X4_SCRIPT(katakana_or_hiragana)
    IRIS_X4_SCRIPT(old_hungarian)
    IRIS_X4_SCRIPT(old_italic)
    IRIS_X4_SCRIPT(javanese)
    IRIS_X4_SCRIPT(kayah_li)
    IRIS_X4_SCRIPT(katakana)
    IRIS_X4_SCRIPT(kawi)
    IRIS_X4_SCRIPT(kharoshthi)
    IRIS_X4_SCRIPT(khmer)
    IRIS_X4_SCRIPT(khojki)
    IRIS_X4_SCRIPT(khitan_small_script)
    IRIS_X4_SCRIPT(kannada)
    IRIS_X4_SCRIPT(kaithi)
    IRIS_X4_SCRIPT(tai_tham)
    IRIS_X4_SCRIPT(lao)
    IRIS_X4_SCRIPT(latin)
    IRIS_X4_SCRIPT(lepcha)
    IRIS_X4_SCRIPT(limbu)
    IRIS_X4_SCRIPT(linear_a)
    IRIS_X4_SCRIPT(linear_b)
    IRIS_X4_SCRIPT(lisu)
    IRIS_X4_SCRIPT(lycian)
    IRIS_X4_SCRIPT(lydian)
    IRIS_X4_SCRIPT(mahajani)
    IRIS_X4_SCRIPT(makasar)
    IRIS_X4_SCRIPT(mandaic)
    IRIS_X4_SCRIPT(manichaean)
    IRIS_X4_SCRIPT(marchen)
    IRIS_X4_SCRIPT(medefaidrin)
    IRIS_X4_SCRIPT(mende_kikakui)
    IRIS_X4_SCRIPT(meroitic_cursive)
    IRIS_X4_SCRIPT(meroitic_hieroglyphs)
    IRIS_X4_SCRIPT(malayalam)
    IRIS_X4_SCRIPT(modi)
    IRIS_X4_SCRIPT(mongolian)
    IRIS_X4_SCRIPT(mro)
    IRIS_X4_SCRIPT(meetei_mayek)
    IRIS_X4_SCRIPT(multani)
    IRIS_X4_SCRIPT(myanmar)
    IRIS_X4_SCRIPT(nag_mundari)
    IRIS_X4_SCRIPT(nandinagari)
    IRIS_X4_SCRIPT(old_north_arabian)
    IRIS_X4_SCRIPT(nabataean)
    IRIS_X4_SCRIPT(newa)
    IRIS_X4_SCRIPT(nko)
    IRIS_X4_SCRIPT(nushu)
    IRIS_X4_SCRIPT(ogham)
    IRIS_X4_SCRIPT(ol_chiki)
    IRIS_X4_SCRIPT(old_turkic)
    IRIS_X4_SCRIPT(oriya)
    IRIS_X4_SCRIPT(osage)
    IRIS_X4_SCRIPT(osmanya)
    IRIS_X4_SCRIPT(old_uyghur)
    IRIS_X4_SCRIPT(palmyrene)
    IRIS_X4_SCRIPT(pau_cin_hau)
    IRIS_X4_SCRIPT(old_permic)
    IRIS_X4_SCRIPT(phags_pa)
    IRIS_X4_SCRIPT(inscriptional_pahlavi)
    IRIS_X4_SCRIPT(psalter_pahlavi)
    IRIS_X4_SCRIPT(phoenician)
    IRIS_X4_SCRIPT(miao)
    IRIS_X4_SCRIPT(inscriptional_parthian)
    IRIS_X4_SCRIPT(rejang)
    IRIS_X4_SCRIPT(hanifi_rohingya)
    IRIS_X4_SCRIPT(runic)
    IRIS_X4_SCRIPT(samaritan)
    IRIS_X4_SCRIPT(old_south_arabian)
    IRIS_X4_SCRIPT(saurashtra)
    IRIS_X4_SCRIPT(signwriting)
    IRIS_X4_SCRIPT(shavian)
    IRIS_X4_SCRIPT(sharada)
    IRIS_X4_SCRIPT(siddham)
    IRIS_X4_SCRIPT(khudawadi)
    IRIS_X4_SCRIPT(sinhala)
    IRIS_X4_SCRIPT(sogdian)
    IRIS_X4_SCRIPT(old_sogdian)
    IRIS_X4_SCRIPT(sora_sompeng)
    IRIS_X4_SCRIPT(soyombo)
    IRIS_X4_SCRIPT(sundanese)
    IRIS_X4_SCRIPT(syloti_nagri)
    IRIS_X4_SCRIPT(syriac)
    IRIS_X4_SCRIPT(tagbanwa)
    IRIS_X4_SCRIPT(takri)
    IRIS_X4_SCRIPT(tai_le)
    IRIS_X4_SCRIPT(new_tai_lue)
    IRIS_X4_SCRIPT(tamil)
    IRIS_X4_SCRIPT(tangut)
    IRIS_X4_SCRIPT(tai_viet)
    IRIS_X4_SCRIPT(telugu)
    IRIS_X4_SCRIPT(tifinagh)
    IRIS_X4_SCRIPT(tagalog)
    IRIS_X4_SCRIPT(thaana)
    IRIS_X4_SCRIPT(thai)
    IRIS_X4_SCRIPT(tibetan)
    IRIS_X4_SCRIPT(tirhuta)
    IRIS_X4_SCRIPT(tangsa)
    IRIS_X4_SCRIPT(toto)
    IRIS_X4_SCRIPT(ugaritic)
    IRIS_X4_SCRIPT(vai)
    IRIS_X4_SCRIPT(vithkuqi)
    IRIS_X4_SCRIPT(warang_citi)
    IRIS_X4_SCRIPT(wancho)
    IRIS_X4_SCRIPT(old_persian)
    IRIS_X4_SCRIPT(cuneiform)
    IRIS_X4_SCRIPT(yezidi)
    IRIS_X4_SCRIPT(yi)
    IRIS_X4_SCRIPT(zanabazar_square)
    IRIS_X4_SCRIPT(inherited)
    IRIS_X4_SCRIPT(common)
    IRIS_X4_SCRIPT(unknown)

#undef IRIS_X4_SCRIPT
};

} // boost::spirit::x4::char_encoding

#endif
