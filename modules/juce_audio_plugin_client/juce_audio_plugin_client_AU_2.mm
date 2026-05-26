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

#include <juce_core/system/juce_TargetPlatform.h>

#if JucePlugin_Build_AU

#include <juce_core/system/juce_CompilerWarnings.h>

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wambiguous-reversed-operator",
                                     "-Wc99-extensions",
                                     "-Wcast-align",
                                     "-Wcomment",
                                     "-Wconversion",
                                     "-Wdeprecated-anon-enum-enum-conversion",
                                     "-Wextra-semi",
                                     "-Wextra-tokens",
                                     "-Wfloat-equal",
                                     "-Wformat-pedantic",
                                     "-Wfour-char-constants",
                                     "-Wgnu-zero-variadic-macro-arguments",
                                     "-Wignored-qualifiers",
                                     "-Wimplicit-fallthrough",
                                     "-Wmissing-prototypes",
                                     "-Wmissing-designated-field-initializers",
                                     "-Wnullable-to-nonnull-conversion",
                                     "-Wparentheses",
                                     "-Wshadow-all",
                                     "-Wswitch-enum",
                                     "-Wunknown-attributes",
                                     "-Wunused",
                                     "-Wunused-parameter",
                                     "-Wzero-as-null-pointer-constant")

// From MacOS 10.13 and iOS 11 Apple has (sensibly!) stopped defining a whole
// set of functions with rather generic names. However, we still need a couple
// of them to compile the files below.
#ifndef verify
 #define verify(assertion) __Verify(assertion)
#endif
#ifndef verify_noerr
 #define verify_noerr(errorCode)  __Verify_noErr(errorCode)
#endif

#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUBase.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUBuffer.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUBufferAllocator.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUEffectBase.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUInputElement.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUMIDIBase.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUMIDIEffectBase.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUOutputElement.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUPlugInDispatch.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/AUScopeElement.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/ComponentBase.cpp>
#include <juce_audio_plugin_client/AU/AudioUnitSDK/MusicDeviceBase.cpp>

#undef verify
#undef verify_noerr

JUCE_END_IGNORE_WARNINGS_GCC_LIKE

#endif
