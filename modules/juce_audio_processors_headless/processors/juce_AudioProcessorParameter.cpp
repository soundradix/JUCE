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

AudioProcessorParameter::~AudioProcessorParameter()
{
   #if JUCE_DEBUG && ! JUCE_DISABLE_AUDIOPROCESSOR_BEGIN_END_GESTURE_CHECKING
    // This will fail if you've called beginChangeGesture() without having made
    // a corresponding call to endChangeGesture...
    jassert (! isPerformingGesture);
   #endif
}

void AudioProcessorParameter::setParameterIndex (int index) noexcept
{
    jassert (parameterIndex < 0 && 0 <= index);
    parameterIndex = index;
}

void AudioProcessorParameter::setOwner (Listener* listenerIn) noexcept
{
    jassert (finalListener == nullptr);
    finalListener = listenerIn;
}

void AudioProcessorParameter::setValueNotifyingHost (float newValue)
{
    setValue (newValue);
    sendValueChangedMessageToListeners (newValue);
}

void AudioProcessorParameter::beginChangeGesture()
{
    // This method can't be used until the parameter has been attached to a processor!
    jassert (parameterIndex >= 0);

   #if JUCE_DEBUG && ! JUCE_DISABLE_AUDIOPROCESSOR_BEGIN_END_GESTURE_CHECKING
    // This means you've called beginChangeGesture twice in succession without
    // a matching call to endChangeGesture. That might be fine in most hosts,
    // but it would be better to avoid doing it.
    jassert (! isPerformingGesture);
    isPerformingGesture = true;
   #endif

    ScopedLock lock (listenerLock);

    for (int i = listeners.size(); --i >= 0;)
        if (auto* l = listeners[i])
            l->parameterGestureChanged (getParameterIndex(), true);

    if (finalListener != nullptr)
        finalListener->parameterGestureChanged (getParameterIndex(), true);
}

void AudioProcessorParameter::endChangeGesture()
{
    // This method can't be used until the parameter has been attached to a processor!
    jassert (parameterIndex >= 0);

   #if JUCE_DEBUG && ! JUCE_DISABLE_AUDIOPROCESSOR_BEGIN_END_GESTURE_CHECKING
    // This means you've called endChangeGesture without having previously
    // called beginChangeGesture. That might be fine in most hosts, but it
    // would be better to keep the calls matched correctly.
    jassert (isPerformingGesture);
    isPerformingGesture = false;
   #endif

    ScopedLock lock (listenerLock);

    for (int i = listeners.size(); --i >= 0;)
        if (auto* l = listeners[i])
            l->parameterGestureChanged (getParameterIndex(), false);

    if (finalListener != nullptr)
        finalListener->parameterGestureChanged (getParameterIndex(), false);
}

void AudioProcessorParameter::sendValueChangedMessageToListeners (float newValue)
{
    ScopedLock lock (listenerLock);

    for (int i = listeners.size(); --i >= 0;)
        if (auto* l = listeners [i])
            l->parameterValueChanged (getParameterIndex(), newValue);

    if (finalListener != nullptr)
        finalListener->parameterValueChanged (getParameterIndex(), newValue);
}

bool AudioProcessorParameter::isOrientationInverted() const                      { return false; }
bool AudioProcessorParameter::isAutomatable() const                              { return true; }
bool AudioProcessorParameter::isMetaParameter() const                            { return false; }
AudioProcessorParameter::Category AudioProcessorParameter::getCategory() const   { return genericParameter; }
int AudioProcessorParameter::getNumSteps() const                                 { return getDefaultNumParameterSteps(); }
bool AudioProcessorParameter::isDiscrete() const                                 { return false; }
bool AudioProcessorParameter::isBoolean() const                                  { return false; }

String AudioProcessorParameter::getText (float value, int /*maximumStringLength*/) const
{
    return String (value, 2);
}

String AudioProcessorParameter::getCurrentValueAsText() const
{
    return getText (getValue(), 1024);
}

StringArray AudioProcessorParameter::getAllValueStrings() const
{
    if (isDiscrete() && valueStrings.isEmpty())
    {
        auto maxIndex = getNumSteps() - 1;

        for (int i = 0; i < getNumSteps(); ++i)
            valueStrings.add (getText ((float) i / (float) maxIndex, 1024));
    }

    return valueStrings;
}

void AudioProcessorParameter::addListener (AudioProcessorParameter::Listener* newListener)
{
    const ScopedLock sl (listenerLock);
    listeners.addIfNotAlreadyThere (newListener);
}

void AudioProcessorParameter::removeListener (AudioProcessorParameter::Listener* listenerToRemove)
{
    const ScopedLock sl (listenerLock);
    listeners.removeFirstMatchingValue (listenerToRemove);
}

int AudioProcessorParameter::getDefaultNumParameterSteps() noexcept
{
    return 0x7fffffff;
}

} // namespace juce
