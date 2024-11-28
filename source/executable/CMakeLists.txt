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
    file(RELATIVE_PATH relative_lib_dir "/${CMAKE_INSTALL_BINDIR}" "/${CMAKE_INSTALL_LIBDIR}")

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
    )

################################################################################
# Compile definitions
################################################################################

if(WIN32)
    set(python_home ".")
else()
    file(RELATIVE_PATH python_home "/${CMAKE_INSTALL_BINDIR}" "/")
endif()

target_compile_definitions(pyncpp_executable PUBLIC
    PYTHON_HOME="${python_home}"
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
    DESTINATION "${CMAKE_INSTALL_BINDIR}"
    COMPONENT Runtime
    )

install(EXPORT pyncpp_export_executable
    DESTINATION "${PYNCPP_SHARE_SUBDIR}"
    )

foreach(config ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})
    export(EXPORT pyncpp_export_executable
        FILE "${PYNCPP_ROOT_BASE}/${config}/${PYNCPP_SHARE_SUBDIR}/pyncpp_export_executable.cmake"
        )
endforeach()
