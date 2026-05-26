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

template <typename... Ts>
constexpr bool isValueOrLvalueReferenceToConst()
{
    return ((   (! std::is_reference_v<Ts>)
             || (std::is_lvalue_reference_v<Ts> && std::is_const_v<std::remove_reference_t<Ts>>)) && ...);
}

template <typename... Args>
class CallbackListenerList
{
public:
    static_assert (isValueOrLvalueReferenceToConst<Args...>(),
                   "CallbackListenerList can only forward values or const lvalue references");

    using Callback = std::function<void (Args...)>;

    ErasedScopeGuard addListener (Callback callback)
    {
        jassert (callback != nullptr);

        const auto it = callbacks.insert (callbacks.end(), std::move (callback));
        listeners.add (&*it);

        return ErasedScopeGuard { [this, it]
        {
            listeners.remove (&*it);
            callbacks.erase (it);
        } };
    }

    void call (Args... args)
    {
        listeners.call ([&] (auto& l) { l (std::forward<Args> (args)...); });
    }

private:
    std::list<Callback> callbacks;
    LightweightListenerList<Callback> listeners;
};

} // namespace juce::detail
