# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Configuration
################################################################################

if(@CMAKE_SIZEOF_VOID_P@ EQUAL 4)
    set(installer "python-${PYNCPP_PYTHON_VERSION}.exe")
    set(win_type WIN)
else()
    set(installer "python-${PYNCPP_PYTHON_VERSION}-amd64.exe")
    set(win_type WIN64)
endif()

set(installer_args
    /passive
    InstallAllUsers=0
    TargetDir="<INSTALL_DIR>"
    AssociateFiles=0
    CompileAll=0
    PrependPath=0
    Shortcuts=0
    Include_debug=1
    Include_doc=0
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

################################################################################
# External project
################################################################################

ExternalProject_Add(pyncpp_python
    PREFIX "${prefix}"
    DOWNLOAD_DIR "${prefix}/download"
    SOURCE_DIR "${prefix}/source"
    BINARY_DIR "${prefix}/build"
    STAMP_DIR "${prefix}/stamp"
    TMP_DIR "${prefix}/tmp"
    INSTALL_DIR "${PYNCPP_PYTHON_DIR}"
    URL "https://www.python.org/ftp/python/${PYNCPP_PYTHON_VERSION}/${installer}"
    URL_MD5 ${PYNCPP_PYTHON_{win_type}_INSTALLER_${PYNCPP_PYTHON_VERSION_MAJOR}_${PYNCPP_PYTHON_VERSION_MINOR}_${PYNCPP_PYTHON_VERSION_PATCH}_MD5}
    DOWNLOAD_NAME "python_installer.exe"
    DOWNLOAD_NO_EXTRACT TRUE
    CONFIGURE_COMMAND \"\"
    INSTALL_COMMAND COMMAND ${CMAKE_COMMAND} -E env "<SOURCE_DIR>/python_installer.exe" ${installer_args}
    )

################################################################################
# Install
################################################################################

install(DIRECTORY "${PYNCPP_PYTHON_DIR}"
    DESTINATION ${PYNCPP_PYTHON_INSTALL_DESTINATION}
    PATTERN "*.pyc" EXCLUDE
    PATTERN "*.exe" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    )
