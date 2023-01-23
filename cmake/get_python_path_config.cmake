# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

macro(PYNCPP_get_python_path_config path_config_var)

    cmake_parse_arguments("ARG"
        ""
        "EXECUTABLE_PATH;STDLIB_PATH"
        ""
        ${ARGN}
        )

    if(ARG_STDLIB_PATH)
        set(stdlib_path "${ARG_STDLIB_PATH}/${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}")
        set(stdext_path "${stdlib_path}/lib-dynload")
    else()
        set(stdlib_path "${PYNCPP_PYTHON_STDLIB}")
        set(stdext_path "${PYNCPP_PYTHON_STDEXT}")
    endif()

    if(ARG_EXECUTABLE_PATH)
        file(RELATIVE_PATH stdlib_path "${ARG_EXECUTABLE_PATH}" "${stdlib_path}")
        file(RELATIVE_PATH stdext_path "${ARG_EXECUTABLE_PATH}" "${stdext_path}")
    endif()

    set(${path_config_var} "${stdlib_path}\n${stdext_path}")

endmacro()
