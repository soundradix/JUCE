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
void ComponentDragger::startDraggingComponent (Component* const componentToDrag, const MouseEvent& e)
{
    jassert (componentToDrag != nullptr);
    jassert (e.mods.isAnyMouseButtonDown()); // The event has to be a drag event!

    if (componentToDrag != nullptr)
        mouseDownWithinTarget = e.getEventRelativeTo (componentToDrag).getMouseDownPosition();
}

void ComponentDragger::dragComponent (Component* const componentToDrag, const MouseEvent& e,
                                      ComponentBoundsConstrainer* const constrainer)
{
    jassert (componentToDrag != nullptr);
    jassert (e.mods.isAnyMouseButtonDown()); // The event has to be a drag event!

    if (componentToDrag != nullptr)
    {
        const auto bounds = componentToDrag->getBounds();

        const auto setBounds = [&] (auto b)
        {
            if (constrainer != nullptr)
                constrainer->setBoundsForComponent (componentToDrag, b, false, false, false, false);
            else
                componentToDrag->setBounds (b);
        };

        if (auto* peer = componentToDrag->isOnDesktop() ? componentToDrag->getPeer() : nullptr)
        {
            // If the component is a window, multiple mouse events can get queued while it's in the same position,
            // so their coordinates become wrong after the first one moves the window, so in that case, we'll use
            // the current mouse position instead of the one that the event contains...

            const auto globalMouseDown = componentToDrag->localPointToGlobal (mouseDownWithinTarget.toFloat());
            const auto peerSpaceMouseDown = peer->globalToLocal (detail::ScalingHelpers::scaledScreenPosToUnscaled (globalMouseDown));
            const auto [multimonitor, logical] = detail::ComponentHelpers::getTopLeftForPeer (*peer, e.source.getScreenPosition(), peerSpaceMouseDown);
            const auto scope = peer->setMultimonitorPositionOverride (multimonitor.roundToInt());
            setBounds (bounds.withPosition (logical.roundToInt()));
        }
        else
        {
            setBounds (bounds + (e.getEventRelativeTo (componentToDrag).getPosition() - mouseDownWithinTarget));
        }
    }
}

} // namespace juce
