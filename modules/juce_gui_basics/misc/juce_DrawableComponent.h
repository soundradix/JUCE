/*
  ==============================================================================

   This file is part of the JUCE framework.
   Copyright (c) Raw Material Software Limited

   JUCE is an open source framework subject to commercial or open source
   licensing.

   By downloading, installing, or using the JUCE framework, or combining the
   JUCE framework with any other source code, object code, content or any other
   copyrightable work, you agree to the terms of the JUCE End User Licence
   Agreement, and all incorporated terms including the JUCE Privacy Policy and
   the JUCE Website Terms of Service, as applicable, which will bind you. If you
   do not agree to the terms of these agreements, we will not license the JUCE
   framework to you, and you must discontinue the installation or download
   process and cease use of the JUCE framework.

   JUCE End User Licence Agreement: https://juce.com/legal/juce-9-licence/
   JUCE Privacy Policy: https://juce.com/juce-privacy-policy
   JUCE Website Terms of Service: https://juce.com/juce-website-terms-of-service/

   Or:

   You may also use this code under the terms of the AGPLv3:
   https://www.gnu.org/licenses/agpl-3.0.en.html

   THE JUCE FRAMEWORK IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL
   WARRANTIES, WHETHER EXPRESSED OR IMPLIED, INCLUDING WARRANTY OF
   MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, ARE DISCLAIMED.

  ==============================================================================
*/

namespace juce
{

class JUCE_API DrawableComponent : public Component,
                                   private Drawable::Listener
{
public:
    /** Wraps a Drawable object in a Component. */
    explicit DrawableComponent (Drawable& drawableIn);

    /** Destructor. */
    ~DrawableComponent() override;

    /** Sets a transform for this drawable that will position it within the specified
        area of its parent component.
    */
    void setTransformToFit (const Rectangle<float>& areaInParent, RectanglePlacement placement);

    /** Creates a path that describes the outline of this DrawableComponent. */
    Path getOutlineAsPath() const;

    /** Returns a reference to the Drawable object displayed by this Component. */
    Drawable& getDrawable()
    {
        return drawable;
    }

    /** Returns a reference to the Drawable object displayed by this Component. */
    const Drawable& getDrawable() const
    {
        return drawable;
    }

    //==============================================================================
    void paint (Graphics& g) override;

    bool hitTest (int x, int y) override;

    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

    //==============================================================================
    /** @internal

        Intended to replace Drawable::setBoundsToEnclose which is only ever called in conjunction
        with getDrawableBounds().
    */
    void resetComponentBoundsToDrawable();

private:
    void drawableBoundsChanged (Drawable*) override;

    Drawable& drawable;
    Point<int> originRelativeToComponent;
};

} // namespace juce
