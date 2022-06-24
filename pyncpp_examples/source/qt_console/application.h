#ifndef PYNCPP_QT_CONSOLE_APPLICATION_H
#define PYNCPP_QT_CONSOLE_APPLICATION_H

#include <QApplication>

namespace pyncpp::qt_console
{

class Application : public QApplication
{
public:
    Application(int& argc, char** argv);
};

} // namespace pyncpp::qt_console

#endif // PYNCPP_QT_CONSOLE_APPLICATION_H
