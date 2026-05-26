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

namespace juce::midi_ci::detail
{

struct PropertyHostUtils
{
    PropertyHostUtils() = delete;

    static void send (BufferOutput& output,
                      uint8_t group,
                      std::byte subID2,
                      MUID targetMuid,
                      std::byte requestID,
                      Span<const std::byte> header,
                      Span<const std::byte> body,
                      int chunkSize)
    {
        MemoryInputStream stream (body.data(), body.size(), false);
        const detail::PropertyDataMessageChunker chunker { output.getOutputBuffer(),
                                                           std::min (chunkSize, 1 << 16),
                                                           subID2,
                                                           requestID,
                                                           header,
                                                           output.getMuid(),
                                                           targetMuid,
                                                           stream };

        std::for_each (chunker.begin(), chunker.end(), [&] (auto) { output.send (group); });
    }
};
} // namespace juce::midi_ci::detail
