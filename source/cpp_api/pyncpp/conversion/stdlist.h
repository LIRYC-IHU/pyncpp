// Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CONVERSION_STDLIST_H
#define PYNCPP_CONVERSION_STDLIST_H

#include "../external/cpython.h"

#include <list>

#include "../export.h"

namespace pyncpp
{

template <class TYPE>
void clearList(std::list<TYPE> list)
{
    list.clear();
}

template <class TYPE>
void clearList(std::list<TYPE*> list)
{
    for (auto &item : list)
    {
        delete item;
    }

    list.clear();
}

void PYNCPP_EXPORT clearList(std::list<PyObject*> list);

} // namespace pyncpp

PYNCPP_EXPORT bool pyncppToPython(const std::list<PyObject*>& stdList, PyObject** output);

template <class TYPE>
bool pyncppToPython(const std::list<TYPE>& stdList, PyObject** output)
{
    bool success = true;
    std::list<PyObject*> objectList;

    for (auto &item : stdList)
    {
        PyObject* pythonItem;

        if (pyncppToPython(item, &pythonItem))
        {
            objectList.push_back(pythonItem);
        }
        else
        {
            success = false;
            break;
        }
    }

    success = success && pyncppToPython(objectList, output);
    pyncpp::clearList(objectList);
    return success;
}

PYNCPP_EXPORT bool pyncppToCPP(const PyObject* object, std::list<PyObject*>& output);

template <class TYPE>
bool pyncppToCPP(const PyObject* object, std::list<TYPE>& output)
{
    bool success = true;
    std::list<PyObject*> objectList;

    if (pyncppToCPP(object, objectList))
    {
        output.clear();

        for (auto pythonItem : objectList)
        {
            TYPE item;

            if (pyncppToCPP(pythonItem, item))
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

        pyncpp::clearList(objectList);
    }

    return success;
}

#endif // PYNCPP_CONVERSION_STDLIST_H
