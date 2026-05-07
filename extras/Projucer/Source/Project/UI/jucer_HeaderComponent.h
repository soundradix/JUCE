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

#include "../../Application/jucer_Headers.h"
#include "../../Utility/UI/jucer_IconButton.h"

class Project;
class ProjectContentComponent;
class ProjectExporter;

//==============================================================================
class HeaderComponent final : public Component,
                              private ValueTree::Listener,
                              private Value::Listener,
                              private Timer
{
public:
    HeaderComponent (ProjectContentComponent* projectContentComponent);

    //==============================================================================
    void resized() override;
    void paint (Graphics&) override;

    //==============================================================================
    void setCurrentProject (Project*);

    void updateExporters();
    std::unique_ptr<ProjectExporter> getSelectedExporter() const;
    bool canCurrentExporterLaunchProject() const;

    void sidebarTabsWidthChanged (int newWidth);

private:
    //==============================================================================
    void valueChanged (Value&) override;
    void timerCallback() override;

    //==============================================================================
    void valueTreeChildAdded (ValueTree& parentTree, ValueTree&) override        { updateIfNeeded (parentTree); }
    void valueTreeChildRemoved (ValueTree& parentTree, ValueTree&, int) override { updateIfNeeded (parentTree); }
    void valueTreeChildOrderChanged (ValueTree& parentTree, int, int) override   { updateIfNeeded (parentTree); }

    void updateIfNeeded (ValueTree tree)
    {
        if (tree == exportersTree)
            updateExporters();
    }

    //==============================================================================
    void initialiseButtons();

    void updateName();
    void updateExporterButton();

    //==============================================================================
    int tabsWidth = 200;

    ProjectContentComponent* projectContentComponent = nullptr;
    Project* project = nullptr;
    ValueTree exportersTree;

    Value projectNameValue;

    ComboBox exporterBox;
    Label configLabel  { "Config Label", "Selected exporter" }, projectNameLabel;

    ImageComponent juceIcon;

    IconButton projectSettingsButton { "Project Settings", getIcons().settings },
               saveAndOpenInIDEButton { "Save and Open in IDE", Image() };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HeaderComponent)
};
