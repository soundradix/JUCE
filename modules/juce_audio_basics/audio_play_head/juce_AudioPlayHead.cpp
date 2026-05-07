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

bool AudioPlayHead::FrameRate::operator== (const FrameRate& other) const
{
    const auto tie = [] (const FrameRate& x) { return std::tie (x.base, x.drop, x.pulldown); };
    return tie (*this) == tie (other);
}

bool AudioPlayHead::TimeSignature::operator== (const TimeSignature& other) const
{
    const auto tie = [] (auto& x) { return std::tie (x.numerator, x.denominator); };
    return tie (*this) == tie (other);
}

bool AudioPlayHead::LoopPoints::operator== (const LoopPoints& other) const
{
    const auto tie = [] (auto& x) { return std::tie (x.ppqStart, x.ppqEnd); };
    return tie (*this) == tie (other);
}

bool AudioPlayHead::CurrentPositionInfo::operator== (const CurrentPositionInfo& other) const noexcept
{
    const auto tie = [] (const CurrentPositionInfo& i)
    {
        return std::tie (i.timeInSamples,
                         i.ppqPosition,
                         i.editOriginTime,
                         i.ppqPositionOfLastBarStart,
                         i.frameRate,
                         i.isPlaying,
                         i.isRecording,
                         i.bpm,
                         i.timeSigNumerator,
                         i.timeSigDenominator,
                         i.ppqLoopStart,
                         i.ppqLoopEnd,
                         i.isLooping);
    };

    return tie (*this) == tie (other);
}

bool AudioPlayHead::PositionInfo::operator== (const PositionInfo& other) const noexcept
{
    const auto tie = [] (const PositionInfo& i)
    {
        return std::make_tuple (i.getTimeInSamples(),
                                i.getTimeInSeconds(),
                                i.getPpqPosition(),
                                i.getEditOriginTime(),
                                i.getPpqPositionOfLastBarStart(),
                                i.getFrameRate(),
                                i.getBarCount(),
                                i.getTimeSignature(),
                                i.getBpm(),
                                i.getLoopPoints(),
                                i.getHostTimeNs(),
                                i.getIsPlaying(),
                                i.getIsRecording(),
                                i.getIsLooping());
    };

    return tie (*this) == tie (other);
}

bool AudioPlayHead::canControlTransport()                                          { return false; }
void AudioPlayHead::transportPlay ([[maybe_unused]] bool shouldStartPlaying)       {}
void AudioPlayHead::transportRecord ([[maybe_unused]] bool shouldStartRecording)   {}
void AudioPlayHead::transportRewind()                                              {}

} // namespace juce
