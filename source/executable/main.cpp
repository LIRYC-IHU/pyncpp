#include <pyncpp.h>

#include <cstdlib>
#include <iostream>

int main(int argc, char** argv)
{
    int exitStatus = EXIT_SUCCESS;

    const char* pythonHome = std::getenv("PYNCPP_PYTHONHOME");

    if (pythonHome != nullptr)
    {
        pyncpp::Manager::setPythonHome(pythonHome);
    }
    else
    {
        pyncpp::Manager::setPythonHome(PYTHON_HOME);
    }

    pyncpp::Manager::setCommandLineArguments(argc, argv);
    pyncpp::Manager& pyncppManager = pyncpp::Manager::instance();

    if (pyncppManager.errorOccured())
    {
        std::cout << "Python initialization error: " << pyncppManager.errorMessage() << "\n" << std::endl;
        exitStatus = EXIT_FAILURE;
    }
    else
    {
        exitStatus = pyncppManager.runMain();
    }

    return exitStatus;
}
