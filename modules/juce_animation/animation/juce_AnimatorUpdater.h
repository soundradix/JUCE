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
/**
    Helper class to update several animators at once, without owning or otherwise extending the
    lifetimes of those animators.

    The intended use case is to register Animators with an updater as opposed to separately calling
    Animator::update() on each of them. Calling update() then will update all registered Animators.
    In case an Animator's underlying implementation is deleted (all Animator objects that were
    strongly referencing it were deleted) it is automatically removed by the AnimatorUpdater.

    If you want to update all your Animators in sync with the display refresh you will probably want
    to use the VBlankAnimatorUpdater.

    The order in which Animator::update() functions are called for registered Animators is not
    specified, as Animators should be implemented in a way where it doesn't matter.

    @see VBlankAnimatorUpdater

    @tags{Animations}
*/
class JUCE_API  AnimatorUpdater
{
public:
    /** Registers an Animator with the updater.
    */
    void addAnimator (const Animator& animator);

    /** Registers an Animator with the updater and specifies a callback to be called upon the
        completion of the Animator.

        This callback can be used for cleanup purposes e.g.

        @code
        animatorUpdater.addAnimator (someComponentPtr->getAnimator(),
                                     [&someComponentPtr] { someComponentPtr.reset(); });
        @endcode
    */
    void addAnimator (const Animator& animator, std::function<void()> onComplete);

    /** Removes an Animator
    */
    void removeAnimator (const Animator& animator);

    /** Calls Animator::update() for all registered Animators that are still alive. References to
        deleted Animators are removed.

        Uses Time::getMillisecondCounterHiRes() to calculate the necessary timestamp. Consider using
        a VBlankAnimatorUpdater instead for using timestamps that are synchronised across all
        VBlankAnimatorUpdater instances.
    */
    void update();

    /** Calls Animator::update() for all registered Animators that are still alive. References to
        deleted Animators are removed.

        The supplied timestamp should be monotonically increasing for correct behaviour. Ideally
        this should be a timestamp supplied by a VBlankAttachment. Consider using the
        VBlankAnimatorUpdater class, which takes care of supplying the right timestamp.

        @see VBlankAnimatorUpdater
    */
    void update (double timestampMs);

private:
    struct JUCE_API  Entry
    {
        Entry() = default;

        Entry(Animator::Weak animatorIn, std::function<void()> onCompleteIn)
            : animator (animatorIn),
              onComplete (std::move (onCompleteIn))
        {}

        Animator::Weak animator;
        std::function<void()> onComplete;
    };

    std::map<void*, Entry> animators;
    std::map<void*, Entry>::iterator currentIterator;

    bool iteratorServiced = false;
    bool reentrancyGuard = false;
};

} // namespace juce
