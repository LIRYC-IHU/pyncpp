# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

function(PYNCPP_add_bindings_module target_name)

    cmake_parse_arguments(PARSE_ARGV 1 "ARG"
        ""
        "SWIG_INTERFACE;OUTPUT_DIR;SWIG_TYPE_TABLE"
        "CPP_SOURCES;OTHER_SOURCES"
        )

    if(ARG_OUTPUT_DIR)
        set(output_dir "${ARG_OUTPUT_DIR}")
    else()
        set(output_dir "${CMAKE_CURRENT_BINARY_DIR}")
    endif()

    set(working_dir "${CMAKE_CURRENT_BINARY_DIR}/swig")

    set_source_files_properties("${ARG_SWIG_INTERFACE}" PROPERTIES
        CPLUSPLUS ON
        USE_SWIG_DEPENDENCIES ON
        )

    swig_add_library(${target_name}
        TYPE MODULE
        LANGUAGE python
        OUTPUT_DIR "${output_dir}"
        OUTFILE_DIR "${working_dir}"
        SOURCES ${ARG_SWIG_INTERFACE} ${ARG_CPP_SOURCES}
        )

    set_target_properties(${target_name} PROPERTIES
        SWIG_USE_TARGET_INCLUDE_DIRECTORIES TRUE
        SWIG_COMPILE_OPTIONS -py3
        OUTPUT_NAME ${target_name}
        LIBRARY_OUTPUT_DIRECTORY ${output_dir}
        )

    if(ARG_SWIG_TYPE_TABLE)
        set_target_properties(${target_name} PROPERTIES
            SWIG_COMPILE_DEFINITIONS SWIG_TYPE_TABLE=${ARG_SWIG_TYPE_TABLE}
            )
    endif()

    target_sources(${target_name}
        PRIVATE
        ${ARG_SWIG_INTERFACE}
        ${ARG_CPP_SOURCES}
        ${ARG_OTHER_SOURCES}
        )

endfunction()
