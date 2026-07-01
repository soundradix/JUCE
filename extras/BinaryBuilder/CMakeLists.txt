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

juce_add_console_app(BinaryBuilder
    NEEDS_CURL FALSE)

juce_generate_juce_header(BinaryBuilder)

target_sources(BinaryBuilder PRIVATE Source/Main.cpp)

target_compile_definitions(BinaryBuilder PRIVATE
    JUCE_USE_CURL=0
    # This is a temporary workaround to allow builds to complete on Xcode 15.
    # Add -Wl,-ld_classic to the OTHER_LDFLAGS build setting if you need to
    # deploy to older versions of macOS/iOS.
    JUCE_SILENCE_XCODE_15_LINKER_WARNING=1)

target_link_libraries(BinaryBuilder PRIVATE
    juce::juce_core
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags)
