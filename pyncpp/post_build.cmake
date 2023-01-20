# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

include(CMakePackageConfigHelpers)

set(exported_modules
    add_bindings_module
    add_path_config_file
    install_path_config_file
    import_python_library
    install_embedded_python
    target_add_path_config_file
    target_install_path_config_file
    )

set(exported_modules_dir "${PYNCPP_BINARY_DIR}/cmake")
file(MAKE_DIRECTORY "${exported_modules_dir}")

foreach(_module ${exported_modules})
    file(COPY "${PYNCPP_CMAKE_DIR}/${_module}.cmake" DESTINATION "${exported_modules_dir}")
endforeach()

configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/PYNCPPConfig.cmake.in" "${PYNCPP_BINARY_DIR}/PYNCPPConfig.cmake"
    INSTALL_PREFIX "${PROJECT_BINARY_DIR}"
    INSTALL_DESTINATION "${PYNCPP_BINARY_DIR}"
    PATH_VARS
    PYNCPP_BINARY_DIR
    Python_DIR
    PYNCPP_EXPORT_FILE
    exported_modules_dir
    )

write_basic_package_version_file("${PYNCPP_BINARY_DIR}/PYNCPPConfigVersion.cmake"
    VERSION "${PYNCPP_VERSION}"
    COMPATIBILITY SameMinorVersion
    )
