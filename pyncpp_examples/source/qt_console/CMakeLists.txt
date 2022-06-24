# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

set(TARGET_NAME pyncpp_qt_console)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

find_package(PYNCPP REQUIRED COMPONENTS Qt5)

add_executable(${TARGET_NAME}
    main.cpp
    application.h
    application.cpp
    )

target_link_libraries(${TARGET_NAME} PUBLIC
    pyncpp
    Qt5::Core
    Qt5::Widgets
    )

if(APPLE)
    set_target_properties(${TARGET_NAME} PROPERTIES MACOSX_BUNDLE TRUE)
    set(CPACK_BUNDLE_NAME ${TARGET_NAME})
    set(_stdlib_prefix "bin/${TARGET_NAME}/Content/Resources")
else()
    set(_stdlib_prefix "resources/python")
endif()

set(_stdlib "${_stdlib_prefix}/lib/python${Python_VERSION_MAJOR}.${Python_VERSION_MINOR}")

PYNCPP_install_executable("${TARGET_NAME}"
    DESTINATION bin
    PYTHON_STDLIB_PATH "${_stdlib}"
    )

PYNCPP_install_standard_library(DESTINATION "${_stdlib}")

if(APPLE)
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")
    set(_installed_bundle "\${CMAKE_INSTALL_PREFIX}/bin/${TARGET_NAME}")

    install(CODE "
        execute_process(COMMAND ${MACDEPLOYQT_EXECUTABLE}
            \"${_installed_bundle}\"
            \"-libpath=${PYNCPP_LIBRARY_DIRS}\"
            )
        ")
endif()

include(CPack)
