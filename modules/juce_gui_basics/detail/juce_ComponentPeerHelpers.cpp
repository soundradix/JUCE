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

#if JUCE_MAC

namespace juce::detail
{

bool ComponentPeerHelpers::isInPerformKeyEquivalent (const ComponentPeer& peer)
{
    jassert (dynamic_cast<const NSViewComponentPeer*> (&peer) != nullptr);
    return static_cast<const NSViewComponentPeer*> (&peer)->inPerformKeyEquivalent;
}

} // namespace juce::detail

#endif
