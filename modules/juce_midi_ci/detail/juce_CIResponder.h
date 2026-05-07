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

namespace juce::midi_ci::detail
{

/*
    Parses individual messages, and additionally gives ResponderDelegates a chance to formulate
    a response to any message that would normally necessitate a reply.
*/
struct Responder
{
    Responder() = delete;

    /*  Parses the message, then calls tryParse on each ResponderDelegate in
        turn until one returns true, indicating that the message has been
        handled. Most 'inquiry' messages should emit one or more reply messages.
        These replies will be written to the provided BufferOutput.
        If none of the provided delegates are able to handle the message, then
        a generic NAK will be written to the BufferOutput.
    */
    static Parser::Status processCompleteMessage (BufferOutput& output,
                                                  ump::BytesOnGroup message,
                                                  Span<ResponderDelegate* const> delegates);
};

} // namespace juce::midi_ci
