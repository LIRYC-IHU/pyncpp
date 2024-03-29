# Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Files
################################################################################

set(headers
    pyncpp.h
    pyncpp/conversion.h
    pyncpp/conversion/object.h
    pyncpp/conversion/primitive.h
    pyncpp/conversion/qlist.h
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
    pyncpp/conversion/qstring.cpp
    pyncpp/conversion/qvariant.cpp
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

set(compile_defnitions)

if(PYNCPP_QT5_SUPPORT)
    list(APPEND compile_definitions PYNCPP_QT5_SUPPORT)
endif()

target_compile_definitions(pyncpp_cpp_api PUBLIC ${compile_definitions})

################################################################################
# Includes
################################################################################

target_include_directories(pyncpp_cpp_api
    PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/pyncpp"
    PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
    "$<INSTALL_INTERFACE:include/pyncpp/cpp_api>"
    )

################################################################################
# Link
################################################################################

target_link_libraries(pyncpp_cpp_api PUBLIC
    ${pyncpp_PYTHON_LIBRARY}
    )

target_link_libraries(pyncpp_cpp_api PRIVATE
    Qt5::Core
    Qt5::Widgets
    )

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
