// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CONVERSION_PRIMITIVE_H
#define PYNCPP_CONVERSION_PRIMITIVE_H

#include "../external/cpython.h"

#include <string>

#include "../export.h"

PYNCPP_EXPORT bool pyncppToPython(bool value, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* object, bool& output);

PYNCPP_EXPORT bool pyncppToPython(int value, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* object, long& output);

PYNCPP_EXPORT bool pyncppToPython(double value, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* object, double& output);

PYNCPP_EXPORT bool pyncppToPython(const char* value, PyObject** output);
PYNCPP_EXPORT bool pyncppToPython(const std::string& value, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* object, std::string& output);

PYNCPP_EXPORT bool pyncppToPython(void* value, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* object, void** output);

template <class TYPE>
bool pyncppToPython(TYPE* value, PyObject** output, bool transferOwnership = false)
{
    bool success = pyncppToPython(static_cast<void*>(value), output);

    if (success && transferOwnership && PyCapsule_CheckExact(*output))
    {
        PyCapsule_SetDestructor(*output, [] (PyObject* object) {
            delete (TYPE*)PyCapsule_GetPointer(object, nullptr);
        });
    }

    return !PyErr_Occurred();
}

template <class TYPE>
bool pyncppToCPP(PyObject* object, TYPE** output, bool transferOwnership = false)
{
    bool success = pyncppToCPP(object, static_cast<void**>(output));

    if (success && transferOwnership && PyCapsule_CheckExact(object))
    {
        PyCapsule_SetDestructor(object, [] (PyObject*) {});
    }

    return !PyErr_Occurred();
}

#endif // PYNCPP_CONVERSION_PRIMITIVE_H
