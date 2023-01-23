// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "function_call.h"

#include "../error.h"
#include "../external/cpython_call.h"

#include "container_accessor.h"

namespace pyncpp
{

struct FunctionCallPrivate
{
    Object callable;
    Object args;
    Object kwargs;
    Object result;
    bool evaluated;
};

FunctionCall::FunctionCall(const AbstractObject& callable, const AbstractObject& args) :
    d(new FunctionCallPrivate)
{
    d->callable = callable;
    d->args = args;
    d->kwargs = cpythonCall(PyDict_New);
    d->evaluated = false;
}

FunctionCall::~FunctionCall() noexcept(false)
{
    if (!d->evaluated)
    {
        PYNCPP_ACQUIRE_GIL;

        PyObject* result = evaluate();
        Py_CLEAR(result);
        PYNCPP_CHECK_ERROR;

        PYNCPP_RELEASE_GIL;
    }
}

FunctionCall& FunctionCall::kwarg(const char* name, const AbstractObject& value)
{
    d->kwargs[name] = value;
    return *this;
}

PyObject* FunctionCall::getReference() const
{
    if (!d->evaluated)
    {
        PYNCPP_ACQUIRE_GIL;

        PyObject* result = evaluate();
        d->evaluated = true;

        if (result)
        {
            d->result = result;
        }
        else
        {
            propagateCurrentError(PYNCPP_GIL_STATE);
        }

        PYNCPP_RELEASE_GIL;
    }

    return *d->result;
}

void FunctionCall::setReference(PyObject* Object)
{
    PYNCPP_ACQUIRE_GIL;
    Py_CLEAR(Object);
    PYNCPP_RELEASE_GIL;
    throw SyntaxError("Cannot assign to a function call");
}

PyObject* FunctionCall::evaluate() const
{
    PyObject* result = nullptr;
    ssize_t numArgs = PyObject_Size(*d->args);

    if (!PyErr_Occurred())
    {
        ssize_t numKwargs = PyObject_Size(*d->kwargs);

        if (!PyErr_Occurred())
        {
            if (numKwargs > 0)
            {
                result = PyObject_Call(*d->callable, *d->args, *d->kwargs);
            }
            else
            {
                if (numArgs > 0)
                {
                    result = PyObject_CallObject(*d->callable, *d->args);
                }
                else
                {
#if PYTHON_VERSION_MINOR > 8
                    result = PyObject_CallNoArgs(*d->callable);
#else
                    result = PyObject_CallObject(*d->callable, nullptr);
#endif
                }
            }
        }
    }

    return result;
}

} // namespace pyncpp
