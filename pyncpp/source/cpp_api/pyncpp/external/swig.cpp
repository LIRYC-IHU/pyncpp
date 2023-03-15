// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "swig.h"

#include <string>

#include "swig_runtime.h"

namespace pyncpp
{

swig_type_info* getSWIGTypeInfo(const char* typeName)
{
    swig_type_info* result = SWIG_TypeQuery(typeName);

    if (!result && !errorOccurred())
    {
        std::string message = std::string("No SWIG type info found for ") + typeName + ".";
        raiseError<TypeError>(message.c_str());
    }

    return result;
}

swig_type_info* getSWIGTypeInfo(const PyObject* object)
{
    SwigPyObject* swigPyObject = SWIG_Python_GetSwigThis(const_cast<PyObject*>(object));
    swig_type_info* result = nullptr;

    if (swigPyObject)
    {
        result = swigPyObject->ty;
    }
    else
    {
        if (!errorOccurred())
        {
            std::string message = std::string(Py_TYPE(object)->tp_name) + " is not a SWIG object.";
            raiseError<TypeError>(message.c_str());
        }
    }

    return result;
}

bool isSWIGWrappedObject(const PyObject* object, const char* typeName)
{
    bool result = SWIG_Python_GetSwigThis(const_cast<PyObject*>(object));

    if (!result && errorOccurred())
    {
        clearError();
    }

    return result;
}

void* extractSWIGWrappedObject(PyObject* object, bool transferOwnership)
{
    swig_type_info* swigTypeInfo = getSWIGTypeInfo(object);
    void* result = nullptr;

    if (swigTypeInfo)
    {
        int flags = SWIG_POINTER_EXCEPTION;

        if (transferOwnership)
        {
            flags &= SWIG_POINTER_DISOWN;
        }

        int success = SWIG_Python_ConvertPtr(object, &result, swigTypeInfo, flags);

        if (!SWIG_IsOK(success))
        {
            result = nullptr;
        }
    }

    return result;
}

PyObject* wrapObjectWithSWIG(void* object, const char* typeName, bool transferOwnership)
{
    if (object == nullptr)
    {
        Py_RETURN_NONE;
    }
    else
    {
        const swig_type_info* swigTypeInfo = getSWIGTypeInfo(typeName);
        PyObject* result = nullptr;

        if (swigTypeInfo)
        {
            result = SWIG_NewPointerObj(object, const_cast<swig_type_info*>(swigTypeInfo), transferOwnership);
        }

        return result;
    }
}

#if PYNCPP_QT5_SUPPORT
PyObject* wrapObjectWithSWIG(QObject* object, bool transferOwnership)
{
    PyObject* result = nullptr;
    const QMetaObject* metaObject =  object ? object->metaObject() : &QObject::staticMetaObject;

    do
    {
        clearError();
        std::string typeName = std::string(metaObject->className()) + "*";
        result = wrapObjectWithSWIG(object, typeName.c_str(), transferOwnership);
        metaObject = metaObject->superClass();
    }
    while (!result && metaObject);

    return result;
}
#endif // PYNCPP_QT5_SUPPORT

} // namespace pyncpp
