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
    message(WARNING "No xcrun was found. Can't generate Icon Composer assets. Please ensure that the"
                    " Xcode command-line tools are installed.")
    return()
endif()

function(get_actool_version output_var)
    execute_process(
        COMMAND xcrun actool --version
        OUTPUT_VARIABLE cmd_output
        ERROR_VARIABLE cmd_error
        RESULT_VARIABLE cmd_result
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(NOT cmd_result EQUAL 0)
        set(${output_var} "" PARENT_SCOPE)
        return()
    endif()

    string(REGEX REPLACE "[ \t\r\n]+" "" cmd_output "${cmd_output}")
    string(REGEX MATCH "<key>short-bundle-version</key>[^<]*<string>(.*)</string>" match_found "${cmd_output}")

    if(match_found)
        set(${output_var} "${CMAKE_MATCH_1}" PARENT_SCOPE)
    else()
        set(${output_var} "" PARENT_SCOPE)
    endif()
endfunction()

get_actool_version(actool_version)

if(NOT "${actool_version}" VERSION_GREATER_EQUAL "26.0")
    message(WARNING "Couldn't find actool with version 26.0 or greater."
                    " Icon Composer assets will not be generated.")
    return()
endif()

get_filename_component(icon_name "${icon_path}" NAME_WE)

execute_process(
    COMMAND "${JUCE_XCRUN}" actool
            --compile "${bundle_dir}/Contents/Resources"
            --platform macosx
            --minimum-deployment-target 10.11
            --output-partial-info-plist /dev/null
            --app-icon "${icon_name}"
            "${icon_path}"
    OUTPUT_VARIABLE result_stdout
    ERROR_VARIABLE result_stderr
    RESULT_VARIABLE result)

if(result)
    message(STATUS "${result_stdout}")
    message(WARNING "${result_stderr}")
    message(ERROR "Failed to generate Icon Composer asset. actool returned with exit code ${result}")
endif()
