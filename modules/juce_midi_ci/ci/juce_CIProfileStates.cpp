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

namespace juce::midi_ci
{

SupportedAndActive ChannelProfileStates::get (const Profile& profile) const
{
    const auto iter = std::lower_bound (entries.begin(), entries.end(), profile);

    if (iter != entries.end() && iter->profile == profile)
        return iter->state;

    return {};
}

std::vector<Profile> ChannelProfileStates::getActive() const
{
    std::vector<Profile> result;

    for (const auto& item : entries)
        if (item.state.isActive())
            result.push_back (item.profile);

    return result;
}

std::vector<Profile> ChannelProfileStates::getInactive() const
{
    std::vector<Profile> result;

    for (const auto& item : entries)
        if (item.state.isSupported())
            result.push_back (item.profile);

    return result;
}

void ChannelProfileStates::set (const Profile& profile, SupportedAndActive state)
{
    const auto iter = std::lower_bound (entries.begin(), entries.end(), profile);

    if (iter != entries.end() && iter->profile == profile)
    {
        if (state != SupportedAndActive{})
            iter->state = state;
        else
            entries.erase (iter);
    }
    else if (state != SupportedAndActive{})
    {
        entries.insert (iter, { profile, state });
    }
}

void ChannelProfileStates::erase (const Profile& profile)
{
    const auto iter = std::lower_bound (entries.begin(), entries.end(), profile);

    if (iter != entries.end() && iter->profile == profile)
        entries.erase (iter);
}

} // namespace juce::midi_ci
