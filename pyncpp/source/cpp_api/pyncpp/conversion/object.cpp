// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "object.h"

#include "../object/object.h"

bool pyncppToPython(PyObject* object, PyObject** output)
{
    *output = object;
    return true;
}

bool pyncppToCPP(PyObject* nativeObject, PyObject*& output)
{
    output = nativeObject;
    return true;
}

bool pyncppToPython(pyncpp::AbstractObject& object, PyObject** output)
{
    *output = object.newReference();
    return true;
}

bool pyncppToCPP(PyObject* nativeObject, pyncpp::Object* output)
{
    (*output).borrow(nativeObject);
    return true;
}
