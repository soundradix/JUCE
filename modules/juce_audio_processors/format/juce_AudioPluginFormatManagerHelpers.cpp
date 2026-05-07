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

namespace juce
{

void addDefaultFormatsToManager ([[maybe_unused]] AudioPluginFormatManager& manager)
{
   #if JUCE_INTERNAL_HAS_AU
    manager.addFormat (std::make_unique<AudioUnitPluginFormat>());
   #endif

   #if JUCE_INTERNAL_HAS_VST
    manager.addFormat (std::make_unique<VSTPluginFormat>());
   #endif

   #if JUCE_INTERNAL_HAS_VST3
    manager.addFormat (std::make_unique<VST3PluginFormat>());
   #endif

   #if JUCE_INTERNAL_HAS_LADSPA
    manager.addFormat (std::make_unique<LADSPAPluginFormat>());
   #endif

   #if JUCE_INTERNAL_HAS_LV2
    manager.addFormat (std::make_unique<LV2PluginFormat>());
   #endif
}

} // namespace juce
