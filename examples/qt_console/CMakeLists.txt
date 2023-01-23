# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
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
    set(_stdlib_install_path "${_bundle_install_path}/Contents/Resources")
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
else()
    install(TARGETS ${TARGET_NAME}
        RUNTIME
        DESTINATION bin
        COMPONENT ${TARGET_NAME}
        EXCLUDE_FROM_ALL
        )

    set(_executable_install_path "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin")
    set(_stdlib_install_path "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/lib")
    set(_pth_file "${_executable_install_path}/${TARGET_NAME}._pth")

    PYNCPP_get_python_path_config(_python_paths
        EXECUTABLE_PATH "${_executable_install_path}"
        STDLIB_PATH "${_stdlib_install_path}"
        )

    install(CODE "
        file(WRITE \"${_pth_file}\" \"${_python_paths}\")
        "
        COMPONENT ${TARGET_NAME}
        EXCLUDE_FROM_ALL
        )
endif()

################################################################################
# Packaging
################################################################################

if(PYNCPP_EXAMPLES_FRAMEWORKS)
   set(CPACK_BUNDLE_NAME ${TARGET_NAME})

   set(CPACK_INSTALL_CMAKE_PROJECTS
       ${CPACK_INSTALL_CMAKE_PROJECTS}
       "${CMAKE_CURRENT_BINARY_DIR};examples;${TARGET_NAME};/"
       "${PYNCPP_DIR};pyncpp;python;/bin/${TARGET_NAME}.app/Contents/Resources"
       PARENT_SCOPE
       )
else()
   set(CPACK_INSTALL_CMAKE_PROJECTS
       ${CPACK_INSTALL_CMAKE_PROJECTS}
       "${CMAKE_CURRENT_BINARY_DIR};examples;${TARGET_NAME};/"
       "${PYNCPP_DIR};pyncpp;ALL;/"
       PARENT_SCOPE
       )
endif()
