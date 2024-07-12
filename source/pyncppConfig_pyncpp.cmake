# Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
# License: BSD-3-Clause

include("${CMAKE_CURRENT_LIST_DIR}/pyncpp_export_cpp_api.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/pyncpp_export_executable.cmake")

set(pyncpp_LIBRARY pyncpp_cpp_api)
set(pyncpp_EXECUTABLE pyncpp_executable)
