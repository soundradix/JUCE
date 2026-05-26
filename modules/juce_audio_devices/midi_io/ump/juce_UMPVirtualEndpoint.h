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
    Represents a virtual device that allows this program to advertise itself to other MIDI-aware
    applications on the system.

    Creating a VirtualEndpoint will install a new endpoint on the system. This endpoint will
    be visible when enumerating endpoints using the Endpoints singleton. If you're displaying
    a list of endpoints in your UI, it's probably a good idea to omit any virtual endpoints
    created by the current application in order to avoid confusion.

    After creating a VirtualEndpoint, it can be opened like any other connection, by calling
    Session::connectInput() and Session::connectOutput(), passing the EndpointId for the
    virtual endpoint.

    @tags{Audio}
*/
class VirtualEndpoint
{
public:
    /** Creates an invalid virtual endpoint that doesn't correspond to any virtual device.

        isAlive() will return false for a default-constructed VirtualEndpoint.
    */
    VirtualEndpoint();
    ~VirtualEndpoint();

    VirtualEndpoint (VirtualEndpoint&&) noexcept;
    VirtualEndpoint& operator= (VirtualEndpoint&&) noexcept;

    VirtualEndpoint (const VirtualEndpoint&) = delete;
    VirtualEndpoint& operator= (const VirtualEndpoint&) = delete;

    /** Retrieves the unique id of this endpoint.

        Pass this ID to Session::connectInput() and/or Session::connectOutput in order to send and
        receive messages through the virtual endpoint.

        Note that this ID is *not* guaranteed to be stable - creating the 'same' virtual device
        across several program invocations may produce a different ID each time.

        To fetch the current details of this device, you can pass this ID to Endpoints::getEndpoint().
    */
    EndpointId getId() const;

    /** Sets new properties for the block at the given zero-based index. The number of function
        blocks on an endpoint may not change.

        Returns true on success, or false otherwise.

        This may fail for several reasons, including:
        - attempting to modify an endpoint with static function blocks
        - attempting to update a block index that doesn't exist on this endpoint
        - attempting to set a block with invalid properties, e.g. the sum of the start index and
          number of included groups is greater than 16
        - platform-specific reasons, e.g. macOS doesn't currently allow changing the number of
          spanned groups in a block
    */
    bool setBlock (uint8_t index, const Block& newBlock);

    /** Assigns a new name to this endpoint, and sends a notification to connected endpoints. */
    bool setName (const String&);

    /** Returns true if this object represents an endpoint that is currently alive, or false if
        the endpoint is not alive. This can happen because the endpoint failed to open, or
        if the session holding the endpoint was closed.

        This function returns false for a default-constructed instance.
    */
    bool isAlive() const;

    explicit operator bool() const { return isAlive(); }

    /** @internal */
    class Impl;

private:
    explicit VirtualEndpoint (std::unique_ptr<Impl>);

    std::unique_ptr<Impl> impl;
};

}
