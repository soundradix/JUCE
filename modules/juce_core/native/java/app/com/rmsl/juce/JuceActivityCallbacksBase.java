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

package com.rmsl.juce;

import android.app.Activity;
import android.app.Application;
import android.os.Bundle;

public class JuceActivityCallbacksBase implements Application.ActivityLifecycleCallbacks
{
    @Override
    public void onActivityCreated (Activity activity, Bundle savedInstanceState) {}

    @Override
    public void onActivityStarted (Activity activity) {}

    @Override
    public void onActivityResumed (Activity activity) {}

    @Override
    public void onActivityPaused (Activity activity) {}

    @Override
    public void onActivityStopped (Activity activity) {}

    @Override
    public void onActivitySaveInstanceState (Activity activity, Bundle outState) {}

    @Override
    public void onActivityDestroyed (Activity activity) {}
}
