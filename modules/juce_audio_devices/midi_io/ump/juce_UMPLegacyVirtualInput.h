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

/**
    Represents a virtual MIDI 1.0 input port.

    @tags{Audio}
*/
class LegacyVirtualInput
{
public:
    /** Creates an invalid virtual port that doesn't correspond to any virtual device. */
    LegacyVirtualInput();
    ~LegacyVirtualInput();

    LegacyVirtualInput (LegacyVirtualInput&&) noexcept;
    LegacyVirtualInput& operator= (LegacyVirtualInput&&) noexcept;

    LegacyVirtualInput (const LegacyVirtualInput&) = delete;
    LegacyVirtualInput& operator= (const LegacyVirtualInput&) = delete;

    /** Retrieves the unique id of this input.

        You can pass this ID to Session::connectInput() in order to receive messages sent to this
        input.

        Note that this ID is *not* guaranteed to be stable - creating the 'same' virtual device
        across several program invocations may produce a different ID each time.

        To fetch the current details of this device, you can pass this ID to Endpoints::getEndpoint().
    */
    EndpointId getId() const;

    bool isAlive() const;

    explicit operator bool() const { return isAlive(); }

    /** @internal */
    class Impl;

private:
    explicit LegacyVirtualInput (std::unique_ptr<Impl>);

    std::unique_ptr<Impl> impl;
};

}
