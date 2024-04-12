// Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CONVERSION_QOBJECT_H
#define PYNCPP_CONVERSION_QOBJECT_H

#include "../external/cpython.h"

#include <QObject>

#include "../export.h"

PYNCPP_EXPORT bool pyncppToPython(QObject* value, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* object, QObject*& output);

template <typename TYPE, std::enable_if_t<std::is_base_of<QObject, TYPE>::value, bool> = true>
bool pyncppToPython(TYPE* value, PyObject** output)
{
    return pyncppToPython(static_cast<QObject*>(value), output);
}

template <typename TYPE, std::enable_if_t<std::is_base_of<QObject, TYPE>::value, bool> = true>
bool pyncppToCPP(PyObject* object, TYPE*& output)
{
    QObject* tmpOutput;

    if (pyncppToCPP(object, tmpOutput))
    {
        output = dynamic_cast<TYPE*>(tmpOutput);
        return output;
    }

    return false;
}

#endif // PYNCPP_CONVERSION_QOBJECT_H
