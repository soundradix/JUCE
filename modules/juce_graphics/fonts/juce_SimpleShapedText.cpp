/*
  ==============================================================================

   This file is part of the JUCE framework.
   Copyright (c) Raw Material Software Limited

   JUCE is an open source framework subject to commercial or open source
   licensing.

   By downloading, installing, or using the JUCE framework, or combining the
   JUCE framework with any other source code, object code, content or any other
   copyrightable work, you agree to the terms of the JUCE End User Licence
   Agreement, and all incorporated terms including the JUCE Privacy Policy and
   the JUCE Website Terms of Service, as applicable, which will bind you. If you
   do not agree to the terms of these agreements, we will not license the JUCE
   framework to you, and you must discontinue the installation or download
   process and cease use of the JUCE framework.

   JUCE End User Licence Agreement: https://juce.com/legal/juce-8-licence/
   JUCE Privacy Policy: https://juce.com/juce-privacy-policy
   JUCE Website Terms of Service: https://juce.com/juce-website-terms-of-service/

   Or:

   You may also use this code under the terms of the AGPLv3:
   https://www.gnu.org/licenses/agpl-3.0.en.html

   THE JUCE FRAMEWORK IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL
   WARRANTIES, WHETHER EXPRESSED OR IMPLIED, INCLUDING WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

using FontForRange = std::pair<Range<int64>, Font>;

class ShapedTextOptions
{
public:
    [[nodiscard]] ShapedTextOptions withJustification (Justification x) const
    {
        return withMember (*this, &ShapedTextOptions::justification, x);
    }

    [[nodiscard]] ShapedTextOptions withMaxWidth (float x) const
    {
        return withMember (*this, &ShapedTextOptions::maxWidth, x);
    }

    [[nodiscard]] ShapedTextOptions withHeight (float x) const
    {
        return withMember (*this, &ShapedTextOptions::height, x);
    }

    [[nodiscard]] ShapedTextOptions withFont (Font x) const
    {
        return withMember (*this, &ShapedTextOptions::fontsForRange,
                           std::vector<FontForRange> { { { 0, std::numeric_limits<int64>::max() },
                                                         x } });
    }

    [[nodiscard]] ShapedTextOptions withFontsForRange (const std::vector<FontForRange>& x) const
    {
        return withMember (*this, &ShapedTextOptions::fontsForRange, x);
    }

    [[nodiscard]] ShapedTextOptions withLanguage (StringRef x) const
    {
        return withMember (*this, &ShapedTextOptions::language, x);
    }

    [[nodiscard]] ShapedTextOptions withFirstLineIndent (float x) const
    {
        return withMember (*this, &ShapedTextOptions::firstLineIndent, x);
    }

    /*  This controls the space between lines using a proportional value, with a default of 1.0,
        meaning single line spacing i.e. the descender of the current line + ascender of the next
        line. This value is multiplied by the leading provided here.
    */
    [[nodiscard]] ShapedTextOptions withLeading (float x) const
    {
        return withMember (*this, &ShapedTextOptions::leading, x);
    }

    /*  This controls the space between lines using an additive absolute value, with a default of 0.0.
        This value is added to the spacing between each two lines.
    */
    [[nodiscard]] ShapedTextOptions withAdditiveLineSpacing (float x) const
    {
        return withMember (*this, &ShapedTextOptions::additiveLineSpacing, x);
    }

    [[nodiscard]] ShapedTextOptions withBaselineAtZero (bool x = true) const
    {
        return withMember (*this, &ShapedTextOptions::baselineAtZero, x);
    }

    [[nodiscard]] ShapedTextOptions withTrailingWhitespacesShouldFit (bool x = true) const
    {
        return withMember (*this, &ShapedTextOptions::trailingWhitespacesShouldFit, x);
    }

    [[nodiscard]] ShapedTextOptions withMaxNumLines (int64 x) const
    {
        return withMember (*this, &ShapedTextOptions::maxNumLines, x);
    }

    [[nodiscard]] ShapedTextOptions withEllipsis (String x = String::charToString ((juce_wchar) 0x2026)) const
    {
        return withMember (*this, &ShapedTextOptions::ellipsis, std::move (x));
    }

    [[nodiscard]] ShapedTextOptions withReadingDirection (std::optional<TextDirection> x) const
    {
        return withMember (*this, &ShapedTextOptions::readingDir, x);
    }

    [[nodiscard]] ShapedTextOptions withAllowBreakingInsideWord (bool x = true) const
    {
        return withMember (*this, &ShapedTextOptions::allowBreakingInsideWord, x);
    }

    const auto& getReadingDirection() const             { return readingDir; }
    const auto& getJustification() const                { return justification; }
    const auto& getMaxWidth() const                     { return maxWidth; }
    const auto& getHeight() const                       { return height; }
    const auto& getFontsForRange() const                { return fontsForRange; }
    const auto& getLanguage() const                     { return language; }
    const auto& getFirstLineIndent() const              { return firstLineIndent; }
    const auto& getLeading() const                      { return leading; }
    const auto& getAdditiveLineSpacing() const          { return additiveLineSpacing; }
    const auto& isBaselineAtZero() const                { return baselineAtZero; }
    const auto& getTrailingWhitespacesShouldFit() const { return trailingWhitespacesShouldFit; }
    const auto& getMaxNumLines() const                  { return maxNumLines; }
    const auto& getEllipsis() const                     { return ellipsis; }
    const auto& getAllowBreakingInsideWord() const      { return allowBreakingInsideWord; }

private:
    Justification justification { Justification::topLeft };
    std::optional<TextDirection> readingDir;
    std::optional<float> maxWidth;
    std::optional<float> height;
    std::vector<FontForRange> fontsForRange { { { 0, std::numeric_limits<int64>::max() },
                                                FontOptions { 15.0f } } };
    String language = SystemStats::getDisplayLanguage();
    float firstLineIndent = 0.0f;
    float leading = 1.0f;
    float additiveLineSpacing = 0.0f;
    bool baselineAtZero = false;
    bool allowBreakingInsideWord = false;
    bool trailingWhitespacesShouldFit;
    int64 maxNumLines = std::numeric_limits<int64>::max();
    String ellipsis;
};

struct ShapedGlyph
{
    uint32_t glyphId;
    int64 cluster;
    bool unsafeToBreak;
    bool whitespace;
    Point<float> advance;
    Point<float> offset;
};

struct GlyphLookupEntry
{
    Range<int64> glyphRange;
    bool ltr = true;
};

class SimpleShapedText
{
public:
    /*  Shapes and lays out the first contiguous sequence of ranges specified in the fonts
        parameter.
    */
    SimpleShapedText (const String* data,
                      const ShapedTextOptions& options);

    /*  The returned container associates line numbers with the range of glyphs (not input codepoints)
        that make up the line.
    */
    const auto& getLineNumbers() const { return lineNumbers; }

    const auto& getResolvedFonts() const { return resolvedFonts; }

    Range<int64> getTextRange (int64 glyphIndex) const;

    int64 getNumLines() const { return (int64) lineNumbers.getRanges().size(); }
    int64 getNumGlyphs() const { return (int64) glyphsInVisualOrder.size(); }

    juce_wchar getCodepoint (int64 glyphIndex) const;

    Span<const ShapedGlyph> getGlyphs (Range<int64> glyphRange) const;

    Span<const ShapedGlyph> getGlyphs() const;

private:
    void shape (const String& data,
                const ShapedTextOptions& options);

    const String& string;
    std::vector<ShapedGlyph> glyphsInVisualOrder;
    detail::RangedValues<int64> lineNumbers;
    detail::RangedValues<Font> resolvedFonts;
    detail::RangedValues<GlyphLookupEntry> glyphLookup;

    JUCE_LEAK_DETECTOR (SimpleShapedText)
};

//==============================================================================
using namespace detail;

constexpr hb_script_t getScriptTag (TextScript type)
{
    JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wswitch-enum")
    switch (type)
    {
    case TextScript::common:     return HB_SCRIPT_COMMON;
    case TextScript::arabic:     return HB_SCRIPT_ARABIC;
    case TextScript::armenian:   return HB_SCRIPT_ARMENIAN;
    case TextScript::bengali:    return HB_SCRIPT_BENGALI;
    case TextScript::bopomofo:   return HB_SCRIPT_BOPOMOFO;
    case TextScript::cyrillic:   return HB_SCRIPT_CYRILLIC;
    case TextScript::devanagari: return HB_SCRIPT_DEVANAGARI;
    case TextScript::ethiopic:   return HB_SCRIPT_ETHIOPIC;
    case TextScript::georgian:   return HB_SCRIPT_GEORGIAN;
    case TextScript::greek:      return HB_SCRIPT_GREEK;
    case TextScript::gujarati:   return HB_SCRIPT_GUJARATI;
    case TextScript::gurmukhi:   return HB_SCRIPT_GURMUKHI;
    case TextScript::hangul:     return HB_SCRIPT_HANGUL;
    case TextScript::han:        return HB_SCRIPT_HAN;
    case TextScript::hebrew:     return HB_SCRIPT_HEBREW;
    case TextScript::hiragana:   return HB_SCRIPT_HIRAGANA;
    case TextScript::katakana:   return HB_SCRIPT_KATAKANA;
    case TextScript::kannada:    return HB_SCRIPT_KANNADA;
    case TextScript::khmer:      return HB_SCRIPT_KHMER;
    case TextScript::lao:        return HB_SCRIPT_LAO;
    case TextScript::latin:      return HB_SCRIPT_LATIN;
    case TextScript::malayalam:  return HB_SCRIPT_MALAYALAM;
    case TextScript::oriya:      return HB_SCRIPT_ORIYA;
    case TextScript::sinhala:    return HB_SCRIPT_SINHALA;
    case TextScript::tamil:      return HB_SCRIPT_TAMIL;
    case TextScript::telugu:     return HB_SCRIPT_TELUGU;
    case TextScript::thaana:     return HB_SCRIPT_THAANA;
    case TextScript::thai:       return HB_SCRIPT_THAI;
    case TextScript::tibetan:    return HB_SCRIPT_TIBETAN;
    case TextScript::adlam:      return HB_SCRIPT_ADLAM;
    case TextScript::balinese:   return HB_SCRIPT_BALINESE;
    case TextScript::bamum:      return HB_SCRIPT_BAMUM;
    case TextScript::batak:      return HB_SCRIPT_BATAK;
    case TextScript::chakma:     return HB_SCRIPT_CHAKMA;
    case TextScript::cham:       return HB_SCRIPT_CHAM;
    case TextScript::cherokee:   return HB_SCRIPT_CHEROKEE;
    case TextScript::javanese:   return HB_SCRIPT_JAVANESE;
    case TextScript::kayahLi:    return HB_SCRIPT_KAYAH_LI;
    case TextScript::taiTham:    return HB_SCRIPT_TAI_THAM;
    case TextScript::lepcha:     return HB_SCRIPT_LEPCHA;
    case TextScript::limbu:      return HB_SCRIPT_LIMBU;
    case TextScript::lisu:       return HB_SCRIPT_LISU;
    case TextScript::mandaic:    return HB_SCRIPT_MANDAIC;
    case TextScript::meeteiMayek:return HB_SCRIPT_MEETEI_MAYEK;
    case TextScript::newa:       return HB_SCRIPT_NEWA;
    case TextScript::nko:        return HB_SCRIPT_NKO;
    case TextScript::olChiki:    return HB_SCRIPT_OL_CHIKI;
    case TextScript::osage:      return HB_SCRIPT_OSAGE;
    case TextScript::miao:       return HB_SCRIPT_MIAO;
    case TextScript::saurashtra: return HB_SCRIPT_SAURASHTRA;
    case TextScript::sundanese:  return HB_SCRIPT_SUNDANESE;
    case TextScript::sylotiNagri:return HB_SCRIPT_SYLOTI_NAGRI;
    case TextScript::syriac:     return HB_SCRIPT_SYRIAC;
    case TextScript::taiLe:      return HB_SCRIPT_TAI_LE;
    case TextScript::newTaiLue:  return HB_SCRIPT_NEW_TAI_LUE;
    case TextScript::tifinagh:   return HB_SCRIPT_TIFINAGH;
    case TextScript::vai:        return HB_SCRIPT_VAI;
    case TextScript::wancho:     return HB_SCRIPT_WANCHO;
    case TextScript::yi:         return HB_SCRIPT_YI;

    case TextScript::hanifiRohingya:               return HB_SCRIPT_HANIFI_ROHINGYA;
    case TextScript::canadianAboriginalSyllabics:  return HB_SCRIPT_CANADIAN_SYLLABICS;
    case TextScript::nyiakengPuachueHmong:         return HB_SCRIPT_NYIAKENG_PUACHUE_HMONG;

    default: break;
    }
    JUCE_END_IGNORE_WARNINGS_GCC_LIKE

    return HB_SCRIPT_COMMON;
}

SimpleShapedText::SimpleShapedText (const String* data,
                                    const ShapedTextOptions& options)
    : string (*data)
{
    shape (string, options);
}

struct Utf8Lookup
{
    Utf8Lookup (const String& s)
    {
        const auto base = s.toUTF8();

        for (auto cursor = base; ! cursor.isEmpty(); ++cursor)
            indices.push_back ((size_t) std::distance (base.getAddress(), cursor.getAddress()));

        beyondEnd = s.getNumBytesAsUTF8();
    }

    auto getByteIndex (int64 codepointIndex) const
    {
        jassert (codepointIndex <= (int64) indices.size());

        if (codepointIndex == (int64) indices.size())
            return beyondEnd;

        return indices[(size_t) codepointIndex];
    }

    auto getCodepointIndex (size_t byteIndex) const
    {
        auto it = std::lower_bound (indices.cbegin(), indices.cend(), byteIndex);

        jassert (it != indices.end());

        return (int64) std::distance (indices.begin(), it);
    }

    auto getByteRange (Range<int64> range)
    {
        return Range<size_t> { getByteIndex (range.getStart()),
                               getByteIndex (range.getEnd()) };
    }

private:
    std::vector<size_t> indices;
    size_t beyondEnd{};
};

enum class ControlCharacter
{
    crFollowedByLf,
    cr,
    lf,
    tab
};

static auto findControlCharacters (Span<juce_wchar> text)
{
    constexpr juce_wchar lf = 0x0a;
    constexpr juce_wchar cr = 0x0d;
    constexpr juce_wchar tab = 0x09;

    std::map<size_t, ControlCharacter> result;

    const auto iMax = text.size();

    for (const auto [i, c] : enumerate (text, size_t{}))
    {
        if (c == lf)
        {
            result[i] = ControlCharacter::lf;
            continue;
        }

        if (c == cr)
        {
            if (iMax - i > 1 && text[i + 1] == lf)
                result[i] = ControlCharacter::crFollowedByLf;
            else
                result[i] = ControlCharacter::cr;

            continue;
        }

        if (c == tab)
            result[i] = ControlCharacter::tab;
    }

    return result;
}

/*  Returns glyphs in logical order as that favours wrapping. */
static std::vector<ShapedGlyph> lowLevelShape (const String& string,
                                               Range<int64> range,
                                               const Font& font,
                                               TextScript script,
                                               const String& language,
                                               uint8_t embeddingLevel)
{
    HbBuffer buffer { hb_buffer_create() };
    hb_buffer_clear_contents (buffer.get());

    hb_buffer_set_cluster_level (buffer.get(), HB_BUFFER_CLUSTER_LEVEL_MONOTONE_GRAPHEMES);
    hb_buffer_set_script (buffer.get(), getScriptTag (script));
    hb_buffer_set_language (buffer.get(), hb_language_from_string (language.toRawUTF8(), -1));

    hb_buffer_set_direction (buffer.get(),
                             (embeddingLevel % 2) == 0 ? HB_DIRECTION_LTR : HB_DIRECTION_RTL);

    Utf8Lookup utf8Lookup { string };

    const auto preContextByteRange = utf8Lookup.getByteRange (Range<int64> { 0, range.getStart() });

    hb_buffer_add_utf8 (buffer.get(),
                        string.toRawUTF8() + preContextByteRange.getStart(),
                        (int) preContextByteRange.getLength(),
                        0,
                        0);

    const Span utf32Span { string.toUTF32().getAddress() + (size_t) range.getStart(),
                           (size_t) range.getLength() };

    const auto controlChars = findControlCharacters (utf32Span);
    auto nextControlChar = controlChars.begin();

    for (const auto pair : enumerate (utf32Span, size_t{}))
    {
        const auto charToAdd = [&]
        {
            if (nextControlChar == controlChars.end() || pair.index != nextControlChar->first)
                return pair.value;

            constexpr juce_wchar wordJoiner       = 0x2060;
            constexpr juce_wchar nonBreakingSpace = 0x00a0;

            const auto replacement = nextControlChar->second == ControlCharacter::crFollowedByLf
                                   ? wordJoiner
                                   : nonBreakingSpace;

            ++nextControlChar;

            return replacement;
        }();

        hb_buffer_add (buffer.get(), static_cast<hb_codepoint_t> (charToAdd), (unsigned int) pair.index);
    }

    const auto postContextByteRange = utf8Lookup.getByteRange (Range<int64> { range.getEnd(), (int64) string.length() });

    hb_buffer_add_utf8 (buffer.get(),
                        string.toRawUTF8() + postContextByteRange.getStart(),
                        (int) postContextByteRange.getLength(),
                        0,
                        0);

    std::vector<hb_feature_t> features;

    // Disable ligatures if we're using non-standard tracking
    const auto tracking           = font.getExtraKerningFactor();
    const auto trackingIsDefault  = approximatelyEqual (tracking, 0.0f, absoluteTolerance (0.001f));

    if (! trackingIsDefault)
        for (const auto key : { hbTag ("liga"), hbTag ("clig"), hbTag ("hlig"), hbTag ("dlig"), hbTag ("calt") })
            features.push_back (hb_feature_t { key, 0, HB_FEATURE_GLOBAL_START, HB_FEATURE_GLOBAL_END });

    hb_buffer_guess_segment_properties (buffer.get());

    auto nativeFont = font.getNativeDetails().font;

    if (nativeFont == nullptr)
    {
        jassertfalse;
        return {};
    }

    hb_shape (nativeFont.get(), buffer.get(), features.data(), (unsigned int) features.size());

    const auto [infos, positions] = [&buffer]
    {
        unsigned int count{};

        return std::make_pair (Span { hb_buffer_get_glyph_infos     (buffer.get(), &count), (size_t) count },
                               Span { hb_buffer_get_glyph_positions (buffer.get(), &count), (size_t) count });
    }();

    jassert (infos.size() == positions.size());

    const auto missingGlyph = hb_buffer_get_not_found_glyph (buffer.get());

    [[maybe_unused]] const auto unknownGlyph = std::find_if (infos.begin(), infos.end(), [missingGlyph] (hb_glyph_info_t inf)
    {
        return inf.codepoint == missingGlyph;
    });

    // It this is hit, the typeface can't display one or more characters.
    // This normally shouldn't happen if font fallback is enabled, unless the String contains
    // control characters that JUCE doesn't know how to handle appropriately.
    jassert (unknownGlyph == infos.end());

    [[maybe_unused]] const auto trackingAmount = ! trackingIsDefault
                                               ? font.getHeight() * font.getHorizontalScale() * tracking
                                               : 0;

    std::vector<size_t> clusterLookup;
    std::vector<size_t> characterLookup;
    std::vector<ShapedGlyph> glyphs;

    std::optional<uint32_t> lastCluster;

    for (size_t i = 0; i < infos.size(); ++i)
    {
        const auto j = (embeddingLevel % 2) == 0 ? i : infos.size() - 1 - i;

        const auto glyphId = infos[j].codepoint;
        const auto xAdvance = positions[j].x_advance;

        // For certain OS, Font and glyph ID combinations harfbuzz will not find extents data and
        // hb_font_get_glyph_extents will return false. In such cases Typeface::getGlyphBounds
        // will return an empty rectangle. Here we need to distinguish this situation from the one
        // where extents information is available and is an empty rectangle, which indicates a
        // whitespace.
        const auto extentsDataAvailable = std::invoke ([&]
        {
            hb_glyph_extents_t extents{};
            return hb_font_get_glyph_extents (font.getTypefacePtr()->getNativeDetails().getFont(),
                                              (hb_codepoint_t) glyphId,
                                              &extents);
        });

        const auto whitespace = extentsDataAvailable
                                && font.getTypefacePtr()->getGlyphBounds (font.getMetricsKind(), (int) glyphId).isEmpty()
                                && xAdvance > 0;

        // Tracking is only applied at the beginning of a new cluster to avoid inserting it before
        // diacritic marks.
        const auto appliedTracking = std::exchange (lastCluster, infos[j].cluster) != infos[j].cluster
                                   ? trackingAmount
                                   : 0;

        glyphs.push_back ({
            glyphId,
            (int64) infos[j].cluster + range.getStart(),
            (infos[j].mask & HB_GLYPH_FLAG_UNSAFE_TO_BREAK) != 0,
            whitespace,
            Point<float> { HbScale::hbToJuce (xAdvance) + appliedTracking, -HbScale::hbToJuce (positions[j].y_advance) },
            Point<float> { HbScale::hbToJuce (positions[j].x_offset), -HbScale::hbToJuce (positions[j].y_offset) },
        });
    }

    return glyphs;
}

template <typename T>
struct SubSpanLookup
{
    explicit SubSpanLookup (Span<T> enclosingSpan)
        : enclosing (enclosingSpan)
    {}

    auto getRange (Span<T> span) const
    {
        jassert (enclosing.begin() <= span.begin() && enclosing.size() >= span.size());

        return Range<int64>::withStartAndLength ((int64) std::distance (enclosing.begin(), span.begin()),
                                                 (int64) span.size());
    }

    auto getSpan (Range<int64> r) const
    {
        jassert (r.getStart() + r.getLength() <= (int64) enclosing.size());

        return Span<T> { enclosing.begin() + r.getStart(), (size_t) r.getLength() };
    }

private:
    Span<T> enclosing;
};

template <typename T>
static auto makeSubSpanLookup (Span<T> s) { return SubSpanLookup<T> { s }; }

struct CanBreakBeforeIterator
{
    explicit CanBreakBeforeIterator (Span<const Unicode::Codepoint> s)
        : span (s),
          cursor (span.begin())
    {}

    const Unicode::Codepoint* next()
    {
        while (++cursor < span.begin() + span.size())
        {
            // Disallow soft break before a hard break
            const auto nextCodepointIsLinebreak = [&]
            {
                const auto nextCursor = cursor + 1;

                if (! (nextCursor < span.begin() + span.size()))
                    return false;

                return nextCursor->codepoint == 0x0a || nextCursor->codepoint == 0x0d;
            }();

            if (cursor->breaking == TextBreakType::soft && ! nextCodepointIsLinebreak)
                return cursor + 1;  // Use the same "can break before" semantics as Harfbuzz
        }

        return nullptr;
    }

    Span<const Unicode::Codepoint> span;
    const Unicode::Codepoint* cursor;
};

/*  Returns integers relative to the initialising Span's begin(), before which a linebreak is
    possible.

    Can be restricted to a sub-range using reset().
*/
struct IntegralCanBreakBeforeIterator
{
    explicit IntegralCanBreakBeforeIterator (Span<const Unicode::Codepoint> s)
        : span (s),
          it (span)
    {}

    void reset()
    {
        reset ({ std::numeric_limits<int64>::min(),  std::numeric_limits<int64>::max() });
    }

    void reset (Range<int64> r)
    {
        jassert ((size_t) r.getLength() <= span.size());

        restrictedTo = r;
        it = CanBreakBeforeIterator { span };
        rangeEndReturned = false;
    }

    std::optional<int64> next()
    {
        const auto intValue = [&] (auto p) { return (int64) std::distance (span.begin(), p); };

        for (auto* ptr = it.next(); ptr != nullptr; ptr = it.next())
        {
            const auto v = intValue (ptr);

            if (v > restrictedTo.getEnd())
                break;

            if (restrictedTo.getStart() < v && v <= restrictedTo.getEnd())
                return v;
        }

        if (std::exchange (rangeEndReturned, true) == false)
            return std::min ((int64) span.size(), restrictedTo.getEnd());

        return std::nullopt;
    }

private:
    Span<const Unicode::Codepoint> span;
    CanBreakBeforeIterator it;
    Range<int64> restrictedTo { std::numeric_limits<int64>::min(),  std::numeric_limits<int64>::max() };

    bool rangeEndReturned = false;
};

struct ShapingParams
{
    TextScript script;
    String language;
    uint8_t embeddingLevel;
    Font resolvedFont;
};

// Used to avoid signedness warning for types for which std::size() is int
template <typename T>
static auto makeSpan (T& array)
{
    return Span { array.getRawDataPointer(), (size_t) array.size() };
}

static std::vector<FontForRange> findSuitableFontsForText (const Font& font,
                                                           const String& text,
                                                           const String& language = {})
{
    detail::RangedValues<std::optional<Font>> fonts;
    fonts.set ({ 0, (int64) text.length() }, font);

    const auto getResult = [&]
    {
        std::vector<FontForRange> result;

        for (const auto [r, v] : fonts)
            result.emplace_back (r, v.value_or (font));

        return result;
    };

    if (! font.getFallbackEnabled())
        return getResult();

    const auto markMissingGlyphs = [&]
    {
        auto it = text.begin();
        std::vector<int64> fontNotFound;

        for (const auto [r, f] : fonts)
        {
            for (auto i = r.getStart(); i < r.getEnd(); ++i)
            {
                if (f.has_value() && ! isFontSuitableForCodepoint (*f, *it))
                    fontNotFound.push_back (i);

                ++it;
            }
        }

        for (const auto i : fontNotFound)
            fonts.set ({ i, i + 1 }, std::nullopt);

        return fontNotFound.size();
    };

    // We keep calling findSuitableFontForText for sub-ranges without a suitable font until we
    // can't find any more suitable fonts or all codepoints have one
    for (auto numMissingGlyphs = markMissingGlyphs(); numMissingGlyphs > 0;)
    {
        std::vector<FontForRange> changes;

        for (const auto [r, f] : fonts)
        {
            if (! f.has_value())
            {
                changes.emplace_back (r, font.findSuitableFontForText (text.substring ((int) r.getStart(),
                                                                                       (int) r.getEnd()),
                                                                       language));
            }
        }

        for (const auto& c : changes)
            fonts.set (c.first, c.second);

        if (const auto newNumMissingGlyphs = markMissingGlyphs();
            std::exchange (numMissingGlyphs, newNumMissingGlyphs) == newNumMissingGlyphs)
        {
            // We failed to resolve any more fonts during the last pass
            break;
        }
    }

    return getResult();
}

static RangedValues<Font> resolveFontsWithFallback (const String& string, const RangedValues<Font>& fonts)
{
    RangedValues<Font> resolved;

    for (const auto [r, f] : fonts)
    {
        auto rf = findSuitableFontsForText (f, string.substring ((int) r.getStart(),
                                                                 (int) std::min (r.getEnd(), (int64) string.length())));

        for (auto& item : rf)
            item.first += r.getStart();

        resolved.setForEach<MergeEqualItems::no> (rf.begin(), rf.end());
    }

    return resolved;
}

struct GlyphsStorage
{
    std::shared_ptr<std::vector<ShapedGlyph>> data;
    bool ltr{};
    Font font;
};

struct OwnedGlyphsSpan
{
public:
    OwnedGlyphsSpan (GlyphsStorage subOwnedGlyphsSpanIn,
                     Span<const ShapedGlyph> glyphsIn,
                     Range<int64> textRangeIn,
                     size_t visualOrderIn)
        : subOwnedGlyphsSpan { std::move (subOwnedGlyphsSpanIn) },
          glyphs { glyphsIn },
          textRange { textRangeIn },
          visualOrder { visualOrderIn }
    {}

    auto& operator* ()        { return glyphs; }
    auto& operator* () const  { return glyphs; }

    auto operator-> ()        { return &glyphs; }
    auto operator-> () const  { return &glyphs; }

    bool operator== (const OwnedGlyphsSpan& other) const
    {
        return glyphs.data() == other.glyphs.data()
               && glyphs.size() == other.glyphs.size();
    }

    bool operator!= (const OwnedGlyphsSpan& other) const
    {
        return ! (*this == other);
    }

    auto getVisualOrder() const { return visualOrder; }
    auto isLtr()          const { return subOwnedGlyphsSpan.ltr; }
    auto getTextRange()   const { return textRange; }
    const auto& getFont() const { return subOwnedGlyphsSpan.font; }

private:
    GlyphsStorage subOwnedGlyphsSpan;
    Span<const ShapedGlyph> glyphs;
    Range<int64> textRange;
    size_t visualOrder;
};

/* Objects of this type contain a ShapedGlyph range that terminates with a glyph after which
   soft-wrapping is possible. There are no soft-break opportunities anywhere else inside the range.
*/
using WrappedGlyphs = std::vector<OwnedGlyphsSpan>;

/* Contains a WrappedGlyphs object and marks a location (a particular glyph) somewhere inside it.

   Allows keeping track of partially consuming such objects to support mid-word breaking where the
   line is shorter than a single word.
*/
struct WrappedGlyphsCursor
{
    WrappedGlyphsCursor (const OwnedGlyphsSpan* dataIn, size_t num)
        : data { dataIn, num }
    {}

    bool empty() const
    {
        return data.empty() || data.back()->empty();
    }

    bool isBeyondEnd() const
    {
        return empty() || data.size() <= index.i;
    }

    auto& operator+= (size_t d)
    {
        while (d > 0 && ! isBeyondEnd())
        {
            const auto delta = std::min (d, data[index.i]->size() - index.j);
            index.j += delta;
            d -= delta;

            if (index.j == data[index.i]->size())
            {
                ++index.i;
                index.j = 0;
            }
        }

        return *this;
    }

    auto& operator++()
    {
        return *this += 1;
    }

    auto& operator*()       { return (*data[index.i])[index.j]; }
    auto& operator*() const { return (*data[index.i])[index.j]; }

    auto* operator->()       { return &(*data[index.i])[index.j]; }
    auto* operator->() const { return &(*data[index.i])[index.j]; }

    size_t size() const
    {
        if (empty() || isBeyondEnd())
            return 0;

        size_t size{};

        for (auto copy = *this; ! copy.isBeyondEnd(); ++copy)
            ++size;

        return size;
    }

    auto getTextRange() const
    {
        Range<int64> textRange;

        for (const auto& chunk : data)
            textRange = textRange.getUnionWith (chunk.getTextRange());

        return textRange;
    }

    bool operator== (const WrappedGlyphsCursor& other) const
    {
        const auto tie = [] (auto& x) { return std::tuple (x.data.data(), x.data.size(), x.index); };
        return tie (*this) == tie (other);
    }

    bool operator!= (const WrappedGlyphsCursor& other) const
    {
        return ! operator== (other);
    }

    auto& back()       { return data.back()->back(); }
    auto& back() const { return data.back()->back(); }

    struct ShapedGlyphSpan
    {
        const ShapedGlyph* start;
        const ShapedGlyph* end;
        size_t visualOrder;
        Range<int64> textRange;
        Font font;
    };

    std::vector<ShapedGlyphSpan> getShapedGlyphSpansUpTo (const WrappedGlyphsCursor& end) const
    {
        std::vector<ShapedGlyphSpan> spans;

        if (data.data() != end.data.data() || data.size() != end.data.size())
        {
            jassertfalse;
            return spans;
        }

        for (auto indexCopy = index; indexCopy < end.index;)
        {
            auto& chunk = data[indexCopy.i];

            const auto glyphsStart = chunk->begin() + indexCopy.j;
            const auto glyphsEnd = chunk->end() - (indexCopy.i < end.index.i ? 0 : chunk->size() - end.index.j);

            const auto directionalStart = chunk.isLtr() ? glyphsStart : glyphsEnd - 1;
            const auto directionalEnd = chunk.isLtr() ? glyphsEnd : glyphsStart - 1;

            const auto textStart = glyphsStart->cluster;
            const auto textEnd = glyphsEnd < chunk->end() ? glyphsEnd->cluster : chunk.getTextRange().getEnd();

            spans.push_back ({ directionalStart,
                               directionalEnd,
                               chunk.getVisualOrder(),
                               { textStart, textEnd },
                               chunk.getFont() });

            ++indexCopy.i;
            indexCopy.j = 0;
        }

        return spans;
    }

private:
    struct Index
    {
        size_t i{}, j{};

        auto asTuple() const { return std::make_tuple (i, j); }
        bool operator== (const Index& other) const { return asTuple() == other.asTuple(); }
        bool operator<  (const Index& other) const { return asTuple() <  other.asTuple(); }
    };

    Span<const OwnedGlyphsSpan> data;
    Index index;
};

template <typename T>
static auto createRangedValues (const std::vector<std::pair<Range<int64>, T>>& pairs, int64 offset = 0)
{
    detail::RangedValues<T> result;

    for (const auto& [range, value] : pairs)
        result.insert (range.movedToStartAt (range.getStart() - offset), value);

    result.eraseUpTo (0);

    return result;
}

struct Shaper
{
    Shaper (const String& stringIn, Range<int64> shapingRange, const ShapedTextOptions& options)
        : string { stringIn.substring ((int) shapingRange.getStart(), (int) shapingRange.getEnd()) }
    {
        const auto analysis = Unicode::performAnalysis (string);

        const auto string32 = std::invoke ([this]
                                           {
                                               std::vector<juce_wchar> s32 ((size_t) string.length());
                                               string.copyToUTF32 (s32.data(), s32.size() * sizeof (juce_wchar));
                                               return s32;
                                           });

        const BidiAlgorithm bidiAlgorithm { string32 };
        const auto bidiParagraph = bidiAlgorithm.createParagraph (0, options.getReadingDirection());
        const auto bidiLine = bidiParagraph.createLine (0, bidiParagraph.getLength());
        bidiLine.computeVisualOrder (visualOrder);

        const auto bidiLevels = bidiParagraph.getResolvedLevels();

        const auto fonts = resolveFontsWithFallback (string,
                                                     createRangedValues (options.getFontsForRange(),
                                                                         shapingRange.getStart()));

        for (Unicode::LineBreakIterator lineIter { makeSpan (analysis) }; auto lineRun = lineIter.next();)
        {
            for (Unicode::ScriptRunIterator scriptIter { *lineRun }; auto scriptRun = scriptIter.next();)
            {
                const auto offsetInText = (size_t) std::distance (analysis.getRawDataPointer(), scriptRun->data());
                const auto length = scriptRun->size();

                const auto begin = bidiLevels.data() + offsetInText;
                const auto end = begin + length;

                for (auto it = begin; it != end;)
                {
                    const auto next = std::find_if (it, end, [&] (const auto& l) { return l != *it; });
                    const auto bidiStart = (int64) std::distance (bidiLevels.data(), it);
                    const auto bidiLength = (int64) std::distance (it, next);
                    const auto bidiRange = Range<int64>::withStartAndLength (bidiStart, bidiLength);

                    for (const auto& [range, font] : fonts.getIntersectionsWith (bidiRange))
                    {
                        shaperRuns.set<MergeEqualItems::no> (range,
                                                             { scriptRun->front().script,
                                                               options.getLanguage(),
                                                               *it,
                                                               font });
                    }

                    it = next;
                }
            }
        }

        IntegralCanBreakBeforeIterator softBreakIterator { makeSpan (analysis) };

        for (auto breakBefore = softBreakIterator.next();
             breakBefore.has_value();
             breakBefore = softBreakIterator.next())
        {
            auto v = *breakBefore;

            if (softBreakBeforePoints.empty() || softBreakBeforePoints.back() != v)
                softBreakBeforePoints.push_back (v);
        }
    }

    WrappedGlyphs getChunksUpToNextSafeBreak (int64 startFrom)
    {
        const auto nextSoftBreakBefore = std::invoke ([&]
                                                      {
                                                          const auto it = std::upper_bound (softBreakBeforePoints.begin(),
                                                                                            softBreakBeforePoints.end(),
                                                                                            startFrom);

                                                          if (it == softBreakBeforePoints.end())
                                                              return (int64) visualOrder.size();

                                                          return *it;
                                                      });

        if (! shapedGlyphs.getRanges().covers ({ startFrom, nextSoftBreakBefore }))
        {
            for (auto it = shaperRuns.find (startFrom);
                 it != shaperRuns.end() && it->range.getStart() < nextSoftBreakBefore;
                 ++it)
            {
                const Range<int64> shapingRange { std::max (startFrom, it->range.getStart()), it->range.getEnd() };
                jassert (! shapingRange.isEmpty());

                auto g = lowLevelShape (string,
                                        shapingRange,
                                        it->value.resolvedFont,
                                        it->value.script,
                                        it->value.language,
                                        it->value.embeddingLevel);

                shapedGlyphs.set<MergeEqualItems::no> (shapingRange,
                                                       {
                                                           std::make_shared<std::vector<ShapedGlyph>> (std::move (g)),
                                                           it->value.embeddingLevel % 2 == 0,
                                                           it->value.resolvedFont
                                                       });
            }
        }

        auto glyphsIt = shapedGlyphs.find (startFrom);

        if (glyphsIt == shapedGlyphs.end())
            return {};

        WrappedGlyphs result;

        while (true)
        {
            const ShapedGlyph* start = glyphsIt->value.data->data();
            const ShapedGlyph* const endIt = glyphsIt->value.data->data() + glyphsIt->value.data->size();

            while (start < endIt && start->cluster < startFrom)
                ++start;

            const ShapedGlyph* end = start;

            while (end < endIt && end->cluster < nextSoftBreakBefore)
                ++end;

            result.push_back ({ glyphsIt->value,
                                Span<const ShapedGlyph> { start, (size_t) std::distance (start, end) },
                                { startFrom, nextSoftBreakBefore },
                                visualOrder[(size_t) start->cluster] });

            if (end != endIt && end->cluster >= nextSoftBreakBefore)
                break;

            ++glyphsIt;

            if (glyphsIt == shapedGlyphs.end())
                break;
        }

        return result;
    }

    String string;
    std::vector<size_t> visualOrder;
    RangedValues<ShapingParams> shaperRuns;
    std::vector<int64> softBreakBeforePoints;
    RangedValues<GlyphsStorage> shapedGlyphs;
};

struct LineState
{
    LineState() = default;

    LineState (float w, bool f)
        : maxWidth { w },
          trailingWhitespaceCanExtendBeyondMargin { f }
    {}

    bool isInTrailingPosition (const ShapedGlyph& glyph) const
    {
        return glyph.cluster >= largestVisualOrderInLine;
    }

    bool isEmpty() const
    {
        return largestVisualOrderInLine < 0;
    }

    int64 largestVisualOrderInLine = -1;
    float maxWidth{};
    float width{};
    bool trailingWhitespaceCanExtendBeyondMargin;
};

struct WrappedGlyphsCursorRange
{
    WrappedGlyphsCursor begin, end;
};

class LineOfWrappedGlyphCursorRanges
{
public:
    LineOfWrappedGlyphCursorRanges() = default;

    LineOfWrappedGlyphCursorRanges (float maxWidth, bool trailingWhitespaceCanExtendBeyondMargin)
        : state { maxWidth, trailingWhitespaceCanExtendBeyondMargin }
    {}

    /*  Consumes as many glyphs from the provided cursor as the line will still fit. Returns the end
        cursor i.e. the state of the cursor after the glyphs have been consumed.

        If the line is empty it will partially consume a WrappedGlyphsCursor, otherwise only all of it
        or none of it.

        Always consumes at least one glyph. If forceConsumeFirstWord is true, it consumes at least
        one word.
    */
    WrappedGlyphsCursor consume (const WrappedGlyphsCursor& glyphIt, bool forceConsumeFirstWord)
    {
        if (forceConsumeFirstWord && state.isEmpty())
        {
            auto [newState, newIt] = consumeIf (state, glyphIt, [] (auto&, auto&) { return true; });
            consumedChunks.push_back ({ glyphIt, newIt });
            state = std::move (newState);
            return newIt;
        }

        auto [newState, newIt] = consumeIf (state, glyphIt, [] (auto& nextState, auto& glyph)
                                            {
                                                const auto remainingWidth = nextState.maxWidth - nextState.width;

                                                return nextState.isEmpty()
                                                       || glyph.advance.getX() <= remainingWidth
                                                       || (nextState.trailingWhitespaceCanExtendBeyondMargin
                                                           && glyph.whitespace
                                                           && nextState.isInTrailingPosition (glyph));
                                            });

        // A OwnedGlyphsSpan always ends in the first valid breakpoint. We can only consume all of it or
        // none of it. Unless the line is still empty, which means that it's too short to fit even
        // a single word.
        if (! state.isEmpty() && ! newIt.isBeyondEnd())
            return glyphIt;

        if (newIt != glyphIt)
            consumedChunks.push_back ({ glyphIt, newIt });

        state = std::move (newState);

        return newIt;
    }

    const auto& getConsumedChunks() const
    {
        return consumedChunks;
    }

private:
    static std::pair<LineState, WrappedGlyphsCursor> consumeIf (const LineState& state,
                                                                const WrappedGlyphsCursor& it,
                                                                std::function<bool (const LineState&, const ShapedGlyph&)> predicate)
    {
        auto newState = state;
        auto newIt = it;

        while (! newIt.isBeyondEnd() && predicate (newState, *newIt))
        {
            newState.width += newIt->advance.getX();
            newState.largestVisualOrderInLine = std::max (newState.largestVisualOrderInLine, newIt->cluster);
            ++newIt;
        }

        return { std::move (newState), std::move (newIt) };
    }

    LineState state;
    std::vector<WrappedGlyphsCursorRange> consumedChunks;
};

struct LineDataAndChunkStorage
{
    std::vector<WrappedGlyphs> chunkStorage;
    std::vector<std::vector<WrappedGlyphsCursorRange>> lines;
};

struct FillLinesOptions
{
    FillLinesOptions withWidth (float x) const
    {
        return withMember (*this, &FillLinesOptions::width, x);
    }

    FillLinesOptions withFirstLinePadding (float x) const
    {
        return withMember (*this, &FillLinesOptions::firstLinePadding, x);
    }

    FillLinesOptions withTrailingWhitespaceCanExtendBeyondMargin (bool x = true) const
    {
        return withMember (*this, &FillLinesOptions::trailingWhitespaceCanExtendBeyondMargin, x);
    }

    FillLinesOptions withForceConsumeFirstWord (bool x = true) const
    {
        return withMember (*this, &FillLinesOptions::forceConsumeFirstWord, x);
    }

    LineDataAndChunkStorage fillLines (Shaper& shaper) const
    {
        LineDataAndChunkStorage result;
        LineOfWrappedGlyphCursorRanges line { width - firstLinePadding, trailingWhitespaceCanExtendBeyondMargin };

        for (auto chunks = shaper.getChunksUpToNextSafeBreak (0);
             ! chunks.empty();)
        {
            result.chunkStorage.push_back (std::move (chunks));
            WrappedGlyphsCursor cursor { result.chunkStorage.back().data(),
                                         result.chunkStorage.back().size() };

            while (! cursor.isBeyondEnd())
            {
                cursor = line.consume (cursor, forceConsumeFirstWord);

                if (! cursor.isBeyondEnd())
                {
                    result.lines.push_back (line.getConsumedChunks());
                    line = LineOfWrappedGlyphCursorRanges { width, trailingWhitespaceCanExtendBeyondMargin };
                }
            }

            chunks = shaper.getChunksUpToNextSafeBreak (cursor.getTextRange().getEnd());
        }

        result.lines.push_back (line.getConsumedChunks());

        return result;
    }

    float width{};
    float firstLinePadding{};
    bool trailingWhitespaceCanExtendBeyondMargin = false;
    bool forceConsumeFirstWord = false;
};

static auto getShapedGlyphSpansInVisualOrder (const std::vector<WrappedGlyphsCursorRange>& lineData)
{
    std::vector<WrappedGlyphsCursor::ShapedGlyphSpan> glyphSpans;

    for (const auto& chunk : lineData)
    {
        auto spans = chunk.begin.getShapedGlyphSpansUpTo (chunk.end);
        glyphSpans.insert (glyphSpans.begin(), spans.begin(), spans.end());
    }

    std::sort (glyphSpans.begin(),
               glyphSpans.end(),
               [] (const auto& a, const auto& b)
               {
                   return a.visualOrder < b.visualOrder;
               });

    return glyphSpans;
}

static auto getLineRanges (const String& data)
{
    std::vector<Range<int64>> lineRanges;

    const auto analysis = Unicode::performAnalysis (data);
    const auto spanLookup = makeSubSpanLookup (makeSpan (analysis));

    for (Unicode::LineBreakIterator lineIter { makeSpan (analysis) }; auto lineRun = lineIter.next();)
        lineRanges.push_back (spanLookup.getRange (*lineRun));

    return lineRanges;
}

static void foldLinesBeyondLineLimit (std::vector<std::vector<WrappedGlyphsCursorRange>>& lines,
                                      size_t maxNumLines)
{
    if (lines.size() <= maxNumLines || maxNumLines == 0)
        return;

    auto& lastLine = lines[maxNumLines - 1];

    for (auto i = maxNumLines; i < lines.size(); ++i)
        lastLine.insert (lastLine.end(), lines[i].begin(), lines[i].end());

    lines.erase (iteratorWithAdvance (lines.begin(), maxNumLines), lines.end());
}

void SimpleShapedText::shape (const String& data,
                              const ShapedTextOptions& options)
{
    for (const auto& lineRange : getLineRanges (data))
    {
        Shaper shaper { data, lineRange, options };
        auto lineDataAndStorage = FillLinesOptions{}.withWidth (options.getMaxWidth().value_or ((float) 1e6))
                                                    .withFirstLinePadding (options.getFirstLineIndent())
                                                    .withTrailingWhitespaceCanExtendBeyondMargin (! options.getTrailingWhitespacesShouldFit())
                                                    .withForceConsumeFirstWord (! options.getAllowBreakingInsideWord())
                                                    .fillLines (shaper);
        auto& lineData = lineDataAndStorage.lines;

        foldLinesBeyondLineLimit (lineData, (size_t) options.getMaxNumLines() - lineNumbers.size());

        if (lineNumbers.size() >= (size_t) options.getMaxNumLines())
            break;

        for (const auto& line : lineData)
        {
            const auto glyphSpansInLine = getShapedGlyphSpansInVisualOrder (line);

            const auto lineStart = (int64) glyphsInVisualOrder.size();

            for (const auto& s : glyphSpansInLine)
            {
                const auto start = (int64) glyphsInVisualOrder.size();
                bool ltr = true;

                if (s.start < s.end)
                {
                    for (auto it = s.start; it < s.end; ++it)
                        glyphsInVisualOrder.push_back (*it);
                }
                else
                {
                    ltr = false;

                    for (auto it = s.start; it > s.end; --it)
                        glyphsInVisualOrder.push_back (*it);
                }

                const auto end = (int64) glyphsInVisualOrder.size();

                for (auto i = start; i < end; ++i)
                    glyphsInVisualOrder[(size_t) i].cluster += lineRange.getStart();

                glyphLookup.set<MergeEqualItems::no> (s.textRange + lineRange.getStart(), { { start, end }, ltr });
                resolvedFonts.set ({ start, end }, s.font);
            }

            const auto lineEnd = (int64) glyphsInVisualOrder.size();
            lineNumbers.set ({ lineStart, lineEnd}, (int64) lineNumbers.size());
        }
    }
}

Span<const ShapedGlyph> SimpleShapedText::getGlyphs (Range<int64> glyphRange) const
{
    const auto r = glyphRange.getIntersectionWith ({ 0, (int64) glyphsInVisualOrder.size() });

    return { glyphsInVisualOrder.data() + r.getStart(), (size_t) r.getLength() };
}

Span<const ShapedGlyph> SimpleShapedText::getGlyphs() const
{
    return glyphsInVisualOrder;
}

juce_wchar SimpleShapedText::getCodepoint (int64 glyphIndex) const
{
    return string[(int) glyphsInVisualOrder[(size_t) glyphIndex].cluster];
}

Range<int64> SimpleShapedText::getTextRange (int64 glyphIndex) const
{
    jassert (isPositiveAndBelow (glyphIndex, getNumGlyphs()));

    // A single glyph can span multiple input codepoints. We can discover this by checking the
    // neighbouring glyphs cluster values. If neighbouring values differ by more than one, then the
    // missing clusters belong to a single glyph.
    //
    // However, we only have to check glyphs that are in the same bidi run as this one, hence the
    // lookup.
    const auto startingCodepoint = glyphsInVisualOrder[(size_t) glyphIndex].cluster;
    const auto glyphRange = glyphLookup.getItemWithEnclosingRange (startingCodepoint)->value.glyphRange;

    const auto glyphRun = Span<const ShapedGlyph> { glyphsInVisualOrder.data() + glyphRange.getStart(),
                                                    (size_t) glyphRange.getLength() };

    const auto indexInRun = glyphIndex - glyphRange.getStart();

    const auto cluster = glyphRun[(size_t) indexInRun].cluster;

    const auto nextAdjacentCluster = [&]
    {
        auto left = [&]
        {
            for (auto i = indexInRun; i >= 0; --i)
                if (auto c = glyphRun[(size_t) i].cluster; c != cluster)
                    return c;

            return cluster;
        }();

        auto right = [&]
        {
            for (auto i = indexInRun; i < (decltype (i)) glyphRun.size(); ++i)
                if (auto c = glyphRun[(size_t) i].cluster; c != cluster)
                    return c;

            return cluster;
        }();

        return std::max (left, right);
    }();

    return Range<int64>::withStartAndLength (cluster, std::max ((int64) 1, nextAdjacentCluster - cluster));
}

#if JUCE_UNIT_TESTS

struct SimpleShapedTextTests : public UnitTest
{
    SimpleShapedTextTests()
        : UnitTest ("SimpleShapedText", UnitTestCategories::text)
    {
    }

    static constexpr const char* testStrings[]
    {
        "Some trivial text",
        "Text with \r\n\r\n line feed and new line characters",
        "\nPrepending new line character",
        "\n\nMultiple prepending new line characters",
        "\n\nMultiple prepending and trailing line feed or new line characters\n\r\n",
        "Try right-clicking on a slider for an options menu. \n\nAlso, holding down CTRL while dragging will turn on a slider's velocity-sensitive mode",
    };

    void runTest (const char* text, float maxWidth)
    {
        const auto defaultTypeface = Font::getDefaultTypefaceForFont (FontOptions{});

        if (defaultTypeface == nullptr)
        {
            DBG ("Skipping test: No default typeface found!");
            return;
        }

        String testString { text };

        SimpleShapedText st { &testString, ShapedTextOptions{}.withFont (FontOptions { defaultTypeface })
                                                              .withMaxWidth (maxWidth) };

        auto success = true;

        for (int64 glyphIndex = 0; glyphIndex < st.getNumGlyphs(); ++glyphIndex)
        {
            const auto textRange = st.getTextRange (glyphIndex);

            // This assumption holds for LTR text if no ligatures are used
            success &= textRange.getStart() == glyphIndex && textRange.getLength() == 1;
        }

        expect (success, String { "Failed for test string: " } + testString.replace ("\r", "<CR>")
                                                                           .replace ("\n", "<LF>"));
    }

    void runTest() override
    {
        beginTest ("getTextRange: LTR Latin text without ligatures - no soft breaks");
        {
            for (auto* testString : testStrings)
                runTest (testString, 100'000.0f);
        }

        beginTest ("getTextRange: LTR Latin text without ligatures - with soft breaks");
        {
            for (auto* testString : testStrings)
                runTest (testString, 60.0f);
        }
    }
};

static SimpleShapedTextTests simpleShapedTextTests;

#endif

} // namespace juce