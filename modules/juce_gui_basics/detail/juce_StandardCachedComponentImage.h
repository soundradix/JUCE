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

namespace juce::detail
{

struct StandardCachedComponentImage : public CachedComponentImage
{
    explicit StandardCachedComponentImage (Component& c) noexcept
        : owner (c)
    {
    }

    void paint (Graphics& g) override
    {
        scale = g.getInternalContext().getPhysicalPixelScaleFactor();
        auto compBounds = owner.getLocalBounds();
        auto imageBounds = compBounds * scale;

        if (image.isNull() || image.getBounds() != imageBounds)
        {
            auto tempImageType = g.getInternalContext().getPreferredImageTypeForTemporaryImages();
            image = Image (owner.isOpaque() ? Image::RGB
                                            : Image::ARGB,
                           jmax (1, imageBounds.getWidth()),
                           jmax (1, imageBounds.getHeight()),
                           ! owner.isOpaque(),
                           *tempImageType);
            image.setBackupEnabled (false);
            validArea.clear();
        }

        // If the cached image is outdated but cannot be backed-up, this indicates that the graphics
        // device holding the most recent copy of the cached image has gone away. Therefore, we've
        // effectively lost the contents of the cache, and we must repaint the entire component.
        if (auto ptr = image.getPixelData())
            if (auto* extensions = ptr->getBackupExtensions())
                if (extensions->needsBackup() && ! extensions->canBackup())
                    validArea.clear();

        if (! validArea.containsRectangle (compBounds))
        {
            Graphics imG (image);
            auto& lg = imG.getInternalContext();

            lg.addTransform (AffineTransform::scale (scale));

            for (auto& i : validArea)
                lg.excludeClipRectangle (i);

            if (! owner.isOpaque())
            {
                lg.setFill (Colours::transparentBlack);
                lg.fillRect (compBounds, true);
                lg.setFill (Colours::black);
            }

            owner.paintEntireComponent (imG, true);
        }

        validArea = compBounds;

        g.setColour (Colours::black.withAlpha (owner.getAlpha()));
        g.drawImageTransformed (image, AffineTransform::scale ((float) compBounds.getWidth()  / (float) imageBounds.getWidth(),
                                                               (float) compBounds.getHeight() / (float) imageBounds.getHeight()), false);
    }

    bool invalidateAll() override                            { validArea.clear(); return true; }
    bool invalidate (const Rectangle<int>& area) override    { validArea.subtract (area); return true; }
    void releaseResources() override                         { image = Image(); }

private:
    Image image;
    RectangleList<int> validArea;
    Component& owner;
    float scale = 1.0f;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StandardCachedComponentImage)
};

} // namespace juce::detail
