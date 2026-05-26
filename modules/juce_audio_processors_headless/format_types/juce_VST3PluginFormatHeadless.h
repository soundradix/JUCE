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

#if JUCE_INTERNAL_HAS_VST3 || DOXYGEN

/**
    Implements a plugin format for VST3s.

    @tags{Audio}
*/
class JUCE_API VST3PluginFormatHeadless   : public AudioPluginFormat
{
public:
    /** Constructor */
    VST3PluginFormatHeadless() = default;

    //==============================================================================
    static String getFormatName()                   { return "VST3"; }
    String getName() const override                 { return getFormatName(); }
    bool canScanForPlugins() const override         { return true; }
    bool isTrivialToScan() const override           { return false; }

    void findAllTypesForFile (OwnedArray<PluginDescription>&, const String& fileOrIdentifier) override;
    bool fileMightContainThisPluginType (const String& fileOrIdentifier) override;
    String getNameOfPluginFromIdentifier (const String& fileOrIdentifier) override;
    bool pluginNeedsRescanning (const PluginDescription&) override;
    StringArray searchPathsForPlugins (const FileSearchPath&, bool recursive, bool) override;
    bool doesPluginStillExist (const PluginDescription&) override;
    FileSearchPath getDefaultLocationsToSearch() override;
    void createARAFactoryAsync (const PluginDescription&, ARAFactoryCreationCallback callback) override;

private:
    //==============================================================================
    void createPluginInstance (const PluginDescription&, double initialSampleRate,
                               int initialBufferSize, PluginCreationCallback) override;
    bool requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VST3PluginFormatHeadless)
};

#endif

} // namespace juce
