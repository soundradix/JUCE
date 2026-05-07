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

//==============================================================================
/**
    An interface class for entities that are interested in disconnection notifications.

    @tags{Audio}
*/
struct DisconnectionListener
{
    DisconnectionListener() = default;

    DisconnectionListener (const DisconnectionListener&) = default;
    DisconnectionListener (DisconnectionListener&&) noexcept = default;

    DisconnectionListener& operator= (const DisconnectionListener&) = default;
    DisconnectionListener& operator= (DisconnectionListener&&) noexcept = default;

    virtual ~DisconnectionListener() = default;

    /** Called to notify that the system has destroyed/disconnected a resource. */
    virtual void disconnected() = 0;
};

}
