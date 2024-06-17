// Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "base_exception.h"

#include <string>

#include "../external/cpython_call.h"

#include "error_handling.h"
#include "exception_traceback.h"
#include "exception_thrower.h"

namespace pyncpp
{

struct BaseExceptionPrivate
{
    std::string message;
};

void BaseException::registerThrower()
{
    internal::registerExceptionThrower<BaseException>(PyExc_BaseException);
}

void BaseException::raise(const char* message)
{
    raiseError(PyExc_BaseException, message);
}

BaseException::BaseException(const BaseException& other) :
    d(new BaseExceptionPrivate)
{
    d->message = other.d->message;
}

BaseException::BaseException(const char* message) :
    d(new BaseExceptionPrivate)
{
    PyObject* nativeException = createNativeException(PyExc_BaseException, message);
    initializeFromNativeException(nativeException);
}

BaseException::BaseException(PyObject* nativeException) :
    d(new BaseExceptionPrivate)
{
    initializeFromNativeException(nativeException);
}

BaseException::~BaseException()
{
    delete d;
}

const char* BaseException::what() const throw()
{
    return d->message.c_str();
}

PyObject* BaseException::createNativeException(PyObject* nativeClass, const char* message)
{
    PYNCPP_ACQUIRE_GIL;
    PyObject* args = Py_BuildValue("(s)", message);
    PyObject* nativeException = PyObject_CallObject(nativeClass, args);
    PyErr_Clear();
    Py_CLEAR(args);
    PYNCPP_RELEASE_GIL;
    return nativeException;
}

void BaseException::initializeFromNativeException(PyObject* nativeException)
{
    if (nativeException)
    {
        PYNCPP_ACQUIRE_GIL;
        d->message = internal::formatExceptionTraceback(nativeException);
        Py_CLEAR(nativeException);
        PYNCPP_RELEASE_GIL;
    }
    else
    {
        d->message = "Undefined error";
    }
}

} // namespace pyncpp
