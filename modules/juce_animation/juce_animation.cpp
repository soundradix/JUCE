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

#ifdef JUCE_ANIMATION_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

//==============================================================================
#include "juce_animation.h"

//==============================================================================
namespace chromium
{

JUCE_BEGIN_IGNORE_WARNINGS_MSVC (4701 6001)
JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wfloat-conversion",
                                     "-Wfloat-equal",
                                     "-Wconditional-uninitialized")


#include "detail/chromium/cubic_bezier.h"
#include "detail/chromium/cubic_bezier.cc"

JUCE_END_IGNORE_WARNINGS_MSVC
JUCE_END_IGNORE_WARNINGS_GCC_LIKE

} // namespace chromium

//==============================================================================
#include "animation/juce_Animator.cpp"
#include "animation/juce_AnimatorSetBuilder.cpp"
#include "animation/juce_AnimatorUpdater.cpp"
#include "animation/juce_Easings.cpp"
#include "animation/juce_ValueAnimatorBuilder.cpp"
