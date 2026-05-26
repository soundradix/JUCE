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

namespace juce::universal_midi_packets
{

class LegacyVirtualOutput::Impl final
{
public:
    class Native
    {
    public:
        virtual ~Native() = default;

        virtual EndpointId getId() const = 0;
    };

    EndpointId getId() const
    {
        return identifier;
    }

    bool isAlive() const
    {
        return native != nullptr;
    }

    static LegacyVirtualOutput makeLegacyVirtualOutput (std::unique_ptr<Native> x)
    {
        if (x != nullptr)
            return LegacyVirtualOutput { rawToUniquePtr (new Impl (std::move (x))) };

        return {};
    }

private:
    explicit Impl (std::unique_ptr<Native> n)
        : native (std::move (n)),
          identifier (native->getId())
    {
    }

    std::unique_ptr<Native> native;
    EndpointId identifier;
};

LegacyVirtualOutput::LegacyVirtualOutput() = default;
LegacyVirtualOutput::~LegacyVirtualOutput() = default;
LegacyVirtualOutput::LegacyVirtualOutput (std::unique_ptr<Impl> x) : impl (std::move (x)) {}
LegacyVirtualOutput::LegacyVirtualOutput (LegacyVirtualOutput&&) noexcept = default;
LegacyVirtualOutput& LegacyVirtualOutput::operator= (LegacyVirtualOutput&&) noexcept = default;

EndpointId LegacyVirtualOutput::getId() const
{
    if (impl != nullptr)
        return impl->getId();

    return {};
}

bool LegacyVirtualOutput::isAlive() const
{
    if (impl != nullptr)
        return impl->isAlive();

    return false;
}

}
