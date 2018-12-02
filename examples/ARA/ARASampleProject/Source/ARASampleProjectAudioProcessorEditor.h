#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "ARASampleProjectAudioProcessor.h"
#include "RegionSequenceView.h"

//==============================================================================
/**
    Editor class for ARA sample project
    This class manages the UI we use to display region sequences in the
    ARA document as well as their current selection state
*/
class ARASampleProjectAudioProcessorEditor: public AudioProcessorEditor,
                                            public AudioProcessorEditorARAExtension,
                                            public ARAEditorView::Listener,
                                            public ARADocument::Listener
{
public:
    ARASampleProjectAudioProcessorEditor (ARASampleProjectAudioProcessor&);
    ~ARASampleProjectAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    // ARAEditorView::Listener overrides
    void onHideRegionSequences (std::vector<ARARegionSequence*> const& regionSequences) override;

    // ARADocument::Listener overrides
    void doEndEditing (ARADocument* document) override;
    void didReorderRegionSequencesInDocument (ARADocument* document) override;

    // function to flag that our view needs to be rebuilt
    void setDirty() { isViewDirty = true; }

public:
    static const int kVisibleSeconds = 10;
    static const int kMinWidth = 500;
    static const int kWidth = 1000;
    static const int kRegionSequenceHeight = 80;
    static const int kMinHeight = 1 * kRegionSequenceHeight;
    static const int kHeight = 5 * kRegionSequenceHeight;
    static const int kTrackHeaderWidth = 20;

private:
    void rebuildView();
    void clearView();

private:

    // we'll be displaying all region sequences in the document in a scrollable view
    Viewport regionSequenceViewPort;
    Component regionSequenceListView;

    OwnedArray<RegionSequenceView> regionSequenceViews;

    bool isViewDirty = false;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ARASampleProjectAudioProcessorEditor)
};
