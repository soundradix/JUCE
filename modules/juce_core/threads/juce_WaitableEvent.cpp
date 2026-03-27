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

WaitableEvent::WaitableEvent (bool manualReset) noexcept
    : useManualReset (manualReset)
{
}

void WaitableEvent::wait() const
{
    std::unique_lock<std::mutex> lock (mutex);

    if (! triggered)
        condition.wait (lock, [this] { return triggered == true; });

    if (! useManualReset)
        reset();
}

bool WaitableEvent::wait (Seconds timeOut) const
{
    std::unique_lock<std::mutex> lock (mutex);

    if (! triggered && ! condition.wait_for (lock, timeOut, [this] { return triggered == true; }))
        return false;

    if (! useManualReset)
        reset();

    return true;
}

bool WaitableEvent::wait (double timeOutMilliseconds) const
{
    if (timeOutMilliseconds >= 0.0)
        return wait (Milliseconds { timeOutMilliseconds });

    wait();
    return true;
}

void WaitableEvent::signal() const
{
    std::lock_guard<std::mutex> lock (mutex);

    triggered = true;
    condition.notify_all();
}

void WaitableEvent::reset() const
{
    triggered = false;
}

} // namespace juce
