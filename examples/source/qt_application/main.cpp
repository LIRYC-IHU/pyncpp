// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include <pyncpp.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    pyncpp::Manager pythonManager;
    QDir pythonHome = qApp->applicationDirPath();
    QString pythonMessage;

    if (!pythonHome.cd(PYTHON_HOME))
    {
        pythonMessage = "The Python interpreter could not be found ";
    }
    else
    {
        if (!pythonManager.initialize(qUtf8Printable(pythonHome.absolutePath())))
        {
            pythonMessage = "Initialization of the Python interpreter failed.";
        }
        else
        {
            pythonMessage = "The Python interpreter is running.";
        }
    }

    QMessageBox pythonMessageBox;
    pythonMessageBox.setWindowTitle("pyncpp qt application example");
    pythonMessageBox.setInformativeText(pythonMessage);
    pythonMessageBox.exec();

    return 0;
}
