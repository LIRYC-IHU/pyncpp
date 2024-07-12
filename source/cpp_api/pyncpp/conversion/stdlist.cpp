// Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "stdlist.h"

namespace pyncpp
{

void clearList(std::list<PyObject*> list)
{
    for (auto &item : list)
    {
        Py_CLEAR(item);
    }

    list.clear();
}

} // namespace pyncpp

bool pyncppToPython(const std::list<PyObject*>& stdList, PyObject** output)
{
    *output = PyList_New(stdList.size());

    if (*output)
    {
        Py_ssize_t i = 0;

        for (auto &item : stdList)
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

bool pyncppToCPP(const PyObject* object, std::list<PyObject*>& output)
{
    bool success = true;
    Py_ssize_t numItems = PySequence_Size(const_cast<PyObject*>(object));

    if (numItems != -1)
    {
        for (Py_ssize_t i = 0; i < numItems; i++)
        {
            PyObject* item = PySequence_GetItem(const_cast<PyObject*>(object), i);

            if (item)
            {
                output.push_back(item);
            }
            else
            {
                pyncpp::clearList(output);
                success = false;
                break;
            }
        }
    }
    else
    {
        success = false;
    }

    return success;
}
