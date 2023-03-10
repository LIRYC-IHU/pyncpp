# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0011 NEW) # Policy PUSH/POP in included scripts
cmake_policy(SET CMP0048 NEW) # Project command manages version variables
cmake_policy(SET CMP0057 NEW) # IN_LIST operator
cmake_policy(SET CMP0078 NEW) # Standard SWIG target names
cmake_policy(SET CMP0086 NEW) # Pass -module option to SWIG

include(get_version_from_git)
include(import_python_library)
include(install_embedded_python)

if(PYNCPP_SWIG_SUPPORT)
    include(add_bindings_module)
endif()

################################################################################
# Project
################################################################################

PYNCPP_get_version_from_git(PYNCPP_VERSION_MAJOR PYNCPP_VERSION_MINOR PYNCPP_VERSION_PATCH)

project(pyncpp VERSION "${PYNCPP_VERSION_MAJOR}.${PYNCPP_VERSION_MINOR}.${PYNCPP_VERSION_PATCH}")

################################################################################
# Settings
################################################################################

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_compile_definitions("PYNCPP_QT5_SUPPORT=$<BOOL:PYNCPP_QT5_SUPPORT>")
add_compile_definitions("PYNCPP_SWIG_SUPPORT=$<BOOL:PYNCPP_SWIG_SUPPORT>")

################################################################################
# Dependencies
################################################################################

find_package(Python
    REQUIRED COMPONENTS Interpreter Development
    CONFIG
    )

if(NOT Python_FOUND)
    find_package(Python
        REQUIRED COMPONENTS Interpreter Development
        )
endif()

PYNCPP_import_python_library(Python_LIBRARY)

if(PYNCPP_QT5_SUPPORT)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
endif()

if(PYNCPP_SWIG_SUPPORT)
    find_package(SWIG ${PYNCPP_SWIG_VERSION} REQUIRED COMPONENTS python)
    include(${SWIG_USE_FILE})
endif()

################################################################################
# Subdirectories
################################################################################

add_subdirectory(cpp_api)

if(PYNCPP_SWIG_SUPPORT)
    add_subdirectory(swig)
endif()

################################################################################
# Install/Export
################################################################################

install(EXPORT pyncpp DESTINATION share)
export(EXPORT pyncpp FILE ${PYNCPP_EXPORT_FILE})
