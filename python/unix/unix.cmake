# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Download Python source code
################################################################################

FetchContent_Declare(python
    GIT_REPOSITORY "https://github.com/fcollot/cpython.git"
    GIT_TAG "v${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}.${PYNCPP_PYTHON_VERSION_PATCH}_with_pth"
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    )

FetchContent_MakeAvailable(python)

################################################################################
# Dependencies
################################################################################

find_package(OpenSSL COMPONENTS SSL)

if(NOT OpenSSL_FOUND)
    string(CONCAT _message
        "OpenSSL could not be found. Some of the standard Python modules may"
        " not be built, and pip install will not be usable to install"
        " additional packages."
        )
    message(WARNING ${_message})
endif()

################################################################################
# Paths
################################################################################

foreach(config ${CMAKE_BUILD_TYPE} ${CMAKE_CONFIGURATION_TYPES})
    file(MAKE_DIRECTORY "${python_BINARY_DIR}/${config}")
endforeach()

set(binary_dir "${python_BINARY_DIR}/$<CONFIG>")

set(PYNCPP_PYTHON_INCLUDE_DIRS
    "${python_SOURCE_DIR}/Include"
    "${binary_dir}"
    )

list(APPEND PYNCPP_LIBRARY_DIRS ${binary_dir})
set(PYNCPP_PYTHON_STDLIB "${python_SOURCE_DIR}/Lib")
set(PYNCPP_PYTHON_STDEXT "${binary_dir}/lib")

if(PYNCPP_PYTHON_FRAMEWORK)
    set(PYNCPP_PYTHON_LIBRARIES "${binary_dir}/Python.framework/Versions/${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}/Python")
else()
    set(PYNCPP_PYTHON_LIBRARIES "${binary_dir}/libpython${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}${CMAKE_SHARED_LIBRARY_SUFFIX}")
endif()

set(PYNCPP_PYTHON_STDLIB_INSTALL_DIR "python${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}")

################################################################################
# Python configuration
################################################################################

set(configure_args
    --without-static-libpython
    --disable-test-modules
    )

list(APPEND configure_args $<$<CONFIG:debug>:--enable-optimizations>)

if(PYNCPP_PYTHON_FRAMEWORK)
    list(APPEND configure_args --enable-framework)
else()
    list(APPEND configure_args --enable-shared)
endif()

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

################################################################################
# Python build
################################################################################

set(landmark_binaries
    "${binary_dir}/python.exe"
    ${PYNCPP_PYTHON_LIBRARIES}
    )

add_custom_command(OUTPUT ${landmark_binaries}
    COMMAND ${CMAKE_COMMAND} -E env CPPFLAGS=${cppflags} LDFLAGS=${ldflags} "${python_SOURCE_DIR}/configure" ${configure_args}
    COMMAND ${CMAKE_MAKE_PROGRAM} "--directory=${binary_dir}"
    COMMAND ${CMAKE_COMMAND}
    -D "PYNCPP_PYTHON_FRAMEWORK:BOOL=${PYNCPP_PYTHON_FRAMEWORK}"
    -D "PYNCPP_PYTHON_VERSION_MAJOR:STRING=${PYNCPP_PYTHON_VERSION_MAJOR}"
    -D "PYNCPP_PYTHON_VERSION_MINOR:STRING=${PYNCPP_PYTHON_VERSION_MINOR}"
    -P "${CMAKE_CURRENT_LIST_DIR}/post_build.cmake"
    WORKING_DIRECTORY "${binary_dir}"
    VERBATIM
    )

add_custom_target(build_cpython DEPENDS ${landmark_binaries})
add_dependencies(python build_cpython)

################################################################################
# Install
################################################################################

if(PYNCPP_PYTHON_FRAMEWORK)
    install(DIRECTORY "${binary_dir}/Python.framework"
        DESTINATION "Frameworks"
        )
else()
    install(FILES "${binary_dir}/libpython${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}${CMAKE_SHARED_LIBRARY_SUFFIX}"
        DESTINATION "lib"
        )
endif()

install(DIRECTORY "${binary_dir}/lib/"
    DESTINATION "lib/${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}/lib-dynload"
    FILES_MATCHING PATTERN "*.so"
    PATTERN "__pycache__" EXCLUDE
    )


install(DIRECTORY "${binary_dir}/lib/"
    DESTINATION "${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}/lib-dynload"
    COMPONENT python
    EXCLUDE_FROM_ALL
    FILES_MATCHING PATTERN "*.so"
    PATTERN "__pycache__" EXCLUDE
    )

install(DIRECTORY "${binary_dir}/lib/"
    DESTINATION "lib/${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}"
    FILES_MATCHING PATTERN "*.py"
    PATTERN "__pycache__" EXCLUDE
    )

install(DIRECTORY "${binary_dir}/lib/"
    DESTINATION "${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}"
    COMPONENT python
    EXCLUDE_FROM_ALL
    FILES_MATCHING PATTERN "*.py"
    PATTERN "__pycache__" EXCLUDE
    )
