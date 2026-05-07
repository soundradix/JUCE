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

#ifdef JUCE_JAVASCRIPT_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

#include "juce_javascript.h"

#ifdef CONFIG_BIGNUM
 JUCE_COMPILER_WARNING ("The QuickJS version embedded inside of JUCE does not support the CONFIG_BIGNUM setting.")
 #undef CONFIG_BIGNUM
#endif

#define CHOC_QUICKJS_NO_STACK_CHECK 1

#define choc juce::detail::choc
#include <juce_javascript/choc/javascript/choc_javascript_QuickJS.h>
#undef choc

#include "detail/juce_QuickJSHelpers.h"

#include "javascript/juce_JSObject.cpp"
#include "javascript/juce_JSCursor.cpp"
#include "javascript/juce_JavascriptEngine.cpp"

#if JUCE_UNIT_TESTS
 #include "javascript/juce_Javascript_test.cpp"
#endif
