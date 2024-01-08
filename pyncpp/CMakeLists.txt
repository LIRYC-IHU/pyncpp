# Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Settings
################################################################################

option(PYNCPP_BUILD_CPP_API "Build the C++ API provided by PYNCPP" ON)

################################################################################
# Configuration
################################################################################

set(cmake_args
    -D "CMAKE_MINIMUM_REQUIRED_VERSION:STRING=${CMAKE_MINIMUM_REQUIRED_VERSION}"
    -D "CMAKE_BUILD_TYPE:PATH=${CMAKE_BUILD_TYPE}"
    -D "CMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD}"
    -D "CMAKE_CXX_STANDARD_REQUIRED:BOOL=${CMAKE_CXX_STANDARD_REQUIRED}"
    -D "CMAKE_MAKE_PROGRAM:PATH=${CMAKE_MAKE_PROGRAM}"
    -D "pyncpp_ROOT:PATH=${PROJECT_BINARY_DIR}"
    -D "PYNCPP_VERSION_MAJOR:STRING=${PYNCPP_VERSION_MAJOR}"
    -D "PYNCPP_VERSION_MINOR:STRING=${PYNCPP_VERSION_MINOR}"
    -D "PYNCPP_VERSION_PATCH:STRING=${PYNCPP_VERSION_PATCH}"
    -D "PYNCPP_SHARE_SUBDIR:PATH=${PYNCPP_SHARE_SUBDIR}"
    -D "PYNCPP_BUILD_CPP_API:BOOL=${PYNCPP_BUILD_CPP_API}"
    )

if(PYNCPP_QT5_SUPPORT)
    list(APPEND cmake_args -D "Qt5_DIR:PATH=${Qt5_DIR}")
endif()

if(UNIX AND NOT APPLE)
    list(APPEND cmake_args -D "CMAKE_INSTALL_RPATH:PATH=\$ORIGIN/../${PYNCPP_PYTHON_INSTALL_DESTINATION}/lib")
endif()

set(prefix "${CMAKE_CURRENT_BINARY_DIR}")

################################################################################
# External project
################################################################################

ExternalProject_Add(pyncpp
    PREFIX "${prefix}"
    DOWNLOAD_DIR "${prefix}/download"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source"
    BINARY_DIR "${prefix}/build"
    STAMP_DIR "${prefix}/stamp"
    TMP_DIR "${prefix}/tmp"
    DEPENDS pyncpp_python pyncpp_config
    CMAKE_ARGS ${cmake_args}
    INSTALL_COMMAND ${CMAKE_COMMAND}
    --install "<BINARY_DIR>"
    --prefix "${PROJECT_BINARY_DIR}"
    --config $<CONFIG>
    )

################################################################################
# Install
################################################################################

install(CODE "
    execute_process(COMMAND ${CMAKE_COMMAND}
        -D \"CMAKE_INSTALL_PREFIX:PATH=\${CMAKE_INSTALL_PREFIX}\"
        -D \"CMAKE_INSTALL_CONFIG_NAME:STRING=\$<CONFIG>\"
        -D \"CMAKE_INSTALL_COMPONENT:STRING=Runtime\"
        -P \"${prefix}/build/cmake_install.cmake\"
        )
    "
    COMPONENT Runtime
    )

install(CODE "
    execute_process(COMMAND ${CMAKE_COMMAND}
        -D \"CMAKE_INSTALL_PREFIX:PATH=\${CMAKE_INSTALL_PREFIX}\"
        -D \"CMAKE_INSTALL_CONFIG_NAME:STRING=\$<CONFIG>\"
        -D \"CMAKE_INSTALL_COMPONENT:STRING=Development\"
        -P \"${prefix}/build/cmake_install.cmake\"
        )
    "
    COMPONENT Development
    )
