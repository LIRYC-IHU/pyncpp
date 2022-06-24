#include "application.h"

#include <pyncpp.h>

namespace pyncpp::qt_console
{

Application::Application(int& argc, char** argv) :
    QApplication(argc, argv)
{
}

} // namespace pyncpp::qt_console
