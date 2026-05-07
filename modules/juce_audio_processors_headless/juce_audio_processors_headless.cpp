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

#ifdef JUCE_AUDIO_PROCESSORS_HEADLESS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

#define JUCE_CORE_INCLUDE_NATIVE_HEADERS 1
#define JUCE_CORE_INCLUDE_OBJC_HELPERS 1

#include "juce_audio_processors_headless.h"

#include <juce_audio_processors_headless/processors/juce_AudioProcessorListener.cpp>
#include <juce_audio_processors_headless/utilities/juce_AAXClientExtensions.cpp>
#include <juce_audio_processors_headless/utilities/juce_VST2ClientExtensions.cpp>
#include <juce_audio_processors_headless/utilities/juce_VST3ClientExtensions.cpp>
#include <juce_audio_processors_headless/processors/juce_AudioProcessorParameter.cpp>
#include <juce_audio_processors_headless/processors/juce_AudioProcessorParameterGroup.cpp>
#include <juce_audio_processors_headless/processors/juce_AudioProcessor.cpp>
#include <juce_audio_processors_headless/processors/juce_PluginDescription.cpp>
#include <juce_audio_processors_headless/processors/juce_AudioPluginInstance.cpp>
#include <juce_audio_processors_headless/processors/juce_AudioProcessorGraph.cpp>
#include <juce_audio_processors_headless/format/juce_AudioPluginFormat.cpp>
#include <juce_audio_processors_headless/utilities/juce_AudioProcessorParameterWithID.cpp>
#include <juce_audio_processors_headless/utilities/juce_RangedAudioParameter.cpp>
#include <juce_audio_processors_headless/utilities/juce_AudioParameterFloat.cpp>
#include <juce_audio_processors_headless/utilities/juce_AudioParameterInt.cpp>
#include <juce_audio_processors_headless/utilities/juce_AudioParameterBool.cpp>
#include <juce_audio_processors_headless/utilities/juce_AudioParameterChoice.cpp>
#include <juce_audio_processors_headless/utilities/ARA/juce_ARA_utils.cpp>
#include <juce_audio_processors_headless/format_types/juce_AudioUnitPluginFormatHeadless.mm>
#include <juce_audio_processors_headless/format_types/juce_LADSPAPluginFormatHeadless.cpp>
#include <juce_audio_processors_headless/format_types/juce_LV2PluginFormatHeadless.cpp>
#include <juce_audio_processors_headless/format_types/juce_VST3PluginFormatHeadless.cpp>
#include <juce_audio_processors_headless/format_types/juce_VSTPluginFormatHeadless.cpp>
#include <juce_audio_processors_headless/format_types/juce_ARAHosting.cpp>
#include <juce_audio_processors_headless/format/juce_AudioPluginFormatManager.cpp>
