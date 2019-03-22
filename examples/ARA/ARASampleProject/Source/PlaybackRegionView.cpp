#include "PlaybackRegionView.h"
#include "DocumentView.h"

//==============================================================================
PlaybackRegionView::PlaybackRegionView (DocumentView& documentView, ARAPlaybackRegion* region)
    : documentView (documentView),
      playbackRegion (region),
      audioThumbCache (1),
      audioThumb (128, documentView.getAudioFormatManger(), audioThumbCache)
{
    audioThumb.addChangeListener (this);

    documentView.getARAEditorView()->addListener (this);
    onNewSelection (documentView.getARAEditorView()->getViewSelection());

    playbackRegion->getRegionSequence()->getDocument<ARADocument>()->addListener (this);
    playbackRegion->getAudioModification<ARAAudioModification>()->addListener (this);
    playbackRegion->getAudioModification()->getAudioSource<ARAAudioSource>()->addListener (this);
    playbackRegion->addListener (this);

    recreatePlaybackRegionReader();
}

PlaybackRegionView::~PlaybackRegionView()
{
    documentView.getARAEditorView()->removeListener (this);

    playbackRegion->removeListener (this);
    playbackRegion->getAudioModification<ARAAudioModification>()->removeListener (this);
    playbackRegion->getAudioModification()->getAudioSource<ARAAudioSource>()->removeListener (this);
    playbackRegion->getRegionSequence()->getDocument<ARADocument>()->removeListener (this);

    audioThumb.removeChangeListener (this);
    audioThumb.clear();
}

//==============================================================================
void PlaybackRegionView::paint (Graphics& g)
{
    auto rect = getLocalBounds();
    g.setColour (isSelected ? Colours::yellow : Colours::black);
    g.drawRect (rect);
    rect.reduce (1, 1);

    const Colour regionColour = convertOptionalARAColour (playbackRegion->getEffectiveColor());
    g.setColour (regionColour);
    g.fillRect (rect);

    if (playbackRegion->getAudioModification()->getAudioSource()->isSampleAccessEnabled())
    {
        auto clipBounds = g.getClipBounds();
        if (clipBounds.getWidth() > 0)
        {
            const auto convertedBounds = clipBounds + getBoundsInParent().getPosition();
            const double startTime = documentView.getPlaybackRegionsViewsTimeForX (convertedBounds.getX());
            const double endTime = documentView.getPlaybackRegionsViewsTimeForX (convertedBounds.getRight());

            const auto regionTimeRange = getTimeRange();

            auto drawBounds = getBounds() - getPosition();
            drawBounds.setHorizontalRange (clipBounds.getHorizontalRange());
            g.setColour (regionColour.contrasting (0.7f));
            audioThumb.drawChannels (g, drawBounds, startTime - regionTimeRange.getStart(), endTime - regionTimeRange.getStart(), 1.0f);
        }
    }
    else
    {
        g.setColour (regionColour.contrasting (1.0f));
        g.setFont (Font (12.0f));
        g.drawText ("Access Disabled", getBounds(), Justification::centred);
    }

    g.setColour (regionColour.contrasting (1.0f));
    g.setFont (Font (12.0f));
    g.drawText (convertOptionalARAString (playbackRegion->getEffectiveName()), rect, Justification::topLeft);
}

//==============================================================================
void PlaybackRegionView::changeListenerCallback (ChangeBroadcaster* /*broadcaster*/)
{
    // our thumb nail has changed
    repaint();
}

void PlaybackRegionView::onNewSelection (const ARA::PlugIn::ViewSelection& viewSelection)
{
    bool selected = ARA::contains (viewSelection.getPlaybackRegions(), playbackRegion);
    if (selected != isSelected)
    {
        isSelected = selected;
        repaint();
    }
}

void PlaybackRegionView::didEndEditing (ARADocument* document)
{
    jassert (document == playbackRegion->getRegionSequence()->getDocument());

    // our reader will pick up any changes in samples or position
    if ((playbackRegionReader ==  nullptr) || ! playbackRegionReader->isValid())
    {
        recreatePlaybackRegionReader();
        documentView.resized();
        repaint();
    }
}

void PlaybackRegionView::didEnableAudioSourceSamplesAccess (ARAAudioSource* audioSource, bool /*enable*/)
{
    jassert (audioSource == playbackRegion->getAudioModification()->getAudioSource());

    repaint();
}

void PlaybackRegionView::willUpdateAudioSourceProperties (ARAAudioSource* audioSource, ARAAudioSource::PropertiesPtr newProperties)
{
    jassert (audioSource == playbackRegion->getAudioModification()->getAudioSource());
    if (playbackRegion->getName() == nullptr && playbackRegion->getAudioModification()->getName() == nullptr && newProperties->name != audioSource->getName())
        repaint();
}

void PlaybackRegionView::willUpdateAudioModificationProperties (ARAAudioModification* audioModification, ARAAudioModification::PropertiesPtr newProperties)
{
    jassert (audioModification == playbackRegion->getAudioModification());
    if (playbackRegion->getName() == nullptr && newProperties->name != audioModification->getName())
        repaint();
}

void PlaybackRegionView::willUpdatePlaybackRegionProperties (ARAPlaybackRegion* region, ARAPlaybackRegion::PropertiesPtr newProperties)
{
    jassert (playbackRegion == region);

    if ((playbackRegion->getName() != newProperties->name) ||
        (playbackRegion->getColor() != newProperties->color))
    {
        repaint();
    }
}

void PlaybackRegionView::didUpdatePlaybackRegionContent (ARAPlaybackRegion* region, ARAContentUpdateScopes scopeFlags)
{
    jassert (playbackRegion == region);

    // Our reader catches this too, but we only check for its validity after host edits.
    // If the update is triggered inside the plug-in, we need to update the view from this call
    // (unless we're within a host edit already).
    if (scopeFlags.affectSamples() &&
        ! playbackRegion->getAudioModification()->getAudioSource()->getDocument()->getDocumentController()->isHostEditingDocument())
    {
        documentView.resized();
        repaint();
    }
}

//==============================================================================
void PlaybackRegionView::recreatePlaybackRegionReader()
{
    audioThumbCache.clear();

    // create a non-realtime playback region reader for our audio thumb
    playbackRegionReader = new ARAPlaybackRegionReader ({playbackRegion}, true);
    // see juce_AudioThumbnail.cpp line 122 - AudioThumbnail does not deal with zero length sources.
    if (playbackRegionReader->lengthInSamples <= 0)
    {
        delete playbackRegionReader;
        playbackRegionReader = nullptr;
        audioThumb.clear();
    }
    else
    {
        audioThumb.setReader (playbackRegionReader, reinterpret_cast<intptr_t> (playbackRegion));   // TODO JUCE_ARA better hash?
    }
}
