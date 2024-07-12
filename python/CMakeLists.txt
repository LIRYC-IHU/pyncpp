# Copyright (c) 2023-2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Settings
################################################################################

set(PYNCPP_PYTHON_SHORT_VERSION "${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}")
set(PYNCPP_PYTHON_SHORT_VERSION ${PYNCPP_PYTHON_SHORT_VERSION} PARENT_SCOPE)
set(PYNCPP_PYTHON_VERSION "${PYNCPP_PYTHON_SHORT_VERSION}.${PYNCPP_PYTHON_VERSION_PATCH}")
set(PYNCPP_PYTHON_VERSION ${PYNCPP_PYTHON_VERSION} PARENT_SCOPE)

set(PYNCPP_PYTHON_ENABLE_OPTMIZATIONS FALSE CACHE BOOL "")

if(WIN32)
    set(subdir "${PYNCPP_LIBRARY_SUBDIR}/python${PYNCPP_PYTHON_VERSION_MAJOR}${PYNCPP_PYTHON_VERSION_MINOR}")
else()
    set(subdir "${PYNCPP_LIBRARY_SUBDIR}/python${PYNCPP_PYTHON_SHORT_VERSION}")
endif()

set(subdir_text
    "Path of the embedded Python interpreter relative to the package root"
    )

set(PYNCPP_PYTHON_SUBDIR "${subdir}" CACHE PATH ${subdir_text})

set(prefix "${CMAKE_CURRENT_BINARY_DIR}")

################################################################################
# Platform-specific
################################################################################

include("${CMAKE_CURRENT_LIST_DIR}/hash.cmake")

if(WIN32)
    include("${CMAKE_CURRENT_SOURCE_DIR}/windows.cmake")
else()
    include("${CMAKE_CURRENT_SOURCE_DIR}/unix.cmake")
endif()

if(WIN32)
    set(_version ${pyncpp_PYTHON_VERSION_MAJOR}${pyncpp_PYTHON_VERSION_MINOR})
else()
    set(_version ${pyncpp_PYTHON_SHORT_VERSION})
endif()

################################################################################
# Config module
################################################################################

foreach(config ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})

    set(share_dir ${PYNCPP_ROOT_BASE}/${config}/${PYNCPP_SHARE_SUBDIR})
    string(TOUPPER ${config} CONFIG)

    configure_file("pyncppConfig_python.cmake.in"
        "${share_dir}/pyncppConfig_python.cmake"
        @ONLY
        )

    install(FILES
        "${share_dir}/pyncppConfig_python.cmake"
        CONFIGURATIONS ${config}
        DESTINATION "${PYNCPP_SHARE_SUBDIR}"
        )

endforeach()

################################################################################
# Variable exports
################################################################################

set(PYNCPP_PYTHON_LINK_LIBRARIES ${PYNCPP_PYTHON_LINK_LIBRARIES} PARENT_SCOPE)
set(PYNCPP_PYTHON_INCLUDE_DIRS ${PYNCPP_PYTHON_INCLUDE_DIRS} PARENT_SCOPE)
set(PYNCPP_PYTHON_SITE_DIR ${PYNCPP_PYTHON_SITE_DIR} PARENT_SCOPE)
