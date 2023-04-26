# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

macro(pyncpp_import_python_library_unix target_name)

    cmake_parse_arguments("ARG"
        ""
        "INSTALLED"
        ""
        ${ARGN}
        )

    add_library(${target_name} SHARED IMPORTED GLOBAL)

    set(lib_path "lib/libpython${pyncpp_PYTHON_SHORT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}")
    set(include_path "include/python${pyncpp_PYTHON_SHORT_VERSION}")

    if(ARG_INSTALLED OR CMAKE_BUILD_TYPE)
        set_target_properties(${target_name} PROPERTIES IMPORTED_LOCATION "${pyncpp_PYTHON_DIR}/${lib_path}")
        set(include_dir "${pyncpp_PYTHON_DIR}/${include_path}")
    else()
        set(include_dir)
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${config} CONFIG)
            set_target_properties(${target_name} PROPERTIES IMPORTED_LOCATION_${CONFIG} "${pyncpp_PYTHON_DIR}/${config}/${lib_path}")
            string(APPEND include_dir $<$<CONFIG:${config}>:${pyncpp_PYTHON_DIR}/${config}/${include_path}>)
        endforeach()
    endif()

    if(APPLE)
        set_target_properties(${target_name} PROPERTIES IMPORTED_NO_SONAME TRUE)
    endif()

    target_include_directories(${target_name} INTERFACE "${include_dir}")

endmacro()
