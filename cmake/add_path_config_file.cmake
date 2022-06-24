# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

function(PYNCPP_add_path_config_file executable)

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        ""
        ""
        "PATHS"
        )

    if(NOT ARG_PATHS)
        message(FATAL_ERROR "Missing PATHS.")
    endif()

    if(TARGET ${executable})
        get_target_property(target_type ${executable} TYPE)
        if(NOT target_type STREQUAL "EXECUTABLE")
            message(FATAL_ERROR "Target must be an executable.")
        endif()

        string(REPLACE "\;" "\\\\;" paths "${ARG_PATHS}")

        add_custom_command(TARGET ${executable} POST_BUILD
            COMMAND ${CMAKE_COMMAND}
            -D "EXECUTABLE:PATH=$<TARGET_FILE_DIR:${executable}>/$<TARGET_FILE_NAME:${executable}>"
            -D "PATHS:STRING=${paths}"
            -P "${CMAKE_CURRENT_FUNCTION_LIST_FILE}"
            VERBATIM
            )
    else()
        get_filename_component(executable_dir "${executable}" DIRECTORY)
        get_filename_component(executable_name "${executable}" NAME_WLE)

        set(path_config_file "${executable_dir}/${executable_name}._pth")
        file(WRITE "${path_config_file}")

        foreach(path ${ARG_PATHS})
            file(RELATIVE_PATH path "${executable_dir}" "${path}")
            file(APPEND "${path_config_file}" "${path}\n")
        endforeach()
    endif()

endfunction()

if(CMAKE_SCRIPT_MODE_FILE AND ((NOT CMAKE_PARENT_LIST_FILE) OR (CMAKE_PARENT_LIST_FILE STREQUAL CMAKE_CURRENT_LIST_FILE)))
    PYNCPP_add_path_config_file(${EXECUTABLE} PATHS ${PATHS})
endif()
