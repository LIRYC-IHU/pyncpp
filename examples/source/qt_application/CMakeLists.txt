# Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Executable
################################################################################

add_executable(qt_application
    main.cpp
    )

################################################################################
# Link
################################################################################

target_link_libraries(qt_application PUBLIC
    ${pyncpp_CPP_API_LIBRARY}
    Qt5::Core
    Qt5::Widgets
    )

################################################################################
# Install
################################################################################

if(PYNCPP_EXAMPLES_FRAMEWORKS)
    install(TARGETS qt_application
        BUNDLE
        DESTINATION bin
        COMPONENT qt_application
        )

    set(_bundle_install_path "\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/bin/qt_application.app")

    install(CODE "
        execute_process(COMMAND ${MACDEPLOYQT_EXECUTABLE} \"${_bundle_install_path}\")
        include(BundleUtilities)
        fixup_bundle(\"${_bundle_install_path}\" \"\" \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/lib\")
        "
        COMPONENT qt_application
        )
else()
    install(TARGETS qt_application
        RUNTIME
        DESTINATION bin
        COMPONENT qt_application
        )
endif()
