# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

include(ExternalProject)

PYNCPP_generate_external_project_directories(python
    PYTHON_PREFIX
    PYTHON_DOWNLOAD_DIR
    PYTHON_SOURCE_DIR
    PYTHON_BINARY_DIR
    PYTHON_STAMP_DIR
    PYTHON_TMP_DIR
    )

# CMake recommends this method to distinguish 32 from 64 bit architectures
if(@CMAKE_SIZEOF_VOID_P@ EQUAL 4)
    set(_installer "python-${PYNCPP_REQUIRED_PYTHON_VERSION}.exe")
else()
    set(_installer "python-${PYNCPP_REQUIRED_PYTHON_VERSION}-amd64.exe")
endif()

set(_download_url "https://www.python.org/ftp/python/${PYNCPP_REQUIRED_PYTHON_VERSION}/${_installer}")

set(_installer_args
    /passive
    InstallAllUsers=0
    TargetDir="${PYTHON_BINARY_DIR}"
    AssociateFiles=0
    CompileAll=0
    PrependPath=0
    Shortcuts=0
    Include_doc=0
    Include_debug=1
    Include_dev=1
    Include_exe=1
    Include_launcher=0
    InstallLauncherAllUsers=0
    Include_lib=1
    Include_pip=1
    Include_symbols=1
    Include_tcltk=0
    Include_test=0
    Include_tools=0
    )

ExternalProject_Add(Python
    URL ${_download_url}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_COMMAND} -E env "<DOWNLOAD_DIR>/${_installer}" ${_installer_args}
    INSTALL_COMMAND ""
    PREFIX "${_prefix}"
    DOWNLOAD_DIR "${PYTHON_DOWNLOAD_DIR}"
    SOURCE_DIR "${PYTHON_SOURCE_DIR}"
    BINARY_DIR "${PYTHON_BINARY_DIR}"
    STAMP_DIR "${PYTHON_STAMP_DIR}"
    TMP_DIR "${PYTHON_TMP_DIR}"
    )

ExternalProject_Add_Step(python post_build
    COMMAND ${CMAKE_COMMAND}
    -D "CMAKE_MODULE_PATH:PATH=${PROJECT_SOURCE_DIR}/cmake"
    -D "PYTHON_PREFIX:PATH=${PYTHON_PREFIX}"
    -D "PYTHON_BINARY_DIR:PATH=${PYTHON_BINARY_DIR}"
    -D "PYNCPP_REQUIRED_PYTHON_VERSION_MAJOR:STRING=${PYNCPP_REQUIRED_PYTHON_VERSION_MAJOR}"
    -D "PYNCPP_REQUIRED_PYTHON_VERSION_MINOR:STRING=${PYNCPP_REQUIRED_PYTHON_VERSION_MINOR}"
    -D "PYNCPP_REQUIRED_PYTHON_VERSION_PATCH:STRING=${PYNCPP_REQUIRED_PYTHON_VERSION_PATCH}"
    -P "${CMAKE_CURRENT_LIST_DIR}/post_build.cmake"
    DEPENDEES build
    )
