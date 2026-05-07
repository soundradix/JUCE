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

std::map<uint32_t, String> VST3ClientExtensions::getCompatibleParameterIds (const VST3Interface::Id&) const
{
    return {};
}

uint32_t VST3ClientExtensions::convertJuceParameterId (const String& parameterId, bool studioOneCompatible)
{
    auto hash = (uint32_t) (parameterId.hashCode());

    if (studioOneCompatible)
        hash &= 0x7fffffff;

    return hash;
}

} // namespace juce
