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

JUCE_BEGIN_IGNORE_DEPRECATION_WARNINGS

float DirectoryEntry::getEstimatedProgress() const
{
    if (auto it = iterator.lock())
        return it->getEstimatedProgress();

    return 0.0f;
}

// We implement this in terms of the deprecated DirectoryIterator,
// but the old DirectoryIterator might go away in the future!
RangedDirectoryIterator::RangedDirectoryIterator (const File& directory,
                                                  bool isRecursive,
                                                  const String& wildCard,
                                                  int whatToLookFor,
                                                  File::FollowSymlinks followSymlinks)
    : iterator (new DirectoryIterator (directory,
                                       isRecursive,
                                       wildCard,
                                       whatToLookFor,
                                       followSymlinks))
{
    entry.iterator = iterator;
    increment();
}

bool RangedDirectoryIterator::next()
{
    const auto result = iterator->next (&entry.directory,
                                        &entry.hidden,
                                        &entry.fileSize,
                                        &entry.modTime,
                                        &entry.creationTime,
                                        &entry.readOnly);
    if (result)
        entry.file = iterator->getFile();
    else
        entry = {};

    return result;
}

void RangedDirectoryIterator::increment()
{
    if (iterator != nullptr && ! next())
        iterator = nullptr;
}

JUCE_END_IGNORE_DEPRECATION_WARNINGS

} // namespace juce
