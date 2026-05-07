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

find_program(JUCE_XCRUN xcrun)

if(NOT JUCE_XCRUN)
    return()
endif()

execute_process(
    COMMAND "${JUCE_XCRUN}" codesign --verify "${src}"
    RESULT_VARIABLE result)

if(result)
    message(STATUS "Replacing invalid signature with ad-hoc signature")
    execute_process(COMMAND "${JUCE_XCRUN}" codesign -f -s - "${src}")
endif()
