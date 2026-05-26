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

//==============================================================================
/**
    Helper class for using linear interpolation between a begin and an end value.

    The ValueType could be any numerical type, or a std::tuple containing numerical types. This
    class is mainly intended to be used with the latter.

    This way you can interpolate multiple values by supplying a single float value, which you can
    access in an Animator's value change callback.

    E.g.
    @code
    const auto boundsToTuple = [] (auto b)
    {
        return std::make_tuple (b.getX(), b.getY(), b.getWidth(), b.getHeight());
    };

    const auto begin = boundsToTuple (component.getBoundsInParent());
    const auto end   = boundsToTuple (targetBounds);
    const auto limits = makeAnimationLimits (begin, end);

    // This is the value change callback of an Animator, where you will transition a Component from
    // one bounds to the next. See the AnimatorsDemo for a more detailed example.
    const auto valueChanged = [&component, limits] (auto v)
    {
        const auto [x, y, w, h] = limits.lerp (v);
        component.setBounds (x, y, w, h);
    };
    @endcode

    @see ValueAnimatorBuilder::ValueChangedCallback

    @tags{Animations}
*/
template <typename ValueType>
class JUCE_API  StaticAnimationLimits
{
public:
    /** Constructor. You can use it to interpolate between a 0 initialised numerical value or tuple
        and the provided end state.
    */
    explicit StaticAnimationLimits (const ValueType& endIn)
        : StaticAnimationLimits ({}, endIn) {}

    /** Constructor. Creates an object that will interpolate between the two provided beginning and
        end states. The ValueType can be a numerical type or a std::tuple containing numerical
        types.
    */
    StaticAnimationLimits (const ValueType& beginIn, const ValueType& endIn)
        : begin (beginIn), end (endIn) {}

    /** Evaluation operator. Returns a value that is a linear interpolation of the beginning and end
        state. It's a shorthand for the lerp() function.
    */
    ValueType operator() (float value) const
    {
        return lerp (value);
    }

    /** Returns a value that is a linear interpolation of the beginning and end state.
    */
    ValueType lerp (float value) const
    {
        using namespace detail::ArrayAndTupleOps;

        if constexpr (std::is_integral_v<ValueType>)
            return (ValueType) std::round ((float) begin + ((float) (end - begin) * value));
        else
            return (ValueType) (begin + ((end - begin) * value));
    }

private:
    ValueType begin{}, end{};
};

/** Creates an instance of StaticAnimationLimits, deducing ValueType from
    the function argument.
*/
template <typename ValueType>
StaticAnimationLimits<ValueType> makeAnimationLimits (const ValueType& end)
{
    return StaticAnimationLimits<ValueType> (end);
}

/** Creates an instance of StaticAnimationLimits, deducing ValueType from
    the function arguments.
*/
template <typename ValueType>
StaticAnimationLimits<ValueType> makeAnimationLimits (const ValueType& begin, const ValueType& end)
{
    return StaticAnimationLimits<ValueType> (begin, end);
}

} // namespace juce
