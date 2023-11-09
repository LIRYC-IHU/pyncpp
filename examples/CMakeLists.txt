# Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Settings
################################################################################

if(APPLE)
    option(PYNCPP_EXAMPLES_FRAMEWORKS "Build examples as frameworks." ON)
endif()

################################################################################
# Dependencies
################################################################################

if(PYNCPP_QT5_SUPPORT)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
endif()

################################################################################
# Configuration
################################################################################

set(cmake_args
    -D "CMAKE_MINIMUM_REQUIRED_VERSION:STRING=${CMAKE_MINIMUM_REQUIRED_VERSION}"
    -D "CMAKE_BUILD_TYPE:PATH=${CMAKE_BUILD_TYPE}"
    -D "CMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}"
    -D "CMAKE_CXX_STANDARD_REQUIRED:BOOL=${CMAKE_CXX_STANDARD_REQUIRED}"
    -D "CMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>"
    -D "CMAKE_MAKE_PROGRAM:PATH=${CMAKE_MAKE_PROGRAM}"
    -D "pyncpp_ROOT:PATH=${PROJECT_BINARY_DIR}"
    -D "PYNCPP_VERSION_MAJOR:STRING=${PYNCPP_VERSION_MAJOR}"
    -D "PYNCPP_VERSION_MINOR:STRING=${PYNCPP_VERSION_MINOR}"
    -D "PYNCPP_VERSION_PATCH:STRING=${PYNCPP_VERSION_PATCH}"
    -D "PYNCPP_EXAMPLES_FRAMEWORKS:BOOL=${PYNCPP_EXAMPLES_FRAMEWORKS}"
    )

if(PYNCPP_QT5_SUPPORT)
    list(APPEND cmake_args -D "Qt5_DIR:PATH=${Qt5_DIR}")
endif()

set(prefix "${CMAKE_CURRENT_BINARY_DIR}")

################################################################################
# External project
################################################################################

ExternalProject_Add(pyncpp_examples
    PREFIX "${prefix}"
    DOWNLOAD_DIR "${prefix}/download"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source"
    BINARY_DIR "${prefix}/build"
    STAMP_DIR "${prefix}/stamp"
    TMP_DIR "${prefix}/tmp"
    INSTALL_DIR "${PROJECT_BINARY_DIR}"
    DEPENDS pyncpp
    CMAKE_ARGS ${cmake_args}
    )

################################################################################
# Install
################################################################################

install(SCRIPT "${prefix}/build/cmake_install.cmake")
