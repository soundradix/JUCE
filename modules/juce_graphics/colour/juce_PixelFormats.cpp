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

//==============================================================================
class PixelDouble
{
public:
    explicit PixelDouble (Colour colour)
        : r ((double) colour.getRed() / 255.0),
          g ((double) colour.getGreen() / 255.0),
          b ((double) colour.getBlue() / 255.0),
          a ((double) colour.getAlpha() / 255.0)
    {
        premultiply();
    }

    explicit PixelDouble (PixelARGB p)
        : r ((double) p.getRed() / 255.0),
          g ((double) p.getGreen() / 255.0),
          b ((double) p.getBlue() / 255.0),
          a ((double) p.getAlpha() / 255.0)
    {
    }

    PixelDouble operator* (double scalar) const
    {
        auto copy = *this;
        copy.r = r * scalar;
        copy.g = g * scalar;
        copy.b = b * scalar;
        copy.a = a * scalar;
        copy.clamp();
        return copy;
    }

    PixelDouble operator+ (PixelDouble other) const
    {
        auto copy = *this;
        copy.r += other.r;
        copy.g += other.g;
        copy.b += other.b;
        copy.a += other.a;
        copy.clamp();
        return copy;
    }

    void blend (PixelDouble src, uint32 extraAlpha = 255)
    {
        src = src * (extraAlpha / 255.0);

        *this = src + *this * (1.0 - src.a);
    }

    PixelARGB toPixelARGB() const
    {
        auto r8 = (uint8) std::round (r * 255.0);
        auto g8 = (uint8) std::round (g * 255.0);
        auto b8 = (uint8) std::round (b * 255.0);
        auto a8 = (uint8) std::round (a * 255.0);
        return PixelARGB (a8, r8, g8, b8);
    }

    bool operator== (const PixelARGB& other) const
    {
        auto pixelARGB = toPixelARGB();
        return    pixelARGB.getAlpha() == other.getAlpha()
               && pixelARGB.getRed()   == other.getRed()
               && pixelARGB.getGreen() == other.getGreen()
               && pixelARGB.getBlue()  == other.getBlue();
    }

    bool operator!= (const PixelARGB& other) const
    {
        return ! operator== (other);
    }

    bool verifyChannelError (const PixelARGB& other, std::array<int64, 4>& acc, int64 maxAllowed = 1) const
    {
        auto expected = toPixelARGB();

        auto dAAbs = std::abs ((int64) expected.getAlpha() - (int64) other.getAlpha());
        auto dRAbs = std::abs ((int64) expected.getRed() - (int64) other.getRed());
        auto dGAbs = std::abs ((int64) expected.getGreen() - (int64) other.getGreen());
        auto dBAbs = std::abs ((int64) expected.getBlue() - (int64) other.getBlue());

        acc[0] += dAAbs;
        acc[1] += dRAbs;
        acc[2] += dGAbs;
        acc[3] += dBAbs;

        return std::max ({ dAAbs, dRAbs, dGAbs, dBAbs }) <= maxAllowed;
    }

private:
    void premultiply()
    {
        r *= a;
        g *= a;
        b *= a;
    }

    void clamp()
    {
        r = std::clamp (r, 0.0, 1.0);
        g = std::clamp (g, 0.0, 1.0);
        b = std::clamp (b, 0.0, 1.0);
        a = std::clamp (a, 0.0, 1.0);
    }

    double r{};
    double g{};
    double b{};
    double a{};
};

static String toString (PixelARGB p)
{
    String s;
    s << "("
      << (int) p.getAlpha() << ", " << (int) p.getRed() << ", " << (int) p.getGreen() << ", " << (int) p.getBlue()
      << ")";
    return s;
}

static String toString (std::array<int64, 4> err)
{
    String s;
    s << "(" << err[0] << ", " << err[1] << ", " << err[2] << ", " << err[3] << ")";
    return s;
}

static PixelARGB makePixelARGB (int a, int r, int g, int b)
{
    int channels[] = { a, r, g, b };

    const auto inputsValid = std::all_of (std::begin (channels),
                                          std::end (channels),
                                          [] (int v) { return 0 <= v && v <= 255; });

    if (! inputsValid)
    {
        jassertfalse;
        return {};
    }

    return PixelARGB { (uint8) a, (uint8) r, (uint8) g, (uint8) b };
}

static bool equals (PixelARGB a, PixelARGB b)
{
    return a.getAlpha() == b.getAlpha()
           && a.getRed() == b.getRed()
           && a.getGreen() == b.getGreen()
           && a.getBlue() == b.getBlue();
}

class PixelFormatsUnitTests : public UnitTest
{
public:
    PixelFormatsUnitTests() : UnitTest ("PixelFormats", UnitTestCategories::graphics)
    {
    }

    static String createErrorMessage (PixelARGB dest, PixelARGB source, PixelDouble expected, PixelARGB actual)
    {
        String errorMessage;

        errorMessage << toString (dest) << " <- " << toString (source)
                     << " expected: " << toString (expected.toPixelARGB())
                     << " actual: " << toString (actual);

        return errorMessage;
    }

    template <typename Callback>
    static void iterateOverValidPremult (Callback&& func)
    {
        const uint8 channelValues[] = { 0, 1, 3, 127, 128, 129, 254, 255 };

        for (auto sA : channelValues)
        {
            for (auto dA : channelValues)
            {
                for (auto sR : channelValues)
                {
                    if (sR > sA)
                        break;

                    for (auto sG : channelValues)
                    {
                        if (sG > sA)
                            break;

                        for (auto sB : channelValues)
                        {
                            if (sB > sA)
                                break;

                            for (auto dR : channelValues)
                            {
                                if (dR > dA)
                                    break;

                                for (auto dG : channelValues)
                                {
                                    if (dG > dA)
                                        break;

                                    for (auto dB : channelValues)
                                    {
                                        if (dB > dA)
                                            break;

                                        if (! func (PixelARGB (sA, sR, sG, sB), PixelARGB (dA, dR, dG, dB)))
                                            return;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    void runTest() override
    {
        beginTest ("PixelRGBA basic blending: opaque black over white");
        {
            const auto dest = makePixelARGB (255, 255, 255, 255);
            const auto source = makePixelARGB (255, 0, 0, 0);
            const auto expected = makePixelARGB (255, 0, 0, 0);

            auto actual = dest;
            actual.blend (source);

            if (! equals (actual, expected))
            {
                expect (false,
                        "Blending opaque black over opaque white should be opaque black. "
                            + createErrorMessage (dest, source, PixelDouble (expected), actual));
                return;
            }
        }

        beginTest ("PixelRGBA basic blending: transparent black over white");
        {
            auto dest = makePixelARGB (255, 255, 255, 255);
            dest.blend (makePixelARGB (0, 0, 0, 0));

            expect (equals (dest, makePixelARGB (255, 255, 255, 255)),
                    "Blending transparent black over white should be white");
        }

        beginTest ("PixelRGBA basic blending: Rounding errors should never exceed 1 in any channel");
        {
            std::array<int64, 4> errors{};

            iterateOverValidPremult ([this, &errors] (PixelARGB source, PixelARGB dest)
            {
                auto actual = dest;
                actual.blend (source);

                PixelDouble sourcePixel (source);
                PixelDouble expected (dest);
                expected.blend (sourcePixel);

                if (! expected.verifyChannelError (actual, errors))
                {
                    expect (false,
                            "Rounding error in excess of 1 encountered. "
                                + createErrorMessage (dest, source, expected, actual));

                    return false;
                }

                return true;
            });

            const auto expectedErrors = std::array<int64, 4> { 56457, 90792, 90792, 90792 };

            expect (errors == expectedErrors,
                    "Rounding errors unexpectedly changed compared to JUCE 8.0.12. Expected: "
                        + toString (expectedErrors) + " actual: " + toString (errors));
        }

        beginTest ("PixelRGBA blending with extra alpha: Rounding errors should never exceed 2 in any channel");
        {
            std::map<uint32, std::array<int64, 4>> errorsForExtraAlpha
            {
                { 0u,   std::array<int64, 4> {} },
                { 1u,   std::array<int64, 4> { 65617, 327445, 327445, 327445 } },
                { 64u,  std::array<int64, 4> { 422362, 1038848, 1038848, 1038848 } },
                { 128u, std::array<int64, 4> { 447344, 1001165, 1001165, 1001165 } },
                { 192u, std::array<int64, 4> { 379065, 1088930, 1088930, 1088930 } },
                { 244u, std::array<int64, 4> { 356952, 1053672, 1053672, 1053672 } },
                { 255u, std::array<int64, 4> { 352508, 1088989, 1088989, 1088989 } }
            };

            for (auto extraAlpha : { 0u, 1u, 64u, 128u, 192u, 244u, 255u })
            {
                std::array<int64, 4> errors{};

                iterateOverValidPremult ([this, extraAlpha, &errors] (PixelARGB source, PixelARGB dest)
                {
                    auto actual = dest;
                    actual.blend (source, extraAlpha);

                    PixelDouble sourcePixel (source);
                    PixelDouble expected (dest);
                    expected.blend (sourcePixel, extraAlpha);

                    if (! expected.verifyChannelError (actual, errors, 2))
                    {
                        expect (false,
                                "Rounding error in excess of 2 encountered. extraAlpha: "
                                    + String (extraAlpha) + ", "
                                    + createErrorMessage (dest, source, expected, actual));

                        return false;
                    }

                    return true;
                });

                const auto expectedErrors = errorsForExtraAlpha[extraAlpha];

                expect (errors == expectedErrors,
                        "Rounding errors unexpectedly changed compared to JUCE 8.0.12. extraAlpha: "
                            + String (extraAlpha) + ", Expected: "
                            + toString (expectedErrors) + " actual: " + toString (errors));
            }
        }
    }
};

static PixelFormatsUnitTests pixelFormatsUnitTests;

#endif

} // namespace juce
