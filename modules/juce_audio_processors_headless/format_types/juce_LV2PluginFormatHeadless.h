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

#if JUCE_INTERNAL_HAS_LV2 || DOXYGEN

/**
    Implements a plugin format for LV2 plugins.

    @tags{Audio}
*/
class JUCE_API LV2PluginFormatHeadless  : public AudioPluginFormat
{
public:
    LV2PluginFormatHeadless();
    ~LV2PluginFormatHeadless() override;

    static String getFormatName()       { return "LV2"; }
    String getName() const override     { return getFormatName(); }

    void findAllTypesForFile (OwnedArray<PluginDescription>& results,
                              const String& fileOrIdentifier) override;

    bool fileMightContainThisPluginType (const String& fileOrIdentifier) override;

    String getNameOfPluginFromIdentifier (const String& fileOrIdentifier) override;

    bool pluginNeedsRescanning (const PluginDescription&) override;

    bool doesPluginStillExist (const PluginDescription&) override;

    bool canScanForPlugins() const override;

    bool isTrivialToScan() const override;

    StringArray searchPathsForPlugins (const FileSearchPath& directoriesToSearch,
                                       bool recursive,
                                       bool allowPluginsWhichRequireAsynchronousInstantiation = false) override;

    FileSearchPath getDefaultLocationsToSearch() override;

    /** @internal */
    class Pimpl;

private:
    bool requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const override;
    void createPluginInstance (const PluginDescription&, double, int, PluginCreationCallback) override;

    std::unique_ptr<Pimpl> pimpl;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LV2PluginFormatHeadless)
};

#endif

} // namespace juce
