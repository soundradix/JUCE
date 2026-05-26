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

#ifdef JUCER_ENABLE_GPL_MODE
 #warning The flag JUCER_ENABLE_GPL_MODE has been removed
#endif

#include "jucer_Headers.h"

#include "jucer_Application.h"
#include "../CodeEditor/jucer_OpenDocumentManager.h"
#include "../CodeEditor/jucer_SourceCodeEditor.h"
#include "../Utility/UI/PropertyComponents/jucer_FilePathPropertyComponent.h"
#include "../Project/UI/jucer_ProjectContentComponent.h"
#include "../Project/UI/Sidebar/jucer_TreeItemTypes.h"
#include "Windows/jucer_UTF8WindowComponent.h"
#include "Windows/jucer_SVGPathDataWindowComponent.h"
#include "Windows/jucer_AboutWindowComponent.h"
#include "Windows/jucer_EditorColourSchemeWindowComponent.h"
#include "Windows/jucer_GlobalPathsWindowComponent.h"
#include "Windows/jucer_PIPCreatorWindowComponent.h"
#include "Windows/jucer_FloatingToolWindow.h"

#include "jucer_CommandLine.h"

#include "../Project/UI/jucer_ProjectContentComponent.cpp"
#include "jucer_Application.cpp"


START_JUCE_APPLICATION (ProjucerApplication)
