// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CONVERSION_QSTRING_H
#define PYNCPP_CONVERSION_QSTRING_H

#include "../external/cpython.h"

#include <QString>

#include "../export.h"

PYNCPP_EXPORT bool pyncppToPython(const QString& value, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* object, QString& output);

#endif // PYNCPP_CONVERSION_QSTRING_H
