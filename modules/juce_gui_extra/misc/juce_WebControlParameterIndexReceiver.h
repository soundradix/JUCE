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

#if JUCE_WEB_BROWSER || DOXYGEN

/** This is a helper class for implementing AudioProcessorEditor::getControlParameterIndex with GUIs
    using a WebBrowserComponent.

    Create an instance of this class and attach it to the WebBrowserComponent by using
    WebBrowserComponent::Options::withOptionsFrom.

    In your frontend code you can use the ControlParameterIndexUpdater class, that emits
    controlParameterIndexChanged events based on the mouse movement, and control parameter index
    annotations attached to DOM elements.

    @tags{GUI}
*/
class JUCE_API  WebControlParameterIndexReceiver : public OptionsBuilder<WebBrowserComponent::Options>
{
public:
    /*  Returns the control parameter index last reported by the WebBrowserComponent GUI to be
        active.
    */
    int getControlParameterIndex() const { return controlParameterIndex; }

    //==============================================================================
    WebBrowserComponent::Options buildOptions (const WebBrowserComponent::Options& initialOptions) override
    {
        return initialOptions.withEventListener ("__juce__controlParameterIndexChanged",
                                                 [this] (auto newIndex)
                                                 {
                                                     controlParameterIndex = (int) newIndex;
                                                 });
    }

private:
    int controlParameterIndex = -1;
};

#endif

}
