# Copyright (c) 2023 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

add_custom_target(python ALL)

if(PYNCPP_CUSTOM_PYTHON)

    ############################################################################
    # Setup custom Python
    ############################################################################

    if(WIN32)
        include("${CMAKE_CURRENT_LIST_DIR}/windows/windows.cmake")
    else()
        include("${CMAKE_CURRENT_LIST_DIR}/unix/unix.cmake")
    endif()

    ## Install stdlib ##########################################################

    file(GLOB_RECURSE files
        LIST_DIRECTORIES FALSE
        RELATIVE "${PYNCPP_PYTHON_STDLIB}"
        "${PYNCPP_PYTHON_STDLIB}/*"
        )

    foreach(file ${files})
        get_filename_component(type ${file} LAST_EXT)
        if(NOT type STREQUAL ".pyc")
            get_filename_component(directory ${file} DIRECTORY)
            if(directory)
                string(REGEX MATCH "^([^/]+)" _ ${directory})
                if(NOT CMAKE_MATCH_1 IN_LIST PYNCPP_EXCLUDED_PYTHON_PACKAGES)
                    install(FILES "${PYNCPP_PYTHON_STDLIB}/${file}"
                        DESTINATION "lib/${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}/${directory}"
                        )

                    install(FILES "${PYNCPP_PYTHON_STDLIB}/${file}"
                        DESTINATION "${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}/${directory}"
                        COMPONENT python
                        EXCLUDE_FROM_ALL
                        )
                endif()
            else()
                install(FILES "${PYNCPP_PYTHON_STDLIB}/${file}"
                    DESTINATION "lib/${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}"
                    )

                install(FILES "${PYNCPP_PYTHON_STDLIB}/${file}"
                    DESTINATION "${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}"
                    COMPONENT python
                    EXCLUDE_FROM_ALL
                    )
            endif()
        endif()
    endforeach()

    ## Export variables ########################################################

    set(PYNCPP_PYTHON_INCLUDE_DIRS ${PYNCPP_PYTHON_INCLUDE_DIRS} PARENT_SCOPE)
    set(PYNCPP_PYTHON_LIBRARIES ${PYNCPP_PYTHON_LIBRARIES} PARENT_SCOPE)
    set(PYNCPP_LIBRARY_DIRS ${PYNCPP_LIBRARY_DIRS} PARENT_SCOPE)
    set(PYNCPP_PYTHON_STDLIB ${PYNCPP_PYTHON_STDLIB} PARENT_SCOPE)
    set(PYNCPP_PYTHON_STDEXT ${PYNCPP_PYTHON_STDEXT} PARENT_SCOPE)
    set(PYNCPP_PYTHON_STDLIB_INSTALL_DIR "${PYNCPP_PYTHON_STDLIB_INSTALL_DIR}" PARENT_SCOPE)

else()

    ############################################################################
    # System Python
    ############################################################################

    if(PYNCPP_REQUIRE_EXACT_PYTHON_VERSION)
        set(_exact EXACT)
    else()
        set(_exact)
    endif()

    find_package(Python ${PYNCPP_REQUIRED_PYTHON_VERSION} ${_exact} REQUIRED
        COMPONENTS Interpreter Development
        )

    ## Export variables ########################################################

    set(PYNCPP_PYTHON_VERSION_MAJOR ${Python_VERSION_MAJOR} PARENT_SCOPE)
    set(PYNCPP_PYTHON_VERSION_MINOR ${Python_PYTHON_VERSION_MINOR} PARENT_SCOPE)
    set(PYNCPP_PYTHON_VERSION_PATCH ${Python_PYTHON_VERSION_PATCH} PARENT_SCOPE)
    set(PYNCPP_PYTHON_INCLUDE_DIRS ${Python_INCLUDE_DIRS} PARENT_SCOPE)
    set(PYNCPP_PYTHON_LIBRARIES ${Python_LIBRARIES} PARENT_SCOPE)
    set(PYNCPP_PYTHON_STDLIB ${Python_STDLIB})
    set(PYNCPP_PYTHON_STDEXT ${Python_STDARCH})

endif()
