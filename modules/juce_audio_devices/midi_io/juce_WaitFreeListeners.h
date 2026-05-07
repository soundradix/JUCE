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

/*
    Similar to ListenerList, but more suitable for the (rare!) cases where
    updates are triggered from a real-time thread. Triggering updates will
    never block, but adding and removing listeners might.

    @tags{Audio}
*/
template <typename Listener>
class WaitFreeListeners
{
public:
    static_assert (alignof (Listener) != 1);

    WaitFreeListeners() = default;

    /** Registers a receiver, *not* wait-free */
    void add (Listener& r)
    {
        auto copy = std::invoke ([&]
        {
            const std::scoped_lock lock { mainCopyMutex };
            mainCopy.emplace (&r, std::make_shared<Entry> (r));

            std::vector<std::shared_ptr<Entry>> entries (mainCopy.size());
            std::transform (mainCopy.begin(), mainCopy.end(), entries.begin(), [] (const auto& p) { return p.second; });
            return entries;
        });

        {
            const SpinLock::ScopedLockType lock { blockingCopyMutex };
            blockingCopy = std::move (copy);
            listChanged = true;
        }
    }

    /** Removes a listener, *not* wait-free. */
    void remove (Listener& l)
    {
        const auto entryToClear = std::invoke ([&]
        {
            const std::scoped_lock lock { mainCopyMutex };
            const auto iter = mainCopy.find (&l);
            return iter != mainCopy.end() ? iter->second : nullptr;
        });

        if (entryToClear != nullptr)
        {
            auto& entry = *entryToClear;

            constexpr size_t expected = 1;
            auto tmp = expected;

            // If the count is zero, clear the entire entry. If the entry is set to zero
            // in the meantime, that means someone else has removed this entry, so we can exit
            // in that case.
            while (! entry.useCount.compare_exchange_weak (tmp, 0) && tmp != 0)
                tmp = expected;
        }

        const std::scoped_lock lock { mainCopyMutex };
        mainCopy.erase (&l);
    }

    /** Notifies all registered receivers, wait-free, may be called concurrently with add/remove,
        and with itself.
    */
    template <typename Callback>
    void call (Callback&& callback) const
    {
        {
            const SpinLock::ScopedTryLockType lock { blockingCopyMutex };

            if (lock.isLocked() && std::exchange (listChanged, false))
                std::swap (callerCopy, blockingCopy);
        }

        for (auto& entry : callerCopy)
        {
            const auto oldUseCount = entry->useCount.fetch_add (1);

            if (oldUseCount != 0)
                callback (*entry->listener);

            entry->useCount -= 1;
        }
    }

    JUCE_DECLARE_NON_COPYABLE (WaitFreeListeners)
    JUCE_DECLARE_NON_MOVEABLE (WaitFreeListeners)

private:
    struct Entry
    {
        explicit Entry (Listener& l) : listener (&l) {}
        Listener* listener{};
        std::atomic<size_t> useCount { 1 };
    };
    std::map<Listener*, std::shared_ptr<Entry>> mainCopy;
    mutable std::vector<std::shared_ptr<Entry>> blockingCopy, callerCopy;

    mutable std::mutex mainCopyMutex;
    mutable SpinLock blockingCopyMutex;
    mutable bool listChanged = false;
};

} // namespace juce
