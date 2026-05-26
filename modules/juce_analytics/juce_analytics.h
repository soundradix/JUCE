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

  ID:                 juce_analytics
  vendor:             juce
  version:            8.0.13
  name:               JUCE analytics classes
  description:        Classes to collect analytics and send to destinations
  website:            http://www.juce.com/juce
  license:            AGPLv3/Commercial
  minimumCppStandard: 17

  dependencies:       juce_gui_basics

 END_JUCE_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define JUCE_ANALYTICS_H_INCLUDED

#include <juce_gui_basics/juce_gui_basics.h>

#include "destinations/juce_AnalyticsDestination.h"
#include "destinations/juce_ThreadedAnalyticsDestination.h"
#include "analytics/juce_Analytics.h"
#include "analytics/juce_ButtonTracker.h"
