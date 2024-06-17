// Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CONVERSION_STDVECTOR_H
#define PYNCPP_CONVERSION_STDVECTOR_H

#include "../external/cpython.h"

#include <vector>

#include "../export.h"

namespace pyncpp
{

template <class TYPE>
void clearVector(std::vector<TYPE> vector)
{
    vector.clear();
}

template <class TYPE>
void clearVector(std::vector<TYPE*> vector)
{
    for (auto &item : vector)
    {
        delete item;
    }

    vector.clear();
}

void PYNCPP_EXPORT clearVectort(std::vector<PyObject*> vector);

} // namespace pyncpp

PYNCPP_EXPORT bool pyncppToPython(const std::vector<PyObject*>& stdVector, PyObject** output);

template <class TYPE>
bool pyncppToPython(const std::vector<TYPE>& stdVector, PyObject** output)
{
    bool success = true;
    std::vector<PyObject*> objectVector;
    objectVector.reserve(stdVector.size());

    for (auto &item : stdVector)
    {
        PyObject* pythonItem;

        if (pyncppToPython(item, &pythonItem))
        {
            objectVector.push_back(pythonItem);
        }
        else
        {
            success = false;
            break;
        }
    }

    success = success && pyncppToPython(objectVector, output);
    pyncpp::clearVector(objectVector);
    return success;
}

PYNCPP_EXPORT bool pyncppToCPP(const PyObject* object, std::vector<PyObject*>& output);

template <class TYPE>
bool pyncppToCPP(const PyObject* object, std::vector<TYPE>& output)
{
    bool success = true;
    std::vector<PyObject*> objectVector;

    if (pyncppToCPP(object, objectVector))
    {
        output.clear();
        output.reserve(objectVector.size());

        for (auto pythonItem : objectVector)
        {
            TYPE item;

            if (pyncppToCPP(pythonItem, item))
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

        pyncpp::clearVector(objectVector);
        output.shrink_to_fit();
    }

    return success;
}

#endif // PYNCPP_CONVERSION_STDVECTOR_H
