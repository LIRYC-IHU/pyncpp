# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

include("${CMAKE_CURRENT_LIST_DIR}/get_python_path_config.cmake")

function(PYNCPP_create_path_config_file executable)

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        ""
        "STDLIB_PATH"
        ""
        )

    get_filename_component(executable_dir "${executable}" DIRECTORY)
    get_filename_component(executable_name "${executable}" NAME_WLE)

    PYNCPP_get_python_path_config(python_paths
        EXECUTABLE_PATH ${executable_dir}
        STDLIB_PATH ${ARG_STDLIB_PATH}
        )

    set(pth_file "${executable_dir}/${executable_name}._pth")
    file(WRITE "${pth_file}" "${python_paths}")

endfunction()

if(CMAKE_SCRIPT_MODE_FILE AND ((NOT CMAKE_PARENT_LIST_FILE) OR (CMAKE_PARENT_LIST_FILE STREQUAL CMAKE_CURRENT_LIST_FILE)))
    PYNCPP_create_path_config_file(${EXECUTABLE} STDLIB_PATH ${STDLIB_PATH})
endif()
