# Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

################################################################################
# Subdirectories
################################################################################

add_subdirectory(cpp_api)
add_subdirectory(executable)

################################################################################
# Ensure pip installed on Windows
################################################################################

if(WIN32)
    add_custom_target(pyncpp_ensure_pip ALL
        COMMAND pyncpp_executable -m ensurepip --upgrade
        VERBATIM
        )
endif()

################################################################################
# Config module
################################################################################

foreach(config ${CMAKE_CONFIGURATION_TYPES} ${CMAKE_BUILD_TYPE})

    set(share_dir ${PYNCPP_ROOT_BASE}/${config}/${PYNCPP_SHARE_SUBDIR})

    file(COPY_FILE
        "${CMAKE_CURRENT_SOURCE_DIR}/pyncppConfig_pyncpp.cmake"
        "${share_dir}/pyncppConfig_pyncpp.cmake"
        )

    install(FILES
        "${share_dir}/pyncppConfig_pyncpp.cmake"
        CONFIGURATIONS ${config}
        DESTINATION "${PYNCPP_SHARE_SUBDIR}"
        )

endforeach()
