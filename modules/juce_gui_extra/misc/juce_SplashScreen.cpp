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

SplashScreen::SplashScreen (const String& title, const Image& image, bool useDropShadow)
    : Component (title),
      backgroundImage (image),
      clickCountToDelete (0)
{
    // You must supply a valid image here!
    jassert (backgroundImage.isValid());

    setOpaque (! backgroundImage.hasAlphaChannel());

   #if JUCE_IOS || JUCE_ANDROID
    const bool useFullScreen = true;
   #else
    const bool useFullScreen = false;
   #endif

    makeVisible (image.getWidth(), image.getHeight(), useDropShadow, useFullScreen);
}

SplashScreen::SplashScreen (const String& title, int width, int height, bool useDropShadow)
    : Component (title),
      clickCountToDelete (0)
{
    makeVisible (width, height, useDropShadow, false);
}

void SplashScreen::makeVisible (int w, int h, bool useDropShadow, bool fullscreen)
{
    clickCountToDelete = Desktop::getInstance().getMouseButtonClickCounter();
    creationTime = Time::getCurrentTime();

    const auto screenSize = Desktop::getInstance().getDisplays().getPrimaryDisplay()->userBounds.getSmallestIntegerContainer();
    const auto size = fullscreen ? screenSize : Rectangle { w, h };

    setAlwaysOnTop (true);
    setVisible (true);
    centreWithSize (size.getWidth(), size.getHeight());
    addToDesktop (useDropShadow ? ComponentPeer::windowHasDropShadow : 0);

    if (fullscreen)
        getPeer()->setFullScreen (true);

    toFront (false);
}

SplashScreen::~SplashScreen() {}

void SplashScreen::deleteAfterDelay (RelativeTime timeout, bool removeOnMouseClick)
{
    // Note that this method must be safe to call from non-GUI threads
    if (! removeOnMouseClick)
        clickCountToDelete = std::numeric_limits<int>::max();

    minimumVisibleTime = timeout;

    startTimer (50);
}

void SplashScreen::paint (Graphics& g)
{
    g.setOpacity (1.0f);
    g.drawImage (backgroundImage, getLocalBounds().toFloat(), RectanglePlacement (RectanglePlacement::fillDestination));
}

void SplashScreen::timerCallback()
{
    if (Time::getCurrentTime() > creationTime + minimumVisibleTime
         || Desktop::getInstance().getMouseButtonClickCounter() > clickCountToDelete)
        delete this;
}

} // namespace juce
