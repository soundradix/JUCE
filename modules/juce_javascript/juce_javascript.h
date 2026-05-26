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


/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the Projucer to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 JUCE Module Format.md file.


 BEGIN_JUCE_MODULE_DECLARATION

  ID:                 juce_javascript
  vendor:             juce
  version:            8.0.13
  name:               JUCE Javascript interpreter
  description:        Javascript interpreter.
  website:            http://www.juce.com/juce
  license:            AGPLv3/Commercial
  minimumCppStandard: 17

  dependencies:       juce_core

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define JUCE_JAVASCRIPT_H_INCLUDED

#include <juce_core/juce_core.h>

#include "javascript/juce_JSObject.h"
#include "javascript/juce_JSCursor.h"
#include "javascript/juce_JavascriptEngine.h"
