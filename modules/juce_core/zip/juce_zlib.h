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

// The purpose of this internal header, included by juce_core and juce_graphics,
// is to place the zlib symbols in a common namespace with consistent preprocessor
// definitions.

#pragma once

namespace juce::zlibNamespace
{
#define ZLIB_CONST 1

#if JUCE_INCLUDE_ZLIB_CODE

 #define HAVE_MEMCPY 1

 #include "juce_core/zip/zlib/zlib.h"

#else
 #include JUCE_ZLIB_INCLUDE_PATH
#endif

} // namespace juce::zlibNamespace
