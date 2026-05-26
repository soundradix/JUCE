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
    An interface for types that implement responses for certain message types.

    @tags{Audio}
*/
class ResponderDelegate
{
public:
    ResponderDelegate() = default;
    virtual ~ResponderDelegate() = default;

    /** If the message is processed successfully, and a response sent, then
        this returns true. Otherwise, returns false, allowing other ResponderDelegates
        to attempt to handle the message if necessary.
    */
    virtual bool tryRespond (ResponderOutput& output, const Message::Parsed& message) = 0;

    JUCE_DECLARE_NON_COPYABLE (ResponderDelegate)
    JUCE_DECLARE_NON_MOVEABLE (ResponderDelegate)
};

} // namespace juce::midi_ci
