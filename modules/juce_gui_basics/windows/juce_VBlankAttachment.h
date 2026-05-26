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

/**
    Helper class to synchronise Component updates to the vertical blank event of the display that
    the Component is presented on. This is useful when animating the Component's contents.

    @tags{Animations}
*/
class JUCE_API  VBlankAttachment final  : private ComponentPeer::VBlankListener,
                                          private ComponentListener
{
public:
    /** Default constructor for creating an empty object. */
    VBlankAttachment() = default;

    /** Constructor. Creates an attachment that will call the passed in function at every vertical
        blank event of the display that the passed in Component is currently visible on.

        The Component must be valid for the entire lifetime of the VBlankAttachment.

        You should prefer the other constructor, where the callback also receives a timestamp
        parameter. This constructor is only provided for compatibility with the earlier JUCE
        implementation.
    */
    VBlankAttachment (Component* c, std::function<void()> callbackIn);

    /** Constructor. Creates an attachment that will call the passed in function at every vertical
        blank event of the display that the passed in Component is currently visible on.

        The Component must be valid for the entire lifetime of the VBlankAttachment.

        The provided callback is called with a monotonically increasing value expressed in seconds
        that corresponds to the time of the next frame to be presented. Use this value to
        synchronise drawing across all classes using a VBlankAttachment.
    */
    VBlankAttachment (Component* c, std::function<void (double)> callbackIn);
    VBlankAttachment (VBlankAttachment&& other);
    VBlankAttachment& operator= (VBlankAttachment&& other);

    /** Destructor. */
    ~VBlankAttachment() override;

    /** Returns true for a default constructed object. */
    bool isEmpty() const { return owner == nullptr; }

private:
    //==============================================================================
    void onVBlank (double timestampSec) override;

    //==============================================================================
    void componentParentHierarchyChanged (Component&) override;

    void updateOwner();
    void updatePeer();
    void cleanup();

    Component* owner = nullptr;
    Component* lastOwner = nullptr;
    std::function<void (double)> callback;
    ComponentPeer* lastPeer = nullptr;

    JUCE_DECLARE_NON_COPYABLE (VBlankAttachment)
};

} // namespace juce
