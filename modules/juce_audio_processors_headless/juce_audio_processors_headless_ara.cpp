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
#include <juce_core/system/juce_TargetPlatform.h>

#include <juce_audio_processors_headless/format/juce_PluginFormatDefs.h>

#include <juce_audio_processors_headless/utilities/ARA/juce_ARADebug.h>

/* Having WIN32_LEAN_AND_MEAN defined at the point of including ARADebug.c will produce warnings.

   To prevent such problems it's easiest to have it in its own translation unit.
*/

#if (JucePlugin_Enable_ARA || JUCE_INTERNAL_HAS_ARA) && (JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX || JUCE_IOS)

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wgnu-zero-variadic-macro-arguments", "-Wmissing-prototypes")
 #include <ARA_Library/Debug/ARADebug.c>
JUCE_END_IGNORE_WARNINGS_GCC_LIKE

#endif
