// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "application.h"

#include <pyncpp.h>

namespace pyncpp::qt_console
{

Application::Application(int& argc, char** argv) :
    QApplication(argc, argv)
{
}

} // namespace pyncpp::qt_console
