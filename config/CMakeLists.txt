# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

include(CMakePackageConfigHelpers)

set(path_vars
    PYNCPP_MODULE_DIR
    PYNCPP_PYTHON_DIR
    PYNCPP_EXPORT_FILE
    )

set(PYNCPP_EXPORT_FILE share/pyncpp/pyncpp.cmake)

################################################################################
# Build tree config files
################################################################################

set(installed FALSE)

configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/PYNCPPConfig.cmake.in" "${PROJECT_BINARY_DIR}/PYNCPPConfig.cmake"
    INSTALL_PREFIX "${PROJECT_BINARY_DIR}"
    INSTALL_DESTINATION "${PROJECT_BINARY_DIR}"
    PATH_VARS ${path_vars}
    )

write_basic_package_version_file("${PROJECT_BINARY_DIR}/PYNCPPConfigVersion.cmake"
    VERSION "@PROJECT_VERSION_MAJOR@.@PROJECT_VERSION_MINOR@.@PROJECT_VERSION_PATCH@"
    COMPATIBILITY SameMinorVersion
    )

################################################################################
# Installed config files
################################################################################

set(PYNCPP_MODULE_DIR share/pyncpp/cmake)
set(PYNCPP_PYTHON_DIR ${PYNCPP_PYTHON_INSTALL_DESTINATION})

set(installed TRUE)

configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/PYNCPPConfig.cmake.in" "${PROJECT_BINARY_DIR}/share/PYNCPPConfig-install.cmake"
    INSTALL_DESTINATION share/pyncpp
    PATH_VARS ${path_vars}
    )

write_basic_package_version_file("${PROJECT_BINARY_DIR}/share/PYNCPPConfigVersion-install.cmake"
    VERSION "@PROJECT_VERSION_MAJOR@.@PROJECT_VERSION_MINOR@.@PROJECT_VERSION_PATCH@"
    COMPATIBILITY SameMinorVersion
    )

install(FILES "${PROJECT_BINARY_DIR}/share/PYNCPPConfig-install.cmake"
    DESTINATION share/pyncpp
    RENAME "PYNCPPConfig.cmake"
    )

install(FILES "${PROJECT_BINARY_DIR}/share/PYNCPPConfigVersion-install.cmake"
    DESTINATION share/pyncpp
    RENAME "PYNCPPConfigVersion.cmake"
    )
