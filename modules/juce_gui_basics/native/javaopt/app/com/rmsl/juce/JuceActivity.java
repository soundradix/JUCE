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

import static android.view.View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN;
import static android.view.View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION;
import static android.view.View.SYSTEM_UI_FLAG_LAYOUT_STABLE;

import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.content.Intent;
import android.view.View;

//==============================================================================
public class JuceActivity   extends Activity
{
    private native void appNewIntent (Intent intent);
    private native void appOnResume();

    @SuppressWarnings ("deprecation")
    private void initEdgeToEdge()
    {
        if (Build.VERSION.SDK_INT < 35)
        {
            View decorView = getWindow().getDecorView();

            final int flags = Build.VERSION.SDK_INT < 30
                    ? (  SYSTEM_UI_FLAG_LAYOUT_STABLE
                       | SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                       | SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN)
                    : 0;

            decorView.setSystemUiVisibility (decorView.getSystemUiVisibility() | flags);
        }

        if (30 <= Build.VERSION.SDK_INT)
            getWindow().setDecorFitsSystemWindows (false);

        if (29 <= Build.VERSION.SDK_INT)
        {
            if (Build.VERSION.SDK_INT < 35)
                getWindow().setStatusBarContrastEnforced (false);

            getWindow().setNavigationBarContrastEnforced (false);
        }
    }

    @Override
    protected void onCreate (Bundle savedInstanceState)
    {
        initEdgeToEdge();

        super.onCreate (savedInstanceState);
    }

    @Override
    protected void onNewIntent (Intent intent)
    {
        super.onNewIntent (intent);
        setIntent (intent);

        appNewIntent (intent);
    }

    @Override
    protected void onResume()
    {
        super.onResume();
        appOnResume();
    }
}
