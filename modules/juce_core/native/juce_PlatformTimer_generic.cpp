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

class GenericPlatformTimer final : private Thread
{
public:
    explicit GenericPlatformTimer (PlatformTimerListener& ptl)
        : Thread { "HighResolutionTimerThread" },
          listener { ptl }
    {
        if (startThread (Priority::highest))
            return;

        // This likely suggests there are too many threads running!
        jassertfalse;
    }

    ~GenericPlatformTimer() override
    {
        stopThread (-1);
    }

    void startTimer (int newIntervalMs)
    {
        jassert (newIntervalMs > 0);
        jassert (timer == nullptr);

        {
            std::scoped_lock lock { runCopyMutex };
            timer = std::make_shared<Timer> (listener, newIntervalMs);
        }

        notify();
    }

    void cancelTimer()
    {
        jassert (timer != nullptr);

        timer->cancel();

        // Note the only race condition we need to protect against
        // here is the copy in run().
        //
        // Calls to startTimer(), cancelTimer(), and getIntervalMs()
        // are already guaranteed to be both thread safe and well
        // synchronised.

        std::scoped_lock lock { runCopyMutex };
        timer = nullptr;
    }

    int getIntervalMs() const
    {
        return isThreadRunning() && timer != nullptr ? timer->getIntervalMs() : 0;
    }

private:
    void run() final
    {
        const auto copyTimer = [&]
        {
            std::scoped_lock lock { runCopyMutex };
            return timer;
        };

        while (! threadShouldExit())
        {
            if (auto t = copyTimer())
                t->run();

            wait (-1);
        }
    }

    class Timer
    {
    public:
        Timer (PlatformTimerListener& l, int i)
            : listener { l }, intervalMs { i } {}

        int getIntervalMs() const
        {
            return intervalMs;
        }

        void cancel()
        {
            stop.signal();
        }

        void run()
        {
           #if JUCE_MAC || JUCE_IOS
            tryToUpgradeCurrentThreadToRealtime (Thread::RealtimeOptions{}.withPeriodMs (intervalMs));
           #endif

            const auto millisecondsUntil = [] (auto time)
            {
                return jmax (0.0, time - Time::getMillisecondCounterHiRes());
            };

            while (! stop.wait (millisecondsUntil (nextEventTime)))
            {
                if (Time::getMillisecondCounterHiRes() >= nextEventTime)
                {
                    listener.onTimerExpired();
                    nextEventTime += intervalMs;
                }
            }
        }

    private:
        PlatformTimerListener& listener;
        const int intervalMs;
        double nextEventTime = Time::getMillisecondCounterHiRes() + intervalMs;
        WaitableEvent stop { true };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Timer)
        JUCE_DECLARE_NON_MOVEABLE (Timer)
    };

    PlatformTimerListener& listener;
    mutable std::mutex runCopyMutex;
    std::shared_ptr<Timer> timer;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GenericPlatformTimer)
    JUCE_DECLARE_NON_MOVEABLE (GenericPlatformTimer)
};

#if ! JUCE_WINDOWS
using PlatformTimer = GenericPlatformTimer;
#endif

} // namespace juce
