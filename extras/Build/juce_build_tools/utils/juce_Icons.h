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

namespace juce::build_tools
{

    class Icons
    {
    public:
        Icons() = default;

        static Icons fromFilesSmallAndBig (const File& small, const File& big);

        const Drawable* getSmall() const { return small.get(); }
        const Drawable* getBig()   const { return big.get(); }

    private:
        std::unique_ptr<Drawable> small;
        std::unique_ptr<Drawable> big;
    };

    Array<const Drawable*> asArray (const Icons&);
    void writeMacIcon (const Icons&, const File&);
    void writeWinIcon (const Icons&, const File&);

    Image getBestIconForSize (const Icons& icons,
                              int size,
                              bool returnNullIfNothingBigEnough);
    Image rescaleImageForIcon (const Drawable& d, int size);

    RelativePath createXcassetsFolderFromIcons (const Icons& icons,
                                                const File& targetFolder,
                                                String projectFilenameRootString,
                                                String iconName);

} // namespace juce::build_tools
