// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "standard_objects.h"

namespace pyncpp
{

Object none()
{
    return cpythonCall(Py_NewRef, Py_None);
}

Object list()
{
    return cpythonCall(PyList_New, 0);
}

Object list(const AbstractObject& object)
{
    return cpythonCall(PySequence_List, *object);
}

Object tuple()
{
    return cpythonCall(PyTuple_New, 0);
}

Object tuple(const AbstractObject& object)
{
    return cpythonCall(PySequence_Tuple, *object);
}

Object dict()
{
    return cpythonCall(PyODict_New);
}

} // namespace pyncpp
