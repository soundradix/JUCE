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

/** Options that can be used to affect the layout produced by GlyphArrangement::addFittedText.

    @see GlyphArrangement::addFittedText

    @tags{Graphics}
*/
class JUCE_API GlyphArrangementOptions final
{
public:
    /** We increment the distance between the baselines of subsequent lines with this value.

        Line spacing is added below the line's descender, and doesn't affect the first line's
        baseline.

        The total distance between baselines is lineHeight * lineHeightMultiple + lineSpacing.
    */
    [[nodiscard]] GlyphArrangementOptions withLineSpacing (float x) const
    {
        return withMember (*this, &GlyphArrangementOptions::lineSpacing, x);
    }

    /** We multiply the original distance between the baselines of subsequent lines with this value.

        The line height multiple is applied to both the ascender and descender, hence it affects
        the first line's baseline.

        The total distance between baselines is lineHeight * lineHeightMultiple + lineSpacing.
    */
    [[nodiscard]] GlyphArrangementOptions withLineHeightMultiple (float x) const
    {
        return withMember (*this, &GlyphArrangementOptions::lineHeightMultiple, x);
    }

    /** @see withLineSpacing() */
    const auto& getLineSpacing() const        { return lineSpacing; }

    /** @see withLineHeightMultiple() */
    const auto& getLineHeightMultiple() const { return lineHeightMultiple; }

    /** Equality operator. */
    [[nodiscard]] bool operator== (const GlyphArrangementOptions& other) const;
    /** Inequality operator. */
    [[nodiscard]] bool operator!= (const GlyphArrangementOptions& other) const;
    /** Less-than operator. Allows GlyphArrangementOptions to be used as keys in a map. */
    [[nodiscard]] bool operator<  (const GlyphArrangementOptions& other) const;
    /** Less-than-or-equal operator. */
    [[nodiscard]] bool operator<= (const GlyphArrangementOptions& other) const;
    /** Greater-than operator. */
    [[nodiscard]] bool operator>  (const GlyphArrangementOptions& other) const;
    /** Greater-than-or-equal operator. */
    [[nodiscard]] bool operator>= (const GlyphArrangementOptions& other) const;

private:
    auto tie() const noexcept;

    float lineSpacing = 0.0f;
    float lineHeightMultiple = 1.0f;
};

} // namespace juce
