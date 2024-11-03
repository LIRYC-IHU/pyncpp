# Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Paths
################################################################################

set(python_source_dir "${prefix}/source")
set(python_build_dir "${python_source_dir}/PCBuild/amd64")
set(python_name "python${PYNCPP_PYTHON_VERSION_MAJOR}${PYNCPP_PYTHON_VERSION_MINOR}")

set(PYNCPP_PYTHON_LINK_LIBRARIES "${PYNCPP_ROOT}/${CMAKE_INSTALL_BINDIR}/${python_name}.lib")
set(PYNCPP_PYTHON_INCLUDE_DIRS "${PYNCPP_ROOT}/${CMAKE_INSTALL_INCLUDEDIR}/${python_name}")

################################################################################
# External project
################################################################################

ExternalProject_Add(pyncpp_python
    PREFIX "${prefix}"
    DOWNLOAD_DIR "${prefix}/download"
    SOURCE_DIR "${python_source_dir}"
    BINARY_DIR "${python_source_dir}"
    STAMP_DIR "${prefix}/stamp"
    TMP_DIR "${prefix}/tmp"
    URL "https://www.python.org/ftp/python/${PYNCPP_PYTHON_VERSION}/Python-${PYNCPP_PYTHON_VERSION}.tgz"
    URL_MD5 ${PYNCPP_PYTHON_UNIX_TGZ_${PYNCPP_PYTHON_VERSION_MAJOR}_${PYNCPP_PYTHON_VERSION_MINOR}_${PYNCPP_PYTHON_VERSION_PATCH}_MD5}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_COMMAND} -E env "<SOURCE_DIR>/PCbuild/build.bat"
    INSTALL_COMMAND ${CMAKE_COMMAND}
    --install "${CMAKE_CURRENT_BINARY_DIR}"
    --prefix "${PYNCPP_ROOT}"
    --config $<CONFIG>
    )

################################################################################
# Install
################################################################################

set(runtime_libraries
    "${python_build_dir}/${python_name}.dll"
    "${python_build_dir}/python${PYNCPP_PYTHON_VERSION_MAJOR}.dll"
    "${python_build_dir}/vcruntime140.dll"
    )

install(FILES ${runtime_libraries}
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT Runtime
    )

install(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}/${python_name}._pth\"
        \".\\n\"
        \"${PYNCPP_PYTHON_SUBDIR}\\n\"
        \"${PYNCPP_PYTHON_SUBDIR}/lib-dynload\\n\"
        \"DLLs\\n\"
        \"import site\"
        )
    "
    COMPONENT Runtime
    )

install(DIRECTORY "${python_source_dir}/Lib/"
    DESTINATION "${CMAKE_INSTALL_BINDIR}/${PYNCPP_PYTHON_SUBDIR}"
    COMPONENT Runtime
    PATTERN "*.pyc" EXCLUDE
    )

install(DIRECTORY "${python_build_dir}/"
    DESTINATION "${CMAKE_INSTALL_BINDIR}/${PYNCPP_PYTHON_SUBDIR}/lib-dynload"
    COMPONENT Runtime
    FILES_MATCHING
    PATTERN "*.dll"
    PATTERN "*.pyd"
    PATTERN "python*" EXCLUDE
    PATTERN "vcruntime*" EXCLUDE
    )

install(DIRECTORY "${python_build_dir}/"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    COMPONENT Development
    FILES_MATCHING
    PATTERN "python*.lib"
    PATTERN "python*.exp"
    )

install(DIRECTORY "${python_source_dir}/Include/"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${python_name}"
    COMPONENT Development
    )

install(FILES "${python_source_dir}/PC/pyconfig.h"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${python_name}"
    COMPONENT Development
    )
