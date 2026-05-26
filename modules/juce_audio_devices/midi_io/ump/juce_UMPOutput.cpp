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

class Output::Impl final : private DisconnectionListener
{
public:
    class Native
    {
    public:
        virtual ~Native() = default;

        virtual EndpointId getEndpointId() const = 0;

        virtual bool send (Iterator b, Iterator e) = 0;
    };

    ~Impl() override
    {
        JUCE_ASSERT_MESSAGE_THREAD
    }

    EndpointId getEndpointId() const
    {
        return identifier;
    }

    bool send (Iterator beginIterator, Iterator endIterator)
    {
        return isAlive() && native->send (beginIterator, endIterator);
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
    static Output makeOutput (Callback&& callback)
    {
        auto impl = rawToUniquePtr (new Impl);
        impl->native = callback (static_cast<DisconnectionListener&> (*impl));

        if (impl->native == nullptr)
            return {};

        impl->identifier = impl->native->getEndpointId();
        return Output { std::move (impl) };
    }

private:
    Impl()
    {
        JUCE_ASSERT_MESSAGE_THREAD
    }

    void disconnected() override
    {
        JUCE_ASSERT_MESSAGE_THREAD
        connected = false;
        disconnectListeners.call ([] (auto& x) { x.disconnected(); });
    }

    ListenerList<DisconnectionListener> disconnectListeners;
    std::unique_ptr<Native> native;
    EndpointId identifier;
    std::atomic<bool> connected { true };
};

Output::Output() = default;
Output::~Output() = default;
Output::Output (std::unique_ptr<Impl> x) : impl (std::move (x)) {}
Output::Output (Output&&) noexcept = default;
Output& Output::operator= (Output&&) noexcept = default;

EndpointId Output::getEndpointId() const
{
    // You should ensure that isAlive() returns true before calling other member functions!
    jassert (isAlive());

    if (impl != nullptr)
        return impl->getEndpointId();

    return {};
}

bool Output::send (Iterator beginIterator, Iterator endIterator)
{
    // You should ensure that isAlive() returns true before calling other member functions!
    jassert (isAlive());

    if (impl != nullptr)
        return impl->send (beginIterator, endIterator);

    return false;
}

void Output::addDisconnectionListener (DisconnectionListener& x)
{
    // You should ensure that isAlive() returns true before calling other member functions!
    jassert (isAlive());

    // This function should only be called on the main thread!
    JUCE_ASSERT_MESSAGE_THREAD

    if (impl != nullptr)
        impl->addDisconnectionListener (x);
}

void Output::removeDisconnectionListener (DisconnectionListener& x)
{
    // This function should only be called on the main thread!
    JUCE_ASSERT_MESSAGE_THREAD

    if (impl != nullptr)
        impl->removeDisconnectionListener (x);
}

bool Output::isAlive() const
{
    if (impl != nullptr)
        return impl->isAlive();

    return false;
}

}
