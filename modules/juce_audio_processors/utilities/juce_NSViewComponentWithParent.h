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

namespace juce
{

/*  This is an NSViewComponent which holds a long-lived NSView which acts
    as the parent view for plugin editors.

    Note that this component does not auto-resize depending on the bounds
    of the owned view. VST2 and VST3 plugins have dedicated interfaces to
    request that the editor bounds are updated. We can call `setSize` on this
    component from inside those dedicated callbacks.
*/
struct NSViewComponentWithParent : public NSViewComponent,
                                   private AsyncUpdater
{
    enum class WantsNudge { no, yes };

    explicit NSViewComponentWithParent (WantsNudge shouldNudge)
        : wantsNudge (shouldNudge)
    {
        auto* view = [[getViewClass().createInstance() init] autorelease];
        object_setInstanceVariable (view, "owner", this);
        setView (view);
    }

    explicit NSViewComponentWithParent (AudioPluginInstance& instance)
        : NSViewComponentWithParent (getWantsNudge (instance)) {}

    ~NSViewComponentWithParent() override
    {
        if (auto* view = static_cast<NSView*> (getView()))
            object_setInstanceVariable (view, "owner", nullptr);

        cancelPendingUpdate();
    }

    JUCE_DECLARE_NON_COPYABLE (NSViewComponentWithParent)
    JUCE_DECLARE_NON_MOVEABLE (NSViewComponentWithParent)

private:
    WantsNudge wantsNudge = WantsNudge::no;

    static WantsNudge getWantsNudge (AudioPluginInstance& instance)
    {
        PluginDescription pd;
        instance.fillInPluginDescription (pd);
        return pd.manufacturerName == "FabFilter" ? WantsNudge::yes : WantsNudge::no;
    }

    void handleAsyncUpdate() override
    {
        if (auto* peer = getTopLevelComponent()->getPeer())
        {
            auto* view = static_cast<NSView*> (getView());
            const auto newArea = peer->getAreaCoveredBy (*this);
            [view setFrame: makeCGRect (newArea.withHeight (newArea.getHeight() + 1))];
            [view setFrame: makeCGRect (newArea)];
        }
    }

    struct InnerNSView final : public ObjCClass<NSView>
    {
        InnerNSView()
            : ObjCClass ("JuceInnerNSView_")
        {
            addIvar<NSViewComponentWithParent*> ("owner");

            addMethod (@selector (isOpaque), [] (id, SEL) { return YES; });

            addMethod (@selector (didAddSubview:), [] (id self, SEL, NSView*)
            {
                if (auto* owner = getIvar<NSViewComponentWithParent*> (self, "owner"))
                    if (owner->wantsNudge == WantsNudge::yes)
                        owner->triggerAsyncUpdate();
            });

            JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wundeclared-selector")
            addMethod (@selector (clipsToBounds), [] (id, SEL) { return YES; });
            JUCE_END_IGNORE_WARNINGS_GCC_LIKE

            registerClass();
        }
    };

    static InnerNSView& getViewClass()
    {
        static InnerNSView result;
        return result;
    }
};

} // namespace juce

#endif
