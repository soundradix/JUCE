#include "RegionSequenceView.h"
#include "DocumentView.h"
#include "TrackHeaderView.h"
#include "PlaybackRegionView.h"

//==============================================================================
RegionSequenceView::RegionSequenceView (DocumentView& ownerDocument, ARARegionSequence* sequence)
    : owner (ownerDocument),
      regionSequence (sequence),
      trackHeaderView (owner.getController().createHeaderViewForRegionSequence (*this))
{
    setInterceptsMouseClicks (false, true);
    regionSequence->addListener (this);

    for (auto playbackRegion : regionSequence->getPlaybackRegions<ARAPlaybackRegion>())
        addRegionSequenceViewAndMakeVisible (playbackRegion);
}

RegionSequenceView::~RegionSequenceView()
{
    detachFromRegionSequence();
}

void RegionSequenceView::addRegionSequenceViewAndMakeVisible (ARAPlaybackRegion* playbackRegion)
{
    auto view = owner.getController().createViewForPlaybackRegion (this, playbackRegion);
    playbackRegionViews.add (view);
    addChildComponent (view);
    owner.setRegionBounds (
        view, owner.getViewport().getVisibleRange(), trackBorders);
}

void RegionSequenceView::detachFromRegionSequence()
{
    // detach header if needed
    trackHeaderView.reset();

    if (regionSequence == nullptr)
        return;

    regionSequence->removeListener (this);

    regionSequence = nullptr;
}

//==============================================================================
void RegionSequenceView::updateRegionsBounds (Range<double> newVisibleRange)
{
    for (auto regionView : playbackRegionViews)
    {
        owner.setRegionBounds (regionView, newVisibleRange, trackBorders);
    }
}

void RegionSequenceView::resized()
{
    // updates all visible PlaybackRegions to new position.
    for (auto region : playbackRegionViews)
        if (region->isVisible())
            region->setBounds (region->getBounds().withHeight (getHeight()-trackBorders.getTopAndBottom()));
}

//==============================================================================
void RegionSequenceView::willRemovePlaybackRegionFromRegionSequence (ARARegionSequence* sequence, ARAPlaybackRegion* playbackRegion)
{
    jassert (regionSequence == sequence);

    for (int i = 0; i < playbackRegionViews.size(); ++i)
    {
        if (playbackRegionViews[i]->getPlaybackRegion() == playbackRegion)
        {
            removeChildComponent (playbackRegionViews[i]);
            playbackRegionViews.remove (i);
            break;
        }
    }

    owner.getController().invalidateRegionSequenceViews();
}

void RegionSequenceView::didAddPlaybackRegionToRegionSequence (ARARegionSequence* sequence, ARAPlaybackRegion* playbackRegion)
{
    jassert (regionSequence == sequence);

    addRegionSequenceViewAndMakeVisible (playbackRegion);

    owner.getController().invalidateRegionSequenceViews();
}

void RegionSequenceView::willDestroyRegionSequence (ARARegionSequence* sequence)
{
    jassert (regionSequence == sequence);

    detachFromRegionSequence();

    owner.getController().invalidateRegionSequenceViews();
}

void RegionSequenceView::willUpdateRegionSequenceProperties (ARARegionSequence* sequence, ARARegionSequence::PropertiesPtr newProperties)
{
    jassert (regionSequence == sequence);
    if (newProperties->color != regionSequence->getColor())
    {
        // repaints any PlaybackRegion that should follow RegionSequence color
        for (auto region : playbackRegionViews)
        {
            if (region->getPlaybackRegion()->getColor() == nullptr)
                region->repaint();
        }
    }
}

BorderSize<int> RegionSequenceView::getTrackBorders()
{
    return trackBorders;
}

void RegionSequenceView::setTrackBorders (BorderSize<int> newBorders)
{
    trackBorders = newBorders;
}