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

struct FontValues
{
    static float limitFontHeight (const float height) noexcept
    {
        return jlimit (0.1f, 10000.0f, height);
    }

    inline static constexpr float defaultFontHeight = 14.0f;
    static float minimumHorizontalScale;

    FontValues() = delete;
};

float FontValues::minimumHorizontalScale = 0.7f;

FontOptions::FontOptions()
    : FontOptions (FontValues::defaultFontHeight, Font::plain)
{
}

FontOptions::FontOptions (float fontHeight)
    : FontOptions (fontHeight, Font::plain)
{
}

FontOptions::FontOptions (float fontHeight, int styleFlags)
    : FontOptions ({}, fontHeight, styleFlags)
{
}

FontOptions::FontOptions (const String& typefaceName, float fontHeight, int styleFlags)
    : FontOptions (typefaceName, FontStyleHelpers::getStyleName (styleFlags), fontHeight)
{
    underlined = (styleFlags & Font::FontStyleFlags::underlined) != 0;
}

FontOptions::FontOptions (const String& typefaceName, const String& typefaceStyle, float fontHeight)
    : name (typefaceName),
      style (typefaceStyle),
      height (FontValues::limitFontHeight (fontHeight))
{
}

FontOptions::FontOptions (const Typeface::Ptr& ptr)
    : name (ptr->getName()),
      style (ptr->getStyle()),
      typeface (ptr),
      height (FontValues::defaultFontHeight)
{
}

auto FontOptions::tie() const
{
    return std::tuple (name,
                       style,
                       typeface.get(),
                       fallbacks,
                       features,
                       metricsKind,
                       ascentOverride,
                       descentOverride,
                       height,
                       pointHeight,
                       tracking,
                       horizontalScale,
                       fallbackEnabled,
                       underlined);
}

struct FontFeatureSettingSortHelper
{
    bool operator() (FontFeatureSetting a, FontFeatureSetting b) const
    {
        return a.tag < b.tag;
    }

    bool operator() (FontFeatureTag a, FontFeatureSetting b) const
    {
        return a < b.tag;
    }

    bool operator() (FontFeatureSetting a, FontFeatureTag b) const
    {
        return a.tag < b;
    }
};

FontOptions FontOptions::withFeatureSetting (FontFeatureSetting newSetting) const
{
    auto copy = *this;

    OrderedContainerHelpers::insertOrAssign (copy.features, newSetting, FontFeatureSettingSortHelper{});

    return copy;
}

FontOptions FontOptions::withFeatureRemoved (FontFeatureTag featureTag) const
{
    auto copy = *this;

    OrderedContainerHelpers::remove (copy.features, featureTag, FontFeatureSettingSortHelper{});

    return copy;
}

bool FontOptions::operator== (const FontOptions& other) const { return tie() == other.tie(); }
bool FontOptions::operator!= (const FontOptions& other) const { return tie() != other.tie(); }
bool FontOptions::operator<  (const FontOptions& other) const { return tie() <  other.tie(); }
bool FontOptions::operator<= (const FontOptions& other) const { return tie() <= other.tie(); }
bool FontOptions::operator>  (const FontOptions& other) const { return tie() >  other.tie(); }
bool FontOptions::operator>= (const FontOptions& other) const { return tie() >= other.tie(); }

#if JUCE_UNIT_TESTS

class FontFeatureContainerTests : public UnitTest
{
public:
    FontFeatureContainerTests() : UnitTest ("FontFeatureContainerTests", UnitTestCategories::text)
    {
    }

    void runTest() override
    {
        beginTest ("Features can be enabled");
        {
            const auto options = FontOptions{}.withFeatureEnabled ("clig");

            expectEquals ((int) options.getFeatureSettings().size(), 1);
            expect (compareFeatureLists (options.getFeatureSettings(),
            {
                FontFeatureSetting ("clig", FontFeatureSetting::featureEnabled)
            }));
        }

        beginTest ("Features can be disabled");
        {
            const auto options = FontOptions{}.withFeatureDisabled ("clig");

            expectEquals ((int) options.getFeatureSettings().size(), 1);
            expect (compareFeatureLists (options.getFeatureSettings(),
            {
                FontFeatureSetting ("clig", FontFeatureSetting::featureDisabled)
            }));
        }

        beginTest ("Features can be removed");
        {
            const auto options = FontOptions{}.withFeatureEnabled ("clig")
                                              .withFeatureRemoved ("clig");

            expectEquals ((int) options.getFeatureSettings().size(), 0);
            expect (compareFeatureLists (options.getFeatureSettings(), {}));
        }

        beginTest ("Duplicate features are not allowed");
        {
            const auto options = FontOptions{}.withFeatureEnabled ("clig")
                                              .withFeatureEnabled ("clig");

            expectEquals ((int) options.getFeatureSettings().size(), 1);
        }

        beginTest ("Features are always sorted by tag");
        {
            const auto options = FontOptions{}.withFeatureEnabled ("clig")
                                              .withFeatureDisabled ("blig")
                                              .withFeatureEnabled ("alig");

            expectEquals ((int) options.getFeatureSettings().size(), 3);
            expect (compareFeatureLists (options.getFeatureSettings(),
            {
                FontFeatureSetting ("alig", FontFeatureSetting::featureEnabled),
                FontFeatureSetting ("blig", FontFeatureSetting::featureDisabled),
                FontFeatureSetting ("clig", FontFeatureSetting::featureEnabled)
            }));
        }
    }

private:
    static bool compareFeatureLists (Span<const FontFeatureSetting> input,
                                     std::initializer_list<const FontFeatureSetting> expected)
    {
        return std::equal (input.begin(), input.end(), expected.begin(), expected.end());
    }
};

static FontFeatureContainerTests fontFeatureContainerTests;

#endif


} // namespace juce
