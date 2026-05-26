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

struct TypefaceFileAndIndex
{
    File file;
    int index{};

    auto tie() const { return std::tuple (file, index); }

    bool operator< (const TypefaceFileAndIndex& other) const { return tie() < other.tie(); }
};

class TypefaceFileCache : public DeletedAtShutdown
{
public:
    ~TypefaceFileCache() override
    {
        clearSingletonInstance();
    }

    template <typename Fn>
    Typeface::Ptr get (const TypefaceFileAndIndex& key, Fn&& getTypeface)
    {
        return cachedTypefaces.get (key, std::forward<Fn> (getTypeface));
    }

    JUCE_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL_INLINE (TypefaceFileCache)

private:
    LruCache<TypefaceFileAndIndex, Typeface::Ptr> cachedTypefaces;
};

} // namespace juce
/** @endcond */
