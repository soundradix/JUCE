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

#include <juce_audio_plugin_client/detail/juce_IncludeModuleHeaders.h>
#include <juce_audio_plugin_client/detail/juce_CreatePluginFilter.h>

namespace juce::detail
{

struct PluginUtilities
{
    PluginUtilities() = delete;

    static int getDesktopFlags (const AudioProcessorEditor& editor)
    {
        return editor.wantsLayerBackedView()
             ? 0
             : ComponentPeer::windowRequiresSynchronousCoreGraphicsRendering;
    }

    static int getDesktopFlags (const AudioProcessorEditor* editor)
    {
        return editor != nullptr ? getDesktopFlags (*editor) : 0;
    }

    static void addToDesktop (AudioProcessorEditor& editor, void* parent)
    {
        editor.addToDesktop (getDesktopFlags (editor), parent);
    }

    static const PluginHostType& getHostType()
    {
        static PluginHostType hostType;
        return hostType;
    }
};

} // namespace juce::detail
