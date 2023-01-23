# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

file(READ "${CMAKE_CURRENT_BINARY_DIR}/pybuilddir.txt" pybuilddir)

if(pybuilddir)
    file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/lib")
    file(RENAME "${pybuilddir}" "${CMAKE_CURRENT_BINARY_DIR}/lib")
endif()

if(APPLE)
    if(NOT CMAKE_INSTALL_NAME_TOOL)
        find_program(CMAKE_INSTALL_NAME_TOOL install_name_tool)
    endif()

    if(PYNCPP_PYTHON_FRAMEWORK)
        set(library "Python.framework/Versions/${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}/Python")
    else()
        set(library "libpython${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}.dylib")
    endif()

    execute_process(
        COMMAND ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/${library}" "${CMAKE_CURRENT_BINARY_DIR}/${library}"
        COMMAND_ERROR_IS_FATAL ANY
        )
endif()


