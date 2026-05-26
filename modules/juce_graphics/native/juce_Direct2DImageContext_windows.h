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

class Direct2DImageContext : public Direct2DGraphicsContext
{
public:
    Direct2DImageContext (ComSmartPtr<ID2D1DeviceContext1>,
                          ComSmartPtr<ID2D1Bitmap1>,
                          const RectangleList<int>&);

    ~Direct2DImageContext() override;

    ComSmartPtr<ID2D1DeviceContext1> getDeviceContext() const;

private:
    struct ImagePimpl;
    std::unique_ptr<ImagePimpl> pimpl;

    Pimpl* getPimpl() const noexcept override;
    void clearTargetBuffer() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Direct2DImageContext)
};

} // namespace juce
