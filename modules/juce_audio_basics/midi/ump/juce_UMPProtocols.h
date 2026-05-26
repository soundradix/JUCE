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

/** @cond */
namespace juce::universal_midi_packets
{

/** Kinds of MIDI message transport.
*/
enum class Transport : uint8_t
{
    bytestream, ///< A stream of variable-length messages. Suitable for MIDI 1.0.
    ump,        ///< A stream of 32-bit words. Suitable for MIDI-1UP and MIDI 2.0.
};

/** The kinds of MIDI protocol that can be formatted into Universal MIDI Packets.
*/
enum class PacketProtocol : uint8_t
{
    MIDI_1_0,
    MIDI_2_0,
};

/** All kinds of MIDI protocol understood by JUCE.
*/
enum class MidiProtocol : uint8_t
{
    bytestream,
    UMP_MIDI_1_0,
    UMP_MIDI_2_0,
};

} // namespace juce::universal_midi_packets
/** @endcond */
