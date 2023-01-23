# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

cmake_minimum_required(VERSION 3.19)

cmake_policy(SET CMP0011 NEW) # Policy PUSH/POP in included scripts
cmake_policy(SET CMP0048 NEW) # Project command manages version variables
cmake_policy(SET CMP0057 NEW) # IN_LIST operator
cmake_policy(SET CMP0068 NEW) # RPATH settings on macOS do not affect install_name

################################################################################
# Project
################################################################################

project(pyncpp_examples VERSION "${PYNCPP_VERSION}")

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

if(APPLE)
    option(PYNCPP_EXAMPLES_FRAMEWORKS "Build frameworks." ON)
endif()

################################################################################
# Dependencies
################################################################################

find_package(PYNCPP REQUIRED COMPONENTS Qt5)

if(PYNCPP_Qt5_FOUND)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
endif()

################################################################################
# Subdirectories
################################################################################

if(PYNCPP_Qt5_FOUND)
    add_subdirectory(qt_console)
endif()

################################################################################
# Packaging
################################################################################

include(CPack)
