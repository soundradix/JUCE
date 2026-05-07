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

#if JUCE_INTERNAL_HAS_VST3

/**
    Implements a plugin format for VST3s.

    @tags{Audio}
*/
class JUCE_API VST3PluginFormat : public VST3PluginFormatHeadless
{
    void createPluginInstance (const PluginDescription&, double, int, PluginCreationCallback) override;
};

#endif

} // namespace juce
