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

namespace juce::detail
{

String AccessibilityHelpers::getApplicationOrPluginName()
{
   #if defined (JucePlugin_Name)
    return JucePlugin_Name;
   #else
    if (auto* app = JUCEApplicationBase::getInstance())
        return app->getApplicationName();

    return "JUCE Application";
   #endif
}

#if ! JUCE_NATIVE_ACCESSIBILITY_INCLUDED
 void AccessibilityHelpers::notifyAccessibilityEvent (const AccessibilityHandler&, Event) {}
#endif

} // namespace juce::detail
