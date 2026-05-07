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

struct Direct2DImageContext::ImagePimpl : public Direct2DGraphicsContext::Pimpl
{
public:
    ImagePimpl (Direct2DImageContext& ownerIn,
                ComSmartPtr<ID2D1DeviceContext1> contextIn,
                ComSmartPtr<ID2D1Bitmap1> bitmapIn,
                const RectangleList<int>& paintAreasIn)
        : Pimpl (ownerIn),
          context (std::move (contextIn)),
          bitmap (std::move (bitmapIn)),
          paintAreas (paintAreasIn)
    {
    }

    Rectangle<int> getFrameSize() const override
    {
        if (bitmap == nullptr)
            return {};

        const auto size = bitmap->GetSize();
        return { (int) size.width, (int) size.height };
    }

    ComSmartPtr<ID2D1DeviceContext1> getDeviceContext() const override
    {
        return context;
    }

    ComSmartPtr<ID2D1Image> getDeviceContextTarget() const override
    {
        return bitmap;
    }

    RectangleList<int> getPaintAreas() const override
    {
        return paintAreas;
    }

private:
    ComSmartPtr<ID2D1DeviceContext1> context;
    ComSmartPtr<ID2D1Bitmap1> bitmap;
    RectangleList<int> paintAreas;

    JUCE_DECLARE_WEAK_REFERENCEABLE (ImagePimpl)
};

//==============================================================================
Direct2DImageContext::Direct2DImageContext (ComSmartPtr<ID2D1DeviceContext1> context,
                                            ComSmartPtr<ID2D1Bitmap1> bitmap,
                                            const RectangleList<int>& paintAreas)
    : pimpl (new ImagePimpl { *this, context, bitmap, paintAreas })
{
   #if JUCE_DIRECT2D_METRICS
    metrics = Direct2DMetricsHub::getInstance()->imageContextMetrics;
   #endif
}

Direct2DImageContext::~Direct2DImageContext() = default;

ComSmartPtr<ID2D1DeviceContext1> Direct2DImageContext::getDeviceContext() const
{
    return getPimpl()->getDeviceContext();
}

Direct2DGraphicsContext::Pimpl* Direct2DImageContext::getPimpl() const noexcept
{
    return pimpl.get();
}

void Direct2DImageContext::clearTargetBuffer()
{
    // The bitmap was already cleared when it was created; do nothing here
}

} // namespace juce
