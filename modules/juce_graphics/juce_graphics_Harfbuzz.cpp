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

#include <juce_core/system/juce_CompilerWarnings.h>

JUCE_BEGIN_IGNORE_WARNINGS_MSVC (4100 4127 4189 4244 4245 4265 4267 4309 4310 4312 4456 4457 4458 4459 4701 4702 4706 6001 6011 6239 6244 6246 6262 6297 6313 6319 6326 6336 6385 6386 28251)

JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wcast-function-type",
                                     "-Wsign-conversion",
                                     "-Wzero-as-null-pointer-constant",
                                     "-Wformat-pedantic",
                                     "-Wextra-semi",
                                     "-Wc++98-compat-extra-semi",
                                     "-Wshadow-field",
                                     "-Wfloat-equal",
                                     "-Wformat",
                                     "-Wpedantic",
                                     "-Wmicrosoft-exception-spec",
                                     "-Wmicrosoft-cast",
                                     "-Wconditional-uninitialized",
                                     "-Wexpansion-to-defined",
                                     "-Wunsafe-loop-optimizations",
                                     "-Wformat-overflow",
                                     "-Woverflow",
                                     "-Wnontrivial-memcall",
                                     "-Wimplicit-fallthrough")

JUCE_BEGIN_IGNORE_DEPRECATION_WARNINGS

#define HAVE_ATEXIT 1

#if JUCE_LINUX || JUCE_BSD
 #ifndef JUCE_USE_FREETYPE
  #define JUCE_USE_FREETYPE 1
 #endif
#endif

#if JUCE_USE_FREETYPE
 #define HAVE_FREETYPE 1
#endif

#if JUCE_WINDOWS
 #define HAVE_DIRECTWRITE 1
 #define _CRT_SECURE_NO_WARNINGS 1
#elif JUCE_MAC || JUCE_IOS
 #define HAVE_CORETEXT 1
#endif

// HB enables some warnings in its header, but we would rather build cleanly
#define HB_NO_PRAGMA_GCC_DIAGNOSTIC_WARNING
#define HB_NO_PRAGMA_GCC_DIAGNOSTIC_ERROR

// This is a hack, because harfbuzz headers define hb_has_builtin to
// expand to a macro that includes a 'defines' expression, which triggers
// -Wexpansion-to-defined on gcc 7. There's no way to turn that warning off
// locally, so we sidestep it.
#if ! defined(__has_builtin) && defined(__GNUC__) && __GNUC__ >= 5
 #define __has_builtin(x) 1
#endif

#include <utility>
#include <juce_graphics/fonts/harfbuzz/hb.hh>
#include <juce_graphics/fonts/harfbuzz/harfbuzz.cc>

#undef HAVE_DIRECTWRITE
#undef HAVE_FREETYPE
#undef HAVE_CORETEXT

JUCE_END_IGNORE_DEPRECATION_WARNINGS
JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC
