// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "qstring.h"

#include "primitive.h"

bool pyncppToPython(const QString& value, PyObject** output)
{
    return pyncppToPython(qUtf8Printable(value), output);
}

bool pyncppToCPP(PyObject* object, QString& output)
{
    std::string outputString;
    bool success = pyncppToCPP(object, outputString);
    output = QString::fromStdString(outputString);
    return success;
}
