/*
  ==============================================================================

   This file is part of the JUCE 9 preview.
   Copyright (c) Raw Material Software Limited

   You may use this code under the terms of the AGPLv3
   (see www.gnu.org/licenses).

   For the JUCE 9 preview this file cannot be licensed commercially.

   JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
   EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
   DISCLAIMED.

  ==============================================================================
*/

namespace juce
{
/** Represents a single OpenType font feature.

    OpenType font features are typographic options that can be enabled or disabled
    to control text rendering behavior. Each feature is identified by a
    four-character tag (like 'liga' for standard ligatures or 'smcp' for small caps).
    These features allow fine-grained control over how text is displayed, including
    ligatures, number styles, stylistic alternates, and positioning adjustments.

    The tag must be exactly 4 characters long.

    @see FontOptions, Font

    @tags{Graphics}
*/
class JUCE_API FontFeatureTag final
{
public:
    /** Constructs a feature from the specified tag string. */
    constexpr FontFeatureTag (const char (&string)[5])
        : tag { (uint32) string[0] << 24
              | (uint32) string[1] << 16
              | (uint32) string[2] << 8
              | (uint32) string[3] }
    {
    }

    /** Constructs a feature from the specified tag value. */
    constexpr explicit FontFeatureTag (uint32 tagValue)
        : tag (tagValue)
    {
    }

    /** Creates a new FontFeature with the specified tag. */
    [[nodiscard]] static FontFeatureTag fromString (const String& tagString);

    /** Returns a string representation of this tag. */
    String toString() const;

    /** Returns the Harfbuzz compatible OpenType tag as an unsigned 32-bit integer. */
    constexpr uint32 getTag() const { return tag; }

    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator<  (FontFeatureTag other) const { return tag <  other.tag; }
    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator<= (FontFeatureTag other) const { return tag <= other.tag; }
    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator>  (FontFeatureTag other) const { return tag >  other.tag; }
    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator>= (FontFeatureTag other) const { return tag >= other.tag; }
    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator== (FontFeatureTag other) const { return tag == other.tag; }
    /** Comparison based on tag value. */
    [[nodiscard]] constexpr bool operator!= (FontFeatureTag other) const { return tag != other.tag; }

private:
    FontFeatureTag() = default;
    uint32 tag;
};

/** Represents a single OpenType font feature setting.

    A font feature setting combines a FontFeatureTag with an optional value that
    controls the behavior of that feature. For example, a 'liga' (standard ligatures)
    feature can be enabled (value=1) or disabled (value=0), while other features like
    'salt' (stylistic alternates) might accept a range of values to select specific
    alternates.

    @see FontFeatureTag, FontOptions, Font

    @tags{Graphics}
*/
class JUCE_API FontFeatureSetting final
{
    constexpr auto tie() const;

public:
    /** Common feature values for convenience. */
    /** Enable this feature (value=1). */
    static constexpr auto featureEnabled  = 1;
    /** Disable this feature (value=0). */
    static constexpr auto featureDisabled = 0;

    /** Constructs a feature setting with the specified tag and optional value. */
    constexpr FontFeatureSetting (FontFeatureTag featureTag, uint32 featureValue) noexcept
        : tag (featureTag),
          value (featureValue)
    {
    }

    [[nodiscard]] constexpr bool operator<  (const FontFeatureSetting& other) const;
    [[nodiscard]] constexpr bool operator<= (const FontFeatureSetting& other) const;
    [[nodiscard]] constexpr bool operator>  (const FontFeatureSetting& other) const;
    [[nodiscard]] constexpr bool operator>= (const FontFeatureSetting& other) const;
    [[nodiscard]] constexpr bool operator== (const FontFeatureSetting& other) const;
    [[nodiscard]] constexpr bool operator!= (const FontFeatureSetting& other) const;

    /** The OpenType feature tag. */
    FontFeatureTag tag;

    /** The value for this feature.
        Common values are 0 (featureDisabled) and 1 (featureEnabled), but some features
        support additional values for specific behaviors.
    */
    uint32 value;
};

} // namespace juce
