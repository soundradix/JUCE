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

struct EndpointAndStaticInfo
{
    Endpoint endpoint;
    StaticDeviceInfo info;
    EndpointId id;
};

class IOHelpers
{
public:
    IOHelpers() = delete;

    static Block makeLegacyBlock (bool isInput)
    {
        const auto direction = isInput ? BlockDirection::receiver : BlockDirection::sender;
        const auto hint = isInput ? BlockUiHint::receiver : BlockUiHint::sender;
        return Block{}.withName ("Legacy MIDI 1.0")
                      .withEnabled (true)
                      .withFirstGroup (0)
                      .withNumGroups (1)
                      .withMIDI1ProxyKind (BlockMIDI1ProxyKind::unrestrictedBandwidth)
                      .withDirection (direction)
                      .withUiHint (hint);
    }

    static EndpointAndStaticInfo makeProxyEndpoint (const MidiDeviceInfo& info, BlockDirection direction)
    {
        jassert (direction != BlockDirection::unknown);

        const auto hint = std::invoke ([&]
        {
            switch (direction)
            {
                case BlockDirection::bidirectional: return BlockUiHint::bidirectional;
                case BlockDirection::sender: return BlockUiHint::sender;
                case BlockDirection::receiver: return BlockUiHint::receiver;
                case BlockDirection::unknown: break;
            }

            return BlockUiHint::unknown;
        });

        const auto block = Block{}.withDirection (direction)
                                  .withUiHint (hint)
                                  .withEnabled (true)
                                  .withFirstGroup (0)
                                  .withNumGroups (1)
                                  .withMIDI1ProxyKind (BlockMIDI1ProxyKind::unrestrictedBandwidth);
        const auto id = std::invoke ([&]
        {
            switch (direction)
            {
                case BlockDirection::bidirectional:
                    return EndpointId::makeSrcDst (info.identifier, info.identifier);
                case BlockDirection::receiver:
                    return EndpointId::make (IOKind::dst, info.identifier);
                case BlockDirection::sender:
                    return EndpointId::make (IOKind::src, info.identifier);
                case BlockDirection::unknown: break;
            }

            return EndpointId{};
        });

        const auto srcId = direction != BlockDirection::receiver ? info.identifier : "";
        const auto dstId = direction != BlockDirection::sender ? info.identifier : "";

        std::array<String, 16> srcIds { srcId };
        std::array<String, 16> dstIds { dstId };

        const auto baseEndpoint = Endpoint{}.withName (info.name)
                                            .withProtocol (PacketProtocol::MIDI_1_0)
                                            .withUMPVersion (1, 1)
                                            .withMidi1Support (true)
                                            .withStaticBlocks (true)
                                            .withBlocks (std::array { block });

        const auto staticInfo = StaticDeviceInfo{}.withLegacyIdentifiersSrc (srcIds)
                                                  .withLegacyIdentifiersDst (dstIds)
                                                  .withHasSource (direction == BlockDirection::sender)
                                                  .withHasDestination (direction == BlockDirection::receiver)
                                                  .withName (info.name)
                                                  .withTransport (Transport::bytestream);

        return { baseEndpoint, staticInfo, id };
    }
};

}
