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

//==============================================================================
/**
    An interface that will receive a callback every time a Device wishes to send a new MIDI-CI
    message.

    @tags{Audio}
*/
struct DeviceMessageHandler
{
    DeviceMessageHandler() = default;
    virtual ~DeviceMessageHandler() = default;
    DeviceMessageHandler (const DeviceMessageHandler&) = default;
    DeviceMessageHandler (DeviceMessageHandler&&) = default;
    DeviceMessageHandler& operator= (const DeviceMessageHandler&) = default;
    DeviceMessageHandler& operator= (DeviceMessageHandler&&) = default;

    /** Called with the bytes of a MIDI-CI message, along with the message's group.

        To send the message on, format the message appropriately (either into bytestream sysex
        or into multiple UMP sysex packets).
    */
    virtual void processMessage (ump::BytesOnGroup) = 0;
};

} // namespace juce::midi_ci
