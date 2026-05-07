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

#pragma once

#if JUCE_MAC

#include <juce_audio_plugin_client/detail/juce_IncludeModuleHeaders.h>

namespace juce::detail
{

struct VSTWindowUtilities
{
    VSTWindowUtilities() = delete;

    static void* attachComponentToWindowRefVST (Component* comp,
                                                int desktopFlags,
                                                void* parentWindowOrView)
    {
        JUCE_AUTORELEASEPOOL
        {
            NSView* parentView = [(NSView*) parentWindowOrView retain];

            const auto defaultFlags = JucePlugin_EditorRequiresKeyboardFocus
                                    ? 0
                                    : ComponentPeer::windowIgnoresKeyPresses;
            comp->addToDesktop (desktopFlags | defaultFlags, parentView);

            // this workaround is because Wavelab provides a zero-size parent view
            if (approximatelyEqual ([parentView frame].size.height, 0.0))
                [((NSView*) comp->getWindowHandle()) setFrameOrigin: NSZeroPoint];

            comp->setVisible (true);
            comp->toFront (false);

            [[parentView window] setAcceptsMouseMovedEvents: YES];
            return parentView;
        }
    }

    static void detachComponentFromWindowRefVST (Component* comp,
                                                void* window)
    {
        JUCE_AUTORELEASEPOOL
        {
            comp->removeFromDesktop();
            [(id) window release];
        }
    }

    static void setNativeHostWindowSizeVST (void* window,
                                            Component* component,
                                            int newWidth,
                                            int newHeight)
    {
        JUCE_AUTORELEASEPOOL
        {
            if (NSView* hostView = (NSView*) window)
            {
                const int dx = newWidth  - component->getWidth();
                const int dy = newHeight - component->getHeight();

                NSRect r = [hostView frame];
                r.size.width += dx;
                r.size.height += dy;
                r.origin.y -= dy;
                [hostView setFrame: r];
            }
        }
    }
};

} // namespace juce::detail

#endif
