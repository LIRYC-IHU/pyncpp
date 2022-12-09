# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

function(PYNCPP_target_add_path_config_file target)

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        "ABSOLUTE_PATHS"
        "STDLIB"
        "PATHS;EXTRA_PATHS"
        )

    if(NOT TARGET ${target})
        message(FATAL_ERROR "Argument is not a target: ${target}")
    endif()

    set(arguments -D "EXECUTABLE:PATH=$<TARGET_FILE_DIR:${target}>/$<TARGET_FILE_NAME:${target}>")

    if(ARG_ABSOLUTE_PATHS)
        set(absolute_paths ON)
    else()
        set(absolute_paths OFF)
    endif()

    list(APPEND arguments -D "ABSOLUTE_PATHS:BOOL=${absolute_paths}")

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

    string(REPLACE ";" "\\\\;" paths "${paths}")
    list(APPEND arguments -D PATHS:STRING=${paths})

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND} ${arguments}
        -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/add_path_config_file.cmake"
        VERBATIM
        )

endfunction()
