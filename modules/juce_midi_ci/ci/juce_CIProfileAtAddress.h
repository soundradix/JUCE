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
    Holds a profile ID, and the address of a group/channel.

    @tags{Audio}
*/
class ProfileAtAddress
{
    auto tie() const;

public:
    Profile profile;            ///< The id of a MIDI-CI profile
    ChannelAddress address;     ///< A group and channel

    bool operator== (const ProfileAtAddress& x) const;
    bool operator!= (const ProfileAtAddress& x) const;

    bool operator<  (const ProfileAtAddress& x) const;
    bool operator<= (const ProfileAtAddress& x) const;
    bool operator>  (const ProfileAtAddress& x) const;
    bool operator>= (const ProfileAtAddress& x) const;
};

} // namespace juce::midi_ci
