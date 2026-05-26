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

class ShapedText::Impl
{
public:
    Impl (String textIn, Options optionsIn)
        : options { std::move (optionsIn) },
          text { std::move (textIn) }
    {
    }

    void draw (const Graphics& g, AffineTransform transform) const
    {
        drawJustifiedText (justifiedText, g, transform);
    }

    float getHeight() const
    {
        return justifiedText.getHeight();
    }

    int64 getNumGlyphs() const
    {
        return simpleShapedText.getNumGlyphs();
    }

    const detail::RangedValues<LineMetrics>& getLineMetricsForGlyphRange() const
    {
        return justifiedText.getLineMetricsForGlyphRange();
    }

    const detail::Ranges& getLineTextRanges() const
    {
        return simpleShapedText.getLineTextRanges();
    }

    auto& getText() const
    {
        return text;
    }

    auto getTextRange (int64 glyphIndex) const
    {
        return simpleShapedText.getTextRange (glyphIndex);
    }

    auto isLtr (int64 glyphIndex) const
    {
        return simpleShapedText.isLtr (glyphIndex);
    }

    int64 getTextIndexForCaret (Point<float> p) const
    {
        const auto getGlyph = [&] (int64 i)
        {
            return simpleShapedText.getGlyphs()[(size_t) i];
        };

        if (getNumGlyphs() == 0)
            return 0;

        const auto glyphOnTheRight = justifiedText.getGlyphIndexToTheRightOf (p);

        if (glyphOnTheRight >= getNumGlyphs())
        {
            const auto glyphOnTheLeft = glyphOnTheRight - 1;
            const auto ltr = simpleShapedText.getGlyphLookup().find (getGlyph (glyphOnTheLeft).cluster)->value.ltr;

            if (ltr)
                return simpleShapedText.getTextIndexAfterGlyph (glyphOnTheLeft);

            return simpleShapedText.getGlyphs()[(size_t) glyphOnTheLeft].cluster;
        }

        const auto ltr = simpleShapedText.getGlyphLookup().find (getGlyph (glyphOnTheRight).cluster)->value.ltr;

        if (ltr)
            return simpleShapedText.getGlyphs()[(size_t) glyphOnTheRight].cluster;

        return simpleShapedText.getTextIndexAfterGlyph (glyphOnTheRight);
    }

    void getGlyphRanges (Range<int64> textRange, std::vector<Range<int64>>& outRanges) const
    {
        simpleShapedText.getGlyphRanges (textRange, outRanges);
    }

    RectangleList<float> getGlyphsBounds (Range<int64> glyphRange) const
    {
        return justifiedText.getGlyphsBounds (glyphRange);
    }

    auto getGlyphAnchor (int64 index) const
    {
        return justifiedText.getGlyphAnchor (index);
    }

    Span<const float> getMinimumRequiredWidthForLines() const
    {
        return justifiedText.getMinimumRequiredWidthForLines();
    }

    //==============================================================================
    auto& getSimpleShapedText() const { return simpleShapedText; }

    auto& getJustifiedText() const { return justifiedText; }

private:
    ShapedTextOptions options;
    String text;
    SimpleShapedText simpleShapedText { &text, options };
    JustifiedText justifiedText { &simpleShapedText, options };
};

ShapedText::ShapedText() : ShapedText ("", {})
{
}

ShapedText::ShapedText (String text) : ShapedText (std::move (text), {})
{
}

ShapedText::ShapedText (String text, Options options)
{
    impl = std::make_shared<Impl> (std::move (text), std::move (options));
}

void ShapedText::draw (const Graphics& g, AffineTransform transform) const
{
    impl->draw (g, transform);
}

float ShapedText::getHeight() const
{
    return impl->getHeight();
}

int64 ShapedText::getNumGlyphs() const
{
    return impl->getNumGlyphs();
}

const detail::RangedValues<LineMetrics>& ShapedText::getLineMetricsForGlyphRange() const
{
    return impl->getLineMetricsForGlyphRange();
}

const detail::Ranges& ShapedText::getLineTextRanges() const
{
    return impl->getLineTextRanges();
}

const String& ShapedText::getText() const
{
    return impl->getText();
}

Range<int64> ShapedText::getTextRange (int64 glyphIndex) const
{
    return impl->getTextRange (glyphIndex);
}

bool ShapedText::isLtr (int64 glyphIndex) const
{
    return impl->isLtr (glyphIndex);
}

int64 ShapedText::getTextIndexForCaret (Point<float> p) const
{
    return impl->getTextIndexForCaret (p);
}

void ShapedText::getGlyphRanges (Range<int64> textRange, std::vector<Range<int64>>& outRanges) const
{
    return impl->getGlyphRanges (textRange, outRanges);
}

RectangleList<float> ShapedText::getGlyphsBounds (Range<int64> glyphRange) const
{
    return impl->getGlyphsBounds (glyphRange);
}

GlyphAnchorResult ShapedText::getGlyphAnchor (int64 index) const
{
    return impl->getGlyphAnchor (index);
}

Span<const float> ShapedText::getMinimumRequiredWidthForLines() const
{
    return impl->getMinimumRequiredWidthForLines();
}

const JustifiedText& ShapedText::getJustifiedText() const { return impl->getJustifiedText(); }

const SimpleShapedText& ShapedText::getSimpleShapedText() const { return impl->getSimpleShapedText(); }

} // namespace juce::detail
