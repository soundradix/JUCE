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

if(NOT EXISTS "${src}")
    message(STATUS "Unable to copy ${src} as it does not exist")
    return()
endif()

get_filename_component(name "${src}" NAME)

if(EXISTS "${dest}/${name}")
    message(STATUS "Destination ${dest}/${name} exists, overwriting")
    file(REMOVE_RECURSE "${dest}/${name}")
endif()

file(INSTALL ${src} DESTINATION ${dest} USE_SOURCE_PERMISSIONS)
