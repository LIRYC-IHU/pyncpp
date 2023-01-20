// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include <QApplication>
#include <QDebug>

#include <pyncpp.h>

namespace pyncpp::qt_console
{

void runTest()
{
    try
    {
        PyObject* globals = cpythonCall(PyDict_New);
        QString sourceCode = "import sys; print(f'sys.path: {sys.path}')";
        cpythonCall(PyRun_String, qUtf8Printable(sourceCode), Py_file_input, globals, nullptr);
        Py_CLEAR(globals);
    }
    catch (Exception& e)
    {
        qCritical() << e.what();
    }
}

} // namespace pyncpp::qt_console

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    pyncpp::Manager pythonManager;
    int success = pythonManager.initialize();

    if (success)
    {
        pyncpp::qt_console::runTest();
        success = app.exec();
    }

    return success;
}
