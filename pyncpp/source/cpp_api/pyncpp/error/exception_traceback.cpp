// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "exception_traceback.h"

namespace pyncpp::internal
{

namespace
{

PyObject* tracebackModule = nullptr;

PyObject* getTracebackModule()
{
    if (!tracebackModule)
    {
        PyObject* tracebackString = PyUnicode_FromString("traceback");
        tracebackModule = PyImport_Import(tracebackString);
        Py_CLEAR(tracebackString);
        PyErr_Clear();
    }

    return tracebackModule;
}

const char* getTracebackItem(PyObject* tracebackList, Py_ssize_t index)
{
    const char* result;
    PyObject* tracebackItem = PySequence_GetItem(tracebackList, index);

    if (tracebackItem)
    {
        PyObject* encodedTracebackItem = PyUnicode_AsEncodedString(tracebackItem, "UTF-8", "strict");
        Py_CLEAR(tracebackItem);

        if (encodedTracebackItem)
        {
            result = PyBytes_AS_STRING(encodedTracebackItem);
            Py_CLEAR(encodedTracebackItem);
         }
    }

    PyErr_Clear();
    return result;
}

std::string concatenateTracebackList(PyObject* tracebackList)
{
    std::string result;
    Py_ssize_t numTracebackItems = PyObject_Length(tracebackList);

    for (Py_ssize_t i = 0; i < numTracebackItems; i++)
    {
        result.append(getTracebackItem(tracebackList, i));
    }

    PyErr_Clear();
    return result;
}

PyObject* getTracebackList(PyObject* nativeException)
{
    PyObject* traceback = PyObject_GetAttrString(nativeException, "__traceback__");
    const char* formatMethodName = traceback ? "format_exception" : "format_exception_only";
    PyObject* formatMethod = PyObject_GetAttrString(getTracebackModule(), formatMethodName);
    PyObject* formatArgs;
    PyObject* tracebackList = nullptr;

    if (traceback)
    {
        formatArgs = Py_BuildValue("OON", Py_TYPE(nativeException), nativeException, traceback);
    }
    else
    {
        formatArgs = Py_BuildValue("OO", Py_TYPE(nativeException), nativeException);
    }

    if (formatMethod && formatArgs)
    {
        tracebackList = PyObject_CallObject(formatMethod, formatArgs);
    }

    PyErr_Clear();
    Py_CLEAR(formatMethod);
    Py_CLEAR(formatArgs);
    return tracebackList;
}

std::string formatWithTracebackModule(PyObject* nativeException)
{
    PyObject* tracebackList = getTracebackList(nativeException);
    std::string result = concatenateTracebackList(tracebackList);
    Py_CLEAR(tracebackList);
    return result;
}

} // namespace

std::string formatExceptionTraceback(PyObject* nativeException)
{
    std::string result;

    if (getTracebackModule())
    {
        result = formatWithTracebackModule(nativeException);
    }

    return result;
}

} // namespace pyncpp::internal
