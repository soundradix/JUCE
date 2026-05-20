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

class LowLevelGraphicsSoftwareRenderer::Impl : public RenderingHelpers::StackBasedLowLevelGraphicsContext<RenderingHelpers::SoftwareRendererSavedState>
{
public:
    using SavedStateType = RenderingHelpers::SoftwareRendererSavedState;

    using StackBasedLowLevelGraphicsContext::StackBasedLowLevelGraphicsContext;

    std::unique_ptr<ImageType> getPreferredImageTypeForTemporaryImages() const override
    {
        jassertfalse;
        return {};
    }
};

LowLevelGraphicsSoftwareRenderer::LowLevelGraphicsSoftwareRenderer (const Image& image)
    : impl (std::make_unique<Impl> (std::make_unique<Impl::SavedStateType> (image, image.getBounds())))
{
    JUCE_TRACE_LOG_PAINT_CALL (etw::startGDIImage, getFrameId());
}

LowLevelGraphicsSoftwareRenderer::LowLevelGraphicsSoftwareRenderer (const Image& image, Point<int> origin,
                                                                    const RectangleList<int>& initialClip)
    : impl (std::make_unique<Impl> (std::make_unique<Impl::SavedStateType> (image, initialClip, origin)))
{
    JUCE_TRACE_EVENT_INT_RECT_LIST (etw::startGDIFrame, etw::softwareRendererKeyword, getFrameId(), initialClip);
}

LowLevelGraphicsSoftwareRenderer::~LowLevelGraphicsSoftwareRenderer()
{
    JUCE_TRACE_LOG_PAINT_CALL (etw::endGDIFrame, getFrameId());
}

bool LowLevelGraphicsSoftwareRenderer::isVectorDevice() const
{
    return impl->isVectorDevice();
}

Rectangle<int> LowLevelGraphicsSoftwareRenderer::getClipBounds() const
{
    return impl->getClipBounds();
}

bool LowLevelGraphicsSoftwareRenderer::isClipEmpty() const
{
    return impl->isClipEmpty();
}

void LowLevelGraphicsSoftwareRenderer::setOrigin (Point<int> o)
{
    impl->setOrigin (o);
}

void LowLevelGraphicsSoftwareRenderer::addTransform (const AffineTransform& t)
{
    impl->addTransform (t);
}

float LowLevelGraphicsSoftwareRenderer::getPhysicalPixelScaleFactor() const
{
    return impl->getPhysicalPixelScaleFactor();
}

bool LowLevelGraphicsSoftwareRenderer::clipRegionIntersects (const Rectangle<int>& r)
{
    return impl->clipRegionIntersects (r);
}

bool LowLevelGraphicsSoftwareRenderer::clipToRectangle (const Rectangle<int>& r)
{
    return impl->clipToRectangle (r);
}

bool LowLevelGraphicsSoftwareRenderer::clipToRectangleList (const RectangleList<int>& r)
{
    return impl->clipToRectangleList (r);
}

void LowLevelGraphicsSoftwareRenderer::excludeClipRectangle (const Rectangle<int>& r)
{
    impl->excludeClipRectangle (r);
}

void LowLevelGraphicsSoftwareRenderer::clipToPath (const Path& path, const AffineTransform& t)
{
    impl->clipToPath (path, t);
}

void LowLevelGraphicsSoftwareRenderer::clipToImageAlpha (const Image& im, const AffineTransform& t)
{
    impl->clipToImageAlpha (im, t);
}

void LowLevelGraphicsSoftwareRenderer::saveState()
{
    impl->saveState();
}

void LowLevelGraphicsSoftwareRenderer::restoreState()
{
    impl->restoreState();
}

void LowLevelGraphicsSoftwareRenderer::beginTransparencyLayer (float opacity)
{
    impl->beginTransparencyLayer (opacity);
}

void LowLevelGraphicsSoftwareRenderer::endTransparencyLayer()
{
    impl->endTransparencyLayer();
}

void LowLevelGraphicsSoftwareRenderer::setFill (const FillType& fillType)
{
    impl->setFill (fillType);
}

void LowLevelGraphicsSoftwareRenderer::setImageBlendMode (BlendMode blendMode)
{
    impl->setImageBlendMode (blendMode);
}

void LowLevelGraphicsSoftwareRenderer::setOpacity (float newOpacity)
{
    impl->setOpacity (newOpacity);
}

void LowLevelGraphicsSoftwareRenderer::setInterpolationQuality (Graphics::ResamplingQuality quality)
{
    impl->setInterpolationQuality (quality);
}

void LowLevelGraphicsSoftwareRenderer::fillRect (const Rectangle<int>& r, bool replace)
{
    impl->fillRect (r, replace);
}

void LowLevelGraphicsSoftwareRenderer::fillRect (const Rectangle<float>& r)
{
    impl->fillRect (r);
}

void LowLevelGraphicsSoftwareRenderer::fillRectList (const RectangleList<float>& list)
{
    impl->fillRectList (list);
}

void LowLevelGraphicsSoftwareRenderer::fillPath (const Path& path, const AffineTransform& t)
{
    impl->fillPath (path, t);
}

void LowLevelGraphicsSoftwareRenderer::drawImage (const Image& im, const AffineTransform& t)
{
    impl->drawImage (im, t);
}

void LowLevelGraphicsSoftwareRenderer::drawLine (const Line<float>& line)
{
    impl->drawLine (line);
}

void LowLevelGraphicsSoftwareRenderer::setFont (const Font& newFont)
{
    impl->setFont (newFont);
}

const Font& LowLevelGraphicsSoftwareRenderer::getFont()
{
    return impl->getFont();
}

uint64_t LowLevelGraphicsSoftwareRenderer::getFrameId() const
{
    return impl->getFrameId();
}


void LowLevelGraphicsSoftwareRenderer::drawGlyphs (Span<const uint16_t> glyphs,
                                                   Span<const Point<float>> positions,
                                                   const AffineTransform& t)
{
    return impl->drawGlyphs (glyphs, positions, t);
}

} // namespace juce
