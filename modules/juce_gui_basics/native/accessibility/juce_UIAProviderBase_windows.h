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
class UIAProviderBase
{
public:
    explicit UIAProviderBase (AccessibilityNativeHandle* nativeHandleIn)
        : nativeHandle (nativeHandleIn, IncrementRef::yes)
    {
    }

    bool isElementValid() const
    {
        if (nativeHandle != nullptr)
            return nativeHandle->isElementValid();

        return false;
    }

    const AccessibilityHandler& getHandler() const
    {
        return nativeHandle->getHandler();
    }

private:
    ComSmartPtr<AccessibilityNativeHandle> nativeHandle;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UIAProviderBase)
};

} // namespace juce
