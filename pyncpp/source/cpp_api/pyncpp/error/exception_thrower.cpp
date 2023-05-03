// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "exception_thrower.h"

#include <unordered_map>

#include "base_exception.h"

namespace pyncpp::internal
{

namespace
{

std::unordered_map<PyObject*, ExceptionThrower> exceptionThrowers;

// A native exception might be a subclass instance of one of the registered
// exception classes. To convert it we must go up the hierarchy to find the
// actual registered class.
//
PyObject* findRegisteredBaseClass(PyObject* nativeClass)
{
    PyObject* result = nullptr;
    PyObject* typeLineage = PyObject_GetAttrString(nativeClass, "__mro__"); // method resolution order

    if (!PyErr_Occurred() && PyTuple_Check(typeLineage))
    {
        Py_ssize_t lineageSize = PyTuple_GET_SIZE(typeLineage);

        for (int i = 1; i < lineageSize; i++)
        {
            result = PyTuple_GET_ITEM(typeLineage, i);

            if (exceptionThrowers.find(result) != exceptionThrowers.end())
            {
                break;
            }
        }
    }

    Py_CLEAR(typeLineage);
    return result;
}

} // namespace

void registerExceptionThrower(PyObject* nativeClass, ExceptionThrower thrower)
{
    exceptionThrowers[nativeClass] = thrower;
}

void clearExceptionThrowers()
{
    exceptionThrowers.clear();
}

void throwException(PyObject* nativeException, PyGILState_STATE& gilState)
{
    PyObject* nativeClass = reinterpret_cast<PyObject*>(Py_TYPE(nativeException));

    if (exceptionThrowers.find(nativeClass) == exceptionThrowers.end())
    {
        // The exception class is not registered so we search if a parent class
        // is registered.
        nativeClass = findRegisteredBaseClass(nativeClass);
    }

    PyGILState_Release(gilState);

    if (exceptionThrowers.find(nativeClass) != exceptionThrowers.end())
    {
        exceptionThrowers[nativeClass](nativeException);
    }
    else
    {
        // This exception is not a registered type, so throw it as a base
        // exception.
        throw BaseException(nativeException);
    }
}

} // namespace pyncpp::internal
