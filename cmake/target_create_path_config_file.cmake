# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

function(PYNCPP_target_create_path_config_file target)

    if(NOT TARGET ${target})
        message(FATAL_ERROR "Argument is not a target: ${target}")
    endif()

    get_target_property(target_type ${target} TYPE)
    if(NOT target_type STREQUAL "EXECUTABLE")
        message(FATAL_ERROR "Target must be an executable: ${target}")
    endif()

    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
        -D "EXECUTABLE:PATH=$<TARGET_FILE:${target}>"
        -D "PYNCPP_PYTHON_STDLIB:PATH=${PYNCPP_PYTHON_STDLIB}"
        -D "PYNCPP_PYTHON_STDEXT:PATH=${PYNCPP_PYTHON_STDEXT}"
        -P "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/create_path_config_file.cmake"
        VERBATIM
        )

 endfunction()
