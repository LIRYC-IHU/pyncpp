// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "error_handling.h"

#include "../external/cpython_call.h"

#include "exception_thrower.h"

namespace pyncpp
{

namespace
{

PyObject* retrieveNativeException()
{
    PyObject* exceptionClass;
    PyObject* exceptionInstance;
    PyObject* traceback;

    PyErr_Fetch(&exceptionClass, &exceptionInstance, &traceback);
    PyErr_NormalizeException(&exceptionClass, &exceptionInstance, &traceback);

    if (traceback)
    {
        PyException_SetTraceback(exceptionInstance, traceback);
    }

    Py_CLEAR(exceptionClass);
    return exceptionInstance;
}

} // namespace

void raiseError(PyObject* nativeExceptionClass, const char* message)
{
    PYNCPP_ACQUIRE_GIL;
    PyErr_SetString(nativeExceptionClass, message);
    PYNCPP_RELEASE_GIL;
}

bool errorOccurred()
{
    PYNCPP_ACQUIRE_GIL;
    bool result = PyErr_Occurred();
    PYNCPP_RELEASE_GIL;
    return result;
}

void clearError()
{
    PYNCPP_ACQUIRE_GIL;
    PyErr_Clear();
    PYNCPP_RELEASE_GIL;
}

void propagateErrorIfOccurred(PyGILState_STATE& gilState)
{
    if (PyErr_Occurred())
    {
        propagateCurrentError(gilState);
    }
}

void propagateCurrentError(PyGILState_STATE& gilState)
{
    PyObject* nativeException = retrieveNativeException();
    PyErr_Clear();
    internal::throwException(nativeException, gilState);
}

} // namespace pyncpp
