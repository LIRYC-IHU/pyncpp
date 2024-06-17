// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_EXCEPTION_THROWER_H
#define PYNCPP_EXCEPTION_THROWER_H

/// These functions are used to propagate Python exceptions to C++. Each Python
/// exception class is registered with a function that converts instances to a
/// corresponding C++ class and throws them.
///

#include "../external/cpython.h"

namespace pyncpp::internal
{

typedef void (*ExceptionThrower)(PyObject* nativeException);

void registerExceptionThrower(PyObject* nativeClass, ExceptionThrower thrower);

template <class EXCEPTION>
void registerExceptionThrower(PyObject* nativeClass)
{
    ExceptionThrower thrower = [](PyObject* nativeException)
    {
        EXCEPTION exception(nativeException);
        throw exception;
    };

    registerExceptionThrower(nativeClass, thrower);
}

void clearExceptionThrowers();

/// Converts a native Python exception to an equivalent C++ exception and throws
/// it. (The input reference is stolen)
//
void throwException(PyObject* nativeException, PyGILState_STATE& gilState);

} // namespace pyncpp::internal

#endif // PYNCPP_EXCEPTION_THROWER_H
