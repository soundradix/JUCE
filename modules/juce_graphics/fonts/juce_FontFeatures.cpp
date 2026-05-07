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

constexpr auto FontFeatureSetting::tie() const
{
    return std::tuple (tag, value);
}

constexpr bool FontFeatureSetting::operator< (const FontFeatureSetting& other) const
{
    return tie() <  other.tie();
}

constexpr bool FontFeatureSetting::operator<= (const FontFeatureSetting& other) const
{
    return tie() <= other.tie();
}

constexpr bool FontFeatureSetting::operator> (const FontFeatureSetting& other) const
{
    return tie() >  other.tie();
}

constexpr bool FontFeatureSetting::operator>= (const FontFeatureSetting& other) const
{
    return tie() >= other.tie();
}

constexpr bool FontFeatureSetting::operator== (const FontFeatureSetting& other) const
{
    return tie() == other.tie();
}

constexpr bool FontFeatureSetting::operator!= (const FontFeatureSetting& other) const
{
    return tie() != other.tie();
}

//==============================================================================
FontFeatureTag FontFeatureTag::fromString (const String& string)
{
    if (string.getNumBytesAsUTF8() != 4)
    {
        jassertfalse;
        return FontFeatureTag (0);
    }

    char characters[5]{};
    memcpy (characters, string.toRawUTF8(), 4);
    return FontFeatureTag { characters };
}

String FontFeatureTag::toString() const
{
    const char characters[5]
    {
        static_cast<char> (tag >> 24),
        static_cast<char> (tag >> 16),
        static_cast<char> (tag >> 8),
        static_cast<char> (tag),
        '\0'
    };

    return String { characters };
}

} // namespace juce
