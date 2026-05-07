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

/*  A single bitmap that represents a subsection of a virtual bitmap. */
struct Direct2DPixelDataPage
{
    /*  The bounds of the stored bitmap inside the virtual bitmap. */
    Rectangle<int> getBounds() const
    {
        if (bitmap == nullptr)
            return {};

        const auto size = bitmap->GetPixelSize();
        return Rectangle { (int) size.width, (int) size.height }.withPosition (topLeft);
    }

    /*  The stored subsection bitmap. */
    ComSmartPtr<ID2D1Bitmap1> bitmap;

    /*  The top-left position of this virtual bitmap inside the virtual bitmap. */
    Point<int> topLeft;
};

} // namespace juce
