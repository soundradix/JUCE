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

namespace juce::midi_ci
{

//==============================================================================
/**
    Matches a subscription ID to a resource name.

    @tags{Audio}
*/
struct Subscription
{
    String subscribeId;
    String resource;

    bool operator<  (const Subscription& other) const { return subscribeId <  other.subscribeId; }
    bool operator<= (const Subscription& other) const { return subscribeId <= other.subscribeId; }
    bool operator>  (const Subscription& other) const { return subscribeId >  other.subscribeId; }
    bool operator>= (const Subscription& other) const { return subscribeId >= other.subscribeId; }

    bool operator== (const Subscription& other) const;
    bool operator!= (const Subscription& other) const { return ! operator== (other); }
};

} // namespace juce::midi_ci
