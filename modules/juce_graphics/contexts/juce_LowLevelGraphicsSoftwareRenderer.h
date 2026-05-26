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

//==============================================================================
/**
    A lowest-common-denominator implementation of LowLevelGraphicsContext that does all
    its rendering in memory.

    User code is not supposed to create instances of this class directly - do all your
    rendering via the Graphics class instead.

    @tags{Graphics}
*/
class JUCE_API  LowLevelGraphicsSoftwareRenderer : public LowLevelGraphicsContext
{
public:
    //==============================================================================
    /** Creates a context to render into an image. */
    LowLevelGraphicsSoftwareRenderer (const Image& imageToRenderOnto);

    /** Creates a context to render into a clipped subsection of an image. */
    LowLevelGraphicsSoftwareRenderer (const Image& imageToRenderOnto, Point<int> origin,
                                      const RectangleList<int>& initialClip);

    /** Destructor. */
    ~LowLevelGraphicsSoftwareRenderer() override;

    std::unique_ptr<ImageType> getPreferredImageTypeForTemporaryImages() const override
    {
        return std::make_unique<SoftwareImageType>();
    }

    bool isVectorDevice() const override;
    Rectangle<int> getClipBounds() const override;
    bool isClipEmpty() const override;

    void setOrigin (Point<int> o) override;
    void addTransform (const AffineTransform& t) override;
    float getPhysicalPixelScaleFactor() const override;
    bool clipRegionIntersects (const Rectangle<int>& r) override;
    bool clipToRectangle (const Rectangle<int>& r) override;
    bool clipToRectangleList (const RectangleList<int>& r) override;
    void excludeClipRectangle (const Rectangle<int>& r) override;
    void clipToPath (const Path& path, const AffineTransform& t) override;
    void clipToImageAlpha (const Image& im, const AffineTransform& t) override;
    void saveState() override;
    void restoreState() override;
    void beginTransparencyLayer (float opacity) override;
    void endTransparencyLayer() override;
    void setFill (const FillType& fillType) override;
    void setOpacity (float newOpacity) override;
    void setInterpolationQuality (Graphics::ResamplingQuality quality) override;
    void fillRect (const Rectangle<int>& r, bool replace) override;
    void fillRect (const Rectangle<float>& r) override;
    void fillRectList (const RectangleList<float>& list) override;
    void fillPath (const Path& path, const AffineTransform& t) override;
    void drawImage (const Image& im, const AffineTransform& t) override;
    void drawLine (const Line<float>& line) override;
    void setFont (const Font& newFont) override;
    const Font& getFont() override;
    uint64_t getFrameId() const override;

    void drawGlyphs (Span<const uint16_t> glyphs,
                     Span<const Point<float>> positions,
                     const AffineTransform& t) override;

private:
    class Impl;
    std::unique_ptr<Impl> impl;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LowLevelGraphicsSoftwareRenderer)
};

} // namespace juce
