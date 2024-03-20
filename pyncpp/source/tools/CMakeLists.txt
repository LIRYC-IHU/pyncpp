# Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

add_custom_target(pyncpp_tools_generic ALL
    COMMAND ${pyncpp_PYTHON_EXECUTABLE} -m pip install "${CMAKE_CURRENT_SOURCE_DIR}"
    VERBATIM
    )

if(PYNCPP_QT5_SUPPORT)
    set(qt_path "$<TARGET_FILE_DIR:Qt5::Core>")
    if(APPLE)
        set(qt_path "${qt_path}/..")
    endif()
    add_custom_target(pyncpp_tools_qt5 ALL
        COMMAND ${pyncpp_PYTHON_EXECUTABLE} -m pip install PySide2==${Qt5_VERSION_MAJOR}.*
        COMMAND ${pyncpp_PYTHON_EXECUTABLE} -m pyncpp_tools link_pyside "${qt_path}" reverse
        DEPENDS pyncpp_tools_generic
        VERBATIM
        )
endif()
