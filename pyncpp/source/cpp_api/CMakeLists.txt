# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Generate SWIG runtime header
################################################################################

if(PYNCPP_SWIG_SUPPORT)
    set(swig_runtime_header_directory "${CMAKE_CURRENT_BINARY_DIR}/swig")
    set(swig_runtime_header "${swig_runtime_header_directory}/swig_runtime.h")

    if(NOT EXISTS "${swig_runtime_header}")
        execute_process(
            COMMAND ${CMAKE_COMMAND} -E make_directory "${swig_runtime_header_directory}"
            COMMAND ${SWIG_EXECUTABLE}
            "-python"
            "-c++"
            "-external-runtime" "${swig_runtime_header}"
            )
    endif()
endif()

################################################################################
# Files
################################################################################

set(headers
    pyncpp.h
    pyncpp/conversion.h
    pyncpp/conversion/object.h
    pyncpp/conversion/primitive.h
    pyncpp/conversion/qlist.h
    pyncpp/conversion/qobject.h
    pyncpp/conversion/qstring.h
    pyncpp/conversion/qvariant.h
    pyncpp/error.h
    pyncpp/error/base_exception.h
    pyncpp/error/error_handling.h
    pyncpp/error/exception_thrower.h
    pyncpp/error/exception_traceback.h
    pyncpp/error/exception_types.h
    pyncpp/export.h
    pyncpp/external/cpython.h
    pyncpp/external/cpython_call.h
    pyncpp/external/swig.h
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
    pyncpp/conversion/qlist.cpp
    pyncpp/conversion/qobject.cpp
    pyncpp/conversion/qstring.cpp
    pyncpp/conversion/qvariant.cpp
    pyncpp/error/base_exception.cpp
    pyncpp/error/error_handling.cpp
    pyncpp/error/exception_thrower.cpp
    pyncpp/error/exception_traceback.cpp
    pyncpp/error/exception_types.cpp
    pyncpp/external/swig.cpp
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
    ${swig_runtime_header}
    )

target_compile_definitions(pyncpp_cpp_api
    PUBLIC
    "PYNCPP_BUILD_EXTENDED_CPP_API=$<BOOL:PYNCPP_BUILD_EXTENDED_CPP_API>"
    "PYNCPP_QT5_SUPPORT=$<BOOL:PYNCPP_QT5_SUPPORT>"
    "PYNCPP_SWIG_SUPPORT=$<BOOL:PYNCPP_SWIG_SUPPORT>"
    )

################################################################################
# Includes
################################################################################

target_include_directories(pyncpp_cpp_api
    PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/pyncpp"
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<BUILD_INTERFACE:${swig_runtime_header_directory}>"
    "$<INSTALL_INTERFACE:include/pyncpp/cpp_api>"
    )

################################################################################
# Link
################################################################################

target_link_libraries(pyncpp_cpp_api PUBLIC
    ${PYNCPP_PYTHON_LIBRARY}
    )

if(PYNCPP_QT5_SUPPORT)
    target_link_libraries(pyncpp_cpp_api PRIVATE
        Qt5::Core
        Qt5::Widgets
        )
endif()

################################################################################
# Install/Export
################################################################################

install(TARGETS pyncpp_cpp_api
    EXPORT pyncpp
    LIBRARY
    DESTINATION "lib"
    )

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/"
    DESTINATION "include/pyncpp/cpp_api"
    PATTERN "CMakeLists.txt" EXCLUDE
    )
