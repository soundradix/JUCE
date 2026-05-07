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

#if JUCE_WEB_BROWSER

WebSliderRelay::WebSliderRelay (StringRef nameIn)
    : name (nameIn)
{
}

void WebSliderRelay::setValue (float newValue)
{
    using namespace detail;

    DynamicObject::Ptr object { new DynamicObject };
    object->setProperty (WebSliderRelayEvents::Event::eventTypeKey,
                         WebSliderRelayEvents::ValueChanged::eventId.toString());
    object->setProperty (WebSliderRelayEvents::ValueChanged::newValueKey, newValue);

    value = newValue;
    emitEvent (object.get());
}

float WebSliderRelay::getValue() const
{
    return value;
}

void WebSliderRelay::addListener (Listener* l)
{
    listeners.add (l);
}

void WebSliderRelay::removeListener (Listener* l)
{
    listeners.remove (l);
}

WebBrowserComponent::Options WebSliderRelay::buildOptions (const WebBrowserComponent::Options& initialOptions)
{
    return initialOptions
        .withEventListener (eventId, [this] (auto object) { handleEvent (object); })
        .withInitialisationData ("__juce__sliders", name)
        .withWebViewLifetimeListener (this);
}

void WebSliderRelay::emitEvent (const var& payload)
{
    if (browser != nullptr)
        browser->emitEventIfBrowserIsVisible (eventId, payload);
}

void WebSliderRelay::webViewConstructed (WebBrowserComponent* browserIn)
{
    browser = browserIn;
    listeners.call (&Listener::initialUpdateRequested, this);
}

void WebSliderRelay::webViewDestructed (WebBrowserComponent*)
{
    browser = nullptr;
}

void WebSliderRelay::handleEvent (const var& event)
{
    using namespace detail;

    if (const auto sliderEvent = WebSliderRelayEvents::Event::extract (event))
    {
        if (const auto valueChanged = WebSliderRelayEvents::ValueChanged::extract (*sliderEvent))
        {
            if (! approximatelyEqual (std::exchange (value, valueChanged->newValue), valueChanged->newValue))
                listeners.call ([this] (Listener& l) { l.sliderValueChanged (this); });

            return;
        }

        if (const auto dragStarted = WebSliderRelayEvents::SliderDragStarted::extract (*sliderEvent))
        {
            listeners.call ([this] (Listener& l) { l.sliderDragStarted (this); });
            return;
        }

        if (const auto dragEnded = WebSliderRelayEvents::SliderDragEnded::extract (*sliderEvent))
        {
            listeners.call ([this] (Listener& l) { l.sliderDragEnded (this); });
            return;
        }

        if (const auto initialUpdate =
                WebSliderRelayEvents::InitialUpdateRequested::extract (*sliderEvent))
        {
            listeners.call ([this] (Listener& l) { l.initialUpdateRequested (this); });
            return;
        }
    }

    jassertfalse;
}

//==============================================================================
WebToggleButtonRelay::WebToggleButtonRelay (StringRef nameIn)
    : name (nameIn)
{
}

void WebToggleButtonRelay::setToggleState (bool newState)
{
    using namespace detail;

    DynamicObject::Ptr object { new DynamicObject };
    object->setProperty (WebToggleButtonRelayEvents::Event::eventTypeKey,
                         WebToggleButtonRelayEvents::ToggleStateChanged::eventId.toString());
    object->setProperty (WebToggleButtonRelayEvents::ToggleStateChanged::valueKey, newState);

    emitEvent (object.get());
}

void WebToggleButtonRelay::addListener (Listener* l)
{
    listeners.add (l);
}

void WebToggleButtonRelay::removeListener (Listener* l)
{
    listeners.remove (l);
}

WebBrowserComponent::Options WebToggleButtonRelay::buildOptions (const WebBrowserComponent::Options& initialOptions)
{
    return initialOptions
              .withEventListener (eventId, [this] (auto object) { handleEvent (object); })
              .withInitialisationData ("__juce__toggles", name)
              .withWebViewLifetimeListener (this);
}

void WebToggleButtonRelay::emitEvent (const var& payload)
{
    if (browser != nullptr)
        browser->emitEventIfBrowserIsVisible (eventId, payload);
}

void WebToggleButtonRelay::webViewConstructed (WebBrowserComponent* browserIn)
{
    browser = browserIn;
    listeners.call (&Listener::initialUpdateRequested);
}

void WebToggleButtonRelay::webViewDestructed (WebBrowserComponent*)
{
    browser = nullptr;
}

void WebToggleButtonRelay::handleEvent (const var& event)
{
    using namespace detail;

    if (const auto buttonEvent = WebToggleButtonRelayEvents::Event::extract (event))
    {
        if (const auto toggleStateChanged = WebToggleButtonRelayEvents::ToggleStateChanged::extract (*buttonEvent))
        {
            listeners.call ([&toggleStateChanged] (Listener& l)
                            { l.toggleStateChanged (toggleStateChanged->value); });
            return;
        }

        if (const auto initialUpdate = WebToggleButtonRelayEvents::InitialUpdateRequested::extract (*buttonEvent))
        {
            listeners.call ([] (Listener& l) { l.initialUpdateRequested(); });
            return;
        }
    }

    jassertfalse;
}

//==============================================================================
WebComboBoxRelay::WebComboBoxRelay (StringRef nameIn)
    : name (nameIn)
{
}

void WebComboBoxRelay::setValue (float newValue)
{
    using namespace detail;

    DynamicObject::Ptr object { new DynamicObject };
    object->setProperty (WebComboBoxRelayEvents::Event::eventTypeKey,
                         WebComboBoxRelayEvents::ValueChanged::eventId.toString());
    object->setProperty (WebComboBoxRelayEvents::ValueChanged::valueKey, newValue);

    emitEvent (object.get());
}

void WebComboBoxRelay::addListener (Listener* l)
{
    listeners.add (l);
}

void WebComboBoxRelay::removeListener (Listener* l)
{
    listeners.remove (l);
}

WebBrowserComponent::Options WebComboBoxRelay::buildOptions (const WebBrowserComponent::Options& initialOptions)
{
    return initialOptions
        .withEventListener (eventId, [this] (auto object) { handleEvent (object); })
        .withInitialisationData ("__juce__comboBoxes", name)
        .withWebViewLifetimeListener (this);
}

void WebComboBoxRelay::emitEvent (const var& payload)
{
    if (browser != nullptr)
        browser->emitEventIfBrowserIsVisible (eventId, payload);
}

void WebComboBoxRelay::webViewConstructed (WebBrowserComponent* browserIn)
{
    browser = browserIn;
    listeners.call (&Listener::initialUpdateRequested);
}

void WebComboBoxRelay::webViewDestructed (WebBrowserComponent*)
{
    browser = nullptr;
}

void WebComboBoxRelay::handleEvent (const var& event)
{
    using namespace detail;

    if (const auto buttonEvent = WebComboBoxRelayEvents::Event::extract (event))
    {
        if (const auto valueChanged = WebComboBoxRelayEvents::ValueChanged::extract (*buttonEvent))
        {
            listeners.call ([&valueChanged] (Listener& l)
                            { l.valueChanged (valueChanged->value); });
            return;
        }

        if (const auto initialUpdate = WebComboBoxRelayEvents::InitialUpdateRequested::extract (*buttonEvent))
        {
            listeners.call ([] (Listener& l) { l.initialUpdateRequested(); });
            return;
        }
    }

    jassertfalse;
}

#endif

} // namespace juce
