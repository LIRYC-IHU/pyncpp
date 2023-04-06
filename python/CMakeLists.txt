# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

set(PYNCPP_PYTHON_VERSION_MAJOR 3 CACHE STRING "Python major version")
set(PYNCPP_PYTHON_VERSION_MINOR 10 CACHE STRING "Python minor version")
set(PYNCPP_PYTHON_VERSION_PATCH 10 CACHE STRING "Python patch version")

set(PYNCPP_PYTHON_SHORT_VERSION "${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}")
set(PYNCPP_PYTHON_SHORT_VERSION ${PYNCPP_PYTHON_SHORT_VERSION} PARENT_SCOPE)
set(PYNCPP_PYTHON_VERSION "${PYNCPP_PYTHON_SHORT_VERSION}.${PYNCPP_PYTHON_VERSION_PATCH}")
set(PYNCPP_PYTHON_VERSION ${PYNCPP_PYTHON_VERSION} PARENT_SCOPE)

set(PYNCPP_PYTHON_DIR "${PROJECT_BINARY_DIR}/python${PYNCPP_PYTHON_SHORT_VERSION}" CACHE PATH "")

if(WIN32)
    set(PYNCPP_PYTHON_INSTALL_DESTINATION "bin" CACHE STRING "")
else()
    set(PYNCPP_PYTHON_INSTALL_DESTINATION "lib/python${PYNCPP_PYTHON_SHORT_VERSION}" CACHE STRING "")
endif()

set(prefix "${CMAKE_CURRENT_BINARY_DIR}")
set(download_dir "${prefix}/download")
set(source_dir "${prefix}/source")
set(binary_dir "${prefix}/build")
set(stamp_dir "${prefix}/stamp")
set(tmp_dir "${prefix}/tmp")

include("${CMAKE_CURRENT_LIST_DIR}/hash.cmake")

if(WIN32)
    include("${CMAKE_CURRENT_SOURCE_DIR}/windows.cmake")
else()
    include("${CMAKE_CURRENT_SOURCE_DIR}/unix.cmake")
endif()
