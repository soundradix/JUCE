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

AudioFormat::AudioFormat (String name, StringArray extensions)
   : formatName (name), fileExtensions (extensions)
{
}

AudioFormat::AudioFormat (StringRef name, StringRef extensions)
   : formatName (name.text), fileExtensions (StringArray::fromTokens (extensions, false))
{
}

AudioFormat::~AudioFormat()
{
}

bool AudioFormat::canHandleFile (const File& f)
{
    for (auto& e : getFileExtensions())
        if (f.hasFileExtension (e))
            return true;

    return false;
}

const String& AudioFormat::getFormatName() const                { return formatName; }
StringArray AudioFormat::getFileExtensions() const              { return fileExtensions; }
bool AudioFormat::isCompressed()                                { return false; }
StringArray AudioFormat::getQualityOptions()                    { return {}; }

MemoryMappedAudioFormatReader* AudioFormat::createMemoryMappedReader (const File&)
{
    return nullptr;
}

MemoryMappedAudioFormatReader* AudioFormat::createMemoryMappedReader (FileInputStream* fin)
{
    delete fin;
    return nullptr;
}

bool AudioFormat::isChannelLayoutSupported (const AudioChannelSet& channelSet)
{
    if (channelSet == AudioChannelSet::mono())      return canDoMono();
    if (channelSet == AudioChannelSet::stereo())    return canDoStereo();

    return false;
}

using StringMap = std::unordered_map<String, String>;

static StringMap toMap (const StringPairArray& array)
{
    StringMap result;

    for (auto i = 0; i < array.size(); ++i)
        result[array.getAllKeys()[i]] = array.getAllValues()[i];

    return result;
}

AudioFormatWriter* AudioFormat::createWriterForRawPtr (OutputStream* streamToWriteTo,
                                                       const AudioFormatWriterOptions& opt)
{
    auto owned = rawToUniquePtr (streamToWriteTo);

    if (auto writer = createWriterFor (owned, opt))
    {
        // Creating the writer succeeded, so it's the writer's responsibility to eventually free
        // the stream
        jassert (owned == nullptr);
        return writer.release();
    }

    // Creating the writer failed, so the stream should remain alive for re-use
    jassert (owned != nullptr);
    owned.release();

    return {};
}

AudioFormatWriter* AudioFormat::createWriterFor (OutputStream* streamToWriteTo,
                                                 double sampleRateToUse,
                                                 unsigned int numberOfChannels,
                                                 int bitsPerSample,
                                                 const StringPairArray& metadataValues,
                                                 int qualityOptionIndex)
{
    auto opt = AudioFormatWriter::Options{}.withSampleRate (sampleRateToUse)
                                           .withNumChannels ((int) numberOfChannels)
                                           .withBitsPerSample (bitsPerSample)
                                           .withMetadataValues (toMap (metadataValues))
                                           .withQualityOptionIndex (qualityOptionIndex);
    return createWriterForRawPtr (streamToWriteTo, opt);
}

AudioFormatWriter* AudioFormat::createWriterFor (OutputStream* streamToWriteTo,
                                                 double sampleRateToUse,
                                                 const AudioChannelSet& channelLayout,
                                                 int bitsPerSample,
                                                 const StringPairArray& metadataValues,
                                                 int qualityOptionIndex)
{
    if (! isChannelLayoutSupported (channelLayout))
        return nullptr;

    auto opt = AudioFormatWriter::Options{}.withSampleRate (sampleRateToUse)
                                           .withChannelLayout (channelLayout)
                                           .withBitsPerSample (bitsPerSample)
                                           .withMetadataValues (toMap (metadataValues))
                                           .withQualityOptionIndex (qualityOptionIndex);
    return createWriterForRawPtr (streamToWriteTo, opt);
}

} // namespace juce
