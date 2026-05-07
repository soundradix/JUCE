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

/*  Allows access to ImagePixelData implementation details by LowLevelGraphicsContext instances.
    The internal templating is mainly to facilitate returning a type with dynamic implementation by value.
*/
class ImagePixelDataNativeExtensions
{
public:
    template <typename Impl>
    explicit ImagePixelDataNativeExtensions (Impl x)
        : impl (std::make_unique<Concrete<Impl>> (std::move (x))) {}

    /*  For subsection images, this returns the top-left pixel inside the root image */
    Point<int> getTopLeft() const { return impl->getTopLeft(); }

   #if JUCE_WINDOWS
    Span<const Direct2DPixelDataPage> getPages (ComSmartPtr<ID2D1Device1> x) const { return impl->getPages (x); }
   #endif

   #if JUCE_MAC || JUCE_IOS
    CGContextRef getCGContext() const { return impl->getCGContext(); }
    CFUniquePtr<CGImageRef> getCGImage (CGColorSpaceRef x) const { return impl->getCGImage (x); }
   #endif

private:
    struct Base
    {
        virtual ~Base() = default;
        virtual Point<int> getTopLeft() const = 0;

       #if JUCE_WINDOWS
        virtual Span<const Direct2DPixelDataPage> getPages (ComSmartPtr<ID2D1Device1>) const = 0;
       #endif

       #if JUCE_MAC || JUCE_IOS
        virtual CGContextRef getCGContext() const = 0;
        virtual CFUniquePtr<CGImageRef> getCGImage (CGColorSpaceRef x) const = 0;
       #endif
    };

    template <typename Impl>
    class Concrete : public Base
    {
    public:
        explicit Concrete (Impl x)
            : impl (std::move (x)) {}

        Point<int> getTopLeft() const override { return impl.getTopLeft(); }

       #if JUCE_WINDOWS
        Span<const Direct2DPixelDataPage> getPages (ComSmartPtr<ID2D1Device1> x) const override { return impl.getPages (x); }
       #endif

       #if JUCE_MAC || JUCE_IOS
        CGContextRef getCGContext() const override { return impl.getCGContext(); }
        CFUniquePtr<CGImageRef> getCGImage (CGColorSpaceRef x) const override { return impl.getCGImage (x); }
       #endif

    private:
        Impl impl;
    };

    std::unique_ptr<Base> impl;
};

} // namespace juce
