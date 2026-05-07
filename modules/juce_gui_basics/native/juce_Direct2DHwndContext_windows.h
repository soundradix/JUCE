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

class Direct2DHwndContext : public Direct2DGraphicsContext
{
public:
    explicit Direct2DHwndContext (HWND windowHandle);
    ~Direct2DHwndContext() override;

    void handleShowWindow();

    void addDeferredRepaint (Rectangle<int> deferredRepaint);

    Image createSnapshot() const override;

private:
    struct HwndPimpl;
    std::unique_ptr<HwndPimpl> pimpl;

    Pimpl* getPimpl() const noexcept override;
    void clearTargetBuffer() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Direct2DHwndContext)
};

} // namespace juce
