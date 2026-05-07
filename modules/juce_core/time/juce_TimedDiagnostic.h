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
    An object for storing and measuring durations for diagnostic purposes.

    This object is designed to have minimal performance impact so that it
    doesn't distort results and is safe to use even in real-time audio contexts.

    @tags{Core}
*/
class JUCE_API TimedDiagnostic
{
public:
    /** Returns the stored duration, converted to the requested time unit, as a
        numeric value.

        For example:
        - get<Milliseconds>() returns the stored duration expressed in milliseconds
        - get<Microseconds>() returns the stored duration expressed in microseconds
    */
    template <typename TimeUnit>
    auto get() const
    {
        return std::chrono::duration_cast<TimeUnit> (Seconds { value }).count();
    }

    /** Sets the stored duration from the given value.

        The provided duration may be expressed in any time unit, such as
        Seconds, Milliseconds, or Microseconds.
    */
    template <typename TimeUnit>
    void set (const TimeUnit& newValue)
    {
        value = std::chrono::duration_cast<Seconds> (newValue).count();
    }

    /** Creates a scoped timer that stores the elapsed time in this diagnostic.

        The returned object measures the time between its construction and
        destruction, and sets that elapsed duration to this TimedDiagnostic.
    */
    ScopedTimeMeasurement createTimer() &
    {
        return ScopedTimeMeasurement { value };
    }

    /** Returns true if this diagnostic does not currently store any measured
        time.
    */
    bool isEmpty() const
    {
        return exactlyEqual (value, 0.0);
    }

    /** Returns the sum of this diagnostic and another diagnostic. */
    TimedDiagnostic operator+ (TimedDiagnostic other) const
    {
        return TimedDiagnostic { *this } += other;
    }

    /** Returns the difference between this diagnostic and another diagnostic. */
    TimedDiagnostic operator- (TimedDiagnostic other) const
    {
        return TimedDiagnostic { *this } -= other;
    }

    /** Adds another diagnostic's stored duration to this one. */
    TimedDiagnostic& operator+= (TimedDiagnostic other)
    {
        value += other.value;
        return *this;
    }

    /** Subtracts another diagnostic's stored duration from this one. */
    TimedDiagnostic& operator-= (TimedDiagnostic other)
    {
        value -= other.value;
        return *this;
    }

    // Creating a timer from a temporary is a bad idea, since the reference to
    // 'value' is likely to dangle when the timer completes.
    ScopedTimeMeasurement createTimer() && = delete;

private:
    double value{};
};

} // namespace juce
