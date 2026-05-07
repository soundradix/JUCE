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

#include <juce_core/system/juce_CompilerWarnings.h>

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wsign-conversion",
                                     "-Wcast-align",
                                     "-Wunused-variable",
                                     "-Wunused-function",
                                     "-Wstringop-overflow",
                                     "-Wimplicit-fallthrough")

JUCE_BEGIN_IGNORE_WARNINGS_MSVC (4189 4706 6011)

//==============================================================================

#define SB_CONFIG_UNITY 1

#include <juce_graphics/unicode/sheenbidi/Source/SheenBidi.c>

JUCE_END_IGNORE_WARNINGS_MSVC
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
