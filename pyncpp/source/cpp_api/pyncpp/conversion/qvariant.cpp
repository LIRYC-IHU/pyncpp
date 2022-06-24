// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "qvariant.h"

#include "../conversion.h"
#include "../error.h"
#include "../external/swig.h"

bool pyncppToPython(const QVariant& value, PyObject** output)
{
    if (!value.isValid())
    {
        *output = Py_None;
        Py_INCREF(*output);
        return true;
    }
    else
    {
        switch (static_cast<int>(value.type()))
        {
        case QMetaType::Bool:
        {
            return pyncppToPython(value.toBool(), output);
        }
        case QMetaType::Int:
        {
            return pyncppToPython(value.toInt(), output);
        }
        case QMetaType::Float:
        case QMetaType::Double:
        {
            return pyncppToPython(value.toDouble(), output);
        }
        case QMetaType::QString:
        {
            return pyncppToPython(value.toString(), output);
        }
        case QMetaType::QVariantList:
        {
            return pyncppToPython(value.toList(), output);
        }
        case QMetaType::QStringList:
        {
            return pyncppToPython(value.toStringList(), output);
        }
        case QMetaType::QVariantHash:
        {
            return pyncppToPython(value.toHash(), output);
        }
        case QMetaType::QObjectStar:
        {
            *output = pyncpp::wrapObjectWithSWIG(value.value<QObject*>());
            return *output;
        }
        default:
            QVariant* valueCopy = new QVariant(value);
            *output = pyncpp::wrapObjectWithSWIG(valueCopy, "QVariant*", true);
            return *output;
        }
    }
}

template <class TYPE>
bool pyncppToCPP(PyObject* object, QVariant& output)
{
    TYPE value;

    if (pyncppToCPP(object, value))
    {
        output = QVariant::fromValue<TYPE>(value);
        return true;
    }
    else
    {
        return false;
    }
}

bool pyncppToCPP(PyObject* object, QVariant& output)
{
    bool success = false;

    if (object == Py_None)
    {
        output = QVariant();
        success = true;
    }
    else if (PyBool_Check(object))
    {
        success = pyncppToCPP<bool>(object, output);
    }
    else if (PyLong_Check(object))
    {
        success = pyncppToCPP<long>(object, output);
    }
    else if (PyFloat_Check(object))
    {
        success = pyncppToCPP<double>(object, output);
    }
    else if (PyUnicode_Check(object))
    {
        success = pyncppToCPP<QString>(object, output);
    }
    else if (PyList_Check(object))
    {
        success = pyncppToCPP<QList<QVariant> >(object, output);
    }
//    else if (PyDict_Check(object))
//    {
//        success = pyncppToCPP<QHash<QString, QVariant> >(object, output);
//    }
//    else if (pyncpp::isSWIGWrappedObject(object, "QObject*"))
//    {
//        QObject* value = (QObject*)pyncpp::extractSWIGWrappedObject(object);

//        if (value)
//        {
//            output = QVariant::fromValue<QObject*>(value);
//            success = true;
//        }
//    }
//    else if (pyncpp::isSWIGWrappedObject(object, "QVariant*"))
//    {
//        QVariant* value = (QVariant*)pyncpp::extractSWIGWrappedObject(object);

//        if (value)
//        {
//            output = value;
//            success = true;
//        }
//    }
    else
    {
        QString message = QString("Cannot convert Python object of type %1 to QVariant.")
                          .arg(Py_TYPE(object)->tp_name);
        pyncpp::raiseError<pyncpp::TypeError>(qUtf8Printable(message));
    }

    return success;
}
