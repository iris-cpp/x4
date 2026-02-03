#ifndef IRIS_X4_CHAR_UNICODE_CHAR_CLASS_HPP
#define IRIS_X4_CHAR_UNICODE_CHAR_CLASS_HPP

/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2025 Nana Sakisaka

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <iris/x4/char/char_class_tags.hpp>
#include <iris/x4/char/char_parser.hpp>
#include <iris/x4/char/detail/cast_char.hpp>
#include <iris/x4/char_encoding/unicode.hpp>

namespace iris::x4 {

namespace char_classes::unicode {

// Common Categories (semantically compatible with `standard::*` variants)

using char_tag = char_classes::char_tag;
using alnum_tag = char_classes::alnum_tag;
using alpha_tag = char_classes::alpha_tag;
using digit_tag = char_classes::digit_tag;
using xdigit_tag = char_classes::xdigit_tag;
using cntrl_tag = char_classes::cntrl_tag;
using graph_tag = char_classes::graph_tag;
using lower_tag = char_classes::lower_tag;
using print_tag = char_classes::print_tag;
using punct_tag = char_classes::punct_tag;
using space_tag = char_classes::space_tag;
using blank_tag = char_classes::blank_tag;
using upper_tag = char_classes::upper_tag;

// Unicode Major Categories

struct letter_tag {};
struct mark_tag {};
struct number_tag {};
struct separator_tag {};
struct other_tag {};
struct punctuation_tag {};
struct symbol_tag {};

// Unicode General Categories

struct uppercase_letter_tag {};
struct lowercase_letter_tag {};
struct titlecase_letter_tag {};
struct modifier_letter_tag {};
struct other_letter_tag {};

struct nonspacing_mark_tag {};
struct enclosing_mark_tag {};
struct spacing_mark_tag {};

struct decimal_number_tag {};
struct letter_number_tag {};
struct other_number_tag {};

struct space_separator_tag {};
struct line_separator_tag {};
struct paragraph_separator_tag {};

struct control_tag {};
struct format_tag {};
struct private_use_tag {};
struct surrogate_tag {};
struct unassigned_tag {};

struct dash_punctuation_tag {};
struct open_punctuation_tag {};
struct close_punctuation_tag {};
struct connector_punctuation_tag {};
struct other_punctuation_tag {};
struct initial_punctuation_tag {};
struct final_punctuation_tag {};

struct math_symbol_tag {};
struct currency_symbol_tag {};
struct modifier_symbol_tag {};
struct other_symbol_tag {};

// Unicode Derived Categories

struct alphabetic_tag {};
struct uppercase_tag {};
struct lowercase_tag {};
struct white_space_tag {};
struct hex_digit_tag {};
struct noncharacter_code_point_tag {};
struct default_ignorable_code_point_tag {};

// Unicode Scripts

struct adlam_tag {};
struct caucasian_albanian_tag {};
struct ahom_tag {};
struct arabic_tag {};
struct imperial_aramaic_tag {};
struct armenian_tag {};
struct avestan_tag {};
struct balinese_tag {};
struct bamum_tag {};
struct bassa_vah_tag {};
struct batak_tag {};
struct bengali_tag {};
struct bhaiksuki_tag {};
struct bopomofo_tag {};
struct brahmi_tag {};
struct braille_tag {};
struct buginese_tag {};
struct buhid_tag {};
struct chakma_tag {};
struct canadian_aboriginal_tag {};
struct carian_tag {};
struct cham_tag {};
struct cherokee_tag {};
struct chorasmian_tag {};
struct coptic_tag {};
struct cypro_minoan_tag {};
struct cypriot_tag {};
struct cyrillic_tag {};
struct devanagari_tag {};
struct dives_akuru_tag {};
struct dogra_tag {};
struct deseret_tag {};
struct duployan_tag {};
struct egyptian_hieroglyphs_tag {};
struct elbasan_tag {};
struct elymaic_tag {};
struct ethiopic_tag {};
struct georgian_tag {};
struct glagolitic_tag {};
struct gunjala_gondi_tag {};
struct masaram_gondi_tag {};
struct gothic_tag {};
struct grantha_tag {};
struct greek_tag {};
struct gujarati_tag {};
struct gurmukhi_tag {};
struct hangul_tag {};
struct han_tag {};
struct hanunoo_tag {};
struct hatran_tag {};
struct hebrew_tag {};
struct hiragana_tag {};
struct anatolian_hieroglyphs_tag {};
struct pahawh_hmong_tag {};
struct nyiakeng_puachue_hmong_tag {};
struct katakana_or_hiragana_tag {};
struct old_hungarian_tag {};
struct old_italic_tag {};
struct javanese_tag {};
struct kayah_li_tag {};
struct katakana_tag {};
struct kawi_tag {};
struct kharoshthi_tag {};
struct khmer_tag {};
struct khojki_tag {};
struct khitan_small_script_tag {};
struct kannada_tag {};
struct kaithi_tag {};
struct tai_tham_tag {};
struct lao_tag {};
struct latin_tag {};
struct lepcha_tag {};
struct limbu_tag {};
struct linear_a_tag {};
struct linear_b_tag {};
struct lisu_tag {};
struct lycian_tag {};
struct lydian_tag {};
struct mahajani_tag {};
struct makasar_tag {};
struct mandaic_tag {};
struct manichaean_tag {};
struct marchen_tag {};
struct medefaidrin_tag {};
struct mende_kikakui_tag {};
struct meroitic_cursive_tag {};
struct meroitic_hieroglyphs_tag {};
struct malayalam_tag {};
struct modi_tag {};
struct mongolian_tag {};
struct mro_tag {};
struct meetei_mayek_tag {};
struct multani_tag {};
struct myanmar_tag {};
struct nag_mundari_tag {};
struct nandinagari_tag {};
struct old_north_arabian_tag {};
struct nabataean_tag {};
struct newa_tag {};
struct nko_tag {};
struct nushu_tag {};
struct ogham_tag {};
struct ol_chiki_tag {};
struct old_turkic_tag {};
struct oriya_tag {};
struct osage_tag {};
struct osmanya_tag {};
struct old_uyghur_tag {};
struct palmyrene_tag {};
struct pau_cin_hau_tag {};
struct old_permic_tag {};
struct phags_pa_tag {};
struct inscriptional_pahlavi_tag {};
struct psalter_pahlavi_tag {};
struct phoenician_tag {};
struct miao_tag {};
struct inscriptional_parthian_tag {};
struct rejang_tag {};
struct hanifi_rohingya_tag {};
struct runic_tag {};
struct samaritan_tag {};
struct old_south_arabian_tag {};
struct saurashtra_tag {};
struct signwriting_tag {};
struct shavian_tag {};
struct sharada_tag {};
struct siddham_tag {};
struct khudawadi_tag {};
struct sinhala_tag {};
struct sogdian_tag {};
struct old_sogdian_tag {};
struct sora_sompeng_tag {};
struct soyombo_tag {};
struct sundanese_tag {};
struct syloti_nagri_tag {};
struct syriac_tag {};
struct tagbanwa_tag {};
struct takri_tag {};
struct tai_le_tag {};
struct new_tai_lue_tag {};
struct tamil_tag {};
struct tangut_tag {};
struct tai_viet_tag {};
struct telugu_tag {};
struct tifinagh_tag {};
struct tagalog_tag {};
struct thaana_tag {};
struct thai_tag {};
struct tibetan_tag {};
struct tirhuta_tag {};
struct tangsa_tag {};
struct toto_tag {};
struct ugaritic_tag {};
struct vai_tag {};
struct vithkuqi_tag {};
struct warang_citi_tag {};
struct wancho_tag {};
struct old_persian_tag {};
struct cuneiform_tag {};
struct yezidi_tag {};
struct yi_tag {};
struct zanabazar_square_tag {};
struct inherited_tag {};
struct common_tag {};
struct unknown_tag {};

} // char_classes::unicode

namespace detail {

struct unicode_char_class_base
{
    using encoding_type = char_encoding::unicode;
    using char_type = char_encoding::unicode::char_type;

#define IRIS_X4_BASIC_CLASSIFY(name) \
    template<class Char> \
    static constexpr bool \
    is(char_classes::unicode::name##_tag, Char ch) noexcept \
    { \
        return (encoding_type::is##name)(detail::cast_char<char_type>(ch)); \
    }

#define IRIS_X4_CLASSIFY(name) \
    template<class Char> \
    static constexpr bool \
    is(char_classes::unicode::name##_tag, Char ch) noexcept \
    { \
        return (encoding_type::is_##name)(detail::cast_char<char_type>(ch)); \
    }

    // Unicode Major Categories

    IRIS_X4_BASIC_CLASSIFY(char)
    IRIS_X4_BASIC_CLASSIFY(alnum)
    IRIS_X4_BASIC_CLASSIFY(alpha)
    IRIS_X4_BASIC_CLASSIFY(digit)
    IRIS_X4_BASIC_CLASSIFY(xdigit)
    IRIS_X4_BASIC_CLASSIFY(cntrl)
    IRIS_X4_BASIC_CLASSIFY(graph)
    IRIS_X4_BASIC_CLASSIFY(lower)
    IRIS_X4_BASIC_CLASSIFY(print)
    IRIS_X4_BASIC_CLASSIFY(punct)
    IRIS_X4_BASIC_CLASSIFY(space)
    IRIS_X4_BASIC_CLASSIFY(blank)
    IRIS_X4_BASIC_CLASSIFY(upper)

    // Unicode Major Categories

    IRIS_X4_CLASSIFY(letter)
    IRIS_X4_CLASSIFY(mark)
    IRIS_X4_CLASSIFY(number)
    IRIS_X4_CLASSIFY(separator)
    IRIS_X4_CLASSIFY(other)
    IRIS_X4_CLASSIFY(punctuation)
    IRIS_X4_CLASSIFY(symbol)

    // Unicode General Categories

    IRIS_X4_CLASSIFY(uppercase_letter)
    IRIS_X4_CLASSIFY(lowercase_letter)
    IRIS_X4_CLASSIFY(titlecase_letter)
    IRIS_X4_CLASSIFY(modifier_letter)
    IRIS_X4_CLASSIFY(other_letter)

    IRIS_X4_CLASSIFY(nonspacing_mark)
    IRIS_X4_CLASSIFY(enclosing_mark)
    IRIS_X4_CLASSIFY(spacing_mark)

    IRIS_X4_CLASSIFY(decimal_number)
    IRIS_X4_CLASSIFY(letter_number)
    IRIS_X4_CLASSIFY(other_number)

    IRIS_X4_CLASSIFY(space_separator)
    IRIS_X4_CLASSIFY(line_separator)
    IRIS_X4_CLASSIFY(paragraph_separator)

    IRIS_X4_CLASSIFY(control)
    IRIS_X4_CLASSIFY(format)
    IRIS_X4_CLASSIFY(private_use)
    IRIS_X4_CLASSIFY(surrogate)
    IRIS_X4_CLASSIFY(unassigned)

    IRIS_X4_CLASSIFY(dash_punctuation)
    IRIS_X4_CLASSIFY(open_punctuation)
    IRIS_X4_CLASSIFY(close_punctuation)
    IRIS_X4_CLASSIFY(connector_punctuation)
    IRIS_X4_CLASSIFY(other_punctuation)
    IRIS_X4_CLASSIFY(initial_punctuation)
    IRIS_X4_CLASSIFY(final_punctuation)

    IRIS_X4_CLASSIFY(math_symbol)
    IRIS_X4_CLASSIFY(currency_symbol)
    IRIS_X4_CLASSIFY(modifier_symbol)
    IRIS_X4_CLASSIFY(other_symbol)

    // Unicode Derived Categories

    IRIS_X4_CLASSIFY(alphabetic)
    IRIS_X4_CLASSIFY(uppercase)
    IRIS_X4_CLASSIFY(lowercase)
    IRIS_X4_CLASSIFY(white_space)
    IRIS_X4_CLASSIFY(hex_digit)
    IRIS_X4_CLASSIFY(noncharacter_code_point)
    IRIS_X4_CLASSIFY(default_ignorable_code_point)

    // Unicode Scripts

    IRIS_X4_CLASSIFY(adlam)
    IRIS_X4_CLASSIFY(caucasian_albanian)
    IRIS_X4_CLASSIFY(ahom)
    IRIS_X4_CLASSIFY(arabic)
    IRIS_X4_CLASSIFY(imperial_aramaic)
    IRIS_X4_CLASSIFY(armenian)
    IRIS_X4_CLASSIFY(avestan)
    IRIS_X4_CLASSIFY(balinese)
    IRIS_X4_CLASSIFY(bamum)
    IRIS_X4_CLASSIFY(bassa_vah)
    IRIS_X4_CLASSIFY(batak)
    IRIS_X4_CLASSIFY(bengali)
    IRIS_X4_CLASSIFY(bhaiksuki)
    IRIS_X4_CLASSIFY(bopomofo)
    IRIS_X4_CLASSIFY(brahmi)
    IRIS_X4_CLASSIFY(braille)
    IRIS_X4_CLASSIFY(buginese)
    IRIS_X4_CLASSIFY(buhid)
    IRIS_X4_CLASSIFY(chakma)
    IRIS_X4_CLASSIFY(canadian_aboriginal)
    IRIS_X4_CLASSIFY(carian)
    IRIS_X4_CLASSIFY(cham)
    IRIS_X4_CLASSIFY(cherokee)
    IRIS_X4_CLASSIFY(chorasmian)
    IRIS_X4_CLASSIFY(coptic)
    IRIS_X4_CLASSIFY(cypro_minoan)
    IRIS_X4_CLASSIFY(cypriot)
    IRIS_X4_CLASSIFY(cyrillic)
    IRIS_X4_CLASSIFY(devanagari)
    IRIS_X4_CLASSIFY(dives_akuru)
    IRIS_X4_CLASSIFY(dogra)
    IRIS_X4_CLASSIFY(deseret)
    IRIS_X4_CLASSIFY(duployan)
    IRIS_X4_CLASSIFY(egyptian_hieroglyphs)
    IRIS_X4_CLASSIFY(elbasan)
    IRIS_X4_CLASSIFY(elymaic)
    IRIS_X4_CLASSIFY(ethiopic)
    IRIS_X4_CLASSIFY(georgian)
    IRIS_X4_CLASSIFY(glagolitic)
    IRIS_X4_CLASSIFY(gunjala_gondi)
    IRIS_X4_CLASSIFY(masaram_gondi)
    IRIS_X4_CLASSIFY(gothic)
    IRIS_X4_CLASSIFY(grantha)
    IRIS_X4_CLASSIFY(greek)
    IRIS_X4_CLASSIFY(gujarati)
    IRIS_X4_CLASSIFY(gurmukhi)
    IRIS_X4_CLASSIFY(hangul)
    IRIS_X4_CLASSIFY(han)
    IRIS_X4_CLASSIFY(hanunoo)
    IRIS_X4_CLASSIFY(hatran)
    IRIS_X4_CLASSIFY(hebrew)
    IRIS_X4_CLASSIFY(hiragana)
    IRIS_X4_CLASSIFY(anatolian_hieroglyphs)
    IRIS_X4_CLASSIFY(pahawh_hmong)
    IRIS_X4_CLASSIFY(nyiakeng_puachue_hmong)
    IRIS_X4_CLASSIFY(katakana_or_hiragana)
    IRIS_X4_CLASSIFY(old_hungarian)
    IRIS_X4_CLASSIFY(old_italic)
    IRIS_X4_CLASSIFY(javanese)
    IRIS_X4_CLASSIFY(kayah_li)
    IRIS_X4_CLASSIFY(katakana)
    IRIS_X4_CLASSIFY(kawi)
    IRIS_X4_CLASSIFY(kharoshthi)
    IRIS_X4_CLASSIFY(khmer)
    IRIS_X4_CLASSIFY(khojki)
    IRIS_X4_CLASSIFY(khitan_small_script)
    IRIS_X4_CLASSIFY(kannada)
    IRIS_X4_CLASSIFY(kaithi)
    IRIS_X4_CLASSIFY(tai_tham)
    IRIS_X4_CLASSIFY(lao)
    IRIS_X4_CLASSIFY(latin)
    IRIS_X4_CLASSIFY(lepcha)
    IRIS_X4_CLASSIFY(limbu)
    IRIS_X4_CLASSIFY(linear_a)
    IRIS_X4_CLASSIFY(linear_b)
    IRIS_X4_CLASSIFY(lisu)
    IRIS_X4_CLASSIFY(lycian)
    IRIS_X4_CLASSIFY(lydian)
    IRIS_X4_CLASSIFY(mahajani)
    IRIS_X4_CLASSIFY(makasar)
    IRIS_X4_CLASSIFY(mandaic)
    IRIS_X4_CLASSIFY(manichaean)
    IRIS_X4_CLASSIFY(marchen)
    IRIS_X4_CLASSIFY(medefaidrin)
    IRIS_X4_CLASSIFY(mende_kikakui)
    IRIS_X4_CLASSIFY(meroitic_cursive)
    IRIS_X4_CLASSIFY(meroitic_hieroglyphs)
    IRIS_X4_CLASSIFY(malayalam)
    IRIS_X4_CLASSIFY(modi)
    IRIS_X4_CLASSIFY(mongolian)
    IRIS_X4_CLASSIFY(mro)
    IRIS_X4_CLASSIFY(meetei_mayek)
    IRIS_X4_CLASSIFY(multani)
    IRIS_X4_CLASSIFY(myanmar)
    IRIS_X4_CLASSIFY(nag_mundari)
    IRIS_X4_CLASSIFY(nandinagari)
    IRIS_X4_CLASSIFY(old_north_arabian)
    IRIS_X4_CLASSIFY(nabataean)
    IRIS_X4_CLASSIFY(newa)
    IRIS_X4_CLASSIFY(nko)
    IRIS_X4_CLASSIFY(nushu)
    IRIS_X4_CLASSIFY(ogham)
    IRIS_X4_CLASSIFY(ol_chiki)
    IRIS_X4_CLASSIFY(old_turkic)
    IRIS_X4_CLASSIFY(oriya)
    IRIS_X4_CLASSIFY(osage)
    IRIS_X4_CLASSIFY(osmanya)
    IRIS_X4_CLASSIFY(old_uyghur)
    IRIS_X4_CLASSIFY(palmyrene)
    IRIS_X4_CLASSIFY(pau_cin_hau)
    IRIS_X4_CLASSIFY(old_permic)
    IRIS_X4_CLASSIFY(phags_pa)
    IRIS_X4_CLASSIFY(inscriptional_pahlavi)
    IRIS_X4_CLASSIFY(psalter_pahlavi)
    IRIS_X4_CLASSIFY(phoenician)
    IRIS_X4_CLASSIFY(miao)
    IRIS_X4_CLASSIFY(inscriptional_parthian)
    IRIS_X4_CLASSIFY(rejang)
    IRIS_X4_CLASSIFY(hanifi_rohingya)
    IRIS_X4_CLASSIFY(runic)
    IRIS_X4_CLASSIFY(samaritan)
    IRIS_X4_CLASSIFY(old_south_arabian)
    IRIS_X4_CLASSIFY(saurashtra)
    IRIS_X4_CLASSIFY(signwriting)
    IRIS_X4_CLASSIFY(shavian)
    IRIS_X4_CLASSIFY(sharada)
    IRIS_X4_CLASSIFY(siddham)
    IRIS_X4_CLASSIFY(khudawadi)
    IRIS_X4_CLASSIFY(sinhala)
    IRIS_X4_CLASSIFY(sogdian)
    IRIS_X4_CLASSIFY(old_sogdian)
    IRIS_X4_CLASSIFY(sora_sompeng)
    IRIS_X4_CLASSIFY(soyombo)
    IRIS_X4_CLASSIFY(sundanese)
    IRIS_X4_CLASSIFY(syloti_nagri)
    IRIS_X4_CLASSIFY(syriac)
    IRIS_X4_CLASSIFY(tagbanwa)
    IRIS_X4_CLASSIFY(takri)
    IRIS_X4_CLASSIFY(tai_le)
    IRIS_X4_CLASSIFY(new_tai_lue)
    IRIS_X4_CLASSIFY(tamil)
    IRIS_X4_CLASSIFY(tangut)
    IRIS_X4_CLASSIFY(tai_viet)
    IRIS_X4_CLASSIFY(telugu)
    IRIS_X4_CLASSIFY(tifinagh)
    IRIS_X4_CLASSIFY(tagalog)
    IRIS_X4_CLASSIFY(thaana)
    IRIS_X4_CLASSIFY(thai)
    IRIS_X4_CLASSIFY(tibetan)
    IRIS_X4_CLASSIFY(tirhuta)
    IRIS_X4_CLASSIFY(tangsa)
    IRIS_X4_CLASSIFY(toto)
    IRIS_X4_CLASSIFY(ugaritic)
    IRIS_X4_CLASSIFY(vai)
    IRIS_X4_CLASSIFY(vithkuqi)
    IRIS_X4_CLASSIFY(warang_citi)
    IRIS_X4_CLASSIFY(wancho)
    IRIS_X4_CLASSIFY(old_persian)
    IRIS_X4_CLASSIFY(cuneiform)
    IRIS_X4_CLASSIFY(yezidi)
    IRIS_X4_CLASSIFY(yi)
    IRIS_X4_CLASSIFY(zanabazar_square)
    IRIS_X4_CLASSIFY(inherited)
    IRIS_X4_CLASSIFY(common)
    IRIS_X4_CLASSIFY(unknown)

#undef IRIS_X4_BASIC_CLASSIFY
#undef IRIS_X4_CLASSIFY
};

} // detail

template<class Tag>
struct unicode_char_class : char_parser<char_encoding::unicode, unicode_char_class<Tag>>
{
    using encoding_type = char_encoding::unicode;
    using tag = Tag;
    using char_type = typename encoding_type::char_type;
    using attribute_type = char_type;

    static constexpr bool has_attribute = true;

    [[nodiscard]] static constexpr bool
    test(char_encoding::unicode::classify_type const classify_ch, auto const& /* ctx */) noexcept
    {
        static_assert(noexcept(encoding_type::ischar(classify_ch) && detail::unicode_char_class_base::is(tag{}, classify_ch)));
        return encoding_type::ischar(classify_ch) && detail::unicode_char_class_base::is(tag{}, classify_ch);
    }

    static constexpr void test(auto const, auto const&) = delete; // Mixing incompatible char types is not allowed
};

#define IRIS_X4_CHAR_CLASS(name) \
    namespace unicode { \
    [[maybe_unused]] inline constexpr unicode_char_class<char_classes::unicode::name##_tag> name{}; \
    } /* unicode */ \
    namespace parsers::unicode { \
    using x4::unicode::name; \
    } /* parsers::unicode */

// Unicode Major Categories

IRIS_X4_CHAR_CLASS(alnum)
IRIS_X4_CHAR_CLASS(alpha)
IRIS_X4_CHAR_CLASS(digit)
IRIS_X4_CHAR_CLASS(xdigit)
IRIS_X4_CHAR_CLASS(cntrl)
IRIS_X4_CHAR_CLASS(graph)
IRIS_X4_CHAR_CLASS(lower)
IRIS_X4_CHAR_CLASS(print)
IRIS_X4_CHAR_CLASS(punct)
IRIS_X4_CHAR_CLASS(space)
IRIS_X4_CHAR_CLASS(blank)
IRIS_X4_CHAR_CLASS(upper)

// Unicode Major Categories

IRIS_X4_CHAR_CLASS(letter)
IRIS_X4_CHAR_CLASS(mark)
IRIS_X4_CHAR_CLASS(number)
IRIS_X4_CHAR_CLASS(separator)
IRIS_X4_CHAR_CLASS(other)
IRIS_X4_CHAR_CLASS(punctuation)
IRIS_X4_CHAR_CLASS(symbol)

// Unicode General Categories

IRIS_X4_CHAR_CLASS(uppercase_letter)
IRIS_X4_CHAR_CLASS(lowercase_letter)
IRIS_X4_CHAR_CLASS(titlecase_letter)
IRIS_X4_CHAR_CLASS(modifier_letter)
IRIS_X4_CHAR_CLASS(other_letter)

IRIS_X4_CHAR_CLASS(nonspacing_mark)
IRIS_X4_CHAR_CLASS(enclosing_mark)
IRIS_X4_CHAR_CLASS(spacing_mark)

IRIS_X4_CHAR_CLASS(decimal_number)
IRIS_X4_CHAR_CLASS(letter_number)
IRIS_X4_CHAR_CLASS(other_number)

IRIS_X4_CHAR_CLASS(space_separator)
IRIS_X4_CHAR_CLASS(line_separator)
IRIS_X4_CHAR_CLASS(paragraph_separator)

IRIS_X4_CHAR_CLASS(control)
IRIS_X4_CHAR_CLASS(format)
IRIS_X4_CHAR_CLASS(private_use)
IRIS_X4_CHAR_CLASS(surrogate)
IRIS_X4_CHAR_CLASS(unassigned)

IRIS_X4_CHAR_CLASS(dash_punctuation)
IRIS_X4_CHAR_CLASS(open_punctuation)
IRIS_X4_CHAR_CLASS(close_punctuation)
IRIS_X4_CHAR_CLASS(connector_punctuation)
IRIS_X4_CHAR_CLASS(other_punctuation)
IRIS_X4_CHAR_CLASS(initial_punctuation)
IRIS_X4_CHAR_CLASS(final_punctuation)

IRIS_X4_CHAR_CLASS(math_symbol)
IRIS_X4_CHAR_CLASS(currency_symbol)
IRIS_X4_CHAR_CLASS(modifier_symbol)
IRIS_X4_CHAR_CLASS(other_symbol)

// Unicode Derived Categories

IRIS_X4_CHAR_CLASS(alphabetic)
IRIS_X4_CHAR_CLASS(uppercase)
IRIS_X4_CHAR_CLASS(lowercase)
IRIS_X4_CHAR_CLASS(white_space)
IRIS_X4_CHAR_CLASS(hex_digit)
IRIS_X4_CHAR_CLASS(noncharacter_code_point)
IRIS_X4_CHAR_CLASS(default_ignorable_code_point)

// Unicode Scripts

IRIS_X4_CHAR_CLASS(adlam)
IRIS_X4_CHAR_CLASS(caucasian_albanian)
IRIS_X4_CHAR_CLASS(ahom)
IRIS_X4_CHAR_CLASS(arabic)
IRIS_X4_CHAR_CLASS(imperial_aramaic)
IRIS_X4_CHAR_CLASS(armenian)
IRIS_X4_CHAR_CLASS(avestan)
IRIS_X4_CHAR_CLASS(balinese)
IRIS_X4_CHAR_CLASS(bamum)
IRIS_X4_CHAR_CLASS(bassa_vah)
IRIS_X4_CHAR_CLASS(batak)
IRIS_X4_CHAR_CLASS(bengali)
IRIS_X4_CHAR_CLASS(bhaiksuki)
IRIS_X4_CHAR_CLASS(bopomofo)
IRIS_X4_CHAR_CLASS(brahmi)
IRIS_X4_CHAR_CLASS(braille)
IRIS_X4_CHAR_CLASS(buginese)
IRIS_X4_CHAR_CLASS(buhid)
IRIS_X4_CHAR_CLASS(chakma)
IRIS_X4_CHAR_CLASS(canadian_aboriginal)
IRIS_X4_CHAR_CLASS(carian)
IRIS_X4_CHAR_CLASS(cham)
IRIS_X4_CHAR_CLASS(cherokee)
IRIS_X4_CHAR_CLASS(chorasmian)
IRIS_X4_CHAR_CLASS(coptic)
IRIS_X4_CHAR_CLASS(cypro_minoan)
IRIS_X4_CHAR_CLASS(cypriot)
IRIS_X4_CHAR_CLASS(cyrillic)
IRIS_X4_CHAR_CLASS(devanagari)
IRIS_X4_CHAR_CLASS(dives_akuru)
IRIS_X4_CHAR_CLASS(dogra)
IRIS_X4_CHAR_CLASS(deseret)
IRIS_X4_CHAR_CLASS(duployan)
IRIS_X4_CHAR_CLASS(egyptian_hieroglyphs)
IRIS_X4_CHAR_CLASS(elbasan)
IRIS_X4_CHAR_CLASS(elymaic)
IRIS_X4_CHAR_CLASS(ethiopic)
IRIS_X4_CHAR_CLASS(georgian)
IRIS_X4_CHAR_CLASS(glagolitic)
IRIS_X4_CHAR_CLASS(gunjala_gondi)
IRIS_X4_CHAR_CLASS(masaram_gondi)
IRIS_X4_CHAR_CLASS(gothic)
IRIS_X4_CHAR_CLASS(grantha)
IRIS_X4_CHAR_CLASS(greek)
IRIS_X4_CHAR_CLASS(gujarati)
IRIS_X4_CHAR_CLASS(gurmukhi)
IRIS_X4_CHAR_CLASS(hangul)
IRIS_X4_CHAR_CLASS(han)
IRIS_X4_CHAR_CLASS(hanunoo)
IRIS_X4_CHAR_CLASS(hatran)
IRIS_X4_CHAR_CLASS(hebrew)
IRIS_X4_CHAR_CLASS(hiragana)
IRIS_X4_CHAR_CLASS(anatolian_hieroglyphs)
IRIS_X4_CHAR_CLASS(pahawh_hmong)
IRIS_X4_CHAR_CLASS(nyiakeng_puachue_hmong)
IRIS_X4_CHAR_CLASS(katakana_or_hiragana)
IRIS_X4_CHAR_CLASS(old_hungarian)
IRIS_X4_CHAR_CLASS(old_italic)
IRIS_X4_CHAR_CLASS(javanese)
IRIS_X4_CHAR_CLASS(kayah_li)
IRIS_X4_CHAR_CLASS(katakana)
IRIS_X4_CHAR_CLASS(kawi)
IRIS_X4_CHAR_CLASS(kharoshthi)
IRIS_X4_CHAR_CLASS(khmer)
IRIS_X4_CHAR_CLASS(khojki)
IRIS_X4_CHAR_CLASS(khitan_small_script)
IRIS_X4_CHAR_CLASS(kannada)
IRIS_X4_CHAR_CLASS(kaithi)
IRIS_X4_CHAR_CLASS(tai_tham)
IRIS_X4_CHAR_CLASS(lao)
IRIS_X4_CHAR_CLASS(latin)
IRIS_X4_CHAR_CLASS(lepcha)
IRIS_X4_CHAR_CLASS(limbu)
IRIS_X4_CHAR_CLASS(linear_a)
IRIS_X4_CHAR_CLASS(linear_b)
IRIS_X4_CHAR_CLASS(lisu)
IRIS_X4_CHAR_CLASS(lycian)
IRIS_X4_CHAR_CLASS(lydian)
IRIS_X4_CHAR_CLASS(mahajani)
IRIS_X4_CHAR_CLASS(makasar)
IRIS_X4_CHAR_CLASS(mandaic)
IRIS_X4_CHAR_CLASS(manichaean)
IRIS_X4_CHAR_CLASS(marchen)
IRIS_X4_CHAR_CLASS(medefaidrin)
IRIS_X4_CHAR_CLASS(mende_kikakui)
IRIS_X4_CHAR_CLASS(meroitic_cursive)
IRIS_X4_CHAR_CLASS(meroitic_hieroglyphs)
IRIS_X4_CHAR_CLASS(malayalam)
IRIS_X4_CHAR_CLASS(modi)
IRIS_X4_CHAR_CLASS(mongolian)
IRIS_X4_CHAR_CLASS(mro)
IRIS_X4_CHAR_CLASS(meetei_mayek)
IRIS_X4_CHAR_CLASS(multani)
IRIS_X4_CHAR_CLASS(myanmar)
IRIS_X4_CHAR_CLASS(nag_mundari)
IRIS_X4_CHAR_CLASS(nandinagari)
IRIS_X4_CHAR_CLASS(old_north_arabian)
IRIS_X4_CHAR_CLASS(nabataean)
IRIS_X4_CHAR_CLASS(newa)
IRIS_X4_CHAR_CLASS(nko)
IRIS_X4_CHAR_CLASS(nushu)
IRIS_X4_CHAR_CLASS(ogham)
IRIS_X4_CHAR_CLASS(ol_chiki)
IRIS_X4_CHAR_CLASS(old_turkic)
IRIS_X4_CHAR_CLASS(oriya)
IRIS_X4_CHAR_CLASS(osage)
IRIS_X4_CHAR_CLASS(osmanya)
IRIS_X4_CHAR_CLASS(old_uyghur)
IRIS_X4_CHAR_CLASS(palmyrene)
IRIS_X4_CHAR_CLASS(pau_cin_hau)
IRIS_X4_CHAR_CLASS(old_permic)
IRIS_X4_CHAR_CLASS(phags_pa)
IRIS_X4_CHAR_CLASS(inscriptional_pahlavi)
IRIS_X4_CHAR_CLASS(psalter_pahlavi)
IRIS_X4_CHAR_CLASS(phoenician)
IRIS_X4_CHAR_CLASS(miao)
IRIS_X4_CHAR_CLASS(inscriptional_parthian)
IRIS_X4_CHAR_CLASS(rejang)
IRIS_X4_CHAR_CLASS(hanifi_rohingya)
IRIS_X4_CHAR_CLASS(runic)
IRIS_X4_CHAR_CLASS(samaritan)
IRIS_X4_CHAR_CLASS(old_south_arabian)
IRIS_X4_CHAR_CLASS(saurashtra)
IRIS_X4_CHAR_CLASS(signwriting)
IRIS_X4_CHAR_CLASS(shavian)
IRIS_X4_CHAR_CLASS(sharada)
IRIS_X4_CHAR_CLASS(siddham)
IRIS_X4_CHAR_CLASS(khudawadi)
IRIS_X4_CHAR_CLASS(sinhala)
IRIS_X4_CHAR_CLASS(sogdian)
IRIS_X4_CHAR_CLASS(old_sogdian)
IRIS_X4_CHAR_CLASS(sora_sompeng)
IRIS_X4_CHAR_CLASS(soyombo)
IRIS_X4_CHAR_CLASS(sundanese)
IRIS_X4_CHAR_CLASS(syloti_nagri)
IRIS_X4_CHAR_CLASS(syriac)
IRIS_X4_CHAR_CLASS(tagbanwa)
IRIS_X4_CHAR_CLASS(takri)
IRIS_X4_CHAR_CLASS(tai_le)
IRIS_X4_CHAR_CLASS(new_tai_lue)
IRIS_X4_CHAR_CLASS(tamil)
IRIS_X4_CHAR_CLASS(tangut)
IRIS_X4_CHAR_CLASS(tai_viet)
IRIS_X4_CHAR_CLASS(telugu)
IRIS_X4_CHAR_CLASS(tifinagh)
IRIS_X4_CHAR_CLASS(tagalog)
IRIS_X4_CHAR_CLASS(thaana)
IRIS_X4_CHAR_CLASS(thai)
IRIS_X4_CHAR_CLASS(tibetan)
IRIS_X4_CHAR_CLASS(tirhuta)
IRIS_X4_CHAR_CLASS(tangsa)
IRIS_X4_CHAR_CLASS(toto)
IRIS_X4_CHAR_CLASS(ugaritic)
IRIS_X4_CHAR_CLASS(vai)
IRIS_X4_CHAR_CLASS(vithkuqi)
IRIS_X4_CHAR_CLASS(warang_citi)
IRIS_X4_CHAR_CLASS(wancho)
IRIS_X4_CHAR_CLASS(old_persian)
IRIS_X4_CHAR_CLASS(cuneiform)
IRIS_X4_CHAR_CLASS(yezidi)
IRIS_X4_CHAR_CLASS(yi)
IRIS_X4_CHAR_CLASS(zanabazar_square)
IRIS_X4_CHAR_CLASS(inherited)
IRIS_X4_CHAR_CLASS(common)
IRIS_X4_CHAR_CLASS(unknown)

#undef IRIS_X4_CHAR_CLASS

} // iris::x4

#endif
