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

#if JUCE_WINDOWS

namespace juce::detail
{

class WindowsHooks::Hooks
{
public:
    Hooks() = default;

    ~Hooks()
    {
        if (keyboardHook != nullptr)
            UnhookWindowsHookEx (keyboardHook);
    }

    static inline std::weak_ptr<Hooks> weak;

private:
    static LRESULT CALLBACK keyboardHookCallback (int nCode, WPARAM wParam, LPARAM lParam)
    {
        auto& msg = *reinterpret_cast<MSG*> (lParam);

        if (nCode == HC_ACTION && wParam == PM_REMOVE && HWNDComponentPeer::offerKeyMessageToJUCEWindow (msg))
        {
            msg = {};
            msg.message = WM_USER;
            return 0;
        }

        return CallNextHookEx (getSingleton()->keyboardHook, nCode, wParam, lParam);
    }

    HHOOK keyboardHook = SetWindowsHookEx (WH_GETMESSAGE,
                                           keyboardHookCallback,
                                           (HINSTANCE) juce::Process::getCurrentModuleInstanceHandle(),
                                           GetCurrentThreadId());
};

auto WindowsHooks::getSingleton() -> std::shared_ptr<Hooks>
{
    auto& weak = Hooks::weak;

    if (auto locked = weak.lock())
        return locked;

    auto strong = std::make_shared<Hooks>();
    weak = strong;
    return strong;
}

} // namespace juce::detail

#endif
