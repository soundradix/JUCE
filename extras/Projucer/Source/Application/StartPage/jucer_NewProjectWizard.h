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

#include "jucer_NewProjectTemplates.h"

//==============================================================================
namespace NewProjectWizard
{
    File getLastWizardFolder();

    ScopedMessageBox createNewProject (const NewProjectTemplates::ProjectTemplate& projectTemplate,
                                       const File& targetFolder, const String& name, var modules, var exporters, var fileOptions,
                                       const String& modulePath, bool useGlobalModulePath,
                                       std::function<void (ScopedMessageBox, std::unique_ptr<Project>)> callback);
}
