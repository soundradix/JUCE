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

uint32_t Utils::getNumWordsForMessageType (uint32_t mt)
{
    switch ((uint8_t) Utils::getMessageType (mt))
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x6:
        case 0x7:
            return 1;
        case 0x3:
        case 0x4:
        case 0x8:
        case 0x9:
        case 0xa:
            return 2;
        case 0xb:
        case 0xc:
            return 3;
        case 0x5:
        case 0xd:
        case 0xe:
        case 0xf:
            return 4;
    }

    jassertfalse;
    return 1;
}

} // namespace juce::universal_midi_packets
