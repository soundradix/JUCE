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

/**
    Implements a plugin format manager for VSTs.

    @tags{Audio}
*/
class JUCE_API VSTPluginFormat : public VSTPluginFormatHeadless
{
public:
    static std::unique_ptr<AudioPluginInstance> createCustomVSTFromMainCall (void* entryPointFunction,
                                                                             double initialSampleRate,
                                                                             int initialBufferSize);

private:
    void createPluginInstance (const PluginDescription&, double, int, PluginCreationCallback) override;
};

} // namespace juce

#endif
