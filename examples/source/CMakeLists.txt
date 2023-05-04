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

set(PYNCPP_EXAMPLES_VERSION "${PYNCPP_VERSION_MAJOR}.${PYNCPP_VERSION_MINOR}.${PYNCPP_VERSION_PATCH}")
project(pyncpp_examples VERSION "${PYNCPP_EXAMPLES_VERSION}")

################################################################################
# Settings
################################################################################

if(APPLE)
    if(PYNCPP_EXAMPLES_FRAMEWORKS)
        set(CMAKE_MACOSX_BUNDLE TRUE)
    else()
        set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
    endif()
elseif(UNIX)
    set(CMAKE_INSTALL_RPATH "\${ORIGIN}/../lib")
endif()

################################################################################
# Dependencies
################################################################################

find_package(pyncpp REQUIRED
    COMPONENTS CPP_API
    OPTIONAL_COMPONENTS Qt5
    )

set(PYNCPP_QT5_SUPPORT ${pyncpp_Qt5_FOUND})

if(PYNCPP_QT5_SUPPORT)
    find_package(Qt5 REQUIRED COMPONENTS Core Widgets)
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
endif()

################################################################################
# Compile definitions
################################################################################

if(WIN32)
    set(python_home ".")
else()
    if(APPLE AND PYNCPP_EXAMPLES_FRAMEWORKS)
        set(python_home "../../../../${pyncpp_PYTHON_INSTALL_DESTINATION}")
    else()
        set(python_home "../${pyncpp_PYTHON_INSTALL_DESTINATION}")
    endif()
endif()

add_compile_definitions(PYTHON_HOME="${python_home}")

################################################################################
# Subdirectories
################################################################################

if(PYNCPP_QT5_SUPPORT)
    add_subdirectory(qt_application)
endif()
