// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "qlist.h"

namespace pyncpp
{

void clearList(QList<PyObject*> list)
{
    while (!list.isEmpty())
    {
        PyObject* item = list.takeFirst();
        Py_CLEAR(item);
    }
}

} // namespace pyncpp

bool pyncppToPython(const QList<PyObject*>& qList, PyObject** output)
{
    *output = PyList_New(qList.length());

    if (*output)
    {
        for (Py_ssize_t i = 0; i < qList.length(); i++)
        {
            PyObject* item = qList.at(i);
            Py_INCREF(item);
            PyList_SET_ITEM(*output, i, item);
        }

        return true;
    }
    else
    {
        return false;
    }
}

bool pyncppToCPP(const PyObject* object, QList<PyObject*>& output)
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
                output.append(item);
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
