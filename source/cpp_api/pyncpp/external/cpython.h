// Copyright (c) 2022, 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CPYTHON_H
#define PYNCPP_CPYTHON_H

/// NOTE: CPython defines pre-processor macros that affect the standard headers.
/// This file must therefore always be included before the standard headers.
///

// Python recommends to always define this macro before including its header.
#define PY_SSIZE_T_CLEAN

#include <Python.h>

#endif // PYNCPP_CPYTHON_H
