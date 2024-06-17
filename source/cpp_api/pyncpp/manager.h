// Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_MANAGER_H
#define PYNCPP_MANAGER_H

#include <filesystem>
#include <memory>
#include <string>
#include <vector>

#include "external/cpython.h"

#include "export.h"

namespace pyncpp
{

struct ManagerPrivate;

class PYNCPP_EXPORT Manager
{
public:
    static void setPythonHome(const std::filesystem::path& pythonHome);
    static const std::filesystem::path& getPythonHome();

    static void addInitialModulePath(const std::filesystem::path& path);
    static const std::vector<std::filesystem::path> getInitialModulePaths();

    static void setCommandLineArguments(int argc, char** argv);
    static const std::vector<std::string> getCommandLineArguments();

    static void setMainModule(const char* name);
    static const std::string getMainModule();

    static Manager& instance();
    static void destroyInstance();

    bool errorOccured();
    void clearError();
    const char* errorMessage();

    bool addModulePath(const std::filesystem::path& path);

    int runMain();

private:
    class ManagerDeleter
    {
    public:
        void operator()(Manager* instance) const
        {
            delete instance;
        }
    };

    static std::unique_ptr<Manager, ManagerDeleter> static_instance;
    ManagerPrivate* const d;

    static std::filesystem::path getExecutablePath();
    static std::filesystem::path absolutePath(const std::filesystem::path& path);

    Manager();
    ~Manager();

    void initializeInterpreterIfNeeded();
    void initializeAPI();

    void finalizeInterpreterIfNeeded();

    void checkStatus(PyStatus status);
};

} // namespace pyncpp

#endif // PYNCPP_MANAGER_H
