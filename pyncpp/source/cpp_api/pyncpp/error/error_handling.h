// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_ERROR_HANDLING_H
#define PYNCPP_ERROR_HANDLING_H

/// This is the API to raise, query and propagate Python errors from the wrapper
/// code.
///

#include "../external/cpython.h"

#include "../export.h"

#define PYNCPP_CHECK_ERROR if(PyErr_Occurred()) { pyncpp::propagateErrorIfOccurred(_pyncpp_gilState); }
#define PYNCPP_THROW_ERROR pyncpp::propagateErrorIfOccurred(_pyncpp_gilState)

namespace pyncpp
{

/// Raises a Python error. This is equivalent to the "raise" keyword in Python.
///
PYNCPP_EXPORT void raiseError(PyObject* nativeExceptionClass, const char* message);

/// Raises the Python error corresponding to the C++ EXCEPTION_TYPE class.
/// (EXCEPTION_TYPE must be BaseException or a subclass).
///
template <class EXCEPTION_TYPE>
void raiseError(const char* message)
{
    EXCEPTION_TYPE::raise(message);
}

/// Checks if a Python error was raised. The corresponding exception can be
/// fetched with retrieveNativeException().
///
PYNCPP_EXPORT bool errorOccurred();

/// Clears the error indicator. Must be called every time an error is handled or
/// the next Python function will mistakingly believe there was an error (the
/// propagation functions do this automatically).
///
PYNCPP_EXPORT void clearError();

/// Checks if there was an error and throws it as a C++ exception.
/// (see the doc for 'propagateCurrentError' for explanations on the GIL state
/// parameter)
///
PYNCPP_EXPORT void propagateErrorIfOccurred(PyGILState_STATE& gilState);

/// Throws the current error as a C++ exception. Must only be called if there
/// was an error.
/// The Python global interpreter lock must be acquired and the thread state
/// passed to this function so that the lock may be released before throwing
/// the exception.
/// IMPORTANT: Recursive calls to PyGILState_Ensure() are currently not
/// supported in this API.
///
PYNCPP_EXPORT void propagateCurrentError(PyGILState_STATE& gilState);

} // namespace pyncpp

#endif // PYNCPP_ERROR_HANDLING_H
