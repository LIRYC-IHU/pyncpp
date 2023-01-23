// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "qobject.h"

#include "../error.h"
#include "../external/swig.h"

bool pyncppToPython(QObject* object, PyObject** output)
{
    *output = pyncpp::wrapObjectWithSWIG(object);
    return !pyncpp::errorOccurred();
}

bool pyncppToCPP(PyObject* nativeObject, QObject*& output)
{
    output = (QObject*)pyncpp::extractSWIGWrappedObject(nativeObject);
    return !pyncpp::errorOccurred();
}
