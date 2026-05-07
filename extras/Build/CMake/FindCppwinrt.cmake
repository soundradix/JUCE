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

include(FindPackageHandleStandardArgs)

if(JUCE_CPPWINRT_PACKAGE_LOCATION)
    set(initial_search_dir ${JUCE_CPPWINRT_PACKAGE_LOCATION})
else()
    set(initial_search_dir "$ENV{USERPROFILE}/AppData/Local/PackageManagement/NuGet/Packages")
endif()

file(GLOB subdirs "${initial_search_dir}/Microsoft.Windows.CppWinRT.*")

if(subdirs)
    list(GET subdirs 0 search_dir)
    list(LENGTH subdirs num_midi2_packages)

    if(num_midi2_packages GREATER 1)
        message(WARNING "Multiple Windows MIDI Services packages found in the local NuGet folder. Proceeding with ${search_dir}.")
    endif()

    find_path(cppwinrt_root_dir bin/cppwinrt.exe HINTS ${search_dir})
    set(cppwinrt_exe "${cppwinrt_root_dir}/bin/cppwinrt.exe")
elseif(NOT cppwinrt_FIND_QUIETLY)
    message(WARNING "Cppwinrt wasn't found in the local packages folder.\n"
            "\n"
            "In order to use cppwinrt functionality, you must install this package."
            "To install NuGet and the CppWinRT package containing the statically linked library, "
            "open a PowerShell and issue the following commands"
            "\n"
            "> Register-PackageSource -provider NuGet -name nugetRepository -location https://www.nuget.org/api/v2\n"
            "> Install-Package Microsoft.Windows.CppWinRT -Scope CurrentUser -Source nugetRepository\n"
            "\n"
            "Alternatively you can use the JUCE_CPPWINRT_PACKAGE_LOCATION CMake variable to specify the directory "
            "where this find script is looking for the *Microsoft.Windows.CppWinRT* package directory.")
endif()

find_package_handle_standard_args(CppWinRT DEFAULT_MSG cppwinrt_exe)

if(CppWinRT_FOUND)
    mark_as_advanced(cppwinrt_root_dir cppwinrt_exe)

    if(NOT TARGET jcppwinrt)
        add_executable(jcppwinrt IMPORTED GLOBAL)
        set_target_properties(jcppwinrt PROPERTIES IMPORTED_LOCATION "${cppwinrt_exe}")
    endif()

    if(NOT TARGET juce_winrt_headers)
        get_target_property(cppwinrt_command jcppwinrt IMPORTED_LOCATION)
        set(cppwinrt_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/cppwinrt_projection")

        execute_process(COMMAND "${cppwinrt_command}" -input sdk -output "${cppwinrt_INCLUDE_DIRS}"
            OUTPUT_VARIABLE command_output
            ERROR_VARIABLE command_output
            RESULT_VARIABLE result_variable)

        if(result_variable)
            message(FATAL_ERROR "Failed to run CppWinRT to generate Windows SDK headers\n${command_output}")
        endif()

        add_library(juce_winrt_headers INTERFACE)
        add_library(juce::juce_winrt_headers ALIAS juce_winrt_headers)
        target_link_libraries(juce_winrt_headers INTERFACE runtimeobject)
        target_include_directories(juce_winrt_headers INTERFACE ${cppwinrt_INCLUDE_DIRS})
    endif()
endif()
