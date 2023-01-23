# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

cmake_minimum_required(VERSION 3.19)

cmake_policy(SET CMP0011 NEW) # Policy PUSH/POP in included scripts
cmake_policy(SET CMP0048 NEW) # Project command manages version variables
cmake_policy(SET CMP0057 NEW) # IN_LIST operator
cmake_policy(SET CMP0068 NEW) # RPATH settings on macOS do not affect install_name
cmake_policy(SET CMP0078 NEW) # Standard SWIG target names
cmake_policy(SET CMP0086 NEW) # Pass -module option to SWIG

set(CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/internal"
    ${CMAKE_MODULE_PATH}
    )

include(CMakePackageConfigHelpers)
include(ExternalProject)
include(FetchContent)

include(get_version_from_git)

################################################################################
# Project
################################################################################

PYNCPP_get_version_from_git(PYNCPP_VERSION_MAJOR PYNCPP_VERSION_MINOR PYNCPP_VERSION_PATCH)
set(PYNCPP_VERSION "${PYNCPP_VERSION_MAJOR}.${PYNCPP_VERSION_MINOR}.${PYNCPP_VERSION_PATCH}")

project(pyncpp VERSION "${PYNCPP_VERSION}")

################################################################################
# Settings
################################################################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CONFIGURATION_TYPES)
    foreach(config ${CMAKE_CONFIGURATION_TYPES})
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${config} ${PROJECT_BINARY_DIR}/${config}/bin)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${config} ${PROJECT_BINARY_DIR}/${config}/lib)
    endforeach()
else()
    if(NOT DEFINED CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE RelWithDebInfo)
    endif()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE}/lib)
endif()

set(PYNCPP_LIBRARY_DIRS
    ${PROJECT_BINARY_DIR}/$<CONFIG>/bin
    ${PROJECT_BINARY_DIR}/$<CONFIG>/lib
    )

set(PYNCPP_EXPORT_FILE ${PROJECT_BINARY_DIR}/PYNCPP_exports.cmake)

set(PYNCPP_PYTHON_VERSION_MAJOR 3)
set(PYNCPP_PYTHON_VERSION_MINOR 10)
set(PYNCPP_PYTHON_VERSION_PATCH 4)
set(PYNCPP_REQUIRE_EXACT_PYTHON_VERSION FALSE)

if(WIN32)
    set(_custom_python_doc "Install Python as an external project (necessary for embedding).")
else()
    set(_custom_python_doc "Build Python as an external project (necessary for embedding).")
endif()

option(PYNCPP_CUSTOM_PYTHON ${_custom_python_doc} ON)

if(APPLE)
    option(PYNCPP_PYTHON_FRAMEWORK "Build Python as a framework." OFF)
endif()

set(PYNCPP_EXCLUDED_PYTHON_PACKAGES
    idlelib
    test
    turtledemo
    tkinter
    )

set(PYNCPP_EXCLUDED_PYTHON_EXTENSIONS
    _tkinter
    )

option(PYNCPP_QT5_SUPPORT "Include support for Qt5 based C++ applications." ON)

option(PYNCPP_SWIG_SUPPORT "Include support for SWIG bindings." ON)

set(PYNCPP_SWIG_VERSION 4.0 CACHE STRING "SWIG version to use for bindings.")

option(PYNCPP_NO_GPL
    "Exclude components that could force the embedding application to be GPL-compatible."
    ON)

option(PYNCPP_BUILD_EXAMPLES "Build example project" ON)

set(OPENSSL_ROOT_DIR CACHE STRING "Root directory of OpenSSL.")

set(FETCHCONTENT_QUIET FALSE)

################################################################################
# Dependencies
################################################################################

if(PYNCPP_QT5_SUPPORT)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
endif()

if(PYNCPP_SWIG_SUPPORT)
    find_package(SWIG ${PYNCPP_SWIG_VERSION} REQUIRED COMPONENTS python)
endif()

################################################################################
# Subdirectories
################################################################################

add_subdirectory(python)
add_subdirectory(cpp_api)

################################################################################
# Examples
################################################################################

if(PYNCPP_BUILD_EXAMPLES)
    ExternalProject_Add(pyncpp_examples
        DEPENDS pyncpp
        DOWNLOAD_COMMAND ""
        INSTALL_COMMAND ""
        CMAKE_ARGS
        -D "CMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
        -D "PYNCPP_DIR:PATH=${PROJECT_BINARY_DIR}"
        -D "PYNCPP_VERSION:STRING=${PYNCPP_VERSION}"
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/examples"
        DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/examples/download"
        BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/examples"
        INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/examples/install"
        STAMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/examples/stamp"
        TMP_DIR "${CMAKE_CURRENT_BINARY_DIR}/examples/tmp"
        )
endif()

################################################################################
# Configuration Files
################################################################################

file(GLOB _modules
    RELATIVE "${PROJECT_SOURCE_DIR}/cmake"
    "${PROJECT_SOURCE_DIR}/cmake/*.cmake"
    )

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/cmake")

foreach(_module ${_modules})
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/cmake/${_module}" DESTINATION "${PROJECT_BINARY_DIR}/cmake")
endforeach()

set(_library_dir_vars)

if(PYNCPP_CUSTOM_PYTHON)
    list(LENGTH PYNCPP_LIBRARY_DIRS num_library_dirs)
    set(library_setters)

    foreach(i RANGE 1 ${num_library_dirs})
        math(EXPR index "${i} - 1")
        list(GET PYNCPP_LIBRARY_DIRS ${index} _library_dir)
        set(PYNCPP_LIBRARY_DIR_${i} ${_library_dir})
        list(APPEND _library_dir_vars PYNCPP_LIBRARY_DIR_${i})
    endforeach()
endif()

configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/PYNCPPConfig.cmake.in" "${PROJECT_BINARY_DIR}/PYNCPPConfig.cmake"
    INSTALL_PREFIX "${PROJECT_BINARY_DIR}"
    INSTALL_DESTINATION "${PROJECT_BINARY_DIR}"
    PATH_VARS
    ${_library_dir_vars}
    PROJECT_BINARY_DIR
    PYNCPP_EXPORT_FILE
    PYNCPP_PYTHON_STDLIB
    PYNCPP_PYTHON_STDEXT
    )

write_basic_package_version_file("${PROJECT_BINARY_DIR}/PYNCPPConfigVersion.cmake"
    VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
    COMPATIBILITY SameMinorVersion
    )

################################################################################
# Export
################################################################################

export(EXPORT pyncpp FILE ${PYNCPP_EXPORT_FILE})
