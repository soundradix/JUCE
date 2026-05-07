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

auto GlyphArrangementOptions::tie() const noexcept
{
    return std::tie (lineSpacing, lineHeightMultiple);
}

bool GlyphArrangementOptions::operator== (const GlyphArrangementOptions& other) const { return tie() == other.tie(); }
bool GlyphArrangementOptions::operator!= (const GlyphArrangementOptions& other) const { return tie() != other.tie(); }
bool GlyphArrangementOptions::operator<  (const GlyphArrangementOptions& other) const { return tie() <  other.tie(); }
bool GlyphArrangementOptions::operator<= (const GlyphArrangementOptions& other) const { return tie() <= other.tie(); }
bool GlyphArrangementOptions::operator>  (const GlyphArrangementOptions& other) const { return tie() >  other.tie(); }
bool GlyphArrangementOptions::operator>= (const GlyphArrangementOptions& other) const { return tie() >= other.tie(); }

} // namespace juce
