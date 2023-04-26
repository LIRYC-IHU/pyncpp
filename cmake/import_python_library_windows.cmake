# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

macro(pyncpp_import_python_library_windows target_name)

    add_library(${target_name} SHARED IMPORTED GLOBAL)

    set(version ${pyncpp_PYTHON_VERSION_MAJOR}${pyncpp_PYTHON_VERSION_MINOR})

    if(CMAKE_BUILD_TYPE)
        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(suffix "_d")
        else()
            set(suffix "")
        endif()

        set_target_properties(${target_name} PROPERTIES
            IMPORTED_LOCATION "${pyncpp_PYTHON_DIR}/python${version}${suffix}.dll"
            IMPORTED_IMPLIB "${pyncpp_PYTHON_DIR}/libs/python${version}${suffix}.lib"
            )
    else()
        foreach(config ${CMAKE_CONFIGURATION_TYPES})
            string(TOUPPER ${config} CONFIG)

            if(config STREQUAL "Debug")
                set(suffix "_d")
            else()
                set(suffix "")
            endif()

            set_target_properties(${target_name} PROPERTIES
                IMPORTED_LOCATION_${CONFIG} "${pyncpp_PYTHON_DIR}/python${version}${suffix}.dll"
                IMPORTED_IMPLIB_${CONFIG} "${pyncpp_PYTHON_DIR}/libs/python${version}${suffix}.lib"
                )
        endforeach()
    endif()

    target_include_directories(${target_name} INTERFACE "${pyncpp_PYTHON_DIR}/include")

endmacro()
