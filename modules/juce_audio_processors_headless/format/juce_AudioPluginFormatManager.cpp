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

//==============================================================================
void addHeadlessDefaultFormatsToManager ([[maybe_unused]] AudioPluginFormatManager& manager)
{
   #if JUCE_INTERNAL_HAS_AU
    manager.addFormat (std::make_unique<AudioUnitPluginFormatHeadless>());
   #endif

   #if JUCE_INTERNAL_HAS_VST
    manager.addFormat (std::make_unique<VSTPluginFormatHeadless>());
   #endif

   #if JUCE_INTERNAL_HAS_VST3
    manager.addFormat (std::make_unique<VST3PluginFormatHeadless>());
   #endif

   #if JUCE_INTERNAL_HAS_LADSPA
    manager.addFormat (std::make_unique<LADSPAPluginFormatHeadless>());
   #endif

   #if JUCE_INTERNAL_HAS_LV2
    manager.addFormat (std::make_unique<LV2PluginFormatHeadless>());
   #endif
}

int AudioPluginFormatManager::getNumFormats() const                         { return formats.size(); }
AudioPluginFormat* AudioPluginFormatManager::getFormat (int index) const    { return formats[index]; }

Array<AudioPluginFormat*> AudioPluginFormatManager::getFormats() const
{
    Array<AudioPluginFormat*> a;
    a.addArray (formats);
    return a;
}

void AudioPluginFormatManager::addFormat (std::unique_ptr<AudioPluginFormat> format)
{
    for (auto* existing : formats)
    {
        if (existing->getName() == format->getName())
        {
            // This format manager already contains a format with this name!
            jassertfalse;
            return;
        }
    }

    formats.add (std::move (format));
}

std::unique_ptr<AudioPluginInstance> AudioPluginFormatManager::createPluginInstance (const PluginDescription& description,
                                                                                     double rate, int blockSize,
                                                                                     String& errorMessage) const
{
    if (auto* format = findFormatForDescription (description, errorMessage))
        return format->createInstanceFromDescription (description, rate, blockSize, errorMessage);

    return {};
}

void AudioPluginFormatManager::createARAFactoryAsync (const PluginDescription& description,
                                                      AudioPluginFormat::ARAFactoryCreationCallback callback) const
{
    String errorMessage;

    if (auto* format = findFormatForDescription (description, errorMessage))
    {
        format->createARAFactoryAsync (description, callback);
    }
    else
    {
        errorMessage = NEEDS_TRANS ("Couldn't find format for the provided description");
        callback ({ {}, std::move (errorMessage) });
    }
}

void AudioPluginFormatManager::createPluginInstanceAsync (const PluginDescription& description,
                                                          double initialSampleRate, int initialBufferSize,
                                                          AudioPluginFormat::PluginCreationCallback callback)
{
    String error;

    if (auto* format = findFormatForDescription (description, error))
        return format->createPluginInstanceAsync (description, initialSampleRate, initialBufferSize, std::move (callback));

    struct DeliverError final : public CallbackMessage
    {
        DeliverError (AudioPluginFormat::PluginCreationCallback c, const String& e)
            : call (std::move (c)), error (e)
        {
            post();
        }

        void messageCallback() override          { call (nullptr, error); }

        AudioPluginFormat::PluginCreationCallback call;
        String error;
    };

    new DeliverError (std::move (callback), error);
}

AudioPluginFormat* AudioPluginFormatManager::findFormatForDescription (const PluginDescription& description,
                                                                       String& errorMessage) const
{
    errorMessage = {};

    for (auto* format : formats)
        if (format->getName() == description.pluginFormatName
              && format->fileMightContainThisPluginType (description.fileOrIdentifier))
            return format;

    errorMessage = NEEDS_TRANS ("No compatible plug-in format exists for this plug-in");

    return {};
}

bool AudioPluginFormatManager::doesPluginStillExist (const PluginDescription& description) const
{
    for (auto* format : formats)
        if (format->getName() == description.pluginFormatName)
            return format->doesPluginStillExist (description);

    return false;
}

} // namespace juce
