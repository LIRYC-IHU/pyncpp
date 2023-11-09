# Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

include(CMakePackageConfigHelpers)

################################################################################
# Build tree config files
################################################################################

set(SPECIFIC_CONFIG)
set(PREFIX_DIR "${PROJECT_BINARY_DIR}")
set(config_dir ${PREFIX_DIR}/${PYNCPP_SHARE_SUBDIR})

configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/pyncppConfig.cmake.in" "${config_dir}/pyncppConfig.cmake"
    INSTALL_PREFIX "${PREFIX_DIR}"
    INSTALL_DESTINATION "${PYNCPP_SHARE_SUBDIR}"
    PATH_VARS PREFIX_DIR
    )

write_basic_package_version_file("${config_dir}/pyncppConfigVersion.cmake"
    VERSION "@PROJECT_VERSION_MAJOR@.@PROJECT_VERSION_MINOR@.@PROJECT_VERSION_PATCH@"
    COMPATIBILITY SameMinorVersion
    )

################################################################################
# Installed config files
################################################################################

foreach(config ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})

    set(SPECIFIC_CONFIG ${config})
    set(PREFIX_DIR "${PROJECT_BINARY_DIR}/${config}")
    set(config_dir ${PREFIX_DIR}/${PYNCPP_SHARE_SUBDIR})

    configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/pyncppConfig.cmake.in" "${config_dir}/pyncppConfig.cmake"
        INSTALL_PREFIX "${PREFIX_DIR}"
        INSTALL_DESTINATION "${config_dir}"
        PATH_VARS PREFIX_DIR
        )

    write_basic_package_version_file("${config_dir}/pyncppConfigVersion.cmake"
        VERSION "@PROJECT_VERSION_MAJOR@.@PROJECT_VERSION_MINOR@.@PROJECT_VERSION_PATCH@"
        COMPATIBILITY SameMinorVersion
        )

    install(FILES
        "${config_dir}/pyncppConfig.cmake"
        "${config_dir}/pyncppConfigVersion.cmake"
        CONFIGURATIONS ${config}
        DESTINATION "${PYNCPP_SHARE_SUBDIR}"
        COMPONENT Development
        )

endforeach()
