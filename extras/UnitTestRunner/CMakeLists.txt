# ==============================================================================
#
#  This file is part of the JUCE 9 preview.
#  Copyright (c) Raw Material Software Limited
#
#  You may use this code under the terms of the AGPLv3
#  (see www.gnu.org/licenses).
#
#  For the JUCE 9 preview this file cannot be licensed commercially.
#
#  JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
#  EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
#  DISCLAIMED.
#
# ==============================================================================

juce_add_console_app(UnitTestRunner)

juce_generate_juce_header(UnitTestRunner)

target_sources(UnitTestRunner PRIVATE Source/Main.cpp)

target_compile_definitions(UnitTestRunner PRIVATE
    JUCE_PLUGINHOST_AU=1
    JUCE_PLUGINHOST_LV2=1
    JUCE_PLUGINHOST_VST3=1
    JUCE_UNIT_TESTS=1
    JUCE_USE_CURL=0
    JUCE_WEB_BROWSER=0
    # This is a temporary workaround to allow builds to complete on Xcode 15.
    # Add -Wl,-ld_classic to the OTHER_LDFLAGS build setting if you need to
    # deploy to older versions of macOS.
    JUCE_SILENCE_XCODE_15_LINKER_WARNING=1)

target_link_libraries(UnitTestRunner PRIVATE
    juce::juce_analytics
    juce::juce_audio_utils
    juce::juce_build_tools
    juce::juce_dsp
    juce::juce_midi_ci
    juce::juce_opengl
    juce::juce_osc
    juce::juce_product_unlocking
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
