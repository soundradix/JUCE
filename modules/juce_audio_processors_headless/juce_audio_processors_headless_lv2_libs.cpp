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

 #ifndef _CRT_SECURE_NO_WARNINGS
  #define _CRT_SECURE_NO_WARNINGS
 #endif

#include <juce_core/system/juce_CompilerWarnings.h>
#include <juce_core/system/juce_TargetPlatform.h>

#include <juce_audio_processors_headless/format/juce_PluginFormatDefs.h>

#if JUCE_INTERNAL_HAS_LV2
 #include <juce_audio_processors_headless/format_types/juce_LV2SupportLibs.cpp>
#endif
