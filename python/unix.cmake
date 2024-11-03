# Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Cache settings
################################################################################

set(PYNCPP_PYTHON_C_FLAGS ${CMAKE_C_FLAGS} CACHE STRING
    "C compilation flags for Python."
    )

set(PYNCPP_PYTHON_CXX_FLAGS ${CMAKE_CXX_FLAGS} CACHE STRING
    "C++ compilation flags for Python."
    )

set(PYNCPP_PYTHON_SHARED_LINKER_FLAGS ${CMAKE_SHARED_LINKER_FLAGS} CACHE STRING
    "Linker flags for Python."
    )

if(LINUX)
    list(APPEND PYNCPP_PYTHON_SHARED_LINKER_FLAGS "-Wl,-rpath,'$$ORIGIN/../lib'")
endif()

################################################################################
# Dependencies
################################################################################

if(APPLE)
    set(default_root_dir "/usr/local/opt/openssl")
else()
    set(default_root_dir)
endif()

set(OPENSSL_ROOT_DIR "${default_root_dir}" CACHE PATH "Root directory of OpenSSL.")

find_package(OpenSSL COMPONENTS SSL)

if(NOT OpenSSL_FOUND)
    string(CONCAT _message
        "OpenSSL could not be found. Some of the standard Python modules will"
        " not be built, and pip will not be available."
        )
    message(WARNING ${_message})
endif()

################################################################################
# Configuration
################################################################################

set(configure_args
    "--prefix=${PYNCPP_ROOT}/${PYNCPP_PYTHON_SUBDIR}"
    --without-static-libpython
    --with-ensurepip
    --with-readline=editline
    --disable-test-modules
    --enable-shared
    )

if(PYNCPP_PYTHON_ENABLE_OPTMIZATIONS)
    list(APPEND configure_args
        --enable-optimizations
        --with-lto
        )
endif()

set(cflags ${PYNCPP_PYTHON_C_FLAGS})
set(cppflags ${PYNCPP_PYTHON_CXX_FLAGS})
set(ldflags ${PYNCPP_PYTHON_SHARED_LINKER_FLAGS})

if(OPENSSL_ROOT_DIR)
    list(APPEND configure_args --with-openssl=${OPENSSL_ROOT_DIR})
else()
    if(OPENSSL_INCLUDE_DIR AND OPENSSL_SSL_LIBRARY)
        string(APPEND cppflags " -I${OPENSSL_INCLUDE_DIR}")
        string(APPEND ldflags " -L${OPENSSL_SSL_LIBRARY}")
    endif()
endif()

################################################################################
# Paths
################################################################################

set(library_name libpython${PYNCPP_PYTHON_SHORT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX})
set(library_path "${PYNCPP_ROOT}/${CMAKE_INSTALL_LIBDIR}/${library_name}")

set(PYNCPP_PYTHON_LINK_LIBRARIES "${library_path}")
set(PYNCPP_PYTHON_INCLUDE_DIRS "${PYNCPP_ROOT}/${PYNCPP_PYTHON_SUBDIR}/include/python${PYNCPP_PYTHON_SHORT_VERSION}")

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
    URL "https://www.python.org/ftp/python/${PYNCPP_PYTHON_VERSION}/Python-${PYNCPP_PYTHON_VERSION}.tgz"
    URL_MD5 ${PYNCPP_PYTHON_UNIX_TGZ_${PYNCPP_PYTHON_VERSION_MAJOR}_${PYNCPP_PYTHON_VERSION_MINOR}_${PYNCPP_PYTHON_VERSION_PATCH}_MD5}
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CFLAGS=${cflags} CPPFLAGS=${cppflags} LDFLAGS=${ldflags} "<SOURCE_DIR>/configure" ${configure_args}
    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} "--directory=<BINARY_DIR>" altinstall
    )

set(post_install_steps
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PYNCPP_ROOT}/${CMAKE_INSTALL_LIBDIR}"
    COMMAND ${CMAKE_COMMAND} -E rename "${PYNCPP_ROOT}/${PYNCPP_PYTHON_SUBDIR}/lib/${library_name}" "${library_path}"
    )

if(APPLE)
    list(APPEND post_install_steps
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/${library_name}" "${library_path}"
        )
else()
    list(APPEND post_install_steps
        COMMAND ${CMAKE_COMMAND} -E rename "${PYNCPP_ROOT}/${PYNCPP_PYTHON_SUBDIR}/lib/${library_name}.1.0" "${library_path}.1.0"
        )
endif()

ExternalProject_Add_Step(pyncpp_python post_install
    DEPENDEES install
    ${post_install_steps}
    )

################################################################################
# Install
################################################################################

set(library_files "${library_path}")

if(LINUX)
    list(APPEND library_files
        "${library_path}.1.0"
        )
endif()

install(FILES ${library_files}
    TYPE LIB
    COMPONENT Runtime
    )

install(DIRECTORY "${PYNCPP_ROOT}/${PYNCPP_PYTHON_SUBDIR}/lib/python${PYNCPP_PYTHON_SHORT_VERSION}"
    TYPE LIB
    COMPONENT Runtime
    PATTERN "*.pyc" EXCLUDE
    )

install(DIRECTORY "${PYNCPP_ROOT}/${PYNCPP_PYTHON_SUBDIR}/include/python${PYNCPP_PYTHON_SHORT_VERSION}"
    TYPE INCLUDE
    COMPONENT Development
    )
