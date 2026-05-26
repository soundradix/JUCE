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
    auto ChannelAddress::tie() const { return std::tie (group, channel); }
    bool ChannelAddress::operator<  (const ChannelAddress& other) const { return tie() <  other.tie(); }
    bool ChannelAddress::operator<= (const ChannelAddress& other) const { return tie() <= other.tie(); }
    bool ChannelAddress::operator>  (const ChannelAddress& other) const { return tie() >  other.tie(); }
    bool ChannelAddress::operator>= (const ChannelAddress& other) const { return tie() >= other.tie(); }
    bool ChannelAddress::operator== (const ChannelAddress& other) const { return tie() == other.tie(); }
    bool ChannelAddress::operator!= (const ChannelAddress& other) const { return ! operator== (other); }
} // namespace juce::midi_ci
