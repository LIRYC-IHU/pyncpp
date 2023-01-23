# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Download Python installer
################################################################################

# CMake recommends this method to distinguish 32 from 64 bit architectures
if(@CMAKE_SIZEOF_VOID_P@ EQUAL 4)
    set(installer "python-${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}.${PYNCPP_PYTHON_VERSION_PATCH}.exe")
else()
    set(installer "python-${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}.${PYNCPP_PYTHON_VERSION_PATCH}-amd64.exe")
endif()

FetchContent_Declare(python
    URL "https://www.python.org/ftp/python/${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}.${PYNCPP_PYTHON_VERSION_PATCH}/${installer}"
    DOWNLOAD_NAME "python_installer.exe"
    DOWNLOAD_NO_EXTRACT TRUE
    )

FetchContent_MakeAvailable(python)

################################################################################
# Python install
################################################################################

set(installer_args
    /passive
    InstallAllUsers=0
    TargetDir="${python_SOURCE_DIR}"
    AssociateFiles=0
    CompileAll=0
    PrependPath=0
    Shortcuts=0
    Include_debug=1
    Include_doc=0
    Include_dev=1
    Include_exe=1
    Include_launcher=0
    InstallLauncherAllUsers=0
    Include_lib=1
    Include_pip=1
    Include_symbols=1
    Include_tcltk=0
    Include_test=0
    Include_tools=0
    )

execute_process(COMMAND ${CMAKE_COMMAND} -E env
    "${python_SOURCE_DIR}/python_installer.exe" ${installer_args}
    WORKING_DIRECTORY ${python_SOURCE_DIR}
    COMMAND_ERROR_IS_FATAL ANY
    )

file(REMOVE "${python_SOURCE_DIR}/python_installer.exe")

################################################################################
# Paths
################################################################################

set(PYNCPP_PYTHON_INCLUDE_DIRS "${python_SOURCE_DIR}/include")

list(APPEND PYNCPP_LIBRARY_DIRS "${python_SOURCE_DIR}")
set(PYNCPP_PYTHON_STDLIB "${python_SOURCE_DIR}/Lib")
set(PYNCPP_PYTHON_STDEXT "${binary_dir}/DLLs")
set(PYNCPP_PYTHON_STDLIB_INSTALL_DIR "lib/python${PYNCPP_PYTHON_VERSION_MAJOR}.${PYNCPP_PYTHON_VERSION_MINOR}")

file(GLOB python_libraries
    RELATIVE "${python_SOURCE_DIR}"
    "${python_SOURCE_DIR}/python*.dll"
    )

file(GLOB vc_libraries
    "${python_SOURCE_DIR}/vcruntime*.dll"
    )

file(GLOB python_extensions
    RELATIVE "${python_SOURCE_DIR}/DLLs"
    "${python_SOURCE_DIR}/DLLs/*.pyd"
    )

file(GLOB python_extension_libraries
    RELATIVE "${python_SOURCE_DIR}/DLLs"
    "${python_SOURCE_DIR}/DLLs/*.dll"
    )

set(PYNCPP_PYTHON_LIBRARIES ${python_libraries})

################################################################################
# Install
################################################################################

set(filter_regex "^.*_d\\....$")
list(FILTER python_libraries EXCLUDE REGEX "${filter_regex}")
list(FILTER python_extensions EXCLUDE REGEX "${filter_regex}")
list(FILTER python_extension_libraries EXCLUDE REGEX "${filter_regex}")

foreach(config ${CMAKE_BUILD_TYPE} ${CMAKE_CONFIGURATION_TYPES})
    if(config STREQUAL "Debug")
        set(debug_suffix "_d")
    else()
        set(debug_suffix "")
    endif()

    set(lib_files)
    set(extension_files)

    foreach(library ${python_libraries})
        get_filename_component(library_name ${library} NAME_WLE)
        list(APPEND lib_files "${python_SOURCE_DIR}/${library_name}${debug_suffix}.dll")
        set(pdb_file "${python_SOURCE_DIR}/${library_name}${debug_suffix}.pdb")

        if(EXISTS ${pdb_file})
            list(APPEND lib_files ${pdb_file})
        endif()
    endforeach()

    install(FILES ${lib_files} ${vc_libraries}
        CONFIGURATIONS ${config}
        DESTINATION "lib"
        )

    foreach(extension ${python_extensions})
        get_filename_component(extension_name ${extension} NAME_WLE)
        list(APPEND extension_files "${python_SOURCE_DIR}/DLLs/${extension_name}${debug_suffix}.pyd")
        set(pdb_file "${python_SOURCE_DIR}/DLLs/${extension_name}${debug_suffix}.pdb")

        if(EXISTS ${pdb_file})
            list(APPEND extension_files ${pdb_file})
        endif()
    endforeach()

    foreach(library ${python_extension_libraries})
        get_filename_component(library_name ${library} NAME_WLE)
        set(name_base "${python_SOURCE_DIR}/DLLs/${library_name}")

        if(EXISTS "${name_base}${debug_suffix}.dll")
            string(APPEND name_base ${debug_suffix})
        endif()

        list(APPEND extension_files "${name_base}.dll")

        if(EXISTS "${name_base}.pdb")
            list(APPEND extension_files "${name_base}.pdb")
        endif()
    endforeach()

    install(FILES ${extension_files}
        CONFIGURATIONS ${config}
        DESTINATION "lib/${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}/lib-dynload"
        COMPONENT python
        EXCLUDE_FROM_ALL
        )
endforeach()
