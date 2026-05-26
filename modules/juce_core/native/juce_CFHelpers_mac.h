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

/* This file contains a few helper functions that are used internally but which
   need to be kept away from the public headers because they use obj-C symbols.
*/
namespace juce
{

struct CFObjectDeleter
{
    void operator() (CFTypeRef object) const noexcept
    {
        if (object != nullptr)
            CFRelease (object);
    }
};

template <typename CFType>
struct CFRefRemover
{
    static_assert (std::is_pointer_v<CFType>, "CFType must be a Ref type");
    using Type = std::remove_pointer_t<CFType>;
};

template <typename CFType>
using CFRemoveRef = typename CFRefRemover<CFType>::Type;

template <typename CFType>
using CFUniquePtr = std::unique_ptr<CFRemoveRef<CFType>, CFObjectDeleter>;

template <typename CFType>
struct CFObjectHolder
{
    CFObjectHolder() = default;
    explicit CFObjectHolder (CFType obj)  : object (obj) {}

    CFObjectHolder (const CFObjectHolder&) = delete;
    CFObjectHolder (CFObjectHolder&&) = delete;

    CFObjectHolder& operator= (const CFObjectHolder&) = delete;
    CFObjectHolder& operator= (CFObjectHolder&&) = delete;

    ~CFObjectHolder() noexcept
    {
        if (object != nullptr)
            CFRelease (object);
    }

    // Public to facilitate passing the pointer address to functions
    CFType object = nullptr;
};

} // namespace juce
