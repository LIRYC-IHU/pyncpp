// Copyright (c) 2022, 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "primitive.h"

#include "../error.h"

bool pyncppToPython(bool value, PyObject** output)
{
    *output = PyBool_FromLong(value);
    return *output;
}

bool pyncppToCPP(PyObject* object, bool& output)
{
    output = PyObject_IsTrue(object);
    return !PyErr_Occurred();
}

bool pyncppToPython(int value, PyObject** output)
{
    *output = PyLong_FromLong(static_cast<long>(value));
    return *output;
}

bool pyncppToCPP(PyObject* object, long& output)
{
    output = PyLong_AsLong(object);
    return !PyErr_Occurred();
}

bool pyncppToPython(double value, PyObject** output)
{
    *output = PyFloat_FromDouble(value);
    return *output;
}

bool pyncppToCPP(PyObject* object, double& output)
{
    output = PyFloat_AsDouble(object);
    return !PyErr_Occurred();
}

bool pyncppToPython(const char* value, PyObject** output)
{
    *output = PyUnicode_FromString(value);
    return *output;
}

bool pyncppToPython(const std::string& value, PyObject** output)
{
    return pyncppToPython(value.c_str(), output);
}

bool pyncppToCPP(PyObject* object, std::string& output)
{
    PyObject* stringAsPyBytes = nullptr;

    if (PyBytes_Check(object))
    {
        stringAsPyBytes = object;
        Py_INCREF(stringAsPyBytes);
    }
    else if (PyUnicode_Check(object))
    {
        stringAsPyBytes = PyUnicode_AsEncodedString(object, "UTF-8", "strict");
    }

    if (!PyErr_Occurred())
    {
        if (stringAsPyBytes)
        {
            output = PyBytes_AS_STRING(stringAsPyBytes);
            Py_CLEAR(stringAsPyBytes);
        }
        else
        {
            // The object is not a string, so apply str() and retry
            PyObject* objectAsString = PyObject_Str(object);

            if (!PyErr_Occurred())
            {
                pyncppToCPP(objectAsString, output);
                Py_CLEAR(objectAsString);
            }
        }
    }

    return !PyErr_Occurred();
}
