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

/*  Class that can visually shape a Unicode string provided a list of Fonts corresponding to
    sub-ranges of the string.
*/
class JUCE_API  ShapedText
{
public:
    using Options = ShapedTextOptions;

    ShapedText();

    explicit ShapedText (String text);

    ShapedText (String text, Options options);

    /*  Returns the text which was used to construct this object. */
    const String& getText() const;

    Span<const ShapedGlyph> getGlyphs() const;

    /*  Returns the text's codepoint range, to which the glyph under the provided index belongs.

        This range will have a length of at least one, and potentially more than one if ligatures
        are enabled.
    */
    Range<int64> getTextRange (int64 glyphIndex) const;

    bool isLtr (int64 glyphIndex) const;

    int64 getTextIndexForCaret (Point<float> p) const;

    void getGlyphRanges (Range<int64> textRange, std::vector<Range<int64>>& outRanges) const;

    RectangleList<float> getGlyphsBounds (Range<int64> glyphRange) const;

    /*  @see JustifiedText::getGlyphAnchor() */
    GlyphAnchorResult getGlyphAnchor (int64 index) const;

    /*  Returns the widths for each line, that the glyphs would require to be rendered without being
        truncated. This will or will not include the space required by trailing whitespaces in the
        line based on the ShapedTextOptions::withTrailingWhitespacesShouldFit() value.

        This value isn't affected by the Justification parameter, it just reports the amount of
        width that would be required to avoid truncation.
     */
    Span<const float> getMinimumRequiredWidthForLines() const;

    /*  @see JustifiedText::accessTogetherWith */
    template <typename Callable, typename... RangedValues>
    void accessTogetherWith (Callable&& callback, RangedValues&&... rangedValues) const
    {
        getJustifiedText().accessTogetherWith (std::forward<Callable> (callback),
                                               std::forward<RangedValues> (rangedValues)...);
    }

    /*  Draws the text. */
    void draw (const Graphics& g, AffineTransform transform) const;

    /*  @see JustifiedText::getHeight
    */
    float getHeight() const;

    int64 getNumGlyphs() const;

    const detail::RangedValues<LineMetrics>& getLineMetricsForGlyphRange() const;

    const detail::Ranges& getLineTextRanges() const;

    /*  @internal */
    const JustifiedText& getJustifiedText() const;

    /*  @internal */
    const SimpleShapedText& getSimpleShapedText() const;

private:
    class Impl;
    std::shared_ptr<Impl> impl;
};

} // namespace juce::detail
