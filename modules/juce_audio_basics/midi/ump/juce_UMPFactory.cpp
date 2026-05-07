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

namespace juce::universal_midi_packets
{

auto EndpointInfo::tie() const
{
    return std::tie (versionMajor, versionMinor, numFunctionBlocks, flags);
}

bool EndpointInfo::operator== (const EndpointInfo& other) const
{
    return tie() == other.tie();
}

bool EndpointInfo::operator!= (const EndpointInfo& other) const
{
    return tie() != other.tie();
}

} // namespace juce::universal_midi_packets
