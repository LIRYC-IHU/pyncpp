# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

include(${CMAKE_CURRENT_LIST_DIR}/install_path_config_file.cmake)

function(PYNCPP_target_install_path_config_file target)

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        ""
        "DESTINATION"
        ""
        )

    if(NOT TARGET ${target})
        message(FATAL_ERROR "Argument is not a target: ${target}")
    endif()

    get_target_property(target_type ${target} TYPE)
    if(NOT target_type STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "Target must be an executable: ${target}")
    endif()

    if(NOT ARG_DESTINATION)
        message(FATAL_ERROR "DESTINATION argument required.")
    endif()

    get_target_property(is_bundle ${target} MACOSX_BUNDLE)

    if(is_bundle)
        set(executable_path "${ARG_DESTINATION}/$<TARGET_FILE_BASE_NAME:${target}>.app/Contents/MacOS/$<TARGET_FILE_NAME:${target}>")
    else()
        set(executable_path "${ARG_DESTINATION}/$<TARGET_FILE_NAME:${target}>")
    endif()

    PYNCPP_install_path_config_file(${executable_path} ${ARGN})

endfunction()
