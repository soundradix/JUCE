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

#ifdef JUCE_GUI_BASICS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of JUCE cpp file"
#endif

#include "juce_gui_basics.h"

#include "detail/juce_ViewportHelpers.h"
#include "detail/juce_ButtonAccessibilityHandler.h"

#include "buttons/juce_ArrowButton.cpp"
#include "buttons/juce_Button.cpp"
#include "buttons/juce_DrawableButton.cpp"
#include "buttons/juce_HyperlinkButton.cpp"
#include "buttons/juce_ImageButton.cpp"
#include "buttons/juce_ShapeButton.cpp"
#include "buttons/juce_TextButton.cpp"
#include "buttons/juce_ToggleButton.cpp"
#include "buttons/juce_ToolbarButton.cpp"

#include "layout/juce_ComponentAnimator.cpp"
#include "layout/juce_ComponentBoundsConstrainer.cpp"
#include "layout/juce_BorderedComponentBoundsConstrainer.cpp"
#include "layout/juce_ComponentBuilder.cpp"
#include "layout/juce_ComponentMovementWatcher.cpp"
#include "layout/juce_ConcertinaPanel.cpp"
#include "layout/juce_FlexBox.cpp"
#include "layout/juce_Grid.cpp"
#include "layout/juce_GridItem.cpp"
#include "layout/juce_GroupComponent.cpp"
#include "layout/juce_MultiDocumentPanel.cpp"
#include "layout/juce_ResizableBorderComponent.cpp"
#include "layout/juce_ResizableCornerComponent.cpp"
#include "layout/juce_ResizableEdgeComponent.cpp"
#include "layout/juce_ScrollBar.cpp"
#include "layout/juce_SidePanel.cpp"
#include "layout/juce_StretchableLayoutManager.cpp"
#include "layout/juce_StretchableLayoutResizerBar.cpp"
#include "layout/juce_StretchableObjectResizer.cpp"
#include "layout/juce_TabbedButtonBar.cpp"
#include "layout/juce_TabbedComponent.cpp"
#include "layout/juce_Viewport.cpp"

#include "properties/juce_BooleanPropertyComponent.cpp"
#include "properties/juce_ButtonPropertyComponent.cpp"
#include "properties/juce_ChoicePropertyComponent.cpp"
#include "properties/juce_MultiChoicePropertyComponent.cpp"
#include "properties/juce_PropertyComponent.cpp"
#include "properties/juce_PropertyPanel.cpp"
#include "properties/juce_SliderPropertyComponent.cpp"
#include "properties/juce_TextPropertyComponent.cpp"
