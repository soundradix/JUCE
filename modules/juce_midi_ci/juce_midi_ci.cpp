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

#ifdef JUCE_MIDI_CI_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

#include "juce_midi_ci.h"

#include <juce_midi_ci/detail/juce_CIMessageMeta.h>
#include <juce_midi_ci/detail/juce_CIMarshalling.h>
#include <juce_midi_ci/detail/juce_CIPropertyDataMessageChunker.h>
#include <juce_midi_ci/detail/juce_CIResponder.h>
#include <juce_midi_ci/detail/juce_CIMessageTypeUtils.h>
#include <juce_midi_ci/detail/juce_CIPropertyHostUtils.h>

#include <juce_midi_ci/detail/juce_CIPropertyDataMessageChunker.cpp>
#include <juce_midi_ci/detail/juce_CIResponder.cpp>

#include <juce_midi_ci/ci/juce_CIChannelAddress.cpp>
#include <juce_midi_ci/ci/juce_CIDevice.cpp>
#include <juce_midi_ci/ci/juce_CIEncodings.cpp>
#include <juce_midi_ci/ci/juce_CIMessages.cpp>
#include <juce_midi_ci/ci/juce_CIParser.cpp>
#include <juce_midi_ci/ci/juce_CIProfileAtAddress.cpp>
#include <juce_midi_ci/ci/juce_CIProfileHost.cpp>
#include <juce_midi_ci/ci/juce_CIProfileStates.cpp>
#include <juce_midi_ci/ci/juce_CIPropertyDelegate.cpp>
#include <juce_midi_ci/ci/juce_CIPropertyExchangeCache.cpp>
#include <juce_midi_ci/ci/juce_CIPropertyHost.cpp>
#include <juce_midi_ci/ci/juce_CIResponderOutput.cpp>
#include <juce_midi_ci/ci/juce_CISubscription.cpp>
#include <juce_midi_ci/ci/juce_CISubscriptionManager.cpp>
