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

/** @cond */
namespace juce
{

template <typename Key, typename Value, size_t maxEntries = 128>
class LruCache
{
public:
    template <typename Fn>
    const Value& get (Key key, Fn&& fn)
    {
        std::unique_lock lock { *mutex };

        if (const auto iter = map.find (key); iter != map.end())
        {
            list.erase (iter->second.listIterator);
            iter->second.listIterator = list.insert (list.end(), iter);
            return iter->second.value;
        }

        const auto localInsertionCounter = insertionCounter;

        // There are two reasons we don't want to have the mutex locked while
        // getting the value
        // 1. If the operation itself results in a call to the same cache it
        //    would cause a deadlock.
        // 2. The generation of the value is likely to be slow therefore we
        //    don't want to force other threads to wait on this operation
        lock.unlock();
        auto value = fn (key);
        lock.lock();

        // While the mutex was unlocked the value may have already been added to
        // the cache. In this case we can skip placing the value at the front as
        // it should've already recently been placed at the front. However, we
        // still need to skip adding the value to the cache for a second time.
        if (localInsertionCounter != insertionCounter)
            if (const auto iter = map.find (key); iter != map.end())
                return iter->second.value;

        ++insertionCounter;

        while (map.size() >= maxEntries)
        {
            const auto toRemove = list.begin();
            map.erase (*toRemove);
            list.erase (toRemove);
        }

        const auto mapIteratorPair = map.emplace (std::move (key), Pair { std::move (value), {} });

        jassert (mapIteratorPair.second);

        mapIteratorPair.first->second.listIterator = list.insert (list.end(), mapIteratorPair.first);
        return mapIteratorPair.first->second.value;
    }

    void clear()
    {
        const std::scoped_lock lock { *mutex };
        list.clear();
        map.clear();
    }

private:
    struct Pair
    {
        using Map = std::map<Key, Pair>;
        using MapIterator = typename Map::const_iterator;
        using List = std::list<MapIterator>;
        using ListIterator = typename List::const_iterator;

        Value value;
        ListIterator listIterator;
    };

    typename Pair::Map map;
    typename Pair::List list;
    uint32_t insertionCounter{};
    std::unique_ptr<std::mutex> mutex = std::make_unique<std::mutex>();
};

} // namespace juce
/** @endcond */
