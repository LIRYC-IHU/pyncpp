# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

macro(pyncpp_import_python_executable target_name)

    add_executable(${target_name} IMPORTED GLOBAL)
    set_target_properties(${target_name} PROPERTIES IMPORTED_LOCATION "${pyncpp_PYTHON_DIR}/python${pyncpp_PYTHON_MAJOR_VERSION}${pyncpp_PYTHON_MINOR_VERSION}.exe")

endmacro()
