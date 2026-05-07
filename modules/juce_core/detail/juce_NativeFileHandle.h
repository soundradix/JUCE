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

namespace juce::detail
{

#if JUCE_WINDOWS
using NativeFileHandle = void*;
#else
class NativeFileHandle
{
    static constexpr int invalidFd = -1;

public:
    int get() const noexcept { return fd; }
    bool isValid() const noexcept { return 0 <= fd; }

    void set (int newFd) noexcept { fd = newFd; }
    void invalidate() noexcept { fd = invalidFd; }

private:
    int fd = invalidFd;
};
#endif

} // namespace juce::detail
