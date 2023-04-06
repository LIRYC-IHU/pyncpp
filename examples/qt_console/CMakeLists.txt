# Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

set(TARGET_NAME qt_console)

################################################################################
# Executable
################################################################################

add_executable(${TARGET_NAME}
    main.cpp
    application.h
    application.cpp
    )

if(APPLE)
    if(PYNCPP_EXAMPLES_FRAMEWORKS)
        set_target_properties(${TARGET_NAME} PROPERTIES
            MACOSX_BUNDLE TRUE
            )
    else()
        set_target_properties(${TARGET_NAME} PROPERTIES
            INSTALL_RPATH "@executable_path/../lib")
    endif()
elseif(UNIX)
    set_target_properties(${TARGET_NAME} PROPERTIES
        INSTALL_RPATH "\${ORIGIN}/../lib")
endif()

################################################################################
# Path config file
################################################################################

PYNCPP_target_create_path_config_file(${TARGET_NAME})

################################################################################
# Link
################################################################################

target_link_libraries(${TARGET_NAME} PUBLIC
    pyncpp
    Qt5::Core
    Qt5::Widgets
    )

################################################################################
# Additional Python modules
################################################################################

PYNCPP_add_python_modules(${TARGET_NAME}_python_modules pip_install_test)

################################################################################
# Install
################################################################################

if(PYNCPP_EXAMPLES_FRAMEWORKS)
    install(TARGETS ${TARGET_NAME}
        BUNDLE
        DESTINATION bin
        COMPONENT ${TARGET_NAME}
        EXCLUDE_FROM_ALL
        )

    set(_bundle_install_path "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/${TARGET_NAME}.app")
    set(_stdlib_destination "bin/${TARGET_NAME}.app/Contents/Resources")
    set(_stdlib_install_path "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/${_stdlib_destination}")
    set(_pth_file "${_bundle_install_path}/Contents/MacOS/${TARGET_NAME}._pth")

    PYNCPP_get_python_path_config(_python_paths
        EXECUTABLE_PATH "${_bundle_install_path}/Contents/MacOS"
        STDLIB_PATH "${_stdlib_install_path}"
        )

    install(CODE "
        execute_process(COMMAND ${MACDEPLOYQT_EXECUTABLE} \"${_bundle_install_path}\")
        include(BundleUtilities)
        fixup_bundle(\"${_bundle_install_path}\" \"\" \"${PYNCPP_LIBRARY_DIRS}\")
        file(WRITE \"${_pth_file}\" \"${_python_paths}\")
        "
        COMPONENT ${TARGET_NAME}
        EXCLUDE_FROM_ALL
        )

    #PYNCPP_install_python_modules(STDLIB_DESTINATION ${_stdlib_destination} COMPONENT ${TARGET_NAME})
else()
    install(TARGETS ${TARGET_NAME}
        RUNTIME
        DESTINATION bin
        COMPONENT ${TARGET_NAME}
        )

    set(_executable_install_path "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin")
    set(_stdlib_install_path "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/lib")

    PYNCPP_get_python_path_config(_python_paths
        EXECUTABLE_PATH "${_executable_install_path}"
        STDLIB_PATH "${_stdlib_install_path}"
        )

    install(CODE "
        file(WRITE \"${_executable_install_path}/${TARGET_NAME}._pth\" \"${_python_paths}\")
        "
        COMPONENT ${TARGET_NAME}
        )
endif()

################################################################################
# Packaging
################################################################################

if(PYNCPP_EXAMPLES_FRAMEWORKS)
   set(CPACK_INSTALL_CMAKE_PROJECTS
       ${CPACK_INSTALL_CMAKE_PROJECTS}
       "${CMAKE_CURRENT_BINARY_DIR};examples;${TARGET_NAME};/"
       "${PYNCPP_DIR};pyncpp;python;/${_stdlib_destination}"
       PARENT_SCOPE
       )
endif()
