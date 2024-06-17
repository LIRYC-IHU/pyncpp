// Copyright (c) 2022, 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CONVERSION_OBJECT_H
#define PYNCPP_CONVERSION_OBJECT_H

#include "../external/cpython.h"

#include "../export.h"

namespace pyncpp
{

class AbstractObject;
class Object;

} // namespace pyncpp

PYNCPP_EXPORT bool pyncppToPython(const pyncpp::AbstractObject& object, PyObject** output);
PYNCPP_EXPORT bool pyncppToCPP(PyObject* nativeObject, pyncpp::Object* output);

#endif // PYNCPP_CONVERSION_OBJECT_H
