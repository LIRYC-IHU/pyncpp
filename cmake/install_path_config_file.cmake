# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

function(PYNCPP_install_path_config_file executable_path)

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        "ABSOLUTE_PATHS"
        "STDLIB"
        "PATHS"
        )

    set(arguments -D "EXECUTABLE:PATH=\${CMAKE_INSTALL_PREFIX}/${executable_path}")

    if(ARG_ABSOLUTE_PATHS)
        set(absolute_paths ON)
    else()
        set(absolute_paths OFF)
    endif()

    list(APPEND arguments -D "ABSOLUTE_PATHS:BOOL=${absolute_paths}")

    set(paths)

    if(ARG_STDLIB)
        list(APPEND paths
            "${ARG_STDLIB}"
            "${ARG_STDLIB}/lib-dynload"
            )
    endif()

    if(ARG_PATHS)
        list(APPEND paths ${ARG_PATHS})
    endif()

    foreach(path ${paths})
        list(APPEND full_paths "\${CMAKE_INSTALL_PREFIX}/${path}")
    endforeach()

    string(REPLACE ";" "\\\\;" full_paths "${full_paths}")
    list(APPEND arguments -D PATHS:STRING=${full_paths})

    install(CODE "
        execute_process(COMMAND ${CMAKE_COMMAND} ${arguments}
            -P \"${CMAKE_CURRENT_FUNCTION_LIST_DIR}/add_path_config_file.cmake\"
            )
        "
        COMPONENT Python
        )

endfunction()
