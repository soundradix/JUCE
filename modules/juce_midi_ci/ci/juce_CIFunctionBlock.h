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

namespace juce::midi_ci
{

/**
    Contains information about a MIDI 2.0 function block.

    @tags{Audio}
*/
struct FunctionBlock
{
    std::byte identifier { 0x7f }; ///< 0x7f == no function block
    uint8_t firstGroup = 0;        ///< The first group that is part of the block, 0-based
    uint8_t numGroups = 1;         ///< The number of groups contained in the block

    bool operator== (const FunctionBlock& other) const
    {
        const auto tie = [] (auto& x) { return std::tie (x.identifier, x.firstGroup, x.numGroups); };
        return tie (*this) == tie (other);
    }

    bool operator!= (const FunctionBlock& other) const { return ! operator== (other); }
};

} // namespace juce::midi_ci
