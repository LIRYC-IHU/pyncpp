// Copyright (c) 2022-2023 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_H
#define PYNCPP_H

#include "pyncpp/conversion.h"
#include "pyncpp/error.h"
#include "pyncpp/external/cpython.h"
#include "pyncpp/external/cpython_call.h"
#include "pyncpp/manager.h"
#include "pyncpp/object.h"

#if PYNCPP_SWIG_SUPPORT
#include "pyncpp/external/swig.h"
#endif  // PYNCPP_SWIG_SUPPORT

#endif // PYNCPP_H
