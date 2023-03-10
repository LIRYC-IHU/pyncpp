# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

include(CMakePackageConfigHelpers)
include(add_path_config_file)

cmake_policy(SET CMP0011 NEW) # Policy PUSH/POP in included scripts

configure_package_config_file("${CMAKE_CURRENT_LIST_DIR}/PythonConfig.cmake.in" "${PYTHON_BINARY_DIR}/PythonConfig.cmake"
    INSTALL_PREFIX "${PYTHON_PREFIX}"
    INSTALL_DESTINATION "${PYTHON_BINARY_DIR}"
    PATH_VARS PYTHON_SOURCE_DIR PYTHON_BINARY_DIR
    )

write_basic_package_version_file("${PYTHON_BINARY_DIR}/PythonConfigVersion.cmake"
    VERSION "${PYNCPP_REQUIRED_PYTHON_VERSION_MAJOR}.${PYNCPP_REQUIRED_PYTHON_VERSION_MINOR}.${PYNCPP_REQUIRED_PYTHON_VERSION_PATCH}"
    COMPATIBILITY SameMinorVersion
    )

find_package(Python REQUIRED
    COMPONENTS Interpreter
    PATHS ${PYTHON_BINARY_DIR}
    NO_DEFAULT_PATHS
    )

PYNCPP_add_path_config_file(${Python_EXECUTABLE}
    PATHS ${Python_STDLIB} ${Python_STDARCH}
    )
