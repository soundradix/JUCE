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

/**
    A key used to uniquely identify ongoing transactions initiated by a ci::Device.

    @tags{Audio}
*/
class RequestKey
{
    auto tie() const;

public:
    /** Constructor. */
    RequestKey (MUID muid, Token64 key) : m (muid), v (key) {}

    /** Returns the muid of the device to which we are subscribed. */
    MUID getMuid() const { return m; }

    /** Returns an identifier unique to this subscription. */
    Token64 getKey() const { return v; }

    /** Equality operator. */
    bool operator== (const RequestKey& other) const;

    /** Inequality operator. */
    bool operator!= (const RequestKey& other) const;

    /** Less-than operator. */
    bool operator<  (const RequestKey& other) const;

private:
    MUID m;
    Token64 v{};
};

/**
    Acting as a ResponderListener, instances of this class can formulate
    appropriate replies to property transactions initiated by remote devices.

    PropertyHost instances also contain methods to inform remote devices about
    changes to local property state.

    Keeps track of property subscriptions requested by remote devices.

    @tags{Audio}
*/
class PropertyHost final : public ResponderDelegate
{
public:
    /** @internal

        Rather than constructing one of these objects yourself, you should configure
        a Device with property exchange support, and then use Device::getPropertyHost()
        to retrieve a property host that has been set up to work with that device.
    */
    PropertyHost (FunctionBlock fb, PropertyDelegate& d, BufferOutput& o, CacheProvider& p)
        : functionBlock (fb), delegate (d), output (o), cacheProvider (p) {}

    /** Sends a "Subscription" message from a device, when acting as a
        subscription responder. You should call this for all registered
        subscribers whenever the subscribed property is modified in a way that
        remote devices don't know about (if a remote device requests a
        property update, there's no need to send a subscription update after
        changing the property accordingly).

        You should *not* attempt to start a new subscription on another device
        using this function. Valid subscription commands are "full", "partial",
        and "notify". Check the property exchange specification for the intended
        use of these commands.

        To terminate a subscription that was initiated by a remote device,
        use terminateSubscription().

        The provided callback will be called once the remote device has confirmed
        receipt of the subscription update. If the state of your application
        changes such that you no longer need to respond/wait for confirmation,
        you can pass the request key to Device::abortPropertyRequest().
    */
    std::optional<RequestKey> sendSubscriptionUpdate (MUID device,
                                                      const PropertySubscriptionHeader& header,
                                                      Span<const std::byte> body,
                                                      std::function<void (const PropertyExchangeResult&)> callback);

    /** Terminates a subscription that was started by a remote device.

        This may be useful if your application has properties that can be
        added and removed - you can terminate subscriptions to subscribed
        properties before removing those properties.
    */
    void terminateSubscription (MUID device, const String& subscribeId);

    /** Returns a set of subscribed resources.

        This set contains all active subscriptionIDs for the given device,
        along with the resources to which those subscriptionIDs refer.
    */
    std::set<Subscription> findSubscriptionsForDevice (MUID device) const;

    /** Returns the number of transactions that have been initiated by other devices, but not yet
        completed, normally because the request has been split into several messages.
    */
    int countOngoingTransactions() const;

    /** @internal */
    bool tryRespond (ResponderOutput&, const Message::Parsed&) override;

private:
    class Visitor;

    struct SubscriptionToken
    {
        size_t uid{};

        bool operator<  (const SubscriptionToken& other) const { return uid <  other.uid; }
        bool operator== (const SubscriptionToken& other) const { return uid == other.uid; }
        bool operator!= (const SubscriptionToken& other) const { return uid != other.uid; }
    };

    static SubscriptionToken uidFromSubscribeId (String id);
    static String subscribeIdFromUid (SubscriptionToken uid);
    static SubscriptionToken findUnusedSubscribeId (const std::map<SubscriptionToken, String>& used);

    FunctionBlock functionBlock;
    PropertyDelegate& delegate;
    BufferOutput& output;
    CacheProvider& cacheProvider;

    std::map<MUID, std::map<SubscriptionToken, String>> registry;
};

} // namespace juce::midi_ci
