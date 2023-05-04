# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

macro(pyncpp_import_python_executable_unix target_name)

    cmake_parse_arguments("ARG"
        ""
        "INSTALLED"
        ""
        ${ARGN}
        )

    add_executable(${target_name} IMPORTED GLOBAL)

    get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

    if(ARG_INSTALLED OR NOT GENERATOR_IS_MULTI_CONFIG)
        set_target_properties(${target_name} PROPERTIES IMPORTED_LOCATION "${pyncpp_PYTHON_DIR}/bin/python${pyncpp_PYTHON_SHORT_VERSION}")
    else()
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${config} CONFIG)
            set_target_properties(${target_name} PROPERTIES IMPORTED_LOCATION_${CONFIG} "${pyncpp_PYTHON_DIR}/${config}/bin/python${pyncpp_PYTHON_SHORT_VERSION}")
        endforeach()
    endif()

endmacro()
