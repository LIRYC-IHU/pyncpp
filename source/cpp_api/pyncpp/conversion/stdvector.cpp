// Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "stdvector.h"

namespace pyncpp
{

void clearVector(std::vector<PyObject*> vector)
{
    for (auto &item : vector)
    {
        Py_CLEAR(item);
    }

    vector.clear();
}

} // namespace pyncpp

bool pyncppToPython(const std::vector<PyObject*>& stdVector, PyObject** output)
{
    *output = PyList_New(stdVector.size());

    if (*output)
    {
        Py_ssize_t i = 0;

        for (auto &item : stdVector)
        {
            Py_INCREF(item);
            PyList_SET_ITEM(*output, i++, item);
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool pyncppToCPP(const PyObject* object, std::vector<PyObject*>& output)
{
    bool success = true;
    Py_ssize_t numItems = PySequence_Size(const_cast<PyObject*>(object));

    if (numItems != -1)
    {
        output.reserve(numItems);

        for (Py_ssize_t i = 0; i < numItems; i++)
        {
            PyObject* item = PySequence_GetItem(const_cast<PyObject*>(object), i);

            if (item)
            {
                output.push_back(item);
            }
            else
            {
                pyncpp::clearVector(output);
                success = false;
                break;
            }
        }

        output.shrink_to_fit();
    }
    else
    {
        success = false;
    }

    return success;
}
