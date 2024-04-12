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

PYNCPP_EXPORT bool pyncppToPython(size_t value, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* object, size_t& output);

PYNCPP_EXPORT bool pyncppToPython(void* value, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* object, void*& output);

#endif // PYNCPP_CONVERSION_PRIMITIVE_H
