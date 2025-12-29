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

#if JUCE_UNIT_TESTS

struct PixelFormatsConstexprTests
{
    PixelFormatsConstexprTests() = delete;

    template <typename PixelType>
    static constexpr auto constexprCheck (PixelType p)
    {
        PixelType other{};
        other.setARGB (p.getBlue(), p.getAlpha(), p.getGreen(), p.getRed());
        p.set (other);
        p.blend (p);
        p.blend (p, 100);
        p.tween (p, 50);
        p.multiplyAlpha (0.9f);
        p.premultiply();
        p.setAlpha (200);
        p.unpremultiply();
        p.desaturate();
        return p.getNativeARGB()
             + p.getInARGBMaskOrder()
             + p.getInARGBMemoryOrder()
             + p.getEvenBytes()
             + p.getOddBytes()
             + p.getAlpha()
             + p.getRed()
             + p.getGreen()
             + p.getBlue()
             + maskPixelComponents  (p.getNativeARGB())
             + clampPixelComponents (p.getNativeARGB());
    }
};

static_assert (PixelFormatsConstexprTests::constexprCheck<PixelARGB>  ({ 10, 20, 30, 40 }) != 0);
static_assert (PixelFormatsConstexprTests::constexprCheck<PixelRGB>   ({ 50, 60, 70 })     != 0);
static_assert (PixelFormatsConstexprTests::constexprCheck<PixelAlpha> ({ 80 })             != 0);

#endif

} // namespace juce
