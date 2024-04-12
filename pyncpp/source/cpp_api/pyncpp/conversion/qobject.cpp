// Copyright (c) 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include <QDebug>

#include "qobject.h"

#include "../error.h"
#include "../object.h"

bool pyncppToPython(QObject* value, PyObject** output)
{
    try
    {
        pyncpp::Module shiboken = pyncpp::import("shiboken2");
        pyncpp::Module qtWidgets = pyncpp::import("PySide2.QtWidgets");
        size_t pointerAsSizeT = reinterpret_cast<size_t>(value);
        pyncpp::Object widgetClass = qtWidgets.attribute("QWidget");
        *output = shiboken.callMethod("wrapInstance", pointerAsSizeT, widgetClass).newReference();
        return *output;
    }
    catch (pyncpp::TypeError& e)
    {
        pyncpp::raiseError<pyncpp::TypeError>(e.what());
    }
    catch (pyncpp::Exception& e)
    {
        pyncpp::raiseError<pyncpp::Exception>(e.what());
    }

    return false;
}

bool pyncppToCPP(PyObject* object, QObject*& output)
{
    try
    {
        pyncpp::Module shiboken = pyncpp::import("shiboken2");
        pyncpp::Object rawPointer = shiboken.callMethod("getCppPointer", object)[0];
        size_t castedPointer = rawPointer.toCPP<size_t>();
        output = reinterpret_cast<QObject*>(castedPointer);
        return true;
    }
    catch (pyncpp::TypeError& e)
    {
        pyncpp::raiseError<pyncpp::TypeError>(e.what());
    }
    catch (pyncpp::Exception& e)
    {
        pyncpp::raiseError<pyncpp::Exception>(e.what());
    }

    return false;
}
