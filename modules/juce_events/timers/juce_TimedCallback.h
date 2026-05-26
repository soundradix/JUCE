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

/** Utility class wrapping a single non-null callback called by a Timer.

    You can use the usual Timer functions to start and stop the TimedCallback. Deleting the
    TimedCallback will automatically stop the underlying Timer.

    With this class you can use the Timer facility without inheritance.

    @see Timer
    @tags{Events}
*/
class TimedCallback final : private Timer
{
public:
    /** Constructor. The passed in callback must be non-null. */
    explicit TimedCallback (std::function<void()> callbackIn)
        : callback (std::move (callbackIn))
    {
        jassert (callback);
    }

    /** Destructor. */
    ~TimedCallback() noexcept override { stopTimer(); }

    using Timer::startTimer;
    using Timer::startTimerHz;
    using Timer::stopTimer;
    using Timer::isTimerRunning;
    using Timer::getTimerInterval;

private:
    void timerCallback() override { callback(); }

    std::function<void()> callback;
};

} // namespace juce
