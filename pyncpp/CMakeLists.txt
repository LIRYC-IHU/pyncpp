# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

include(ExternalProject)
include(CMakePackageConfigHelpers)

PYNCPP_generate_external_project_directories(pyncpp
    PYNCPP_PREFIX
    PYNCPP_DOWNLOAD_DIR
    _ignore
    PYNCPP_BINARY_DIR
    PYNCPP_STAMP_DIR
    PYNCPP_TMP_DIR
    )

set(PYNCPP_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source")

set(PYNCPP_EXPORT_FILE ${CMAKE_CURRENT_BINARY_DIR}/PYNCPP_exports.cmake)

set(cmake_args
    -D "CMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
    -D "CMAKE_MODULE_PATH:PATH=${PROJECT_SOURCE_DIR}/cmake"
    -D "PYNCPP_EXPORT_FILE:FILEPATH=${PYNCPP_EXPORT_FILE}"
    -D PYNCPP_QT5_SUPPORT:BOOL=${PYNCPP_QT5_SUPPORT}
    -D PYNCPP_SWIG_SUPPORT:BOOL=${PYNCPP_SWIG_SUPPORT}
    )

if(PYNCPP_USE_CUSTOM_PYTHON)
    ExternalProject_Get_property(python PREFIX)
    set(Python_DIR ${PREFIX})
    list(APPEND cmake_args
        -D "Python_DIR:PATH=${Python_DIR}"
        )
endif()

if(PYNCPP_QT5_SUPPORT)
    list(APPEND cmake_args -D "Qt5_DIR:PATH=${Qt5_DIR}")
endif()

if(PYNCPP_SWIG_SUPPORT)
    list(APPEND cmake_args
        -D "PYNCPP_SWIG_VERSION:STRING=${PYNCPP_SWIG_VERSION}"
        -D "SWIG_EXECUTABLE:PATH=${SWIG_EXECUTABLE}"
        )
endif()

ExternalProject_Add(pyncpp
    DEPENDS python
    DOWNLOAD_COMMAND ""
    INSTALL_COMMAND ""
    CMAKE_ARGS ${cmake_args}
    PREFIX "${PYNCPP_PREFIX}"
    DOWNLOAD_DIR "${PYNCPP_DOWNLOAD_DIR}"
    SOURCE_DIR "${PYNCPP_SOURCE_DIR}"
    BINARY_DIR "${PYNCPP_BINARY_DIR}"
    STAMP_DIR "${PYNCPP_STAMP_DIR}"
    TMP_DIR "${PYNCPP_TMP_DIR}"
    )

################################################################################
# Export modules
################################################################################

set(_exported_modules
    add_bindings_module
    add_path_config_file
    import_python_library
    install_executable
    install_standard_library
    )

set(_exported_modules_dir "${CMAKE_CURRENT_BINARY_DIR}/cmake")
file(MAKE_DIRECTORY "${_exported_modules_dir}")

foreach(_module ${_exported_modules})
    file(COPY "${PROJECT_SOURCE_DIR}/cmake/${_module}.cmake" DESTINATION "${_exported_modules_dir}")
endforeach()

################################################################################
# Config files
################################################################################

configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/PYNCPPConfig.cmake.in" "${PROJECT_BINARY_DIR}/PYNCPPConfig.cmake"
    INSTALL_PREFIX "${PROJECT_BINARY_DIR}"
    INSTALL_DESTINATION "${PROJECT_BINARY_DIR}"
    PATH_VARS
    PYNCPP_BINARY_DIR
    Python_DIR
    PYNCPP_EXPORT_FILE
    _exported_modules_dir
    )

write_basic_package_version_file("${PROJECT_BINARY_DIR}/PYNCPPConfigVersion.cmake"
    VERSION "${PYNCPP_VERSION}"
    COMPATIBILITY SameMinorVersion
    )
