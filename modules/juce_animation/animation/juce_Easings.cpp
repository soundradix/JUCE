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

std::function<float (float)> Easings::createCubicBezier (float x1, float y1, float x2, float y2)
{
    // The x axis represents time, it's important this always stays in the range 0 - 1
    jassert (isPositiveAndNotGreaterThan (x1, 1.0f));
    jassert (isPositiveAndNotGreaterThan (x2, 1.0f));

    chromium::gfx::CubicBezier cubicBezier { (double) x1, (double) y1, (double) x2, (double) y2 };
    return [bezier = std::move (cubicBezier)] (float v) { return (float) bezier.Solve (v); };
}

std::function<float (float)> Easings::createCubicBezier (Point<float> controlPoint1,
                                                         Point<float> controlPoint2)
{
    return createCubicBezier (controlPoint1.getX(),
                              controlPoint1.getY(),
                              controlPoint2.getX(),
                              controlPoint2.getY());
}

std::function<float (float)> Easings::createEase()
{
    const static auto f = createCubicBezier (0.25f, 0.1f, 0.25f, 1.0f);
    return f;
}

std::function<float (float)> Easings::createEaseIn()
{
    const static auto f = createCubicBezier (0.42f, 0.0f, 1.0f, 1.0f);
    return f;
}

std::function<float (float)> Easings::createEaseOut()
{
    const static auto f = createCubicBezier (0.0f, 0.0f, 0.58f, 1.0f);;
    return f;
}

std::function<float (float)> Easings::createEaseInOut()
{
    const static auto f = createCubicBezier (0.42f, 0.0f, 0.58f, 1.0f);
    return f;
}

std::function<float (float)> Easings::createLinear()
{
    return [] (auto x){ return x; };
}

std::function<float (float)> Easings::createEaseOutBack()
{
    const static auto f = createCubicBezier (0.34f, 1.56f, 0.64f, 1.0f);
    return f;
}

std::function<float (float)> Easings::createEaseInOutCubic()
{
    const static auto f = createCubicBezier (0.65f, 0.0f, 0.35f, 1.0f);
    return f;
}

std::function<float (float)> Easings::createSpring (const SpringEasingOptions& options)
{
    return [=] (float v)
    {
        const auto t = std::clamp (v, 0.0f, 1.0f);
        const auto omega = 2.0f * MathConstants<float>::pi * options.getFrequency();
        const auto physicalValue = 1.0f - std::exp (-options.getAttenuation() * t) * std::cos (omega * t);
        const auto squish = 1.0f / options.getExtraAttenuationRange();
        const auto shift = 1.0f - options.getExtraAttenuationRange();
        const auto weight = std::clamp (std::pow (squish * (std::max (t - shift, 0.0f)), 2.0f), 0.0f, 1.0f);
        return weight + (1.0f - weight) * physicalValue;
    };
}

std::function<float (float)> Easings::createBounce (int numBounces)
{
    jassert (numBounces >= 0);
    numBounces = std::max (0, numBounces);

    const auto alpha = std::pow (0.05f, 1.0f / (float) numBounces);

    const auto fallTime = [] (float h)
    {
        return std::sqrt (2.0f * h);
    };

    std::vector<float> bounceTimes;
    bounceTimes.reserve ((size_t) (numBounces + 1));
    bounceTimes.push_back (fallTime (1.0f));

    for (int i = 1; i < numBounces + 1; ++i)
        bounceTimes.push_back (bounceTimes.back() + 2.0f * fallTime (std::pow (alpha, (float) i)));

    for (auto& bounce : bounceTimes)
        bounce /= bounceTimes.back();

    return [alpha, times = std::move (bounceTimes)] (float v)
    {
        v = std::clamp (v, 0.0f, 1.0f);

        const auto boundIt = std::lower_bound (times.begin(), times.end(), v);

        if (boundIt == times.end())
            return 1.0f;

        const auto i = (size_t) std::distance (times.begin(), boundIt);
        const auto height = i == 0 ? 1.0f : std::pow (alpha, (float) i);
        const auto center = i == 0 ? 0.0f : (times[i] + times[i - 1]) / 2.0f;
        const auto distToZero = i == 0 ? times[i] : (times[i] - times[i - 1]) / 2.0f;
        return 1.0f - height * (1.0f - std::pow (1.0f / distToZero * (v - center), 2.0f));
    };
}

std::function<float (float)> Easings::createOnOffRamp()
{
    return [] (float x) { return 1.0f - std::abs (2.0f * (x - 0.5f)); };
}

} // namespace juce
