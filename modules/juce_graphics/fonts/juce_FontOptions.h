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

/**
    Options that describe a particular font.

    Used to construct Font instances in a fluent style.

    @see Typeface, Font

    @tags{Graphics}
*/
class JUCE_API FontOptions final
{
public:
    /** Constructs the default set of options. */
    FontOptions();

    /** Constructs the default set of options with a custom height. */
    explicit FontOptions (float fontHeight);

    /** Constructs the default set of options with a custom height and style.

        @param fontHeight   the height in pixels (can be fractional)
        @param styleFlags   the style to use - this can be a combination of the
                            Font::bold, Font::italic and Font::underlined, or
                            just Font::plain for the normal style.

        @see Font::FontStyleFlags, Font::getDefaultSansSerifFontName
    */
    FontOptions (float fontHeight, int styleFlags);

    /** Constructs the default set of options with a given typeface and parameters.

        @param typefaceName the font family of the typeface to use
        @param fontHeight   the height in pixels (can be fractional)
        @param styleFlags   the style to use - this can be a combination of the
                            Font::bold, Font::italic and Font::underlined, or
                            just Font::plain for the normal style.
        @see Font::FontStyleFlags, Font::getDefaultSansSerifFontName
    */
    FontOptions (const String& typefaceName, float fontHeight, int styleFlags);

    /** Constructs the default set of options with a given typeface and parameters.

        @param typefaceName  the font family of the typeface to use
        @param typefaceStyle the font style of the typeface to use
        @param fontHeight    the height in pixels (can be fractional)
    */
    FontOptions (const String& typefaceName, const String& typefaceStyle, float fontHeight);

    /** Constructs the default set of options with a given typeface. */
    FontOptions (const Typeface::Ptr& typeface);

    /** Returns a copy of these options with a new typeface name.
        If the options include a non-null Typeface::Ptr, this will be ignored.
        Otherwise, a suitable typeface will be located based on the typeface name and style strings.
    */
    [[nodiscard]] FontOptions withName            (String x)               const { return withMember (*this, &FontOptions::name, x); }

    /** Returns a copy of these options with a new typeface style.
        If the options include a non-null Typeface::Ptr, this will be ignored.
        Otherwise, a suitable typeface will be located based on the typeface name and style strings.
    */
    [[nodiscard]] FontOptions withStyle           (String x)               const { return withMember (*this, &FontOptions::style, x); }

    /** Returns a copy of these options with a new typeface.
        If the typeface is non-null, it takes precedence over the name and style strings.
    */
    [[nodiscard]] FontOptions withTypeface        (Typeface::Ptr x)        const { return withMember (*this, &FontOptions::typeface, x); }

    /** Returns a copy of these options with a new set of preferred fallback family names. */
    [[nodiscard]] FontOptions withFallbacks       (std::vector<String> x)  const { return withMember (*this, &FontOptions::fallbacks, std::move (x)); }

    /** Returns a copy of these options with font fallback enabled or disabled. */
    [[nodiscard]] FontOptions withFallbackEnabled (bool x = true)          const { return withMember (*this, &FontOptions::fallbackEnabled, x); }

    /** Returns a copy of these options with the specified height in pixels (can be fractional). */
    [[nodiscard]] FontOptions withHeight          (float x)                const { return withMember (*this, &FontOptions::height, x); }

    /** Returns a copy of these options with the specified extra kerning factor (also called "tracking"). */
    [[nodiscard]] FontOptions withKerningFactor   (float x)                const { return withMember (*this, &FontOptions::tracking, x); }

    /** Returns a copy of these options with the specified horizontal scale factor, defaults to 1.0. */
    [[nodiscard]] FontOptions withHorizontalScale (float x)                const { return withMember (*this, &FontOptions::horizontalScale, x); }

    /** Returns a copy of these options with underline enabled or disabled, defaults to disabled. */
    [[nodiscard]] FontOptions withUnderline       (bool x = true)          const { return withMember (*this, &FontOptions::underlined, x); }

    /** @see withName() */
    [[nodiscard]] auto getName()            const { return name; }
    /** @see withStyle() */
    [[nodiscard]] auto getStyle()           const { return style; }
    /** @see withTypeface() */
    [[nodiscard]] auto getTypeface()        const { return typeface; }
    /** @see withFallbacks() */
    [[nodiscard]] auto getFallbacks()       const { return fallbacks; }
    /** @see withHeight() */
    [[nodiscard]] auto getHeight()          const { return height; }
    /** @see withKerningFactor() */
    [[nodiscard]] auto getKerningFactor()   const { return tracking; }
    /** @see withHorizontalScale() */
    [[nodiscard]] auto getHorizontalScale() const { return horizontalScale; }
    /** @see withFallbackEnabled() */
    [[nodiscard]] auto getFallbackEnabled() const { return fallbackEnabled; }
    /** @see withUnderline() */
    [[nodiscard]] auto getUnderline()       const { return underlined; }

    /** Equality operator. */
    [[nodiscard]] bool operator== (const FontOptions& other) const;
    /** Inequality operator. */
    [[nodiscard]] bool operator!= (const FontOptions& other) const;
    /** Less-than operator. Allows FontOptions to be used as keys in a map. */
    [[nodiscard]] bool operator<  (const FontOptions& other) const;
    /** Less-than-or-equal operator. */
    [[nodiscard]] bool operator<= (const FontOptions& other) const;
    /** Greater-than operator. */
    [[nodiscard]] bool operator>  (const FontOptions& other) const;
    /** Greater-than-or-equal operator. */
    [[nodiscard]] bool operator>= (const FontOptions& other) const;

private:
    auto tie() const;

    String name, style;
    Typeface::Ptr typeface;
    std::vector<String> fallbacks;
    float height{};
    float tracking{};
    float horizontalScale = 1.0f;
    bool fallbackEnabled = true;
    bool underlined{};
};

} // namespace juce
