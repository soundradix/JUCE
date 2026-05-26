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

#include <dlfcn.h>

int main (int argc, const char* const* argv)
{
    if (argc >= 3)
        if (auto* handle = dlopen (argv[1], RTLD_LAZY))
            if (auto* function = reinterpret_cast<int (*) (int, const char* const*)> (dlsym (handle, argv[2])))
                return function (argc - 3, argv + 3);

    return 1;
}
