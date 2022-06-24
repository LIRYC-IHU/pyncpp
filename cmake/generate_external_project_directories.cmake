# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

macro(PYNCPP_generate_external_project_directories name prefix download source binary stamp tmp)

    set(${prefix} "${PROJECT_BINARY_DIR}/${name}")
    set(${download} "${${prefix}}/download")
    set(${source} "${${prefix}}/source")
    set(${binary} "${${prefix}}/build")
    set(${stamp} "${${prefix}}/stamp")
    set(${tmp} "${${prefix}}/tmp")

endmacro()
