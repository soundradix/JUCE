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

class Session::Impl
{
public:
    class Native
    {
    public:
        virtual ~Native() = default;

        virtual String getName() const = 0;
        virtual std::unique_ptr<Input::Impl::Native> connectInput (DisconnectionListener&,
                                                                   const EndpointId&,
                                                                   PacketProtocol,
                                                                   Consumer&) = 0;
        virtual std::unique_ptr<Output::Impl::Native> connectOutput (DisconnectionListener&,
                                                                     const EndpointId&) = 0;

        /*  Creates a full MIDI 2.0 UMP endpoint, or returns nullptr on failure. */
        virtual std::unique_ptr<VirtualEndpoint::Impl::Native> createNativeVirtualEndpoint (const String&,
                                                                                            const DeviceInfo&,
                                                                                            const String&,
                                                                                            PacketProtocol,
                                                                                            Span<const Block>,
                                                                                            BlocksAreStatic)
        {
            // If this is hit, you're trying to create a virtual (app-to-app) MIDI 2.0 endpoint, but
            // the current MIDI backend does not implement this feature.
            jassertfalse;
            return {};
        }

        /*  Creates a virtual MIDI 1.0 port. This is intended for use on platforms that don't
            support virtual MIDI 2.0.
        */
        virtual std::unique_ptr<LegacyVirtualInput::Impl::Native> createLegacyVirtualInput (const String&)
        {
            // If this is hit, you're trying to create a virtual (app-to-app) MIDI 1.0 endpoint, but
            // the current MIDI backend does not implement this feature.
            jassertfalse;
            return {};
        }

        /*  Creates a virtual MIDI 1.0 port. This is intended for use on platforms that don't
            support virtual MIDI 2.0.
        */
        virtual std::unique_ptr<LegacyVirtualOutput::Impl::Native> createLegacyVirtualOutput (const String&)
        {
            // If this is hit, you're trying to create a virtual (app-to-app) MIDI 1.0 endpoint, but
            // the current MIDI backend does not implement this feature.
            jassertfalse;
            return {};
        }
    };

    String getName() const
    {
        return native->getName();
    }

    Input makeInput (const EndpointId& endpointId, PacketProtocol protocol)
    {
        return Input::Impl::makeInput ([&] (DisconnectionListener& d, Consumer& c)
        {
            return native->connectInput (d, endpointId, protocol, c);
        });
    }

    Output makeOutput (const EndpointId& endpointId)
    {
        return Output::Impl::makeOutput ([&] (DisconnectionListener& d)
        {
            return native->connectOutput (d, endpointId);
        });
    }

    VirtualEndpoint createVirtualEndpoint (const String& name,
                                           const DeviceInfo& info,
                                           const String& productInstance,
                                           PacketProtocol protocol,
                                           Span<const Block> blocks,
                                           BlocksAreStatic areStatic)
    {
        if (blocks.size() > 32)
        {
            // UMP endpoints support a maximum of 32 function blocks
            jassertfalse;
            return {};
        }

        if (name.getNumBytesAsUTF8() > 98)
        {
            // Per the spec, there's a length restriction on endpoint names
            jassertfalse;
            return {};
        }

        const auto isBlockNameTooLong = [] (const Block& b)
        {
            return b.getName().getNumBytesAsUTF8() > 91;
        };

        if (std::any_of (blocks.begin(), blocks.end(), isBlockNameTooLong))
        {
            // Per the spec, there's a length restriction on block names
            jassertfalse;
            return {};
        }

        const auto isBlockDisabled = [] (const Block& b) { return ! b.isEnabled(); };

        if (areStatic == BlocksAreStatic::yes && std::any_of (blocks.begin(), blocks.end(), isBlockDisabled))
        {
            // You may not request a disabled function block if the function block topology is static
            jassertfalse;
            return {};
        }

        auto umpEndpointNative = native->createNativeVirtualEndpoint (name,
                                                                      info,
                                                                      productInstance,
                                                                      protocol,
                                                                      blocks,
                                                                      areStatic);

        if (umpEndpointNative == nullptr)
            return {};

        auto result = VirtualEndpoint::Impl::makeVirtualEndpoint (std::move (umpEndpointNative));

        if (! result)
            return {};

       #if JUCE_ASSERTIONS_ENABLED_OR_LOGGED
        if (const auto endpoint = Endpoints::getInstance()->getEndpoint (result.getId()))
        {
            jassert (endpoint->getName() == name);
            jassert (endpoint->getProductInstanceId() == productInstance);
            jassert (endpoint->getProtocol() == protocol);
            jassert (endpoint->hasStaticBlocks() == (areStatic == BlocksAreStatic::yes));
            jassert (std::equal (blocks.begin(),
                                 blocks.end(),
                                 endpoint->getBlocks().begin(),
                                 endpoint->getBlocks().end()));
        }
        else
        {
            // Unable to find this endpoint, even though we just created it!
            jassertfalse;
        }

        if (const auto staticInfo = Endpoints::getInstance()->getStaticDeviceInfo (result.getId()))
        {
            jassert (staticInfo->getTransport() == Transport::ump);
        }
        else
        {
            jassertfalse;
        }
       #endif

        return result;
    }

    LegacyVirtualInput createLegacyVirtualInput (const String& name)
    {
        if (auto result = native->createLegacyVirtualInput (name))
        {
            [[maybe_unused]] const auto id = result->getId();
            jassert (id.dst.isNotEmpty());
            return LegacyVirtualInput::Impl::makeLegacyVirtualInput (std::move (result));
        }

        return {};
    }

    LegacyVirtualOutput createLegacyVirtualOutput (const String& name)
    {
        if (auto result = native->createLegacyVirtualOutput (name))
        {
            [[maybe_unused]] const auto id = result->getId();
            jassert (id.src.isNotEmpty());
            return LegacyVirtualOutput::Impl::makeLegacyVirtualOutput (std::move (result));
        }

        return {};
    }

    static Session makeSession (std::unique_ptr<Native> x)
    {
        if (x == nullptr)
            return Session { nullptr };

        return Session { rawToUniquePtr (new Impl (std::move (x))) };
    }

private:
    explicit Impl (std::unique_ptr<Native> n) : native (std::move (n)) {}

    // Order of data members is important to ensure that all inputs+outputs are destroyed before the
    // native session
    std::unique_ptr<Native> native;
};

String Session::getName() const
{
    if (impl != nullptr)
        return impl->getName();

    return {};
}

Input Session::connectInput (const EndpointId& x, PacketProtocol p)
{
    if (impl != nullptr)
        return impl->makeInput (x, p);

    return {};
}

Output Session::connectOutput (const EndpointId& x)
{
    if (impl != nullptr)
        return impl->makeOutput (x);

    return {};
}

VirtualEndpoint Session::createVirtualEndpoint (const String& name,
                                                const DeviceInfo& deviceInfo,
                                                const String& productInstanceID,
                                                PacketProtocol protocol,
                                                Span<const Block> initialBlocks,
                                                BlocksAreStatic areStatic)
{
    if (impl == nullptr)
        return {};

    return impl->createVirtualEndpoint (name,
                                        deviceInfo,
                                        productInstanceID,
                                        protocol,
                                        initialBlocks,
                                        areStatic);
}

LegacyVirtualInput Session::createLegacyVirtualInput (const juce::String& name)
{
    if (impl == nullptr)
        return {};

    return impl->createLegacyVirtualInput (name);
}

LegacyVirtualOutput Session::createLegacyVirtualOutput (const juce::String& name)
{
    if (impl == nullptr)
        return {};

    return impl->createLegacyVirtualOutput (name);
}

bool Session::isAlive() const
{
    return impl != nullptr;
}

Session::Session (std::shared_ptr<Impl> x) : impl (std::move (x)) {}
Session::Session (const Session&) = default;
Session::Session (Session&&) noexcept = default;
Session& Session::operator= (const Session&) = default;
Session& Session::operator= (Session&&) noexcept = default;
Session::~Session() = default;

}
