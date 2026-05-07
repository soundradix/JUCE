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

namespace detail
{
    using ColorSpacePtr     = CFUniquePtr<CGColorSpaceRef>;
    using ContextPtr        = CFUniquePtr<CGContextRef>;
    using DataProviderPtr   = CFUniquePtr<CGDataProviderRef>;
    using ImagePtr          = CFUniquePtr<CGImageRef>;
    using GradientPtr       = CFUniquePtr<CGGradientRef>;
    using ColorPtr          = CFUniquePtr<CGColorRef>;
    using PathPtr           = CFUniquePtr<CGPathRef>;
    using MutablePathPtr    = CFUniquePtr<CGMutablePathRef>;
}

//==============================================================================
class CoreGraphicsContext   : public LowLevelGraphicsContext
{
public:
    CoreGraphicsContext (CGContextRef context, float flipHeight);
    ~CoreGraphicsContext() override;

    //==============================================================================
    bool isVectorDevice() const override         { return false; }

    void setOrigin (Point<int>) override;
    void addTransform (const AffineTransform&) override;
    float getPhysicalPixelScaleFactor() const override;
    bool clipToRectangle (const Rectangle<int>&) override;
    bool clipToRectangleList (const RectangleList<int>&) override;
    void excludeClipRectangle (const Rectangle<int>&) override;
    void clipToPath (const Path&, const AffineTransform&) override;
    void clipToImageAlpha (const Image&, const AffineTransform&) override;
    bool clipRegionIntersects (const Rectangle<int>&) override;
    Rectangle<int> getClipBounds() const override;
    bool isClipEmpty() const override;

    //==============================================================================
    void saveState() override;
    void restoreState() override;
    void beginTransparencyLayer (float opacity) override;
    void endTransparencyLayer() override;

    //==============================================================================
    void setFill (const FillType&) override;
    void setOpacity (float) override;
    void setInterpolationQuality (Graphics::ResamplingQuality) override;

    //==============================================================================
    void fillAll() override;
    void fillRect (const Rectangle<int>&, bool replaceExistingContents) override;
    void fillRect (const Rectangle<float>&) override;
    void fillRectList (const RectangleList<float>&) override;
    void fillPath (const Path&, const AffineTransform&) override;
    void strokePath (const Path& path, const PathStrokeType& strokeType, const AffineTransform& transform) override;
    void drawImage (const Image& sourceImage, const AffineTransform&) override;

    //==============================================================================
    void drawLine (const Line<float>&) override;
    void setFont (const Font&) override;
    const Font& getFont() override;
    void drawGlyphs (Span<const uint16_t>,
                     Span<const Point<float>>,
                     const AffineTransform&) override;

    uint64_t getFrameId() const override { return 0; }

    std::unique_ptr<ImageType> getPreferredImageTypeForTemporaryImages() const override
    {
        return std::make_unique<NativeImageType>();
    }

    void drawEllipse (const Rectangle<float>& area, float lineThickness) override;
    void fillEllipse (const Rectangle<float>& area) override;

    void drawRoundedRectangle (const Rectangle<float>& r, float cornerSize, float lineThickness) override;
    void fillRoundedRectangle (const Rectangle<float>& r, float cornerSize) override;

    void drawLineWithThickness (const Line<float>& line, float lineThickness) override;

private:
    //==============================================================================
    detail::ContextPtr context;
    const CGFloat flipHeight;
    detail::ColorSpacePtr rgbColourSpace, greyColourSpace;
    mutable std::optional<Rectangle<int>> lastClipRect;

    struct SavedState;
    std::unique_ptr<SavedState> state;
    OwnedArray<SavedState> stateStack;

    template <class RectType>
    CGRect convertToCGRectFlipped (RectType r) const noexcept;
    void setContextClipToCurrentPath (bool useNonZeroWinding);
    void drawCurrentPath (CGPathDrawingMode mode);
    void drawGradient();
    void createPath (const Path&, const AffineTransform&) const;
    void flip() const;
    void applyTransform (const AffineTransform&) const;
    void drawImage (const Image&, const AffineTransform&, bool fillEntireClipAsTiles);
    bool clipToRectangleListWithoutTest (const RectangleList<float>&);
    void fillCGRect (const CGRect&, bool replaceExistingContents);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CoreGraphicsContext)
};

} // namespace juce
