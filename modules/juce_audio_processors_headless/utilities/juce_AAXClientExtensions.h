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

   JUCE End User Licence Agreement: https://juce.com/legal/juce-8-licence/
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

class AAX_IComponentDescriptor;

namespace juce
{

/**
    A custom AAX algorithm, for a plug-in that replaces JUCE's native-only algorithm with its own:
    one that also runs on Avid HDX DSP (TI), say, or that uses AAX Hybrid processing.

    An AAX algorithm runs apart from the plug-in's data model (on HDX, on another chip), so it can't
    call into the AudioProcessor. It sees only the fields of its context structure, which the
    plug-in describes, and which the data model feeds with packets and private-data initialisation.

    JUCE's AAX wrapper uses one only when the plug-in sets JucePlugin_AAXCustomAlgorithm=1, and
    otherwise compiles without any of this. When it's set and
    AAXClientExtensions::getAAXCustomAlgorithm() returns an algorithm:
    - JUCE calls describeComponent() for each main bus layout it registers, in place of adding its
      own context fields and native ProcessProc. The plug-in adds its fields, properties and
      ProcessProcs (native and/or TI).
    - JUCE's own native algorithm never runs, so the AudioProcessor's processBlock() is called only
      from AAX_IEffectParameters::RenderAudio_Hybrid(), when the algorithm registers for AAX Hybrid.
      Its buffer holds the Hybrid audio the algorithm sends to the host, and returns the Hybrid audio
      the host sends back to the algorithm. It carries no MIDI, and its playhead is the host's
      transport, which runs ahead of the algorithm by AAX_IController::GetHybridSignalLatency().
    - The data model forwards ResetFieldData() here, and calls postPackets() from
      GenerateCoefficients() and from its periodic TimerWakeup().
    - JUCE's master-bypass parameter posts no packet of its own; postPackets() gets its state.
    - JUCE's AAX meters aren't connected: the algorithm adds any meter taps it needs.

    @see AAXClientExtensions::getAAXCustomAlgorithm

    @tags{Audio}
*/
struct JUCE_API AAXCustomAlgorithm
{
    /** Posts a packet to one of the algorithm's data-in ports (AAX_IController::PostPacket()). */
    struct JUCE_API PacketPoster
    {
        virtual ~PacketPoster() = default;

        /** Returns false when the host didn't take the packet. */
        virtual bool postPacket (int32 fieldIndex, const void* payload, uint32 payloadSize) = 0;
    };

    virtual ~AAXCustomAlgorithm() = default;

    /** Describes the algorithm component for one main bus layout: its context fields, properties
        and ProcessProcs.

        pluginIdNative is the id from AAXClientExtensions::getPluginIDForMainBusConfig(), which JUCE
        has checked for uniqueness. Ids for the other platforms (e.g. TI) are the plug-in's own.
    */
    virtual void describeComponent (AAX_IComponentDescriptor& component,
                                    const AudioChannelSet& mainInputLayout,
                                    const AudioChannelSet& mainOutputLayout,
                                    int32 pluginIdNative) const = 0;

    /** Initialises a private-data field, as AAX_IEffectParameters::ResetFieldData(). */
    virtual void resetFieldData (int32 fieldIndex, void* data, uint32 dataSize) const = 0;

    /** Posts the algorithm's packets. Called on the host's non-real-time threads.

        postAll is true from GenerateCoefficients(), where every port's packet should be posted, and
        false from the periodic TimerWakeup() (about every 30 ms), where posting what changed is
        enough.

        @note HDX drops packets posted outside GenerateCoefficients() to ports 16 and 24
              (PT-206161), so give any port posted when postAll is false another field index.
    */
    virtual void postPackets (PacketPoster& poster, bool isBypassed, bool postAll) = 0;
};

//==============================================================================
/**
    An interface to allow an AudioProcessor to implement extended AAX-specific functionality.

    To use this class, create an object that inherits from it, implement the methods, then return
    a pointer to the object in your AudioProcessor `getAAXClientExtensions()` method.

    @see AudioProcessor, VST2ClientExtensions, VST3ClientExtensions

    @tags{Audio}
*/
struct JUCE_API AAXClientExtensions
{
    virtual ~AAXClientExtensions() = default;

    /** AAX plug-ins need to report a unique "plug-in id" for every audio layout
        configuration that your AudioProcessor supports on the main bus. Override this
        function if you want your AudioProcessor to use a custom "plug-in id" (for example
        to stay backward compatible with older versions of JUCE).

        The default implementation will compute a unique integer from the input and output
        layout and add this value to the 4 character code 'jcaa' (for native AAX) or 'jyaa'
        (for AudioSuite plug-ins).
    */
    virtual int32 getPluginIDForMainBusConfig (const AudioChannelSet& mainInputLayout,
                                               const AudioChannelSet& mainOutputLayout,
                                               bool idForAudioSuite) const;

    /** Returns an optional filename (including extension) for a page file to be used.

        A page file allows an AAX plugin to specify how its parameters are displayed on
        various control surfaces. For more information read the Page Table Guide in the
        AAX SDK documentation.

        By default this file will be searched for in `*.aaxplugin/Contents/Resources`.

        @see getPageFileSearchPath
    */
    virtual String getPageFileName() const;

    /** Optionally returns a search path for finding a page table file.

        This can be useful for specifying a location outside the plugin bundle so users can
        make changes to a page table file without breaking any code signatures.

        If this function returns a default-constructed File, then a default location will be used.
        The AAX SDK states this location will be `*.aaxplugin/Contents/Resources`.

        @note The returned path should be an absolute path to a directory.

        @see getPageFileName
    */
    virtual File getPageFileSearchPath() const { return {}; }

    /** Returns an algorithm that replaces JUCE's native AAX algorithm, or nullptr (the default) to
        keep JUCE's. It's queried on the temporary instance JUCE describes the plug-in with, and on
        each instance's data model, and only when the plug-in sets JucePlugin_AAXCustomAlgorithm=1.

        @see AAXCustomAlgorithm
    */
    virtual AAXCustomAlgorithm* getAAXCustomAlgorithm() const { return nullptr; }
};

} // namespace juce
