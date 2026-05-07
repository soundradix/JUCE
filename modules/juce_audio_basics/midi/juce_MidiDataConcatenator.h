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

namespace juce
{

enum class SysexExtractorCallbackKind
{
    notSysex,
    ongoingSysex,
    lastSysex,
};

class BytestreamSysexExtractor
{
public:
    void reset()
    {
        state = RunningStatus{};
    }

    template <typename Callback>
    void push (Span<const std::byte> bytes, Callback&& callback)
    {
        for (const auto pair : enumerate (bytes))
        {
            const auto index = pair.index;
            const auto byte = pair.value;

            state = std::invoke ([&]() -> State
            {
                if (auto* inSysex = std::get_if<InSysex> (&state))
                {
                    if (byte == std::byte { 0xf0 })
                    {
                        callback (SysexExtractorCallbackKind::lastSysex,
                                  Span { bytes.data() + index - inSysex->numBytes, inSysex->numBytes });
                        return InSysex { 1 };
                    }

                    if (byte == std::byte { 0xf7 })
                    {
                        callback (SysexExtractorCallbackKind::lastSysex,
                                  Span { bytes.data() + index - inSysex->numBytes, inSysex->numBytes + 1 });
                        return RunningStatus{};
                    }

                    if (isRealtimeMessage (byte))
                    {
                        callback (SysexExtractorCallbackKind::ongoingSysex,
                                  Span { bytes.data() + index - inSysex->numBytes, inSysex->numBytes });
                        callback (SysexExtractorCallbackKind::notSysex,
                                  Span { bytes.data() + index, 1 });
                        return InSysex{};
                    }

                    if (isStatusByte (byte))
                    {
                        callback (SysexExtractorCallbackKind::lastSysex,
                                  Span { bytes.data() + index - inSysex->numBytes, inSysex->numBytes });
                        return RunningStatus { 1, { byte } };
                    }

                    return InSysex { inSysex->numBytes + 1 };
                }

                if (auto* runningStatus = std::get_if<RunningStatus> (&state))
                {
                    if (byte == std::byte { 0xf0 })
                        return InSysex { 1 };

                    const auto nextRunningStatus = std::invoke ([&]
                    {
                        if (isRealtimeMessage (byte))
                        {
                            callback (SysexExtractorCallbackKind::notSysex,
                                      Span { bytes.data() + index, 1 });
                            return *runningStatus;
                        }

                        if (isInitialByte (byte))
                            return RunningStatus{}.withAppendedByte (byte);

                        if (0 < runningStatus->size && runningStatus->size < runningStatus->data.size())
                            return runningStatus->withAppendedByte (byte);

                        // If we get to this branch, we're trying to process a non-status byte
                        // without having seen any previous status byte, so ignore the current byte
                        return RunningStatus{};
                    });

                    if (const auto completeMessage = nextRunningStatus.getCompleteMessage(); ! completeMessage.empty())
                    {
                        callback (SysexExtractorCallbackKind::notSysex,
                                  completeMessage);
                        return RunningStatus{}.withAppendedByte (nextRunningStatus.data[0]);
                    }

                    return nextRunningStatus;
                }

                // Can only happen if the variant is valueless by exception, which indicates a much
                // more severe problem!
                std::terminate();
            });
        }

        if (auto* inSysex = std::get_if<InSysex> (&state))
        {
            callback (SysexExtractorCallbackKind::ongoingSysex,
                      Span { bytes.data() + bytes.size() - inSysex->numBytes, inSysex->numBytes });
            state = InSysex{};
        }
    }

private:
    static bool isRealtimeMessage (std::byte byte)  { return std::byte (0xf8) <= byte && byte <= std::byte (0xfe); }
    static bool isStatusByte      (std::byte byte)  { return std::byte (0x80) <= byte; }
    static bool isInitialByte     (std::byte byte)  { return isStatusByte (byte) && byte != std::byte (0xf7); }

    struct InSysex
    {
        size_t numBytes{};
    };

    struct RunningStatus
    {
        // These constructors are required to work around a bug in GCC 7
        RunningStatus() {}

        RunningStatus (uint8_t sizeIn, std::array<std::byte, 3> dataIn)
            : size (sizeIn), data (dataIn) {}

        uint8_t size{};
        std::array<std::byte, 3> data{};

        Span<const std::byte> getCompleteMessage() const
        {
            if (size == 0)
                return {};

            const auto expectedSize = MidiMessage::getMessageLengthFromFirstByte ((uint8_t) data[0]);
            return Span { data.data(), size == expectedSize ? size : (size_t) 0 };
        }

        void appendByte (std::byte x)
        {
            jassert (size < data.size());
            data[size++] = x;
        }

        RunningStatus withAppendedByte (std::byte x) const
        {
            auto result = *this;
            result.appendByte (x);
            return result;
        }
    };

    using State = std::variant<RunningStatus, InSysex>;
    State state;
};

//==============================================================================
/**
    Helper class that takes chunks of incoming midi bytes, packages them into
    messages, and dispatches them to a midi callback.

    @tags{Audio}
*/
class MidiDataConcatenator
{
public:
    MidiDataConcatenator (int initialBufferSize)
    {
        pendingSysexData.reserve ((size_t) initialBufferSize);
    }

    MidiDataConcatenator (MidiDataConcatenator&&) noexcept = default;
    MidiDataConcatenator& operator= (MidiDataConcatenator&&) noexcept = default;

    void reset()
    {
        extractor.reset();
        pendingSysexData.clear();
        pendingSysexTime = 0;
    }

    template <typename UserDataType, typename CallbackType>
    void pushMidiData (Span<const std::byte> bytes,
                       double time,
                       UserDataType* input,
                       CallbackType& callback)
    {
        extractor.push (bytes, [&] (SysexExtractorCallbackKind kind, Span<const std::byte> bytesThisTime)
        {
            switch (kind)
            {
                case SysexExtractorCallbackKind::notSysex:
                    callback.handleIncomingMidiMessage (input,
                                                        MidiMessage (bytesThisTime.data(),
                                                                     (int) bytesThisTime.size(),
                                                                     time));
                    return;

                case SysexExtractorCallbackKind::ongoingSysex:
                {
                    if (pendingSysexData.empty())
                        pendingSysexTime = time;

                    pendingSysexData.insert (pendingSysexData.end(), bytesThisTime.begin(), bytesThisTime.end());
                    return;
                }

                case SysexExtractorCallbackKind::lastSysex:
                {
                    pendingSysexData.insert (pendingSysexData.end(), bytesThisTime.begin(), bytesThisTime.end());

                    if (pendingSysexData.empty())
                    {
                        jassertfalse;
                        return;
                    }

                    if (pendingSysexData.back() == std::byte { 0xf7 })
                    {
                        callback.handleIncomingMidiMessage (input,
                                                            MidiMessage (pendingSysexData.data(),
                                                                         (int) pendingSysexData.size(),
                                                                         pendingSysexTime));
                    }
                    else
                    {
                        callback.handlePartialSysexMessage (input,
                                                            unalignedPointerCast<const uint8*> (pendingSysexData.data()),
                                                            (int) pendingSysexData.size(),
                                                            pendingSysexTime);
                    }

                    pendingSysexData.clear();

                    return;
                }
            }
        });
    }

    template <typename UserDataType, typename CallbackType>
    void pushMidiData (const void* inputData,
                       int numBytes,
                       double time,
                       UserDataType* input,
                       CallbackType& callback)
    {
        pushMidiData ({ static_cast<const std::byte*> (inputData), (size_t) numBytes }, time, input, callback);
    }

private:
    BytestreamSysexExtractor extractor;
    std::vector<std::byte> pendingSysexData;
    double pendingSysexTime = 0;

    JUCE_DECLARE_NON_COPYABLE (MidiDataConcatenator)
};

} // namespace juce
