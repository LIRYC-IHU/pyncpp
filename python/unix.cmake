# Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
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
    set(default_root_dir "/usr/local/opt/openssl@1.1")
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
    "--prefix=${PROJECT_BINARY_DIR}/${PYNCPP_PYTHON_SUBDIR}"
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
        string(APPEND cppflags " -I${OPENSSL_INCLUDE_DIR}")
        string(APPEND ldflags " -L${OPENSSL_SSL_LIBRARY}")
    endif()
endif()

################################################################################
# Paths
################################################################################

set(executable_path "${PROJECT_BINARY_DIR}/${PYNCPP_PYTHON_SUBDIR}/bin/python${PYNCPP_PYTHON_SHORT_VERSION}")
set(library_path "${PROJECT_BINARY_DIR}/${PYNCPP_PYTHON_SUBDIR}/lib/libpython${PYNCPP_PYTHON_SHORT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}")

if(APPLE)
    set(relative_library_path "@executable_path/../lib/libpython${PYNCPP_PYTHON_SHORT_VERSION}${CMAKE_SHARED_LIBRARY_SUFFIX}")
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
    URL "https://www.python.org/ftp/python/${PYNCPP_PYTHON_VERSION}/Python-${PYNCPP_PYTHON_VERSION}.tgz"
    URL_MD5 ${PYNCPP_PYTHON_UNIX_TGZ_${PYNCPP_PYTHON_VERSION_MAJOR}_${PYNCPP_PYTHON_VERSION_MINOR}_${PYNCPP_PYTHON_VERSION_PATCH}_MD5}
    CONFIGURE_COMMAND ${CMAKE_COMMAND} -E env CFLAGS=${cflags} CPPFLAGS=${cppflags} LDFLAGS=${ldflags} "<SOURCE_DIR>/configure" ${configure_args}
    INSTALL_COMMAND ${CMAKE_MAKE_PROGRAM} "--directory=<BINARY_DIR>" altinstall
    )

set(post_install_arguments
    COMMAND "${executable_path}" "${CMAKE_CURRENT_SOURCE_DIR}/relocatable_sysconfig.py" <INSTALL_DIR>
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/relocatable_sysconfig.py"
    )

if(APPLE)
    list(PREPEND post_install_arguments
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -change "${library_path}" "${relative_library_path}" "${executable_path}"
        )
else()
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/unix_launcher.sh.in" "${prefix}/tmp/unix_launcher.sh" @ONLY)
    list(PREPEND post_install_arguments
        COMMAND ${CMAKE_COMMAND} -E rename "${executable_path}" "${executable_path}_bin"
        COMMAND ${CMAKE_COMMAND} -E copy "${prefix}/tmp/unix_launcher.sh" "${executable_path}"
        )
endif()

ExternalProject_Add_Step(pyncpp_python post_install
    DEPENDEES install
    ${post_install_arguments}
    )

################################################################################
# Install
################################################################################

set(programs "${executable_path}")

if(NOT APPLE)
    list(APPEND programs "${executable_path}_bin")
endif()

install(PROGRAMS ${programs}
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}/bin"
    COMPONENT Runtime
    )

install(FILES
    "${library_path}"
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}/lib"
    COMPONENT Runtime
    )

install(DIRECTORY "${PROJECT_BINARY_DIR}/${PYNCPP_PYTHON_SUBDIR}/lib/python${PYNCPP_PYTHON_SHORT_VERSION}"
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}/lib"
    COMPONENT Runtime
    PATTERN "*.pyc" EXCLUDE
    )

install(DIRECTORY "${PROJECT_BINARY_DIR}/${PYNCPP_PYTHON_SUBDIR}/include/"
    DESTINATION "${PYNCPP_PYTHON_SUBDIR}/include"
    COMPONENT Development
    )

if(NOT APPLE)
    if(NOT CMAKE_OBJDUMP)
        message(FATAL_ERROR "The objdump utility (CMAKE_OBJDUMP) is required for pyncpp installation.")
    else()
        install(CODE "
            execute_process(COMMAND ${CMAKE_OBJDUMP} -p \"${library_path}\" OUTPUT_VARIABLE objdump_output)
            string(REGEX MATCH \"SONAME *(libpython${PYNCPP_PYTHON_VERSION_MAJOR}\.${PYNCPP_PYTHON_VERSION_MINOR}\.so[0-9\.]*)\" soname_match \"\${objdump_output}\")
            if(soname_match)
                file(INSTALL \"${PROJECT_BINARY_DIR}/${PYNCPP_PYTHON_SUBDIR}/lib/\${CMAKE_MATCH_1}\"
                    DESTINATION \"${PYNCPP_PYTHON_SUBDIR}/lib\"
                    )
                file(CREATE_LINK \"python${PYNCPP_PYTHON_SHORT_VERSION}/lib/\${CMAKE_MATCH_1}\"
                    \"\$ENV{DESTDIR}\${CMAKE_INSTALL_PREFIX}/lib/\${CMAKE_MATCH_1}\"
                    SYMBOLIC
                    )
            endif()
            "
            )
    endif()
endif()
