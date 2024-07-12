// Copyright (c) 2022, 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_STANDARD_OBJECTS_H
#define PYNCPP_STANDARD_OBJECTS_H

/// This header provides functions to create standard Python objects (for
/// numeric and string objects, use the corresponding Object constructors
/// directly, i.e. Object("foo"))
///

#include "../external/cpython.h"

#include <array>
#include <string>

#include "../conversion.h"
#include "../export.h"
#include "../external/cpython_call.h"

#include "object.h"

namespace pyncpp
{

PYNCPP_EXPORT Object none();

PYNCPP_EXPORT Object list();

/// If 'object' is a sequence, returns a list containing the same elements.
///
PYNCPP_EXPORT Object list(const AbstractObject& object);

/// If the input type can be converted to a Python sequence, returns a list
/// containing the same elements.
///
template <class TYPE>
Object list(TYPE input)
{
    PYNCPP_ACQUIRE_GIL;

    PyObject* result;
    pyncppToPython(input, &result);
    PYNCPP_CHECK_ERROR;

    if (!PyList_Check(result))
    {
        PyObject* oldResult = result;
        result = PySequence_List(oldResult);
        Py_CLEAR(oldResult);
        PYNCPP_CHECK_ERROR;
    }

    PYNCPP_RELEASE_GIL;

    return result;
}

PYNCPP_EXPORT Object tuple();

/// If 'object' is a sequence, returns a tuple containing the same elements.
///
PYNCPP_EXPORT Object tuple(const AbstractObject& object);

/// If the input type can be converted to a Python sequence, returns a tuple
/// containing the same elements.
///
template <class TYPE>
Object tuple(const TYPE& input)
{
    PYNCPP_ACQUIRE_GIL;

    PyObject* result;
    pyncppToPython(input, &result);
    PYNCPP_CHECK_ERROR;

    if (!PyTuple_Check(result))
    {
        PyObject* oldResult = result;
        result = PySequence_Tuple(oldResult);
        Py_CLEAR(oldResult);
        PYNCPP_CHECK_ERROR;
    }

    PYNCPP_RELEASE_GIL;

    return result;
}

PYNCPP_EXPORT Object dict();

/// Convert the input to a dict if possible.
///
template <class TYPE>
Object dict(TYPE input)
{
    PYNCPP_ACQUIRE_GIL;

    PyObject* result;
    pyncppToPython(input, &result);
    PYNCPP_CHECK_ERROR;

    if (!PyDict_Check(result))
    {
        PYNCPP_RELEASE_GIL;
        std::string message = std::string("Cannot convert object of type ")
                              + typeid(TYPE).name()
                              + " to Python dict.";
        throw TypeError(message.c_str());
    }

    PYNCPP_RELEASE_GIL;

    return result;
}

} // namespace pyncpp

#endif // PYNCPP_STANDARD_OBJECTS_H
