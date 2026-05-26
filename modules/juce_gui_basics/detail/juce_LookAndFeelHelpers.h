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

namespace juce::detail
{

struct LookAndFeelHelpers
{
    LookAndFeelHelpers() = delete;

    static Colour createBaseColour (Colour buttonColour,
                                    bool hasKeyboardFocus,
                                    bool shouldDrawButtonAsHighlighted,
                                    bool shouldDrawButtonAsDown) noexcept
    {
        const float sat = hasKeyboardFocus ? 1.3f : 0.9f;
        const Colour baseColour (buttonColour.withMultipliedSaturation (sat));

        if (shouldDrawButtonAsDown)        return baseColour.contrasting (0.2f);
        if (shouldDrawButtonAsHighlighted) return baseColour.contrasting (0.1f);

        return baseColour;
    }

    static TextLayout layoutTooltipText (TypefaceMetricsKind metrics, const String& text, Colour colour) noexcept
    {
        const float tooltipFontSize = 13.0f;
        const int maxToolTipWidth = 400;

        AttributedString s;
        s.setWordWrap (AttributedString::WordWrap::byChar);
        s.setJustification (Justification::centred);
        s.append (text, FontOptions (tooltipFontSize, Font::bold).withMetricsKind (metrics), colour);

        TextLayout tl;
        tl.createLayoutWithBalancedLineLengths (s, (float) maxToolTipWidth);
        return tl;
    }
};

} // namespace juce::detail
