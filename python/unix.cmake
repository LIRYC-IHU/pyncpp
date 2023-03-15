# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
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

################################################################################
# Dependencies
################################################################################

set(OPENSSL_ROOT_DIR "" CACHE PATH "Root directory of OpenSSL.")

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
    "--prefix=<INSTALL_DIR>"
    --without-static-libpython
    --with-ensurepip
    --with-readline=editline
    --disable-test-modules
    --enable-shared
    $<$<CONFIG:Release>:--enable-optimizations>
    $<$<CONFIG:Release>:--with-lto>
    )

set(cflags ${PYNCPP_PYTHON_C_FLAGS})
set(cppflags ${PYNCPP_PYTHON_CXX_FLAGS})
set(ldflags ${PYNCPP_PYTHON_SHARED_LINKER_FLAGS})

if(OPENSSL_ROOT_DIR)
    list(APPEND configure_args --with-openssl=${OPENSSL_ROOT_DIR})
else()
    if(OPENSSL_INCLUDE_DIR AND OPENSSL_SSL_LIBRARY)
        string(APPEND cppflags "-I${OPENSSL_INCLUDE_DIR}")
        string(APPEND ldflags "-L${OPENSSL_SSL_LIBRARY}")
    endif()
endif()

################################################################################
# Paths
################################################################################

if(GENERATOR_IS_MULTI_CONFIG)
    set(python_dir)
    set(executable_path)
    foreach(config ${CMAKE_CONFIGURATION_TYPES})
        string(APPEND python_dir $<$<CONFIG:${config}>:${PYNCPP_PYTHON_DIR}/${config}>)
        string(APPEND executable_path $<$<CONFIG:${config}>:${PYNCPP_PYTHON_DIR}/${config}/python${PYNCPP_PYTHON_SHORT_VERSION}>)
    endforeach()
else()
    set(python_dir "${PYNCPP_PYTHON_DIR}")
    set(executable_path "${PYNCPP_PYTHON_DIR}/bin/python${PYNCPP_PYTHON_SHORT_VERSION}")
endif()

if(APPLE)
    set(relative_library_path "@executable_path/../lib/libpython${PYNCPP_PYTHON_SHORT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}")
else()
    set(relative_library_path "\$ORIGIN/../lib/libpython${PYNCPP_PYTHON_SHORT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

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
    INSTALL_DIR "${python_dir}"
    URL "https://www.python.org/ftp/python/${PYNCPP_PYTHON_VERSION}/Python-${PYNCPP_PYTHON_VERSION}.tgz"
    URL_MD5 ${PYNCPP_PYTHON_UNIX_TGZ_${PYNCPP_PYTHON_VERSION_MAJOR}_${PYNCPP_PYTHON_VERSION_MINOR}_${PYNCPP_PYTHON_VERSION_PATCH}_MD5}
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CFLAGS=${cflags} CPPFLAGS=${cppflags} LDFLAGS=${ldflags} "<SOURCE_DIR>/configure" ${configure_args}
    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} "--directory=<BINARY_DIR>" altinstall
    )

ExternalProject_Add_Step(pyncpp_python post_install
    DEPENDEES install
    COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change "${library_path}" "${relative_library_path}" "${executable_path}"
    COMMAND "${executable_path}" "${CMAKE_CURRENT_SOURCE_DIR}/relocatable_sysconfig.py" <INSTALL_DIR>
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/relocatable_sysconfig.py"
    )

################################################################################
# Install
################################################################################

install(DIRECTORY "${python_dir}/"
    DESTINATION ${PYNCPP_PYTHON_INSTALL_DESTINATION}
    PATTERN "*.pyc" EXCLUDE
    PATTERN "bin/*" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    )
