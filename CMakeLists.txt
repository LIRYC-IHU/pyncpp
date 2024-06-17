# Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

cmake_minimum_required(VERSION 3.19)

include(CMakePackageConfigHelpers)
include(ExternalProject)

################################################################################
# Version
################################################################################

find_package(Git)
set(_success FALSE)

if(GIT_EXECUTABLE)
    execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --always
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE _git_describe
        )

    if (_git_describe MATCHES "^v([0-9]+)\\.([0-9]+)(-([0-9]+))?.*$")
        set(PYNCPP_VERSION_MAJOR ${CMAKE_MATCH_1})
        set(PYNCPP_VERSION_MINOR ${CMAKE_MATCH_2})
        if(CMAKE_MATCH_4)
            set(PYNCPP_VERSION_PATCH ${CMAKE_MATCH_4})
        else()
            set(PYNCPP_VERSION_PATCH 0)
        endif()
        set(_success TRUE)
    endif()
endif()

if(_success)
    message(STATUS "Version number generated from git: ${PYNCPP_VERSION_MAJOR}.${PYNCPP_VERSION_MINOR}.${PYNCPP_VERSION_PATCH}")
else()
    set(PYNCPP_VERSION_MAJOR 0)
    set(PYNCPP_VERSION_MINOR 0)
    set(PYNCPP_VERSION_PATCH 0)
    message(WARNING "Cannot use git to generate a version number. Defaulting to 0.0.0")
endif()

################################################################################
# Project
################################################################################

set(PYNCPP_VERSION "${PYNCPP_VERSION_MAJOR}.${PYNCPP_VERSION_MINOR}.${PYNCPP_VERSION_PATCH}")
project(pyncpp VERSION "${PYNCPP_VERSION}")

################################################################################
# Settings
################################################################################

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

get_property(GENERATOR_IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(NOT GENERATOR_IS_MULTI_CONFIG AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(APPLE)
    option(PYNCPP_MACOS_BUNDLE "(macOS only) build as a bundle" ON)
    set(PYNCPP_MACOS_BUNDLE_NAME "pyncpp" CACHE STRING "")
endif()

################################################################################
# Paths
################################################################################

set(PYNCPP_ROOT_BASE "${CMAKE_BINARY_DIR}")
set(PYNCPP_ROOT "${PYNCPP_ROOT_BASE}/$<CONFIG>")

if(UNIX)
    if(APPLE AND PYNCPP_MACOS_BUNDLE)
        set(contents_subdir "${PYNCPP_MACOS_BUNDLE_NAME}.app/Contents")
        set(runtime_subdir "${contents_subdir}/MacOS")
        set(library_subdir "${contents_subdir}/Frameworks")
    else()
        set(runtime_subdir "bin")
        set(library_subdir "lib")
    endif()

    set(archive_subdir "lib")
    set(share_subdir "share/cmake/pyncpp")
else()
    set(runtime_subdir "bin")
    set(library_subdir "bin")
    set(archive_subdir "libs")
    set(share_subdir "cmake")
endif()

set(PYNCPP_RUNTIME_SUBDIR "${runtime_subdir}" CACHE PATH "")
set(PYNCPP_LIBRARY_SUBDIR "${library_subdir}" CACHE PATH "")
set(PYNCPP_ARCHIVE_SUBDIR "${archive_subdir}" CACHE PATH "")
set(PYNCPP_SHARE_SUBDIR "${share_subdir}" CACHE PATH "")
set(PYNCPP_INCLUDE_SUBDIR "include" CACHE PATH "")
set(PYNCPP_UTILS_SUBDIR "bin" CACHE PATH "")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PYNCPP_ROOT}/${PYNCPP_RUNTIME_SUBDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PYNCPP_ROOT}/${PYNCPP_LIBRARY_SUBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PYNCPP_ROOT}/${PYNCPP_ARCHIVE_SUBDIR}")

################################################################################
# Subdirectories
################################################################################

add_subdirectory(python)
add_subdirectory(source)

################################################################################
# Config files
################################################################################

foreach(config ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})

    set(prefix_dir "${PYNCPP_ROOT_BASE}/${config}")
    set(share_dir "${prefix_dir}/${PYNCPP_SHARE_SUBDIR}")

    configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/pyncppConfig.cmake.in" "${share_dir}/pyncppConfig.cmake"
        INSTALL_PREFIX "${prefix_dir}"
        INSTALL_DESTINATION "${share_dir}"
        )

    write_basic_package_version_file("${share_dir}/pyncppConfigVersion.cmake"
        VERSION ${PYNCPP_VERSION}
        COMPATIBILITY SameMinorVersion
        )

    install(FILES
        "${share_dir}/pyncppConfig.cmake"
        "${share_dir}/pyncppConfigVersion.cmake"
        CONFIGURATIONS ${config}
        DESTINATION "${PYNCPP_SHARE_SUBDIR}"
        )

endforeach()
