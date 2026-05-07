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
void AnimatorUpdater::addAnimator (const Animator& animator)
{
    addAnimator (animator, nullptr);
}

void AnimatorUpdater::addAnimator (const Animator& animator, std::function<void()> onComplete)
{
    Entry entry { animator.makeWeak(), std::move (onComplete) };
    animators[entry.animator.getKey()] = std::move (entry);
}

void AnimatorUpdater::removeAnimator (const Animator& animator)
{
    if (auto it = animators.find (animator.makeWeak().getKey()); it != animators.end())
    {
        if (it == currentIterator)
        {
            ++currentIterator;
            iteratorServiced = false;
        }

        animators.erase (it);
    }
}

void AnimatorUpdater::update()
{
    update (Time::getMillisecondCounterHiRes());
}

void AnimatorUpdater::update (double timestampMs)
{
    if (reentrancyGuard)
    {
        // If this is hit, one of the animators is trying to update itself
        // recursively. This is a bad idea! Inspect the callstack to find the
        // cause of the problem.
        jassertfalse;
        return;
    }

    const ScopedValueSetter setter { reentrancyGuard, true };

    for (currentIterator = animators.begin(); currentIterator != animators.end();)
    {
        auto& current = *currentIterator;

        if (const auto locked = current.second.animator.lock())
        {
            iteratorServiced = true;

            if (locked->update (timestampMs) == Animator::Status::finished)
                NullCheckedInvocation::invoke (current.second.onComplete);

            if (iteratorServiced && currentIterator != animators.end())
                ++currentIterator;
        }
        else
        {
            currentIterator = animators.erase (currentIterator);
        }
    }
}

} // namespace juce
