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

namespace juce
{

DropShadow::DropShadow (Colour shadowColour, const int r, Point<int> o) noexcept
    : colour (shadowColour), radius (r), offset (o)
{
    jassert (radius > 0);
}

void DropShadow::drawForImage (Graphics& g, const Image& srcImage) const
{
    jassert (radius > 0);

    if (! srcImage.isValid())
        return;

    auto blurred = srcImage.convertedToFormat (Image::SingleChannel);
    blurred.setBackupEnabled (false);

    blurred.getPixelData()->applySingleChannelBoxBlurEffect (radius);

    g.setColour (colour);
    g.drawImageAt (blurred, offset.x, offset.y, true);
}

void DropShadow::drawForPath (Graphics& g, const Path& path) const
{
    jassert (radius > 0);

    auto area = (path.getBounds().getSmallestIntegerContainer() + offset)
            .expanded (radius + 1)
            .getIntersection (g.getClipBounds().expanded (radius + 1));

    if (area.getWidth() <= 2 || area.getHeight() <= 2)
        return;

    const auto tempImageType = g.getInternalContext().getPreferredImageTypeForTemporaryImages();
    Image pathImage { Image::SingleChannel, area.getWidth(), area.getHeight(), true, *tempImageType };
    pathImage.setBackupEnabled (false);

    {
        Graphics g2 (pathImage);
        g2.setColour (Colours::white);
        g2.fillPath (path, AffineTransform::translation ((float) (offset.x - area.getX()),
                                                         (float) (offset.y - area.getY())));
    }

    pathImage.getPixelData()->applySingleChannelBoxBlurEffect (radius);

    g.setColour (colour);
    g.drawImageAt (pathImage, area.getX(), area.getY(), true);
}

static void drawShadowSection (Graphics& g, ColourGradient& cg, Rectangle<float> area,
                               bool isCorner, float centreX, float centreY, float edgeX, float edgeY)
{
    cg.point1 = area.getRelativePoint (centreX, centreY);
    cg.point2 = area.getRelativePoint (edgeX, edgeY);
    cg.isRadial = isCorner;

    g.setGradientFill (cg);
    g.fillRect (area);
}

void DropShadow::drawForRectangle (Graphics& g, const Rectangle<int>& targetArea) const
{
    ColourGradient cg (colour, 0, 0, colour.withAlpha (0.0f), 0, 0, false);

    for (float i = 0.05f; i < 1.0f; i += 0.1f)
        cg.addColour (1.0 - i, colour.withMultipliedAlpha (i * i));

    const float radiusInset = (float) radius / 2.0f;
    const float expandedRadius = (float) radius + radiusInset;

    auto area = targetArea.toFloat().reduced (radiusInset) + offset.toFloat();

    auto r = area.expanded (expandedRadius);
    auto top = r.removeFromTop (expandedRadius);
    auto bottom = r.removeFromBottom (expandedRadius);

    drawShadowSection (g, cg, top.removeFromLeft  (expandedRadius), true, 1.0f, 1.0f, 0, 1.0f);
    drawShadowSection (g, cg, top.removeFromRight (expandedRadius), true, 0, 1.0f, 1.0f, 1.0f);
    drawShadowSection (g, cg, top, false, 0, 1.0f, 0, 0);

    drawShadowSection (g, cg, bottom.removeFromLeft  (expandedRadius), true, 1.0f, 0, 0, 0);
    drawShadowSection (g, cg, bottom.removeFromRight (expandedRadius), true, 0, 0, 1.0f, 0);
    drawShadowSection (g, cg, bottom, false, 0, 0, 0, 1.0f);

    drawShadowSection (g, cg, r.removeFromLeft  (expandedRadius), false, 1.0f, 0, 0, 0);
    drawShadowSection (g, cg, r.removeFromRight (expandedRadius), false, 0, 0, 1.0f, 0);

    g.setColour (colour);
    g.fillRect (area);
}

//==============================================================================
DropShadowEffect::DropShadowEffect() = default;
DropShadowEffect::~DropShadowEffect() = default;

void DropShadowEffect::setShadowProperties (const DropShadow& newShadow)
{
    shadow = newShadow;
}

void DropShadowEffect::applyEffect (Image& image, Graphics& g, float scaleFactor, float alpha)
{
    DropShadow s (shadow);
    s.radius = roundToInt ((float) s.radius * scaleFactor);
    s.colour = s.colour.withMultipliedAlpha (alpha);
    s.offset.x = roundToInt ((float) s.offset.x * scaleFactor);
    s.offset.y = roundToInt ((float) s.offset.y * scaleFactor);

    s.drawForImage (g, image);

    g.setOpacity (alpha);
    g.drawImageAt (image, 0, 0);
}

} // namespace juce
