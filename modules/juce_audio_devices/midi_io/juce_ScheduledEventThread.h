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

/**
    @internal

    Allows events to be queued up, then for each event calls the OutputCallback at the time
    dictated by that event's timestamp.

    Event must have a getTimeStamp() member function that returns the output time of the event.
*/
template <typename Event>
class ScheduledEventThread : private Thread
{
public:
    using OutputCallback = std::function<void (const Event&)>;

    explicit ScheduledEventThread (OutputCallback&& c)
        : Thread (SystemStats::getJUCEVersion() + ": MIDI Out"),
          outputCallback (std::move (c))
    {
        jassert (outputCallback != nullptr);
    }

    ~ScheduledEventThread() override
    {
        stop();
    }

    void clearAllPendingMessages()
    {
        {
            const std::scoped_lock sl (mutex);
            pendingMessages.clear();
        }

        condvar.notify_one();
    }

    void start()
    {
        {
            const std::scoped_lock sl (mutex);
            backgroundThreadRunning = true;
        }

        startThread (Priority::high);
    }

    void stop()
    {
        {
            const std::scoped_lock sl (mutex);
            backgroundThreadRunning = false;
        }

        condvar.notify_one();
        stopThread (-1);
    }

    void addEvent (const Event& event)
    {
        // You've got to call startBackgroundThread() for this to actually work.
        jassert (isThreadRunning());

        {
            const std::scoped_lock sl (mutex);
            pendingMessages.insert (event);
        }

        condvar.notify_one();
    }

    bool isRunning() const
    {
        const std::scoped_lock sl (mutex);
        return backgroundThreadRunning;
    }

private:
    void run() override
    {
        for (;;)
        {
            std::unique_lock lock (mutex);
            condvar.wait (lock, [&]
            {
                return ! pendingMessages.empty() || ! backgroundThreadRunning;
            });

            if (! backgroundThreadRunning)
                return;

            const auto now = Time::getMillisecondCounter();
            const auto event = *pendingMessages.begin();
            const auto timestamp = event.getTimeStamp();

            if (timestamp > now + 20)
            {
                const auto millis = static_cast<int64_t> (timestamp - (now + 20));
                condvar.wait_for (lock, std::chrono::milliseconds (millis));
                continue;
            }

            if (timestamp > now)
                Time::waitForMillisecondCounter ((uint32) timestamp);

            if (timestamp > now - 200)
                outputCallback (event);

            pendingMessages.erase (pendingMessages.begin());
        }
    }

    struct Comparator
    {
        bool operator() (const Event& a, const Event& b) const
        {
            return a.getTimeStamp() < b.getTimeStamp();
        }
    };

    mutable std::mutex mutex;
    std::condition_variable condvar;
    std::multiset<Event, Comparator> pendingMessages;
    OutputCallback outputCallback;
    bool backgroundThreadRunning = false;
};

} // namespace juce
