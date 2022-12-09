# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

function(PYNCPP_add_path_config_file executable_path)

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        "ABSOLUTE_PATHS"
        "STDLIB"
        "PATHS;EXTRA_PATHS"
        )

    if(ARG_PATHS)
        set(paths ${ARG_PATHS})
    else()
        if(ARG_STDLIB)
            set(paths
                "${ARG_STDLIB}"
                "${ARG_STDLIB}/lib-dynload"
                )
        else()
            set(paths
                "${Python_STDLIB}"
                "${Python_STDARCH}"
                )
        endif()
        if(ARG_EXTRA_PATHS)
            list(APPEND paths ${ARG_EXTRA_PATHS})
        endif()
    endif()

    get_filename_component(executable_dir "${executable_path}" DIRECTORY)
    get_filename_component(executable_name "${executable_path}" NAME_WLE)

    set(path_config_file "${executable_dir}/${executable_name}._pth")
    file(WRITE "${path_config_file}")

    foreach(path ${paths})
        if(ARG_ABSOLUTE_PATHS)
            if(NOT IS_ABSOLUTE "${path}")
                message(FATAL_ERROR "Path is not absolute: ${path}")
            endif()
        else()
            if(IS_ABSOLUTE "${path}")
                file(RELATIVE_PATH path "${executable_dir}" "${path}")
            endif()
        endif()
        file(APPEND "${path_config_file}" "${path}\n")
    endforeach()

endfunction()

if(CMAKE_SCRIPT_MODE_FILE AND ((NOT CMAKE_PARENT_LIST_FILE) OR (CMAKE_PARENT_LIST_FILE STREQUAL CMAKE_CURRENT_LIST_FILE)))

    set(arguments ${EXECUTABLE})

    if(ABSOLUTE_PATHS)
        list(APPEND arguments ABSOLUTE_PATHS)
    endif()

    if(PATHS)
        list(APPEND arguments PATHS ${PATHS})
    endif()

    PYNCPP_add_path_config_file(${arguments})

endif()
