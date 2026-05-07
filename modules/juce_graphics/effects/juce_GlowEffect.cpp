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

GlowEffect::GlowEffect() = default;
GlowEffect::~GlowEffect() = default;

void GlowEffect::setGlowProperties (float newRadius, Colour newColour, Point<int> pos)
{
    radius = newRadius;
    colour = newColour;
    offset = pos;
}

void GlowEffect::applyEffect (Image& image, Graphics& g, float scaleFactor, float alpha)
{
    auto blurred = image.createCopy();
    blurred.setBackupEnabled (false);

    if (auto ptr = blurred.getPixelData())
        ptr->applyGaussianBlurEffect (radius * scaleFactor);

    g.setColour (colour.withMultipliedAlpha (alpha));
    g.drawImageAt (blurred, offset.x, offset.y, true);

    g.setOpacity (alpha);
    g.drawImageAt (image, offset.x, offset.y, false);
}

} // namespace juce
