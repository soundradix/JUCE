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

class VirtualEndpoint::Impl final
{
public:
    class Native
    {
    public:
        virtual ~Native() = default;

        virtual EndpointId getId() const = 0;
        virtual bool setBlock (uint8_t, const Block&) = 0;
        virtual bool setName (const String&) = 0;
    };

    EndpointId getId() const
    {
        return identifier;
    }

    bool setBlock (uint8_t i, const Block& b)
    {
        if (native != nullptr)
            return native->setBlock (i, b);

        return false;
    }

    bool setName (const String& n)
    {
        if (native != nullptr)
            return native->setName (n);

        return false;
    }

    bool isAlive() const
    {
        return native != nullptr;
    }

    static VirtualEndpoint makeVirtualEndpoint (std::unique_ptr<Native> x)
    {
        if (x != nullptr)
            return VirtualEndpoint { rawToUniquePtr (new Impl (std::move (x))) };

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

VirtualEndpoint::VirtualEndpoint() = default;
VirtualEndpoint::~VirtualEndpoint() = default;
VirtualEndpoint::VirtualEndpoint (std::unique_ptr<Impl> x) : impl (std::move (x)) {}
VirtualEndpoint::VirtualEndpoint (VirtualEndpoint&&) noexcept = default;
VirtualEndpoint& VirtualEndpoint::operator= (VirtualEndpoint&&) noexcept = default;

EndpointId VirtualEndpoint::getId() const
{
    if (impl != nullptr)
        return impl->getId();

    return {};
}

bool VirtualEndpoint::setBlock (uint8_t i, const Block& b)
{
    if (impl != nullptr)
        return impl->setBlock (i, b);

    return false;
}

bool VirtualEndpoint::setName (const String& x)
{
    if (impl != nullptr)
        return impl->setName (x);

    return false;
}

bool VirtualEndpoint::isAlive() const
{
    if (impl != nullptr)
        return impl->isAlive();

    return false;
}

}
