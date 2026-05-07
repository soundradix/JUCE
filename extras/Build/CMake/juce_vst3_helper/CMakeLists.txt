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

cmake_minimum_required(VERSION 3.22)

project(juce_vst3_manifest_helper LANGUAGES CXX)

add_executable(${helper_name} ${source_file})

file(READ "${shared_defs_file}" shared_defs)
file(READ "${shared_incs_file}" shared_incs)

target_compile_definitions(${helper_name} PRIVATE ${shared_defs})
target_include_directories(${helper_name} PRIVATE ${shared_incs})

target_compile_features(${helper_name} PRIVATE cxx_std_17)

set_target_properties(${helper_name} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}"
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    target_link_libraries(${helper_name} PRIVATE stdc++fs)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_libraries(${helper_name} PRIVATE "-framework Foundation")
endif()
