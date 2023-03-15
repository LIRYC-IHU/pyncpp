# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

macro(PYNCPP_import_python_executable target_name)

    add_executable(${target_name} IMPORTED GLOBAL)

    if(PYNCPP_PYTHON_DIR)
        set_target_properties(${target_name} PROPERTIES IMPORTED_LOCATION "${PYNCPP_PYTHON_DIR}/bin/python${PYNCPP_PYTHON_SHORT_VERSION}")
    endif()

    foreach(config ${PYNCPP_CONFIGURATION_TYPES})
        string(TOUPPER ${config} CONFIG)
        set_target_properties(${target_name} PROPERTIES IMPORTED_LOCATION_${CONFIG} "${PYNCPP_PYTHON_DIR_${CONFIG}}/bin/python${PYNCPP_PYTHON_SHORT_VERSION}")
    endforeach()

endmacro()
