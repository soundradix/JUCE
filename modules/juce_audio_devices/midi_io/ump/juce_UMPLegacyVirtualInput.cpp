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

class LegacyVirtualInput::Impl final
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

    static LegacyVirtualInput makeLegacyVirtualInput (std::unique_ptr<Native> x)
    {
        if (x != nullptr)
            return LegacyVirtualInput { rawToUniquePtr (new Impl (std::move (x))) };

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

LegacyVirtualInput::LegacyVirtualInput() = default;
LegacyVirtualInput::~LegacyVirtualInput() = default;
LegacyVirtualInput::LegacyVirtualInput (std::unique_ptr<Impl> x) : impl (std::move (x)) {}
LegacyVirtualInput::LegacyVirtualInput (LegacyVirtualInput&&) noexcept = default;
LegacyVirtualInput& LegacyVirtualInput::operator= (LegacyVirtualInput&&) noexcept = default;

EndpointId LegacyVirtualInput::getId() const
{
    if (impl != nullptr)
        return impl->getId();

    return {};
}

bool LegacyVirtualInput::isAlive() const
{
    if (impl != nullptr)
        return impl->isAlive();

    return false;
}

}
