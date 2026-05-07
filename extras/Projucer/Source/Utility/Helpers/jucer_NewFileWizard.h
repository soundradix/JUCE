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

#include "../../Project/jucer_Project.h"

//==============================================================================
class NewFileWizard
{
public:
    //==============================================================================
    NewFileWizard();
    ~NewFileWizard();

    //==============================================================================
    class Type
    {
    public:
        virtual ~Type() = default;

        //==============================================================================
        virtual String getName() = 0;
        virtual void createNewFile (Project&, Project::Item projectGroupToAddTo) = 0;

        void showFailedToWriteMessage (const File& file);

    protected:
        //==============================================================================
        void askUserToChooseNewFile (const String& suggestedFilename, const String& wildcard,
                                     const Project::Item& projectGroupToAddTo,
                                     std::function<void (File)> callback);

    private:
        std::unique_ptr<FileChooser> chooser;
        ScopedMessageBox messageBox;
    };

    //==============================================================================
    void addWizardsToMenu (PopupMenu&) const;
    bool runWizardFromMenu (int chosenMenuItemID, Project&,
                            const Project::Item& projectGroupToAddTo) const;

    void registerWizard (Type*);

private:
    OwnedArray<Type> wizards;
};
