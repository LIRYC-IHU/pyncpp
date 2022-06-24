# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0011 NEW) # Policy PUSH/POP in included scripts
cmake_policy(SET CMP0048 NEW) # Project command manages version variables
cmake_policy(SET CMP0057 NEW) # IN_LIST operator

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

include(generate_external_project_directories)
include(get_version_from_git)

################################################################################
# Project
################################################################################

PYNCPP_get_version_from_git(PYNCPP_VERSION_MAJOR PYNCPP_VERSION_MINOR PYNCPP_VERSION_PATCH)
set(PYNCPP_VERSION "${PYNCPP_VERSION_MAJOR}.${PYNCPP_VERSION_MINOR}.${PYNCPP_VERSION_PATCH}")

project(pyncpp_superbuild VERSION "${PYNCPP_VERSION}")

################################################################################
# Project settings
################################################################################

set(CMAKE_CXX_STANDARD 17)

set(PYNCPP_REQUIRED_PYTHON_VERSION_MAJOR 3)
set(PYNCPP_REQUIRED_PYTHON_VERSION_MINOR 10)
set(PYNCPP_REQUIRED_PYTHON_VERSION_PATCH 4)
set(PYNCPP_REQUIRED_PYTHON_VERSION "${PYNCPP_REQUIRED_PYTHON_VERSION_MAJOR}.${PYNCPP_REQUIRED_PYTHON_VERSION_MINOR}.${PYNCPP_REQUIRED_PYTHON_VERSION_PATCH}")

set(PYNCPP_REQUIRE_EXACT_PYTHON_VERSION FALSE)

if(WIN32)
    set(_use_custom_python_doc "Install Python as an external project (recommended).")
else()
    set(_use_custom_python_doc "Build Python as an external project (recommended).")
endif()

option(PYNCPP_USE_CUSTOM_PYTHON
    ${_use_custom_python_doc}
    ON)

set(PYNCPP_EXCLUDED_PYTHON_MODULES
    idlelib
    test
    turtledemo
    tkinter
    CACHE STRING
    "Standard Python modules to exclude.")

set(PYNCPP_EXCLUDED_PYTHON_EXTENSIONS
    CACHE STRING
    "Standard Python extensions to exclude.")

option(PYNCPP_QT5_SUPPORT
    "Include support for Qt5 based C++ applications."
    ON)

option(PYNCPP_SWIG_SUPPORT
    "Include support for SWIG bindings."
    ON)

set(PYNCPP_SWIG_VERSION 4.0 CACHE STRING "SWIG version to use for bindings.")

option(PYNCPP_NO_GPL
    "Exclude components that could require the embedding application to be GPL-compatible."
    ON)

option(PYNCPP_BUILD_EXAMPLES ON)

set(OPENSSL_ROOT_DIR CACHE STRING
    "Root directory of OpenSSL.")

################################################################################
# Dependencies
################################################################################

if(PYNCPP_QT5_SUPPORT)
    find_package(Qt5 REQUIRED COMPONENTS Core)
endif()

if(PYNCPP_SWIG_SUPPORT)
    find_package(SWIG ${PYNCPP_SWIG_VERSION} REQUIRED COMPONENTS python)
endif()

if(PYNCPP_USE_CUSTOM_PYTHON)
    find_package(OpenSSL COMPONENTS SSL)
    if(NOT OpenSSL_FOUND)
        string(CONCAT _message
            "OpenSSL could not be found. Some of the standard Python modules may"
            " not be built, and pip install will not be usable to install"
            " additional packages."
            )
        message(WARNING ${_message})
    endif()
else()
    if(PYNCPP_REQUIRE_EXACT_PYTHON_VERSION)
        set(_exact EXACT)
    else()
        set(_exact)
    endif()
    find_package(Python ${PYNCPP_REQUIRED_PYTHON_VERSION} ${_exact} REQUIRED
        COMPONENTS Interpreter Development
        )
endif()

################################################################################
# Subdirectories
################################################################################

if(PYNCPP_USE_CUSTOM_PYTHON)
    add_subdirectory(python)
endif()

add_subdirectory(pyncpp)

if(PYNCPP_BUILD_EXAMPLES)
    add_subdirectory(pyncpp_examples)
endif()
