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

//==============================================================================
class UIAToggleProvider  : public UIAProviderBase,
                           public ComBaseClassHelper<IToggleProvider>
{
public:
    using UIAProviderBase::UIAProviderBase;

    //==============================================================================
    JUCE_COMRESULT Toggle() override
    {
        if (! isElementValid())
            return (HRESULT) UIA_E_ELEMENTNOTAVAILABLE;

        const auto& handler = getHandler();

        if (handler.getActions().invoke (AccessibilityActionType::toggle)
            || handler.getActions().invoke (AccessibilityActionType::press))
        {
            VARIANT newValue;
            VariantHelpers::setInt (getCurrentToggleState(), &newValue);

            sendAccessibilityPropertyChangedEvent (handler, UIA_ToggleToggleStatePropertyId, newValue);

            return S_OK;
        }

        return (HRESULT) UIA_E_NOTSUPPORTED;
    }

    JUCE_COMRESULT get_ToggleState (ToggleState* pRetVal) override
    {
        return withCheckedComArgs (pRetVal, *this, [&]
        {
            *pRetVal = getCurrentToggleState();
            return S_OK;
        });
    }

private:
    ToggleState getCurrentToggleState() const
    {
        return getHandler().getCurrentState().isChecked() ? ToggleState_On
                                                          : ToggleState_Off;
    }

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIAToggleProvider)
};

} // namespace juce
