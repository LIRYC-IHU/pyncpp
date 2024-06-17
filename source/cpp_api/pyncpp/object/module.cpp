// Copyright (c) 2022, 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "module.h"

#include "../error.h"
#include "../external/cpython_call.h"

#include "standard_objects.h"

namespace pyncpp
{

Module Module::main()
{
    return getModuleObject("__main__");
}

Module Module::import(const char* name)
{
    return getModuleObject(name);
}

Module::Module(PyObject* reference) :
    Object(ensureModuleObject(reference))
{
}

Module::Module(const AbstractObject& other) :
    Object(ensureModuleObject(other.newReference()))
{
}

std::string Module::name()
{
    return cpythonCall(PyModule_GetName, **this);
}

Object Module::dict()
{
    return Object::borrowed(cpythonCall(PyModule_GetDict, **this));
}

PyObject* Module::ensureModuleObject(PyObject* reference)
{
    if (!PyModule_Check(reference))
    {
        std::string typeName = Py_TYPE(reference)->tp_name;
        Py_CLEAR(reference);

        throw TypeError((typeName + " is not a module").c_str());
    }

    return reference;
}

PyObject* Module::getModuleObject(const char* name)
{
    Object pythonName = name;
    PyObject* moduleObject = cpythonCall(PyImport_GetModule, *pythonName);

    if (!moduleObject)
    {
        Object fromList = list<std::string>({"*"});
        moduleObject = cpythonCall(PyImport_ImportModuleLevel, name, nullptr, nullptr, *fromList, 0);
    }

    return moduleObject;
}

Module import(const char* name)
{
    return Module::import(name);
}

} // namespace pyncpp

