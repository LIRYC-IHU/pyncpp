# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0011 NEW) # Policy PUSH/POP in included scripts
cmake_policy(SET CMP0048 NEW) # Project command manages version variables

include(get_version_from_git)

PYNCPP_get_version_from_git(PYNCPP_EXAMPLES_VERSION_MAJOR PYNCPP_EXAMPLES_VERSION_MINOR PYNCPP_EXAMPLES_VERSION_PATCH)

project(pyncpp_examples VERSION "${PYNCPP_EXAMPLES_VERSION_MAJOR}.${PYNCPP_EXAMPLES_VERSION_MINOR}.${PYNCPP_EXAMPLES_VERSION_PATCH}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PYNCPP REQUIRED)

if(PYNCPP_Qt5_FOUND)
    add_subdirectory(qt_console)
endif()

if(NOT APPLE)
    set(CPACK_INSTALL_CMAKE_PROJECTS
        ${CMAKE_CURRENT_BINARY_DIR} PYNCPP_EXAMPLES ALL /
        ${PYNCPP_DIR} PYNCPP ALL /
        )
endif()

include(CPack)
