// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_SWIG_H
#define PYNCPP_SWIG_H

#include "cpython.h"

#if PYNCPP_QT5_SUPPORT
#include <QObject>
#endif // PYNCPP_QT5_SUPPORT

#include "../error.h"
#include "../export.h"

struct swig_type_info;

namespace pyncpp
{

PYNCPP_EXPORT swig_type_info* getSWIGTypeInfo(const char* typeName);

PYNCPP_EXPORT swig_type_info* getSWIGTypeInfo(const PyObject* object);

PYNCPP_EXPORT bool isSWIGWrappedObject(const PyObject* object);

PYNCPP_EXPORT void* extractSWIGWrappedObject(PyObject* object, bool transferOwnership = false);

PYNCPP_EXPORT PyObject* wrapObjectWithSWIG(void* object, const char* typeName, bool transferOwnership = false);

#if PYNCPP_QT5_SUPPORT
PYNCPP_EXPORT PyObject* wrapObjectWithSWIG(QObject* object, bool transferOwnership = false);
#endif // PYNCPP_QT5_SUPPORT

} // namespace pyncpp

#endif // PYNCPP_SWIG_H
