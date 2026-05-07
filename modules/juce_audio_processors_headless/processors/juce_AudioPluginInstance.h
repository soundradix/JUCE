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

// MSVC does not like it if you override a deprecated method even if you
// keep the deprecation attribute. Other compilers are more forgiving.
JUCE_BEGIN_IGNORE_WARNINGS_MSVC (4996)

//==============================================================================
/**
    Base class for an active instance of a plugin.

    This derives from the AudioProcessor class, and adds some extra functionality
    that helps when wrapping dynamically loaded plugins.

    This class is not needed when writing plugins, and you should never need to derive
    your own sub-classes from it. The plugin hosting classes use it internally and will
    return AudioPluginInstance objects which wrap external plugins.

    @see AudioProcessor, AudioPluginFormat

    @tags{Audio}
*/
class JUCE_API  AudioPluginInstance   : public AudioProcessor
{
public:
    //==============================================================================
    /** Destructor.

        Make sure that you delete any UI components that belong to this plugin before
        deleting the plugin.
    */
    ~AudioPluginInstance() override = default;

    //==============================================================================
    /** Fills-in the appropriate parts of this plugin description object. */
    virtual void fillInPluginDescription (PluginDescription&) const = 0;

    /** Returns a PluginDescription for this plugin.
        This is just a convenience method to avoid calling fillInPluginDescription.
    */
    PluginDescription getPluginDescription() const;

    /** Obsolete: Prefer to use getVSTClient(), getVST3Client(),
        getAudioUnitClient(), and/or getARAClient().
    */
    void getExtensions (class ExtensionsVisitor&) const = delete;

    /** Returns ARA-specific extensions, or nullptr if extensions are unavailable. */
    virtual       AudioPluginExtensions::ARAClient* getARAClient()       { return nullptr; }
    /** Returns ARA-specific extensions, or nullptr if extensions are unavailable. */
    virtual const AudioPluginExtensions::ARAClient* getARAClient() const { return nullptr; }

    /** Returns AU-specific extensions, or nullptr if extensions are unavailable. */
    virtual       AudioPluginExtensions::AudioUnitClient* getAudioUnitClient()       { return nullptr; }
    /** Returns AU-specific extensions, or nullptr if extensions are unavailable. */
    virtual const AudioPluginExtensions::AudioUnitClient* getAudioUnitClient() const { return nullptr; }

    /** Returns VST-specific extensions, or nullptr if extensions are unavailable. */
    virtual       AudioPluginExtensions::VSTClient* getVSTClient()       { return nullptr; }
    /** Returns VST-specific extensions, or nullptr if extensions are unavailable. */
    virtual const AudioPluginExtensions::VSTClient* getVSTClient() const { return nullptr; }

    /** Returns VST3-specific extensions, or nullptr if extensions are unavailable. */
    virtual       AudioPluginExtensions::VST3Client* getVST3Client()       { return nullptr; }
    /** Returns VST3-specific extensions, or nullptr if extensions are unavailable. */
    virtual const AudioPluginExtensions::VST3Client* getVST3Client() const { return nullptr; }

    using HostedParameter = HostedAudioProcessorParameter;

    /** Adds a parameter to this instance.

        @see AudioProcessor::addParameter()
    */
    void addHostedParameter (std::unique_ptr<HostedParameter>);

    /** Adds multiple parameters to this instance.

        In debug mode, this will also check that all added parameters derive from
        HostedParameter.

        @see AudioProcessor::addParameterGroup()
    */
    void addHostedParameterGroup (std::unique_ptr<AudioProcessorParameterGroup>);

    /** Adds multiple parameters to this instance.

        In debug mode, this will also check that all added parameters derive from
        HostedParameter.

        @see AudioProcessor::setParameterTree()
    */
    void setHostedParameterTree (AudioProcessorParameterGroup);

    /** Gets the parameter at a particular index.

        If you want to find lots of parameters by their IDs, you should probably build and
        use a map<String, HostedParameter*> by looping through all parameters.
    */
    HostedParameter* getHostedParameter (int index) const;

    /** @cond */
    // Rather than using these methods you should call the corresponding methods
    // on the AudioProcessorParameter objects returned from getParameters().
    // See the implementations of the methods below for some examples of how to
    // do this.
    //
    // In addition to being marked as deprecated these methods will assert on
    // the first call.
    [[deprecated]] String getParameterID (int index) override;
    [[deprecated]] float getParameter (int parameterIndex) override;
    [[deprecated]] void setParameter (int parameterIndex, float newValue) override;
    [[deprecated]] const String getParameterName (int parameterIndex) override;
    [[deprecated]] String getParameterName (int parameterIndex, int maximumStringLength) override;
    [[deprecated]] const String getParameterText (int parameterIndex) override;
    [[deprecated]] String getParameterText (int parameterIndex, int maximumStringLength) override;
    [[deprecated]] int getParameterNumSteps (int parameterIndex) override;
    [[deprecated]] bool isParameterDiscrete (int parameterIndex) const override;
    [[deprecated]] bool isParameterAutomatable (int parameterIndex) const override;
    [[deprecated]] float getParameterDefaultValue (int parameterIndex) override;
    [[deprecated]] String getParameterLabel (int parameterIndex) const override;
    [[deprecated]] bool isParameterOrientationInverted (int parameterIndex) const override;
    [[deprecated]] bool isMetaParameter (int parameterIndex) const override;
    [[deprecated]] AudioProcessorParameter::Category getParameterCategory (int parameterIndex) const override;
    /** @endcond */

protected:
    //==============================================================================
    /** Structure used to describe plugin parameters */
    struct Parameter   : public HostedParameter
    {
    public:
        Parameter();

        String getText (float value, int maximumStringLength) const override;
        float getValueForText (const String& text) const override;

    private:
        const StringArray onStrings, offStrings;
    };

    using AudioProcessor::AudioProcessor;

private:
    // It's not safe to add a plain AudioProcessorParameter to an AudioPluginInstance.
    // Instead, all parameters must be HostedParameters.
    using AudioProcessor::addParameter;
    using AudioProcessor::addParameterGroup;
    using AudioProcessor::setParameterTree;

    void assertOnceOnDeprecatedMethodUse() const noexcept;

    static bool deprecationAssertiontriggered;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioPluginInstance)
};

JUCE_END_IGNORE_WARNINGS_MSVC

} // namespace juce
