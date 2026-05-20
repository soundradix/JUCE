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

namespace juce::detail
{

/*  The two-point conical gradient is defined by two circles. */
struct GradientCircle
{
    Point<float> c;
    float r{};

    GradientCircle transformedBy (const AffineTransform& t) const;
};

/*  Helper class to correctly interpret the parameters of a radial gradient. */
struct RadialGradientView
{
    RadialGradientView (const ColourGradient* gradientIn)
        : gradient (*gradientIn)
    {
        jassert (gradientIn != nullptr);
        jassert (gradient.isRadial);
    }

    /*  The end circle. This defines the outline of the gradient.

        This corresponds to the 'cx', 'cy' and 'r' SVG parameters.
    */
    GradientCircle getEndCircle() const;

    /*  The start/focus circle.

        This corresponds to the 'fx', 'fy' and 'fr' SVG parameters.
    */
    GradientCircle getStartCircle() const;
private:
    bool isLegacy() const noexcept;

    const ColourGradient& gradient;
};

struct SpreadMethods
{
    SpreadMethods() = delete;

    static constexpr float pad (float t) noexcept
    {
        return std::clamp (t, 0.0f, 1.0f);
    }

    static float reflect (float t) noexcept
    {
        float integral{};
        const auto fractional = std::modf (std::max (0.0f, t), &integral);

        return (((int) integral) % 2 == 0) ? fractional : (1.0f - fractional);
    }

    static float repeat (float t) noexcept
    {
        float integral{};
        return std::modf (std::max (0.0f, t), &integral);
    }

    static auto* getFunction (ColourGradient::SpreadMethod method) noexcept
    {
        switch (method)
        {
            case ColourGradient::SpreadMethod::pad:     return &pad;
            case ColourGradient::SpreadMethod::reflect: return &reflect;
            case ColourGradient::SpreadMethod::repeat:  return &repeat;
        }

        jassertfalse;
        return &pad;
    }
};

/*  Helper class for implementing https://skia.org/docs/dev/design/conical/

    Variable naming follows the notations used in the article whenever makes sense.

    ---

    https://www.w3.org/TR/2015/WD-SVG2-20150915/pservers.html#RadialGradients
*/
class TwoPointConicalGradient
{
public:
    static inline const auto regularisationTolerance = absoluteTolerance (0.001f);
    static inline const auto inputTolerance = relativeTolerance (0.001f);

    /*  Returns an object if this class can compute the provided gradient object.

        Returns std::nullopt, if the provided parameters would be a degenerate case for the
        two-point conical equations.
    */
    static std::optional<TwoPointConicalGradient> create (const ColourGradient& gradient);

    /*  Returns std::nullopt if the point is outside the calculable range.

        This happens if the start circle touches, or falls outside the end circle, and the point is
        outside the cone.
    */
    std::optional<float> calculateWeight (Point<float> p) const;

private:
    TwoPointConicalGradient (GradientCircle startCircle, GradientCircle endCircle);

    static bool isConicalGradientApplicable (const GradientCircle& s, const GradientCircle& e);

    bool swapped = false;
    GradientCircle c0, c1;
    float f {};
    AffineTransform regularisationTransform;
};

} // namespace juce::detail
