// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CONVERSION_QOBJECT_H
#define PYNCPP_CONVERSION_QOBJECT_H

#include "../external/cpython.h"

#include <QObject>

#include "../export.h"

PYNCPP_EXPORT bool pyncppToPython(QObject* object, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* nativeObject, QObject*& output);

#endif // PYNCPP_CONVERSION_QOBJECT_H
