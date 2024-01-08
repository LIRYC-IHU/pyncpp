# Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

include(CMakePackageConfigHelpers)

foreach(config ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})

    set(CONFIG ${config})
    set(PREFIX_DIR "${PROJECT_BINARY_DIR}/${config}")

    configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/pyncppConfig.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/pyncppConfig-${config}.cmake"
        INSTALL_PREFIX "${PREFIX_DIR}"
        INSTALL_DESTINATION "${PREFIX_DIR}/${PYNCPP_SHARE_SUBDIR}"
        PATH_VARS PREFIX_DIR
        )

    write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/pyncppConfigVersion-${config}.cmake"
        VERSION "@PROJECT_VERSION_MAJOR@.@PROJECT_VERSION_MINOR@.@PROJECT_VERSION_PATCH@"
        COMPATIBILITY SameMinorVersion
        )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/pyncppConfig-${config}.cmake"
        CONFIGURATIONS ${config}
        DESTINATION "${PYNCPP_SHARE_SUBDIR}"
        COMPONENT Development
        RENAME "pyncppConfig.cmake"
        )

    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/pyncppConfigVersion-${config}.cmake"
        CONFIGURATIONS ${config}
        DESTINATION "${PYNCPP_SHARE_SUBDIR}"
        COMPONENT Development
        RENAME "pyncppConfigVersion.cmake"
        )

endforeach()

add_custom_target(pyncpp_config ALL
    COMMAND ${CMAKE_COMMAND}
    -D "CMAKE_INSTALL_PREFIX:PATH=${PROJECT_BINARY_DIR}"
    -D "CMAKE_INSTALL_CONFIG_NAME:STRING=$<CONFIG>"
    -D "CMAKE_INSTALL_COMPONENT:STRING=Development"
    -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_install.cmake"
    COMPONENT Development
    )
