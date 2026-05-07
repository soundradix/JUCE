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
    Represents a Function Block (FB) or Group Terminal Block (GTB).

    GTBs are only available in USB MIDI, so devices on non-USB transports will not expose GTB
    information. In the case that a device only exposes GTBs and not FBs, default values will
    be used for any fields that are unavailable in the GTB definition.

    @see BlockInfo

    @tags{Audio}
*/
class Block
{
public:
    Block() = default;

    [[nodiscard]] Block withName (const String& x) const { return withMember (*this, &Block::name, x); }

    [[nodiscard]] Block withFirstGroup (uint8_t x) const { return withMember (*this, &Block::info, info.withFirstGroup (x)); }
    [[nodiscard]] Block withNumGroups (uint8_t x) const { return withMember (*this, &Block::info, info.withNumGroups (x)); }
    [[nodiscard]] Block withMaxSysex8Streams (uint8_t x) const { return withMember (*this, &Block::info, info.withMaxSysex8Streams (x)); }
    [[nodiscard]] Block withEnabled (bool x = true) const { return withMember (*this, &Block::info, info.withEnabled (x)); }

    [[nodiscard]] Block withUiHint (BlockUiHint x) const
    {
        return withMember (*this, &Block::info, info.withUiHint (x));
    }

    [[nodiscard]] Block withMIDI1ProxyKind (BlockMIDI1ProxyKind x) const
    {
        return withMember (*this, &Block::info, info.withMIDI1ProxyKind (x));
    }

    [[nodiscard]] Block withDirection (BlockDirection x) const
    {
        return withMember (*this, &Block::info, info.withDirection (x));
    }

    /** The name of the block, if any. */
    String getName() const { return name; }

    /** All info relating to this block. */
    BlockInfo getInfo() const { return info; }

    /** In the range 0x0 to 0xF inclusive. */
    uint8_t getFirstGroup() const
    {
        return info.getFirstGroup();
    }

    /** In the range 0x01 to 0x10 inclusive. */
    uint8_t getNumGroups() const
    {
        return info.getNumGroups();
    }

    /** The number of simultaneous Sysex8 streams that are supported. */
    uint8_t getMaxSysex8Streams() const
    {
        return info.getMaxSysex8Streams();
    }

    /** Some blocks may support bidirectional communication (e.g. for CI) but function predominantly
        as a sender or receiver. The direction returned here is a hint to the user that doesn't
        necessarily reflect the hardware capabilities.
    */
    BlockUiHint getUiHint() const
    {
        return info.getUiHint();
    }

    /** If this block is a proxy for a MIDI 1.0 stream, describes the capabilities of that stream. */
    BlockMIDI1ProxyKind getMIDI1ProxyKind() const
    {
        return info.getMIDI1ProxyKind();
    }

    /** Returns the message transmission directions that are supported by this block.
        Use this to determine block capabilities.
        For information that will be displayed to the user, prefer getUIHint().
    */
    BlockDirection getDirection() const
    {
        return info.getDirection();
    }

    /** True if this block is enabled. */
    bool isEnabled() const
    {
        return info.isEnabled();
    }

    /** True if the names of the two blocks match. */
    bool nameMatches (const Block& other) const
    {
        return name == other.name;
    }

    /** True if the BlockInfo of the two blocks match. */
    bool infoMatches (const Block& other) const
    {
        return info == other.info;
    }

    bool operator== (const Block& other) const;
    bool operator!= (const Block& other) const;

private:
    BlockInfo info;
    String name;
};

}
