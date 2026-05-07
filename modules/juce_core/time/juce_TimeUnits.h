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

/** A type used to store years with double precision. */
using Years = std::chrono::duration<double, std::ratio<31556952>>;

/** A type used to store months with double precision. */
using Months = std::chrono::duration<double, std::ratio<2629746>>;

/** A type used to store weeks with double precision. */
using Weeks = std::chrono::duration<double, std::ratio<604800>>;

/** A type used to store days with double precision. */
using Days = std::chrono::duration<double, std::ratio<86400>>;

/** A type used to store hours with double precision. */
using Hours = std::chrono::duration<double, std::ratio<3600>>;

/** A type used to store minutes with double precision. */
using Minutes = std::chrono::duration<double, std::ratio<60>>;

/** A type used to store seconds with double precision. */
using Seconds = std::chrono::duration<double, std::ratio<1>>;

/** A type used to store milliseconds with double precision. */
using Milliseconds = std::chrono::duration<double, std::milli>;

/** A type used to store microseconds with double precision. */
using Microseconds = std::chrono::duration<double, std::micro>;

/** A type used to store nanoseconds with double precision. */
using Nanoseconds = std::chrono::duration<double, std::nano>;

} // namespace juce
