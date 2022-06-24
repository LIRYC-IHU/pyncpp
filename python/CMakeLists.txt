# Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

if(WIN32)
    add_subdirectory(windows)
else()
    add_subdirectory(unix)
endif()
