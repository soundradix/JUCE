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

#if JUCE_WINDOWS || DOXYGEN

//==============================================================================
/**
    Audio format which uses the Windows Media codecs (Windows only).

    @tags{Audio}
*/
class WindowsMediaAudioFormat  : public AudioFormat
{
public:
    //==============================================================================
    WindowsMediaAudioFormat();
    ~WindowsMediaAudioFormat() override;

    //==============================================================================
    Array<int> getPossibleSampleRates() override;
    Array<int> getPossibleBitDepths() override;
    bool canDoStereo() override;
    bool canDoMono() override;
    bool isCompressed() override;

    //==============================================================================
    AudioFormatReader* createReaderFor (InputStream*, bool deleteStreamIfOpeningFails) override;

    std::unique_ptr<AudioFormatWriter> createWriterFor (std::unique_ptr<OutputStream>& streamToWriteTo,
                                                        const AudioFormatWriterOptions& options) override;

    using AudioFormat::createWriterFor;
};

#endif

}
