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

#if JucePlugin_Enable_ARA

// Include ARA SDK headers
JUCE_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wgnu-zero-variadic-macro-arguments",
                                     "-Wunused-parameter",
                                     "-Wfloat-equal")
JUCE_BEGIN_IGNORE_WARNINGS_MSVC (6387)

#include <ARA_Library/PlugIn/ARAPlug.h>

JUCE_END_IGNORE_WARNINGS_GCC_LIKE
JUCE_END_IGNORE_WARNINGS_MSVC

namespace juce
{

using ARAViewSelection = ARA::PlugIn::ViewSelection;
using ARAContentUpdateScopes = ARA::ContentUpdateScopes;
using ARARestoreObjectsFilter = ARA::PlugIn::RestoreObjectsFilter;
using ARAStoreObjectsFilter = ARA::PlugIn::StoreObjectsFilter;

/** Converts an ARA::ARAUtf8String to a JUCE String. */
inline String convertARAString (ARA::ARAUtf8String str)
{
    return String (CharPointer_UTF8 (str));
}

/** Converts a potentially NULL ARA::ARAUtf8String to a JUCE String.

    Returns the JUCE equivalent of the provided string if it's not nullptr, and the fallback string
    otherwise.
*/
inline String convertOptionalARAString (ARA::ARAUtf8String str, const String& fallbackString = String())
{
    return (str != nullptr) ? convertARAString (str) : fallbackString;
}

/** Converts an ARA::ARAColor* to a JUCE Colour. */
inline uint32 convertARAColourARGB (const ARA::ARAColor* colour)
{
    static constexpr auto floatToByte = [] (float x)
    {
        return (uint32) jlimit (0, 255, roundToInt (x * 255.0f));
    };

    return 0xff000000
         | (floatToByte (colour->r) << 0x10)
         | (floatToByte (colour->g) << 0x08)
         | (floatToByte (colour->b) << 0x00);
}

/** Converts a potentially NULL ARA::ARAColor* to a JUCE Colour.

    Returns the JUCE equivalent of the provided colour if it's not nullptr, and the fallback colour
    otherwise.
*/
inline uint32 convertOptionalARAColour (const ARA::ARAColor* colour, uint32 fallbackColour = {})
{
    return (colour != nullptr) ? convertARAColourARGB (colour) : fallbackColour;
}

} // namespace juce

#include "juce_ARAModelObjects.h"
#include "juce_ARADocumentController.h"
#include "juce_AudioProcessor_ARAExtensions.h"
#include "juce_ARAPlugInInstanceRoles.h"

#endif
