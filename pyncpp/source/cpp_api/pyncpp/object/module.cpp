// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
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

Module Module::import(QString name)
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

QString Module::name()
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
        QString typeName = Py_TYPE(reference)->tp_name;
        Py_CLEAR(reference);

        throw TypeError(QString("'%1' is not a module").arg(typeName));
    }

    return reference;
}

PyObject* Module::getModuleObject(QString name)
{
    Object pythonName = name;
    PyObject* moduleObject = cpythonCall(PyImport_GetModule, *pythonName);

    if (!moduleObject)
    {
        Object fromList = list<QString>({"*"});
        moduleObject = cpythonCall(PyImport_ImportModuleLevel, qUtf8Printable(name), nullptr, nullptr, *fromList, 0);
    }

    return moduleObject;
}

Module import(QString name)
{
    return Module::import(name);
}

} // namespace pyncpp

