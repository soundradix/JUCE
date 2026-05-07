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

struct Direct2DGraphicsContext::Pimpl : private DxgiAdapterListener
{
public:
    explicit Pimpl (Direct2DGraphicsContext& ownerIn);
    ~Pimpl() override;

    virtual SavedState* startFrame();
    virtual HRESULT finishFrame();

    virtual bool prepare();
    virtual void teardown();
    virtual bool checkPaintReady();

    virtual RectangleList<int> getPaintAreas() const = 0;
    virtual Rectangle<int> getFrameSize() const = 0;
    virtual ComSmartPtr<ID2D1DeviceContext1> getDeviceContext() const = 0;
    virtual ComSmartPtr<ID2D1Image> getDeviceContextTarget() const = 0;

    SavedState* getCurrentSavedState() const;
    SavedState* pushFirstSavedState (Rectangle<int> initialClipRegion);

    SavedState* pushSavedState();
    SavedState* popSavedState();

    void popAllSavedStates();

    void setDeviceContextTransform (AffineTransform transform);
    void resetDeviceContextTransform();

    DxgiAdapter::Ptr getDefaultAdapter() const
    {
        return directX->adapters.getDefaultAdapter();
    }

    auto getDirect2DFactory() const
    {
        return directX->getD2DFactory();
    }

    auto getDirectWriteFactory() const
    {
        return directWrite->getDWriteFactory();
    }

    auto getDirectWriteFactory4() const
    {
        return directWrite->getDWriteFactory4();
    }

    auto& getFontCollection() const
    {
        return directWrite->getFonts();
    }

    uint64_t getFrameId() const
    {
        return owner.getFrameId();
    }

    Direct2DMetrics::Ptr getMetrics() const
    {
        return owner.metrics;
    }

    bool fillSpriteBatch (const RectangleList<float>& list);

    DirectWriteGlyphRun glyphRun;

private:
    static void resetTransform (ID2D1DeviceContext1* context);
    static void setTransform (ID2D1DeviceContext1* context, AffineTransform newTransform);

    DxgiAdapter::Ptr findAdapter() const;

    void adapterCreated (DxgiAdapter::Ptr newAdapter) override;
    void adapterRemoved (DxgiAdapter::Ptr expiringAdapter) override;

    Direct2DGraphicsContext& owner;
    SharedResourcePointer<DirectX> directX;
    SharedResourcePointer<Direct2DFactories> directWrite;

    std::optional<Direct2DDeviceResources> deviceResources;

    std::vector<std::unique_ptr<Direct2DGraphicsContext::SavedState>> savedClientStates;

   #if JUCE_DIRECT2D_METRICS
    int64 paintStartTicks = 0;
   #endif

    JUCE_DECLARE_WEAK_REFERENCEABLE (Pimpl)
};

} // namespace juce
