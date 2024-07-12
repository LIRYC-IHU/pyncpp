# Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Settings
################################################################################

set(executable_name python)

################################################################################
# Files
################################################################################

set(sources
    main.cpp
    )

################################################################################
# Library
################################################################################

add_executable(pyncpp_executable
    ${sources}
    )

################################################################################
# Target properties
################################################################################

if(UNIX)
    file(RELATIVE_PATH relative_lib_dir "/${PYNCPP_UTILS_SUBDIR}" "/${PYNCPP_LIBRARY_SUBDIR}")

    if(APPLE)
        set(rpath "@executable_path/${relative_lib_dir}")
    else()
        set(rpath "\$ORIGIN/${relative_lib_dir}")
    endif()

    set_target_properties(pyncpp_executable PROPERTIES
        INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${rpath}"
        )
endif()

set_target_properties(pyncpp_executable PROPERTIES
    OUTPUT_NAME ${executable_name}
    RUNTIME_OUTPUT_DIRECTORY "${PYNCPP_ROOT}/${PYNCPP_UTILS_SUBDIR}"
    )

################################################################################
# Compile definitions
################################################################################

file(RELATIVE_PATH relative_python_dir "/${PYNCPP_UTILS_SUBDIR}" "/${PYNCPP_PYTHON_SUBDIR}")

target_compile_definitions(pyncpp_executable PUBLIC
    PYTHON_HOME="${relative_python_dir}"
    )

################################################################################
# Link
################################################################################

target_link_libraries(pyncpp_executable PUBLIC
    pyncpp_cpp_api
    )

################################################################################
# Install/Export
################################################################################

install(TARGETS pyncpp_executable
    EXPORT pyncpp_export_executable

    RUNTIME
    DESTINATION "${PYNCPP_UTILS_SUBDIR}"
    )

install(EXPORT pyncpp_export_executable
    DESTINATION "${PYNCPP_SHARE_SUBDIR}"
    )

foreach(config ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})
    export(EXPORT pyncpp_export_executable
        FILE "${PYNCPP_ROOT_BASE}/${config}/${PYNCPP_SHARE_SUBDIR}/pyncpp_export_executable.cmake"
        )
endforeach()
