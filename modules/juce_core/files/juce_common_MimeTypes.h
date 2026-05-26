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

#pragma once

namespace juce::detail
{

struct MimeTypeTable
{
    static void registerCustomMimeTypeForFileExtension (const String& mimeType, const String& fileExtension);

    static StringArray getMimeTypesForFileExtension (const String& fileExtension);

    static StringArray getFileExtensionsForMimeType (const String& mimeType);
};

} // namespace juce::detail
