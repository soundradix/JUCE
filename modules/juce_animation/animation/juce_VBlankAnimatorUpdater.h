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

/**
    Similar to AnimatorUpdater, but automatically calls update() whenever the screen refreshes.

    @tags{Animations}
*/
class JUCE_API  VBlankAnimatorUpdater : private AnimatorUpdater
{
public:
    /** Constructs a VBlankAnimatorUpdater that is synchronised to the refresh rate of the monitor
        that the provided Component is being displayed on.
    */
    explicit VBlankAnimatorUpdater (Component* c)
        : vBlankAttachment (c, [this] (double timestampSec)
                               {
                                   update (timestampSec * 1000.0);
                               })
    {
    }

    using AnimatorUpdater::addAnimator, AnimatorUpdater::removeAnimator;

private:
    VBlankAttachment vBlankAttachment;
};

} // namespace juce
