// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_MANAGER_H
#define PYNCPP_MANAGER_H

#include <functional>

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

    bool initialize();

    bool isRunning();

private:
    ManagerPrivate* const d;

    void initializeOutputFunctions();
    void initializeInterpreter();
    void initializeAPI();

    void finalize();
    void finalizeInterpreter();
};

} // namespace pyncpp

#endif // PYNCPP_MANAGER_H
