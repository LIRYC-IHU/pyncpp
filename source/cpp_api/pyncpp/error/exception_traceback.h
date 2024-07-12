// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_EXCEPTION_TRACEBACK_H
#define PYNCPP_EXCEPTION_TRACEBACK_H

#include "../external/cpython.h"

#include <string>

#include "../export.h"

namespace pyncpp::internal
{

/// Retrieves the traceback of an exception and converts it to a user-readable
/// string.
///
PYNCPP_EXPORT std::string formatExceptionTraceback(PyObject* nativeException);

} // namespace pyncpp::internal

#endif // PYNCPP_EXCEPTION_TRACEBACK_H
