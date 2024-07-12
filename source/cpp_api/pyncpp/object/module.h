// Copyright (c) 2022, 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_MODULE_H
#define PYNCPP_MODULE_H

#include "../external/cpython.h"

#include "../export.h"

#include "object.h"

namespace pyncpp
{

class PYNCPP_EXPORT Module : public Object
{
public:
    static Module main();

    static Module import(const char* name);

    Module(PyObject* reference);

    Module(const AbstractObject& other);

    std::string name();

    Object dict();

private:
    static PyObject* ensureModuleObject(PyObject* reference);
    static PyObject* getModuleObject(const char* name);
};

PYNCPP_EXPORT Module import(const char* name);

} // namespace pyncpp

#endif // PYNCPP_MODULE_H
