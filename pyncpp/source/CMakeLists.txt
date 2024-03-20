# Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

cmake_minimum_required(VERSION ${CMAKE_MINIMUM_REQUIRED_VERSION})

cmake_policy(SET CMP0011 NEW) # Policy PUSH/POP in included scripts
cmake_policy(SET CMP0048 NEW) # Project command manages version variables
cmake_policy(SET CMP0057 NEW) # IN_LIST operator
cmake_policy(SET CMP0068 NEW) # RPATH settings on macOS do not affect install_name

################################################################################
# Project
################################################################################

set(PYNCPP_VERSION "${PYNCPP_VERSION_MAJOR}.${PYNCPP_VERSION_MINOR}.${PYNCPP_VERSION_PATCH}")
project(pyncpp VERSION "${PYNCPP_VERSION}")

################################################################################
# Dependencies
################################################################################

find_package(pyncpp REQUIRED OPTIONAL_COMPONENTS Qt5 SWIG)

set(PYNCPP_QT5_SUPPORT ${pyncpp_Qt5_FOUND})

if(PYNCPP_QT5_SUPPORT)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
endif()

################################################################################
# Subdirectories
################################################################################

add_subdirectory(cpp_api)
add_subdirectory(tools)

################################################################################
# Install/Export
################################################################################

install(EXPORT pyncpp_exports
    DESTINATION "${PYNCPP_SHARE_SUBDIR}"
    COMPONENT Development
    )
