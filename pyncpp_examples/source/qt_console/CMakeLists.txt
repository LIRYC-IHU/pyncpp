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
    install(TARGETS ${TARGET_NAME} BUNDLE DESTINATION bin)
    set(_bundle_install_path "bin/$<TARGET_FILE_BASE_NAME:${TARGET_NAME}>.app")
    PYNCPP_install_embedded_python(${TARGET_NAME} bin)
else()
    install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION bin)
    PYNCPP_install_embedded_python(${TARGET_NAME} lib/${TARGET_NAME})
endif()

PYNCPP_target_add_path_config_file(${TARGET_NAME})
#PYNCPP_target_install_path_config_file(${TARGET_NAME} DESTINATION bin STDLIB ${_stdlib_install_path})

if(APPLE)
    get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

    install(CODE "
        execute_process(COMMAND ${MACDEPLOYQT_EXECUTABLE}
            \"\${CMAKE_INSTALL_PREFIX}/${_bundle_install_path}\"
            )
        include(BundleUtilities)
        fixup_bundle(\"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${_bundle_install_path}\" \"\" \"${PYNCPP_LIBRARY_DIRS}\")
        ")
endif()
