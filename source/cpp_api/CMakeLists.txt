# Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Files
################################################################################

set(headers
    pyncpp.h
    pyncpp/conversion.h
    pyncpp/conversion/object.h
    pyncpp/conversion/primitive.h
    pyncpp/conversion/stdlist.h
    pyncpp/conversion/stdvector.h
    pyncpp/error.h
    pyncpp/error/base_exception.h
    pyncpp/error/error_handling.h
    pyncpp/error/exception_thrower.h
    pyncpp/error/exception_traceback.h
    pyncpp/error/exception_types.h
    pyncpp/export.h
    pyncpp/external/cpython.h
    pyncpp/external/cpython_call.h
    pyncpp/manager.h
    pyncpp/object.h
    pyncpp/object/abstract_object.h
    pyncpp/object/attribute_accessor.h
    pyncpp/object/container_accessor.h
    pyncpp/object/function_call.h
    pyncpp/object/module.h
    pyncpp/object/object.h
    pyncpp/object/standard_objects.h
    )

set(sources
    pyncpp/conversion/object.cpp
    pyncpp/conversion/primitive.cpp
    pyncpp/conversion/stdlist.cpp
    pyncpp/conversion/stdvector.cpp
    pyncpp/error/base_exception.cpp
    pyncpp/error/error_handling.cpp
    pyncpp/error/exception_thrower.cpp
    pyncpp/error/exception_traceback.cpp
    pyncpp/error/exception_types.cpp
    pyncpp/manager.cpp
    pyncpp/object/abstract_object.cpp
    pyncpp/object/attribute_accessor.cpp
    pyncpp/object/container_accessor.cpp
    pyncpp/object/function_call.cpp
    pyncpp/object/module.cpp
    pyncpp/object/object.cpp
    pyncpp/object/standard_objects.cpp
    )

################################################################################
# Library
################################################################################

add_library(pyncpp_cpp_api SHARED
    ${headers}
    ${sources}
    )

add_dependencies(pyncpp_cpp_api pyncpp_python)

################################################################################
# Target properties
################################################################################

if(UNIX)
    if(APPLE)
        set(rpath "@loader_path")
    else()
        set(rpath "\$ORIGIN")
    endif()

    set_target_properties(pyncpp_cpp_api PROPERTIES
        INSTALL_RPATH "${CMAKE_INSTALL_RPATH};${rpath}"
        )
endif()

################################################################################
# Compile definitions
################################################################################

if(WIN32)
    set(os_name WINDOWS)
elseif(LINUX)
    set(os_name LINUX)
elseif(APPLE)
    set(os_name MACOS)
endif()

target_compile_definitions(pyncpp_cpp_api PUBLIC
    OS_${os_name}
    DEFAULT_PYTHON_HOME="${PYNCPP_RELATIVE_PYTHON_DIR}"
    )

################################################################################
# Includes
################################################################################

if(WIN32)
    set(python_subdir ${PYNCPP_PYTHON_MAJOR_VERSION}${PYNCPP_PYTHON_MINOR_VERSION})
else()
    set(python_subdir ${PYNCPP_PYTHON_SHORT_VERSION})
endif()

target_include_directories(pyncpp_cpp_api
    PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/pyncpp"
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:${PYNCPP_INCLUDE_SUBDIR}/pyncpp>"
    "$<INSTALL_INTERFACE:${PYNCPP_INCLUDE_SUBDIR}/${python_subdir}>"
    )

foreach(python_dir ${PYNCPP_PYTHON_INCLUDE_DIRS})
    target_include_directories(pyncpp_cpp_api
        PUBLIC
        "$<BUILD_INTERFACE:${python_dir}>"
        )
endforeach()

################################################################################
# Link
################################################################################

target_link_libraries(pyncpp_cpp_api PUBLIC
    ${PYNCPP_PYTHON_LINK_LIBRARIES}
    )

################################################################################
# Install/Export
################################################################################

install(TARGETS pyncpp_cpp_api
    EXPORT pyncpp_export_cpp_api

    RUNTIME
    DESTINATION "${PYNCPP_RUNTIME_SUBDIR}"
    COMPONENT Runtime

    LIBRARY
    DESTINATION "${PYNCPP_LIBRARY_SUBDIR}"
    COMPONENT Runtime

    ARCHIVE
    DESTINATION "${PYNCPP_ARCHIVE_SUBDIR}"
    )

install(EXPORT pyncpp_export_cpp_api
    DESTINATION "${PYNCPP_SHARE_SUBDIR}"
    )

foreach(config ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})
    export(EXPORT pyncpp_export_cpp_api
        FILE "${PYNCPP_ROOT_BASE}/${config}/${PYNCPP_SHARE_SUBDIR}/pyncpp_export_cpp_api.cmake"
        )
endforeach()

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
    DESTINATION "${PYNCPP_INCLUDE_SUBDIR}/pyncpp"
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "CMakeLists.txt" EXCLUDE
    )
