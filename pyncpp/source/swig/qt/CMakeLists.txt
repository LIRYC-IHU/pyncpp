# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Sources
################################################################################

set(other_sources
    pyncpp.i
    string.i
    variant.i
    list.i
    hash.i
    base.i
    object.i
    widgets.i
    application.i
    )

################################################################################
# Library
################################################################################

PYNCPP_add_bindings_module(pyncpp_qt_bindings
    SWIG_INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/qt.i"
    OTHER_SOURCES ${other_sources}
    )

################################################################################
# Includes
################################################################################

target_include_directories(pyncpp_qt_bindings
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    )

################################################################################
# Link
################################################################################

target_link_libraries(pyncpp_qt_bindings
    PUBLIC
    Qt5::Widgets
    ${PROJECT_NAME}
    )
