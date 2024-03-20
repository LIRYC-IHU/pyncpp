# Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# External project
################################################################################

ExternalProject_Add(pyncpp_python
    PREFIX "${prefix}"
    DOWNLOAD_DIR "${prefix}/download"
    SOURCE_DIR "${prefix}/source"
    BINARY_DIR "${prefix}/source"
    STAMP_DIR "${prefix}/stamp"
    TMP_DIR "${prefix}/tmp"
    URL "https://www.python.org/ftp/python/${PYNCPP_PYTHON_VERSION}/Python-${PYNCPP_PYTHON_VERSION}.tgz"
    URL_MD5 ${PYNCPP_PYTHON_UNIX_TGZ_${PYNCPP_PYTHON_VERSION_MAJOR}_${PYNCPP_PYTHON_VERSION_MINOR}_${PYNCPP_PYTHON_VERSION_PATCH}_MD5}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_COMMAND} -E env "<SOURCE_DIR>/PCbuild/build.bat" $<$<CONFIG:Debug>:-d>
    INSTALL_COMMAND ${CMAKE_COMMAND}
    --install "${CMAKE_CURRENT_BINARY_DIR}"
    --prefix "${PROJECT_BINARY_DIR}"
    --config $<CONFIG>
    )

set(library_name "python${PYNCPP_PYTHON_VERSION_MAJOR}${PYNCPP_PYTHON_VERSION_MINOR}$<$<CONFIG:Debug>:_d>")

ExternalProject_Add_Step(pyncpp_python post_install
    DEPENDEES install
    COMMAND "${PROJECT_BINARY_DIR}/${PYNCPP_PYTHON_SUBDIR}/python$<$<CONFIG:Debug>:_d>.exe" -m ensurepip --upgrade
    )

################################################################################
# Install
################################################################################

set(build_dir ${CMAKE_CURRENT_BINARY_DIR}/source/PCBuild/amd64)
set(source_dir ${CMAKE_CURRENT_BINARY_DIR}/source)

install(PROGRAMS
    "${build_dir}/pythonw$<$<CONFIG:Debug>:_d>.exe"
    "${build_dir}/python$<$<CONFIG:Debug>:_d>.exe"
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}"
    COMPONENT Runtime
    )

set(runtime_files
    "${build_dir}/${library_name}.dll"
    "${build_dir}/python${PYNCPP_PYTHON_VERSION_MAJOR}$<$<CONFIG:Debug>:_d>.dll"
    "${build_dir}/vcruntime140.dll"
    )

install(FILES ${runtime_files}
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}"
    COMPONENT Runtime
    )

install(CODE "
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${PYNCPP_PYTHON_SUBDIR}/${library_name}._pth\"
        \".\\n\"
        \"Lib\\n\"
        \"DLLs\\n\"
        \"import site\"
        )
    file(WRITE \"\${CMAKE_INSTALL_PREFIX}/bin/${library_name}._pth\"
        \"../${PYNCPP_PYTHON_SUBDIR}\\n\"
        \"../${PYNCPP_PYTHON_SUBDIR}/Lib\\n\"
        \"../${PYNCPP_PYTHON_SUBDIR}/DLLs\\n\"
        \"import site\"
        )
    file(CREATE_LINK
        \"\${CMAKE_INSTALL_PREFIX}/${PYNCPP_PYTHON_SUBDIR}/${library_name}.dll\"
        \"\${CMAKE_INSTALL_PREFIX}/bin/${library_name}.dll\")
    "
    COMPONENT Runtime
    )

install(DIRECTORY "${source_dir}/Lib/"
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}/Lib"
    COMPONENT Runtime
    PATTERN "*.pyc" EXCLUDE
    )

install(CODE "
    if(NOT \"\${CMAKE_INSTALL_PREFIX}\" STREQUAL \"${PROJECT_BINARY_DIR}\")
        file(INSTALL \"${PROJECT_BINARY_DIR}/${PYNCPP_PYTHON_SUBDIR}/Lib/site-packages\"
            DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${PYNCPP_PYTHON_SUBDIR}/Lib\"
            PATTERN \"*.pyc\" EXCLUDE
            )
    endif()
    "
    COMPONENT Runtime
    )

install(DIRECTORY "${build_dir}/"
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}/DLLs"
    COMPONENT Runtime
    FILES_MATCHING
    PATTERN "*.dll"
    PATTERN "*.pyd"
    PATTERN "python*" EXCLUDE
    PATTERN "vcruntime*" EXCLUDE
    )

install(DIRECTORY "${build_dir}/"
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}/libs"
    COMPONENT Development
    FILES_MATCHING
    PATTERN "python*.lib"
    PATTERN "python*.exp"
    )

install(CODE "
    file(MAKE_DIRECTORY \"\${CMAKE_INSTALL_PREFIX}/lib\")
    file(CREATE_LINK
        \"\${CMAKE_INSTALL_PREFIX}/${PYNCPP_PYTHON_SUBDIR}/libs/${library_name}.lib\"
        \"\${CMAKE_INSTALL_PREFIX}/lib/${library_name}.lib\")
    "
    COMPONENT Development
    )

install(DIRECTORY "${source_dir}/Include/"
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}/include"
    COMPONENT Development
    )

install(FILES "${source_dir}/PC/pyconfig.h"
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}/include"
    COMPONENT Development
    )
