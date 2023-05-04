// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CPYTHON_CALL_H
#define PYNCPP_CPYTHON_CALL_H

#include <type_traits>

#include "../error/error_handling.h"

#define PYNCPP_GIL_STATE _pyncpp_gilState
#define PYNCPP_ACQUIRE_GIL PyGILState_STATE PYNCPP_GIL_STATE = PyGILState_Ensure()
#define PYNCPP_RELEASE_GIL PyGILState_Release(PYNCPP_GIL_STATE)

namespace pyncpp
{

template <class FUNCTION_TYPE, class... ARGS>
inline std::invoke_result_t<FUNCTION_TYPE, ARGS...> cpythonCall(FUNCTION_TYPE& function, ARGS... args)
{
    PYNCPP_ACQUIRE_GIL;

    std::invoke_result_t<FUNCTION_TYPE, ARGS...> result = function(std::forward<ARGS>(args)...);
    PYNCPP_CHECK_ERROR;

    PYNCPP_RELEASE_GIL;
    return result;
}

template <class FUNCTION_TYPE, class... ARGS, typename = std::enable_if_t<std::is_void_v<std::invoke_result_t<FUNCTION_TYPE, ARGS...> > > >
inline void cpythonCall(FUNCTION_TYPE& function, ARGS... args)
{
    PYNCPP_ACQUIRE_GIL;

    function(std::forward<ARGS>(args)...);
    PYNCPP_CHECK_ERROR;

    PYNCPP_RELEASE_GIL;
}

} // namespace pyncpp

#endif // PYNCPP_CPYTHON_CALL_H
