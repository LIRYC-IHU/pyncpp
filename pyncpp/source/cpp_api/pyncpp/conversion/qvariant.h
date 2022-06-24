// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CONVERSION_QVARIANT_H
#define PYNCPP_CONVERSION_QVARIANT_H

#include "../external/cpython.h"

#include <QVariant>

#include "../export.h"

PYNCPP_EXPORT bool pyncppToPython(const QVariant& value, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* object, QVariant& output);

#endif // PYNCPP_CONVERSION_QVARIANT_H
