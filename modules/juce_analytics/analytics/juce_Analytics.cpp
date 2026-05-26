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

void Analytics::addDestination (AnalyticsDestination* destination)
{
    destinations.add (destination);
}

OwnedArray<AnalyticsDestination>& Analytics::getDestinations()
{
    return destinations;
}

void Analytics::setUserId (String newUserId)
{
    userId = newUserId;
}

void Analytics::setUserProperties (StringPairArray properties)
{
    userProperties = properties;
}

void Analytics::logEvent (const String& eventName,
                          const StringPairArray& parameters,
                          int eventType)
{
    if (! isSuspended)
    {
        AnalyticsDestination::AnalyticsEvent event
        {
            eventName,
            eventType,
            Time::getMillisecondCounter(),
            parameters,
            userId,
            userProperties
        };

        for (auto* destination : destinations)
            destination->logEvent (event);
    }
}

void Analytics::setSuspended (bool shouldBeSuspended)
{
    isSuspended = shouldBeSuspended;
}

}
