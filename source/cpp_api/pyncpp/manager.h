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
    /// Set the path of the standard Python libraries (for more detailes see the
    /// official Python documentation for PYTHONHOME and PYTHONPLATLIBDIR).
    /// Must be called before the first call to 'instance()'.
    ///
    static void setPythonHome(const std::filesystem::path& pythonHome, const char* platLibDir = nullptr);
    static const std::filesystem::path& getPythonHome();
    static const std::string& getPlatLibDir();

    /// Set sys.path explicitly (using this function disables the
    /// default paths).
    /// Must be called before the first call to 'instance()'.
    ///
    static void addInitialModulePath(const std::filesystem::path& path);
    static const std::vector<std::filesystem::path> getInitialModulePaths();

    /// Set sys.argv (see the official documentation for more details).
    /// Must be called before the first call to 'instance()'.
    ///
    static void setCommandLineArguments(int argc, char** argv);
    static const std::vector<std::string> getCommandLineArguments();

    /// Set the module to run on startup (equivalent to Python's -m option).
    /// Must be called before the first call to 'instance()'.
    ///
    static void setMainModule(const char* name);
    static const std::string& getMainModule();

    /// Initialize Python if needed and return the manager singleton.
    ///
    static Manager& instance();

    /// Finalize Python and destroy the current manager singleton.
    ///
    static void destroyInstance();


    /// Check if an error occured during intialization.
    ///
    bool errorOccured();

    /// Clear the initialization error.
    ///
    void clearError();

    /// Retrieve the initialization error message.
    ///
    const char* errorMessage();

    /// Append 'path' to sys.path.
    ///
    bool appendModulePath(const std::filesystem::path& path);

    /// Prepend 'path' to sys.path.
    ///
    bool prependModulePath(const std::filesystem::path& path);

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

    // Convert relative paths to absolute using the executable path as the
    // starting point.
    //
    static std::filesystem::path absolutePath(const std::filesystem::path& path);

    Manager();
    ~Manager();

    void initializeInterpreterIfNeeded();
    void initializeAPI();

    void finalizeInterpreterIfNeeded();

    void setHomeConfig(PyConfig& config);
    void setCommandLineArgumentsConfig(PyConfig& config);
    void setModulePathsConfig(PyConfig& config);
    void setMainModuleConfig(PyConfig& config);

    // If 'virtualEnvHome' points to a valid virtual env, return the home path
    // of the underlying Python installation, or an empty path otherwise.
    //
    std::filesystem::path getActualHomeFromVirtualEnvIfExists(std::filesystem::path virtualEnvHome);

    // Check the initialization status and throw the appropriate exception of
    // and error occured.
    //
    void checkStatus(PyStatus status);

    bool addModulePath(const std::filesystem::path& path, bool prepend);
};

} // namespace pyncpp

#endif // PYNCPP_MANAGER_H
