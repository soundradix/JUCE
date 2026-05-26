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

#pragma once

// The following definitions are PRIVATE and should not be queried or modified
// by user code. These are exclusively used to enable and disable JUCE
// implementation details.

#if defined (JUCE_INTERNAL_HAS_VST) \
 || defined (JUCE_INTERNAL_HAS_VST3) \
 || defined (JUCE_INTERNAL_HAS_AU) \
 || defined (JUCE_INTERNAL_HAS_LADSPA) \
 || defined (JUCE_INTERNAL_HAS_LV2)
 #error These preprocessor definitions should not be set by the build system. Use the JUCE_PLUGINHOST_* definitions instead.
#endif

#if JUCE_PLUGINHOST_VST && (JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX || JUCE_BSD || JUCE_IOS)
 #define JUCE_INTERNAL_HAS_VST 1
#else
 #define JUCE_INTERNAL_HAS_VST 0
#endif

#if JUCE_PLUGINHOST_VST3 && (JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX || JUCE_BSD)
 #define JUCE_INTERNAL_HAS_VST3 1
#else
 #define JUCE_INTERNAL_HAS_VST3 0
#endif

#if JUCE_PLUGINHOST_AU && (JUCE_MAC || JUCE_IOS)
 #define JUCE_INTERNAL_HAS_AU 1
#else
 #define JUCE_INTERNAL_HAS_AU 0
#endif

#if JUCE_PLUGINHOST_LADSPA && (JUCE_LINUX || JUCE_BSD)
 #define JUCE_INTERNAL_HAS_LADSPA 1
#else
 #define JUCE_INTERNAL_HAS_LADSPA 0
#endif

#if JUCE_PLUGINHOST_LV2 && (JUCE_MAC || JUCE_LINUX || JUCE_BSD || JUCE_WINDOWS)
 #define JUCE_INTERNAL_HAS_LV2 1
#else
 #define JUCE_INTERNAL_HAS_LV2 0
#endif

#if JUCE_PLUGINHOST_ARA && (JUCE_INTERNAL_HAS_VST3 || JUCE_INTERNAL_HAS_AU) && (JUCE_MAC || JUCE_WINDOWS || JUCE_LINUX)
 #define JUCE_INTERNAL_HAS_ARA 1
#else
 #define JUCE_INTERNAL_HAS_ARA 0
#endif
