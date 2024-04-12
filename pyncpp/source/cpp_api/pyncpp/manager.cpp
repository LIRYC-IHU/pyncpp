// Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "manager.h"

#include <stdexcept>

#ifdef PYNCPP_QT5_SUPPORT
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QShortcut>
#include <QWidget>
#include <QLabel>
#else
#include <iostream>
#endif

#include "../pyncpp.h"

namespace pyncpp
{

struct ManagerPrivate
{
    bool isRunning = false;
    Manager::OutputFunction infoOutput = nullptr;
    Manager::OutputFunction warningOutput = nullptr;
    Manager::OutputFunction errorOutput = nullptr;
    wchar_t* pythonHome = nullptr;
    Object* console = nullptr;

#ifdef PYNCPP_QT5_SUPPORT
    QShortcut* consoleShortcut = nullptr;
#endif
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

void Manager::runConsole()
{
    if (!d->console)
    {
        Module toolsModule = Module::import("pyncpp_tools");
        d->console = new Object(toolsModule.attribute("Console")());
        d->console->callMethod("run");
        QWidget* console = d->console->toCPP<QWidget*>();
        console->setWindowTitle("woohooo!!");
        d->console->callMethod("layout").callMethod("addWidget", new QLabel("woooooooo!!!!!"));
    }
}

void Manager::setConsoleVisible(bool isVisible)
{
    d->console->callMethod("setVisible", isVisible);
}

bool Manager::isConsoleVisible()
{
    return d->console->callMethod("isVisible").toCPP<bool>();
}

void Manager::setConsoleShortcut(QKeySequence keySequence, QWidget* widget)
{
    if (!d->consoleShortcut)
    {
        d->consoleShortcut = new QShortcut(widget);
        d->consoleShortcut->setContext(Qt::ApplicationShortcut);
        widget->connect(d->consoleShortcut, &QShortcut::activated, [=] () {
            d->console->callMethod("setVisible", (not d->console->callMethod("isVisible")));
        });
    }

    d->consoleShortcut->setKey(keySequence);
}

void Manager::deleteConsole()
{
    if (d->console)
    {
        qDebug() << "deleting console";
#ifdef PYNCPP_QT5_SUPPORT
        d->console->callMethod("deleteLater");
#endif
        delete d->console;
        d->console = nullptr;

        delete d->consoleShortcut;
        d->consoleShortcut = nullptr;
    }
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
        std::string message = std::string("pyncpp initialization failed: ") + e.what();
        d->errorOutput(message.c_str());
    }

    return d->isRunning;
}

void Manager::initializeOutputFunctions()
{
    if (!d->infoOutput)
    {
#ifdef PYNCPP_QT5_SUPPORT
        d->infoOutput = [] (const char* text) { qInfo() << text; };
#else
        d->infoOutput = [] (const char* text) { std::cout << text << std::endl; };
#endif
    }

    if (!d->warningOutput)
    {
#ifdef PYNCPP_QT5_SUPPORT
        d->warningOutput = [] (const char* text) { qWarning() << text; };
#else
        d->warningOutput = [] (const char* text) { std::cout << text << std::endl; };
#endif
    }

    if (!d->errorOutput)
    {
#ifdef PYNCPP_QT5_SUPPORT
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
    internal::initializeExceptions();
}

void Manager::finalize()
{
    finalizeInterpreter();
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
