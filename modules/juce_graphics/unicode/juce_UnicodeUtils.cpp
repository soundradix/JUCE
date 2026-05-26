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

template <typename T>
static constexpr bool contains (std::initializer_list<T> span, const T& b)
{
    for (const auto& i : span)
        if (i == b)
            return true;

    return false;
}

struct UnicodeAnalysisPoint
{
    char32_t character = 0;
    UnicodeEntry data{};

    UnicodeAnalysisPoint (char32_t characterIn, UnicodeEntry entry)
        : character { characterIn },
          data { std::move (entry) }
    {}

    LineBreakType getBreakType() const
    {
        return data.bt;
    }

    auto getGeneralCategory() const
    {
        return SBCodepointGetGeneralCategory (character);
    }

    auto getScriptType() const
    {
        return SBCodepointGetScript (character);
    }
};

//==============================================================================
/*  Types of breaks between characters. */
enum class TextBreakType
{
    none, // The sequence of characters should not be broken.

    soft, // The sequence of characters can be broken, if required.

    hard  // The sequence of characters must be broken here.
};

} // namespace juce
