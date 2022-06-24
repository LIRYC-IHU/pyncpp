# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

macro(PYNCPP_import_python_library target_name)

    add_library(${target_name} SHARED IMPORTED)

    target_include_directories(${target_name} INTERFACE ${Python_INCLUDE_DIRS})

    if(WIN32)
        foreach(_library ${Python_LIBRARIES})
            get_filename_component(_libext "${_library}" LAST_EXT)
            get_filename_component(_libname "${_library}" NAME_WLE)

            if(_libext STREQUAL ".dll")
                if(_libname MATCHES ".*_d.")
                    set(_runtime_lib_debug "${_library}")
                else()
                    set(_runtime_lib "${_library}")
                endif()
            elseif(_libex STREQUAL ".lib")
                if(_libname MATCHES ".*_d.")
                    set(_stub_lib_debug "${_library}")
                else()
                    set(_stub_lib "${_library}")
                endif()
            endif()
        endforeach()

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(_location "${_runtime_lib_debug}")
            set(_implib "${_stub_lib_debug}")
        elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
            set(_location "${_runtime_lib}")
            set(_implib "${_stub_lib}")
        elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
            set(_location "${_runtime_lib_debug}")
            set(_implib "${_stub_lib_debug}")
        elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
            set(_location "${_runtime_lib}")
            set(_implib "${_stub_lib}")
        endif()

        set_target_properties(${target_name} PROPERTIES
            IMPORTED_LOCATION_DEBUG "${_runtime_lib_debug}"
            IMPORTED_IMPLIB_DEBUG "${_stub_lib_debug}"
            IMPORTED_LOCATION_RELEASE "${_runtime_lib}"
            IMPORTED_IMPLIB_RELEASE "${_stub_lib}"
            IMPORTED_LOCATION_RELWITHDEBINFO "${_runtime_lib_debug}"
            IMPORTED_IMPLIB_RELWITHDEBINFO "${_stub_lib_debug}"
            IMPORTED_LOCATION_MINSIZEREL "${_runtime_lib}"
            IMPORTED_IMPLIB_MINSIZEREL "${_stub_lib}"
            IMPORTED_LOCATION "${_location}"
            IMPORTED_IMPLIB "${_implib}"
            )
    else()
        set_target_properties(${target_name} PROPERTIES
            IMPORTED_LOCATION "${Python_LIBRARIES}"
            IMPORTED_SONAME "${Python_LIBRARIES}"
            )
    endif()

endmacro()
