# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

cmake_policy(SET CMP0057 NEW) # IN_LIST operator

function(PYNCPP_install_standard_library)

    cmake_parse_arguments(PARSE_ARGV 0 "ARG"
        ""
        "DESTINATION"
        ""
        )

    if(NOT ARG_DESTINATION)
        message(FATAL_ERROR "Missing DESTINATION.")
    endif()

    set(stdlib_dir "${ARG_DESTINATION}")
    set(stdext_dir "${stdlib_dir}/lib-dynload")

    _install_top_level_modules("${stdlib_dir}")
    _install_packages("${stdlib_dir}")
    _install_extensions("${stdext_dir}")

endfunction()

function(_install_top_level_modules destination)

    set(files_to_install)
    file(GLOB files "${Python_STDLIB}/*.py")
    file(GLOB config_file "${Python_STDARCH}/*.py")
    list(APPEND files ${config_file})

    foreach(file ${files})
        get_filename_component(module_name "${file}" NAME_WE)
        if(NOT module_name IN_LIST PYNCPP_EXCLUDED_PYTHON_MODULES)
            list(APPEND files_to_install "${file}")
        endif()
    endforeach()

    install(FILES ${files_to_install} DESTINATION "${destination}")

endfunction()

function(_install_packages destination)

    file(GLOB init_files "${Python_STDLIB}/*/__init__.py")

    foreach(init_file ${init_files})
        get_filename_component(package_dir "${init_file}" DIRECTORY)
        get_filename_component(package_name ${package_dir} NAME)
        if(NOT package_name IN_LIST PYNCPP_EXCLUDED_PYTHON_MODULES)
            _recursively_install_packages("${package_dir}" "${destination}/${package_name}")
        endif()
    endforeach()

endfunction()

function(_recursively_install_packages package_dir destination)

    set(files_to_install)
    set(sub_packages)
    file(GLOB files "${package_dir}/*")

    foreach(file ${files})
        if(IS_DIRECTORY ${file})
            get_filename_component(sub_package ${file} NAME)
            list(APPEND sub_packages "${sub_package}")
        else()
            get_filename_component(file_type "${file}" LAST_EXT)
            if(NOT file_type MATCHES ".pyc") # do not take cache files
                list(APPEND files_to_install "${file}")
            endif()
        endif()
    endforeach()

    install(FILES ${files_to_install} DESTINATION "${destination}")

    foreach(sub_package ${sub_packages})
        _recursively_install_packages("${package_dir}/${sub_package}" "${destination}/${sub_package}")
    endforeach()

endfunction()

function(_install_extensions destination)

    if(WIN32)
        set(type "*.dll")
    else()
        set(type "*.so")
    endif()

    set(files_to_install)
    file(GLOB files LIST_DIRECTORIES FALSE "${Python_STDARCH}/${type}")

    foreach(file ${files})
        get_filename_component(module_name "${file}" NAME_WE)
        if(NOT module_name IN_LIST PYNCPP_EXCLUDED_PYTHON_MODULES)
            list(APPEND files_to_install "${file}")
        endif()
    endforeach()

    install(FILES ${files_to_install} DESTINATION ${destination})

endfunction()


