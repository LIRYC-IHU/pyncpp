// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "manager.h"

#include <stdexcept>

#if PYNCPP_QT5_SUPPORT
#include <QApplication>
#include <QDebug>
#include <QDir>
#else
#include <iostream>
#endif

#include "external/cpython.h"

namespace pyncpp
{

struct ManagerPrivate
{
    bool isRunning = false;
    Manager::OutputFunction infoOutput = nullptr;
    Manager::OutputFunction warningOutput = nullptr;
    Manager::OutputFunction errorOutput = nullptr;
    wchar_t* pythonHome;
};

Manager::Manager() :
    d(new ManagerPrivate)
{
}

Manager::~Manager()
{
    finalize();
}

void Manager::setOutputFunctions(OutputFunction info, OutputFunction warning, OutputFunction error)
{
    d->infoOutput = info;
    d->warningOutput = warning;
    d->errorOutput = error;
}

bool Manager::isRunning()
{
    return d->isRunning;
}

bool Manager::initialize(const char* pythonHome)
{
    try
    {
        initializeOutputFunctions();
        initializeInterpreter(pythonHome);
        initializeAPI();
        d->isRunning = true;
    }
    catch (std::exception& e)
    {
        std::string message = std::string("PYNCPP initialization failed: ") + e.what();
        d->errorOutput(message.c_str());
    }

    return d->isRunning;
}

void Manager::initializeOutputFunctions()
{
    if (!d->infoOutput)
    {
#if PYNCPP_QT5_SUPPORT
        d->infoOutput = [] (const char* text) { qInfo() << text; };
#else
        d->infoOutput = [] (const char* text) { std::cout << text << std::endl; };
#endif
    }

    if (!d->warningOutput)
    {
#if PYNCPP_QT5_SUPPORT
        d->warningOutput = [] (const char* text) { qWarning() << text; };
#else
        d->warningOutput = [] (const char* text) { std::cout << text << std::endl; };
#endif
    }

    if (!d->errorOutput)
    {
#if PYNCPP_QT5_SUPPORT
        d->errorOutput = [] (const char* text) { qCritical() << text; };
#else
        d->errorOutput = [] (const char* text) { std::cerr << text << std::endl; };
#endif
    }
}

void Manager::initializeInterpreter(const char* pythonHome)
{
    d->pythonHome = Py_DecodeLocale(qUtf8Printable(QDir::toNativeSeparators(pythonHome)), nullptr);
    Py_SetPythonHome(d->pythonHome);

    Py_Initialize();

    if (Py_IsInitialized())
    {
        std::string message = std::string("Python interpreter initialized: ") + Py_GetVersion();
        d->infoOutput(message.c_str());
    }
    else
    {
        throw std::runtime_error("Could not initialize the Python interpreter.");
    }
}

void Manager::initializeAPI()
{
    //internal::initializeExceptions();
}

void Manager::finalize()
{
    finalizeInterpreter();
    d->isRunning = false;
}

void Manager::finalizeInterpreter()
{
    if (Py_IsInitialized())
    {
        Py_FinalizeEx();
        d->infoOutput("Python interpreter terminated.");
    }
}

} // namespace pyncpp
