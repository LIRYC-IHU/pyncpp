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

set(configure_args
    --enable-shared
    --without-static-libpython
    --disable-test-modules
    $<$<CONFIG:Release>:--enable-optimizations>
    )

if(PYNCPP_NO_GPL)
    list(APPEND configure_args --with-readline=editline)
endif()

set(cppflags ${CMAKE_CXX_FLAGS})
set(ldflags)

if(OPENSSL_ROOT_DIR)
    list(APPEND configure_args --with-openssl=${OPENSSL_ROOT_DIR})
else()
    if(OPENSSL_INCLUDE_DIR AND OPENSSL_SSL_LIBRARY)
        string(APPEND cppflags "-I${OPENSSL_INCLUDE_DIR}")
        string(APPEND ldflags "-L${OPENSSL_SSL_LIBRARY}")
    endif()
endif()

if(cppflags)
    set(cppflags "CPPFLAGS='${cppflags}'")
endif()

if(ldflags)
    set(ldflags "LDFLAGS='${ldflags}'")
endif()

ExternalProject_Add(python
    GIT_REPOSITORY "https://github.com/fcollot/cpython.git"
    GIT_TAG "v${PYNCPP_REQUIRED_PYTHON_VERSION}_with_pth"
    GIT_SHALLOW True
    GIT_PROGRESS True
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env "<SOURCE_DIR>/configure" ${configure_args} ${cppflags} ${ldflags}
    INSTALL_COMMAND ""
    PREFIX "${PYTHON_PREFIX}"
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
    -D "PYTHON_SOURCE_DIR:PATH=${PYTHON_SOURCE_DIR}"
    -D "PYTHON_BINARY_DIR:PATH=${PYTHON_BINARY_DIR}"
    -D "PYNCPP_REQUIRED_PYTHON_VERSION_MAJOR:STRING=${PYNCPP_REQUIRED_PYTHON_VERSION_MAJOR}"
    -D "PYNCPP_REQUIRED_PYTHON_VERSION_MINOR:STRING=${PYNCPP_REQUIRED_PYTHON_VERSION_MINOR}"
    -D "PYNCPP_REQUIRED_PYTHON_VERSION_PATCH:STRING=${PYNCPP_REQUIRED_PYTHON_VERSION_PATCH}"
    -P "${CMAKE_CURRENT_LIST_DIR}/post_build.cmake"
    DEPENDEES build
    )

if(APPLE)
    set(library_name "libpython${PYNCPP_REQUIRED_PYTHON_VERSION_MAJOR}.${PYNCPP_REQUIRED_PYTHON_VERSION_MINOR}.dylib")
    ExternalProject_Add_Step(python relocate_binaries
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "${PYTHON_BINARY_DIR}/${library_name}" "${PYTHON_BINARY_DIR}/${library_name}"
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change "/usr/local/lib/${library_name}" "${PYTHON_BINARY_DIR}/${library_name}" "${PYTHON_BINARY_DIR}/python.exe"
        DEPENDEES build
        )
endif()
