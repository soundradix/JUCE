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

namespace juce::detail
{

struct ScalingHelpers
{
    template <typename PointOrRect>
    static PointOrRect unscaledScreenPosToScaled (float scale, PointOrRect pos) noexcept
    {
        return ! approximatelyEqual (scale, 1.0f) ? pos / scale : pos;
    }

    template <typename PointOrRect>
    static PointOrRect scaledScreenPosToUnscaled (float scale, PointOrRect pos) noexcept
    {
        return ! approximatelyEqual (scale, 1.0f) ? pos * scale : pos;
    }

    // For these, we need to avoid getSmallestIntegerContainer being used, which causes
    // judder when moving windows
    static Rectangle<int> unscaledScreenPosToScaled (float scale, Rectangle<int> pos) noexcept
    {
        return ! approximatelyEqual (scale, 1.0f) ? Rectangle<int> (roundToInt ((float) pos.getX() / scale),
                                                                    roundToInt ((float) pos.getY() / scale),
                                                                    roundToInt ((float) pos.getWidth() / scale),
                                                                    roundToInt ((float) pos.getHeight() / scale)) : pos;
    }

    static Rectangle<int> scaledScreenPosToUnscaled (float scale, Rectangle<int> pos) noexcept
    {
        return ! approximatelyEqual (scale, 1.0f) ? Rectangle<int> (roundToInt ((float) pos.getX() * scale),
                                                                    roundToInt ((float) pos.getY() * scale),
                                                                    roundToInt ((float) pos.getWidth() * scale),
                                                                    roundToInt ((float) pos.getHeight() * scale)) : pos;
    }

    static Rectangle<float> unscaledScreenPosToScaled (float scale, Rectangle<float> pos) noexcept
    {
        return ! approximatelyEqual (scale, 1.0f) ? Rectangle<float> (pos.getX() / scale,
                                                                      pos.getY() / scale,
                                                                      pos.getWidth() / scale,
                                                                      pos.getHeight() / scale) : pos;
    }

    static Rectangle<float> scaledScreenPosToUnscaled (float scale, Rectangle<float> pos) noexcept
    {
        return ! approximatelyEqual (scale, 1.0f) ? Rectangle<float> (pos.getX() * scale,
                                                                      pos.getY() * scale,
                                                                      pos.getWidth() * scale,
                                                                      pos.getHeight() * scale) : pos;
    }

    template <typename PointOrRect>
    static PointOrRect unscaledScreenPosToScaled (PointOrRect pos) noexcept
    {
        return unscaledScreenPosToScaled (Desktop::getInstance().getGlobalScaleFactor(), pos);
    }

    template <typename PointOrRect>
    static PointOrRect scaledScreenPosToUnscaled (PointOrRect pos) noexcept
    {
        return scaledScreenPosToUnscaled (Desktop::getInstance().getGlobalScaleFactor(), pos);
    }

    template <typename PointOrRect>
    static PointOrRect unscaledScreenPosToScaled (const Component& comp, PointOrRect pos) noexcept
    {
        return unscaledScreenPosToScaled (comp.getDesktopScaleFactor(), pos);
    }

    template <typename PointOrRect>
    static PointOrRect scaledScreenPosToUnscaled (const Component& comp, PointOrRect pos) noexcept
    {
        return scaledScreenPosToUnscaled (comp.getDesktopScaleFactor(), pos);
    }

    static Point<int>       addPosition      (Point<int> p,       const Component& c) noexcept  { return p + c.getPosition(); }
    static Rectangle<int>   addPosition      (Rectangle<int> p,   const Component& c) noexcept  { return p + c.getPosition(); }
    static Point<float>     addPosition      (Point<float> p,     const Component& c) noexcept  { return p + c.getPosition().toFloat(); }
    static Rectangle<float> addPosition      (Rectangle<float> p, const Component& c) noexcept  { return p + c.getPosition().toFloat(); }
    static Point<int>       subtractPosition (Point<int> p,       const Component& c) noexcept  { return p - c.getPosition(); }
    static Rectangle<int>   subtractPosition (Rectangle<int> p,   const Component& c) noexcept  { return p - c.getPosition(); }
    static Point<float>     subtractPosition (Point<float> p,     const Component& c) noexcept  { return p - c.getPosition().toFloat(); }
    static Rectangle<float> subtractPosition (Rectangle<float> p, const Component& c) noexcept  { return p - c.getPosition().toFloat(); }

    static Point<float> screenPosToLocalPos (const Component& comp, Point<float> pos)
    {
        if (auto* peer = comp.getPeer())
        {
            auto& peerComp = peer->getComponent();
            return comp.getLocalPoint (&peerComp, unscaledScreenPosToScaled (peerComp, peer->globalToLocal (pos)));
        }

        return comp.getLocalPoint (nullptr, unscaledScreenPosToScaled (comp, pos));
    }

    static Point<float> convertPhysicalScreenPointToLogical (Point<float> p) noexcept
    {
        return scaledScreenPosToUnscaled (Desktop::getInstance().getDisplays().physicalToLogical (p, nullptr));
    }

    static Point<float> convertLogicalScreenPointToPhysical (Point<float> p) noexcept
    {
        return Desktop::getInstance().getDisplays().logicalToPhysical (unscaledScreenPosToScaled (p), nullptr);
    }
};

} // namespace juce::detail
