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

class AccessibilityNativeHandle  : public ComBaseClassHelper<IRawElementProviderSimple,
                                                             IRawElementProviderFragment,
                                                             IRawElementProviderFragmentRoot,
                                                             IRawElementProviderHwndOverride>
{
public:
    explicit AccessibilityNativeHandle (AccessibilityHandler& handler);

    //==============================================================================
    void invalidateElement() noexcept         { valid = false; }
    bool isElementValid() const noexcept      { return valid; }

    const AccessibilityHandler& getHandler()  { return accessibilityHandler; }

    //==============================================================================
    JUCE_COMRESULT QueryInterface (REFIID refId, void** result) override;

    //==============================================================================
    JUCE_COMRESULT get_HostRawElementProvider (IRawElementProviderSimple** provider) override;
    JUCE_COMRESULT get_ProviderOptions (ProviderOptions* options) override;
    JUCE_COMRESULT GetPatternProvider (PATTERNID pId, IUnknown** provider) override;
    JUCE_COMRESULT GetPropertyValue (PROPERTYID propertyId, VARIANT* pRetVal) override;

    JUCE_COMRESULT Navigate (NavigateDirection direction, IRawElementProviderFragment** pRetVal) override;
    JUCE_COMRESULT GetRuntimeId (SAFEARRAY** pRetVal) override;
    JUCE_COMRESULT get_BoundingRectangle (UiaRect* pRetVal) override;
    JUCE_COMRESULT GetEmbeddedFragmentRoots (SAFEARRAY** pRetVal) override;
    JUCE_COMRESULT SetFocus() override;
    JUCE_COMRESULT get_FragmentRoot (IRawElementProviderFragmentRoot** pRetVal) override;

    JUCE_COMRESULT ElementProviderFromPoint (double x, double y, IRawElementProviderFragment** pRetVal) override;
    JUCE_COMRESULT GetFocus (IRawElementProviderFragment** pRetVal) override;

    JUCE_COMRESULT GetOverrideProviderForHwnd (HWND hwnd, IRawElementProviderSimple** pRetVal) override;

private:
    //==============================================================================
    String getElementName() const;
    bool isFragmentRoot() const     { return accessibilityHandler.getComponent().isOnDesktop(); }

    //==============================================================================
    AccessibilityHandler& accessibilityHandler;

    static int idCounter;
    std::array<int, 2> rtid { UiaAppendRuntimeId, ++idCounter };
    bool valid = true;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AccessibilityNativeHandle)
};

}
