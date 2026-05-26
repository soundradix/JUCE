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

#if JUCE_INTERNAL_HAS_VST || DOXYGEN

namespace juce
{

//==============================================================================
/**
    Implements a plugin format manager for VSTs.

    @tags{Audio}
*/
class JUCE_API  VSTPluginFormatHeadless   : public AudioPluginFormat
{
public:
    //==============================================================================
    VSTPluginFormatHeadless() = default;

    //==============================================================================
    /** Given a suitable function pointer to a VSTPluginMain function, this will attempt to
        instantiate and return a plugin for it.
    */
    static AudioPluginInstance* createCustomVSTFromMainCall (void* entryPointFunction,
                                                             double initialSampleRate,
                                                             int initialBufferSize);

    using ExtraFunctions = AudioPluginExtensions::VSTClient::ExtraFunctions;

    //==============================================================================
    /** Given a VstEffectInterface* (aka vst::AEffect*), this will return the juce AudioPluginInstance
        that is being used to wrap it
    */
    static AudioPluginInstance* getPluginInstanceFromVstEffectInterface (void* aEffect);

    //==============================================================================
    static String getFormatName()                   { return "VST"; }
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

    /** Can be overridden to receive a callback when each member of a shell plugin is about to be
        tested during a call to findAllTypesForFile().
        Only the name and uid members of the PluginDescription are guaranteed to be valid when
        this is called.
    */
    virtual void aboutToScanVSTShellPlugin (const PluginDescription&);

private:
    //==============================================================================
    void createPluginInstance (const PluginDescription&, double initialSampleRate,
                               int initialBufferSize, PluginCreationCallback) override;
    bool requiresUnblockedMessageThreadDuringCreation (const PluginDescription&) const override;
    void recursiveFileSearch (StringArray&, const File&, bool recursive);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VSTPluginFormatHeadless)
};

} // namespace juce

#endif
