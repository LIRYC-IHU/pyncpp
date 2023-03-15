// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_CPYTHON_H
#define PYNCPP_CPYTHON_H

/// NOTE: CPython defines pre-processor macros that affect the standard headers.
/// This file must therefore always be included before the standard headers.
///

#if PYNCPP_QT5_SUPPORT
// One of Python's structs has a member named slots, and this causes conflicts
// with the Qt keyword of the same name.
#undef slots
#define slots _slots
#endif // PYNCPP_QT5_SUPPORT

// Python recommends to always define this macro before including its header.
#define PY_SSIZE_T_CLEAN

#include <Python.h>

#if PYNCPP_QT5_SUPPORT
#undef slots
#define slots Q_SLOTS
#endif // PYNCPP_QT5_SUPPORT

#endif // PYNCPP_CPYTHON_H
