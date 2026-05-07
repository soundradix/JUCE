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

#if JUCE_INTERNAL_HAS_LV2

#include <juce_audio_processors_headless/format_types/juce_LV2PluginFormatImpl.h>

namespace juce
{

//==============================================================================
LV2PluginFormatHeadless::LV2PluginFormatHeadless()
    : pimpl (std::make_unique<Pimpl>()) {}

LV2PluginFormatHeadless::~LV2PluginFormatHeadless() = default;

void LV2PluginFormatHeadless::findAllTypesForFile (OwnedArray<PluginDescription>& results,
                                                   const String& fileOrIdentifier)
{
    pimpl->findAllTypesForFile (results, fileOrIdentifier);
}

bool LV2PluginFormatHeadless::fileMightContainThisPluginType (const String& fileOrIdentifier)
{
    return pimpl->fileMightContainThisPluginType (fileOrIdentifier);
}

String LV2PluginFormatHeadless::getNameOfPluginFromIdentifier (const String& fileOrIdentifier)
{
    return pimpl->getNameOfPluginFromIdentifier (fileOrIdentifier);
}

bool LV2PluginFormatHeadless::pluginNeedsRescanning (const PluginDescription& desc)
{
    return pimpl->pluginNeedsRescanning (desc);
}

bool LV2PluginFormatHeadless::doesPluginStillExist (const PluginDescription& desc)
{
    return pimpl->doesPluginStillExist (desc);
}

bool LV2PluginFormatHeadless::canScanForPlugins() const { return true; }
bool LV2PluginFormatHeadless::isTrivialToScan() const { return true; }

StringArray LV2PluginFormatHeadless::searchPathsForPlugins (const FileSearchPath& directoriesToSearch,
                                                            bool recursive,
                                                            bool allowAsync)
{
    return pimpl->searchPathsForPlugins (directoriesToSearch, recursive, allowAsync);
}

FileSearchPath LV2PluginFormatHeadless::getDefaultLocationsToSearch()
{
    return pimpl->getDefaultLocationsToSearch();
}

bool LV2PluginFormatHeadless::requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const
{
    return false;
}

void LV2PluginFormatHeadless::createPluginInstance (const PluginDescription& desc,
                                                    double sampleRate,
                                                    int bufferSize,
                                                    PluginCreationCallback callback)
{
    Pimpl::createPluginInstance<lv2_host::LV2AudioPluginInstanceHeadless> (*this, desc, sampleRate, bufferSize, std::move (callback));
}

} // namespace juce

#endif
