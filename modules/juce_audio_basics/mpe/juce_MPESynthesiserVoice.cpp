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

MPESynthesiserVoice::MPESynthesiserVoice()
{
}

MPESynthesiserVoice::~MPESynthesiserVoice()
{
}

//==============================================================================
bool MPESynthesiserVoice::isCurrentlyPlayingNote (MPENote note) const noexcept
{
    return isActive() && currentlyPlayingNote.noteID == note.noteID;
}

bool MPESynthesiserVoice::isPlayingButReleased() const noexcept
{
    return isActive() && currentlyPlayingNote.keyState == MPENote::off;
}

void MPESynthesiserVoice::clearCurrentNote() noexcept
{
    currentlyPlayingNote = MPENote();
}

} // namespace juce
