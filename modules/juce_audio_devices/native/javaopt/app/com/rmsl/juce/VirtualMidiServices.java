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


import static android.content.pm.PackageManager.COMPONENT_ENABLED_STATE_DISABLED;
import static android.content.pm.PackageManager.COMPONENT_ENABLED_STATE_ENABLED;
import static android.content.pm.PackageManager.DONT_KILL_APP;

import android.annotation.TargetApi;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.media.midi.MidiDeviceService;
import android.media.midi.MidiReceiver;
import android.media.midi.MidiUmpDeviceService;
import android.os.Build;

import java.io.IOException;
import java.util.Collections;
import java.util.List;

public class VirtualMidiServices
{
    /** Argument is the receiver that should be sent outgoing messages.
        The return value is a newly-created receiver that will receive incoming messages.
    */
    private static native MidiReceiver addVirtualPort (MidiReceiver output, boolean isUmp);

    /** Argument is a receiver previously returned from addVirtualPort. */
    private static native void removeVirtualPort (MidiReceiver output);

    public static class VirtualPort extends MidiReceiver
    {
        @Override
        public void onSend (byte[] msg, int offset, int count, long timestamp) throws IOException
        {
            if (onSend != null)
                onSend.send (msg, offset, count, timestamp);
        }

        public MidiReceiver onSend;
    }

    public static class VirtualBytestreamService extends MidiDeviceService
    {
        @Override
        public MidiReceiver[] onGetInputPortReceivers()
        {
            return new MidiReceiver[] { port };
        }

        @Override
        public void onCreate()
        {
            super.onCreate();
            MidiReceiver[] receivers = getOutputPortReceivers();
            port.onSend = addVirtualPort (receivers != null && receivers.length > 0 ? receivers[0] : null, false);
        }

        @Override
        public void onDestroy()
        {
            removeVirtualPort (port.onSend);
            super.onDestroy();
        }

        private final VirtualPort port = new VirtualPort();
    }

    @TargetApi(Build.VERSION_CODES.VANILLA_ICE_CREAM)
    public static class VirtualUmpService extends MidiUmpDeviceService
    {
        @Override
        public List<MidiReceiver> onGetInputPortReceivers()
        {
            return Collections.singletonList (port);
        }

        @Override
        public void onCreate()
        {
            super.onCreate();
            List<MidiReceiver> receivers = getOutputPortReceivers();
            port.onSend = addVirtualPort (! receivers.isEmpty() ? receivers.getFirst() : null, true);
        }

        @Override
        public void onDestroy()
        {
            removeVirtualPort (port.onSend);
            super.onDestroy();
        }

        private final VirtualPort port = new VirtualPort();
    }

    private static void setVirtualMidiServiceEnabled (Context context, Class<?> klass, boolean x)
    {
        PackageManager pm = context.getPackageManager();

        if (pm == null)
            return;

        ComponentName componentName = new ComponentName (context, klass);

        int desiredState = x ? COMPONENT_ENABLED_STATE_ENABLED : COMPONENT_ENABLED_STATE_DISABLED;
        int actualState = pm.getComponentEnabledSetting (componentName);

        if (desiredState == actualState)
            return;

        pm.setComponentEnabledSetting (componentName, desiredState, DONT_KILL_APP);
    }

    public static void setVirtualMidiBytestreamEnabled (Context context, boolean x)
    {
        setVirtualMidiServiceEnabled (context, VirtualBytestreamService.class, x);
    }

    public static void setVirtualMidiUmpEnabled (Context context, boolean x)
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.VANILLA_ICE_CREAM)
            setVirtualMidiServiceEnabled (context, VirtualUmpService.class, x);
    }

    private static boolean isServiceAvailable (Context context, Class k)
    {
        PackageManager packageManager = context.getPackageManager();
        Intent intent = new Intent (context, k);
        List services = packageManager.queryIntentServices (intent, PackageManager.MATCH_DEFAULT_ONLY);

        return services != null && ! services.isEmpty();
    }

    public static boolean isVirtualBytestreamAvailable (Context context)
    {
        return isServiceAvailable (context, VirtualBytestreamService.class);
    }

    public static boolean isVirtualUmpAvailable (Context context)
    {
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.VANILLA_ICE_CREAM)
            return false;

        return isServiceAvailable (context, VirtualUmpService.class);
    }
}
