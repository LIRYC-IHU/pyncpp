# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

find_package(Git)

macro(PYNCPP_get_version_from_git major minor patch)

    set(_success FALSE)

    if(GIT_EXECUTABLE)
        execute_process(COMMAND ${GIT_EXECUTABLE} describe --tags --always
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE _git_describe
            )

        if (_git_describe MATCHES "^v\\.([0-9]+)\\.([0-9]+)(-([0-9]+))?.*$")
            set(${major} ${CMAKE_MATCH_1})
            set(${minor} ${CMAKE_MATCH_2})
            if(CMAKE_MATCH_4)
                set(${patch} ${CMAKE_MATCH_4})
            else()
                set(${patch} 0)
            endif()
            set(_success TRUE)
        endif()
    endif()

    if(_success)
        message(STATUS "Version number generated from git: ${${major}}.${${minor}}.${${patch}}")
    else()
        set(${major} 0)
        set(${minor} 0)
        set(${patch} 0)
        message(WARNING "Cannot use git to generate a version number. Defaulting to 0.0.0")
    endif()

endmacro()
