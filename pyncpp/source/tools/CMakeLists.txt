# Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/pyncpp_tools")

set(PYNCPP_PYTHON_GLOBALS
    "pyncpp_version = '${PYNCPP_VERSION}'"
    )

if(PYNCPP_QT5_SUPPORT)
    list(APPEND PYNCPP_PYTHON_GLOBALS
        "pyncpp_qt5_support = True"
        )
else()
    list(APPEND PYNCPP_PYTHON_GLOBALS
        "pyncpp_qt5_support = False"
        )
endif()

list(JOIN PYNCPP_PYTHON_GLOBALS "\n" PYNCPP_PYTHON_GLOBALS)

configure_file(setup.py.in setup.py @ONLY)
configure_file(global_values.py.in pyncpp_tools/global_values.py @ONLY)

add_custom_target(pyncpp_tools ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/pyncpp_tools" "${CMAKE_CURRENT_BINARY_DIR}/pyncpp_tools"
    COMMAND ${pyncpp_PYTHON_EXECUTABLE} -m pip install "${CMAKE_CURRENT_BINARY_DIR}"
    VERBATIM
    )

if(PYNCPP_QT5_SUPPORT)
    set(qt_path "$<TARGET_FILE_DIR:Qt5::Core>")
    if(APPLE)
        set(qt_path "${qt_path}/..")
    endif()

    add_custom_target(pyncpp_tools_qt5 ALL
        COMMAND ${pyncpp_PYTHON_EXECUTABLE} -m pyncpp_tools temporarily_link_pyside_to_qt "${qt_path}"
        DEPENDS pyncpp_tools
        VERBATIM
        )
endif()
