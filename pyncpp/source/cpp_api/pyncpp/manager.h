// Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_MANAGER_H
#define PYNCPP_MANAGER_H

#include <functional>

#ifdef PYNCPP_QT5_SUPPORT
#include <QKeySequence>
#include <QWidget>
#endif

#include "export.h"

namespace pyncpp
{

struct ManagerPrivate;

class PYNCPP_EXPORT Manager
{
public:
    typedef std::function<void(const char*)> OutputFunction;

    Manager();    
    ~Manager();

    void setOutputFunctions(OutputFunction info, OutputFunction warning, OutputFunction error);

    bool initialize(const char* pythonHome);
    void finalize();

    bool isRunning();

    void runConsole();
    void setConsoleVisible(bool isVisible);
    bool isConsoleVisible();
#ifdef PYNCPP_QT5_SUPPORT
    void setConsoleShortcut(QKeySequence keySequence, QWidget* widget);
#endif
    void deleteConsole();

private:
    ManagerPrivate* const d;

    void initializeOutputFunctions();
    void initializeInterpreter(const char* pythonHome);
    void initializeAPI();

    void finalizeInterpreter();
};

} // namespace pyncpp

#endif // PYNCPP_MANAGER_H
