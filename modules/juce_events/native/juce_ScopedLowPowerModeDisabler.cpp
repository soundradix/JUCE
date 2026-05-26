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

#if JUCE_MAC

class ScopedLowPowerModeDisabler::Pimpl
{
public:
    Pimpl()
    {
        activity = [[NSProcessInfo processInfo] beginActivityWithOptions: NSActivityUserInitiatedAllowingIdleSystemSleep
                                                                  reason: @"App must remain in high-power mode"];
    }

    ~Pimpl()
    {
        [[NSProcessInfo processInfo] endActivity: activity];
    }

private:
    id activity;

    JUCE_DECLARE_NON_COPYABLE (Pimpl)
    JUCE_DECLARE_NON_MOVEABLE (Pimpl)
};

#else

class ScopedLowPowerModeDisabler::Pimpl {};

#endif

//==============================================================================
ScopedLowPowerModeDisabler::ScopedLowPowerModeDisabler()
    : pimpl (std::make_unique<Pimpl>()) {}

ScopedLowPowerModeDisabler::~ScopedLowPowerModeDisabler() = default;

} // namespace juce
