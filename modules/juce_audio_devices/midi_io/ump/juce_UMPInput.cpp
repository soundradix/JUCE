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

class Input::Impl final : private Consumer,
                          private DisconnectionListener
{
public:
    class Native
    {
    public:
        virtual ~Native() = default;

        /*  Returns the ID of the endpoint to which this connection is connected. */
        virtual EndpointId getEndpointId() const = 0;

        /*  The protocol to which incoming messages are converted. */
        virtual PacketProtocol getProtocol() const = 0;
    };

    ~Impl() override
    {
        JUCE_ASSERT_MESSAGE_THREAD
    }

    EndpointId getEndpointId() const
    {
        return identifier;
    }

    PacketProtocol getProtocol() const
    {
        return protocol;
    }

    void addConsumer (Consumer& x)
    {
        consumers.add (x);
    }

    void removeConsumer (Consumer& x)
    {
        consumers.remove (x);
    }

    void addDisconnectionListener (DisconnectionListener& x)
    {
        disconnectListeners.add (&x);
    }

    void removeDisconnectionListener (DisconnectionListener& x)
    {
        disconnectListeners.remove (&x);
    }

    bool isAlive() const
    {
        return connected && native != nullptr;
    }

    template <typename Callback>
    static Input makeInput (Callback&& callback)
    {
        auto impl = rawToUniquePtr (new Impl);
        impl->native = callback (static_cast<DisconnectionListener&> (*impl), static_cast<Consumer&> (*impl));

        if (impl->native == nullptr)
            return {};

        impl->identifier = impl->native->getEndpointId();
        impl->protocol = impl->native->getProtocol();
        return Input { std::move (impl) };
    }

private:
    Impl()
    {
        JUCE_ASSERT_MESSAGE_THREAD
    }

    void consume (Iterator b, Iterator e, double t) override
    {
        consumers.call ([&] (auto& l) { l.consume (b, e, t); });
    }

    void disconnected() override
    {
        JUCE_ASSERT_MESSAGE_THREAD
        connected = false;
        disconnectListeners.call ([] (auto& x) { x.disconnected(); });
    }

    ListenerList<DisconnectionListener> disconnectListeners;
    WaitFreeListeners<Consumer> consumers;
    EndpointId identifier;
    PacketProtocol protocol;
    std::unique_ptr<Native> native;
    std::atomic<bool> connected { true };
};

Input::Input() = default;
Input::~Input() = default;
Input::Input (std::unique_ptr<Impl> x) : impl (std::move (x)) {}
Input::Input (Input&&) noexcept = default;
Input& Input::operator= (Input&&) noexcept = default;

EndpointId Input::getEndpointId() const
{
    if (impl != nullptr)
        return impl->getEndpointId();

    return {};
}

PacketProtocol Input::getProtocol() const
{
    // You should ensure that isAlive() returns true before calling other member functions!
    jassert (isAlive());

    if (impl != nullptr)
        return impl->getProtocol();

    return {};
}

void Input::addConsumer (Consumer& x)
{
    // You should ensure that isAlive() returns true before calling other member functions!
    jassert (isAlive());

    // This function should only be called on the main thread!
    JUCE_ASSERT_MESSAGE_THREAD

    if (impl != nullptr)
        impl->addConsumer (x);
}

void Input::removeConsumer (Consumer& x)
{
    // This function should only be called on the main thread!
    JUCE_ASSERT_MESSAGE_THREAD

    if (impl != nullptr)
        impl->removeConsumer (x);
}

void Input::addDisconnectionListener (DisconnectionListener& x)
{
    // You should ensure that isAlive() returns true before calling other member functions!
    jassert (isAlive());

    // This function should only be called on the main thread!
    JUCE_ASSERT_MESSAGE_THREAD

    if (impl != nullptr)
        impl->addDisconnectionListener (x);
}

void Input::removeDisconnectionListener (DisconnectionListener& x)
{
    // This function should only be called on the main thread!
    JUCE_ASSERT_MESSAGE_THREAD

    if (impl != nullptr)
        impl->removeDisconnectionListener (x);
}

bool Input::isAlive() const
{
    if (impl != nullptr)
        return impl->isAlive();

    return false;
}

}
