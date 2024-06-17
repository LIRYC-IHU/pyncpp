// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CONVERSION_QLIST_H
#define PYNCPP_CONVERSION_QLIST_H

#include "../external/cpython.h"

#include <QList>

#include "../export.h"

namespace pyncpp
{

template <class TYPE>
void clearList(QList<TYPE> list)
{
    list.clear();
}

template <class TYPE>
void clearList(QList<TYPE*> list)
{
    while (!list.isEmpty())
    {
        delete list.takeFirst();
    }
}

void PYNCPP_EXPORT clearList(QList<PyObject*> list);

} // namespace pyncpp

PYNCPP_EXPORT bool pyncppToPython(const QList<PyObject*>& qList, PyObject** output);

template <class TYPE>
bool pyncppToPython(const QList<TYPE>& qList, PyObject** output)
{
    bool success = true;
    QList<PyObject*> objectList;

    foreach (TYPE item, qList)
    {
        PyObject* pythonItem;

        if (pyncppToPython(item, &pythonItem))
        {
            objectList.append(pythonItem);
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

PYNCPP_EXPORT bool pyncppToCPP(const PyObject* object, QList<PyObject*>& output);

template <class TYPE>
bool pyncppToCPP(const PyObject* object, QList<TYPE>& output)
{
    bool success = true;
    QList<PyObject*> objectList;

    if (pyncppToCPP(object, objectList))
    {
        output.clear();

        foreach (PyObject* pythonItem, objectList)
        {
            TYPE item;

            if (pyncppToCPP(pythonItem, item))
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

        pyncpp::clearList(objectList);
    }

    return success;
}

#endif // PYNCPP_CONVERSION_QLIST_H
