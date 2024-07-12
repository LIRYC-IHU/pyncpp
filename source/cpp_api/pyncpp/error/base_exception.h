// Copyright (c) 2022-2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_BASE_EXCEPTION_H
#define PYNCPP_BASE_EXCEPTION_H

/// The base class for propagating Python related exceptions. Direct instances
/// of this class are considered system failures and must only be caught by top
/// level code. Other functions should catch Exception (or a subclass) instead.
///

#include "../external/cpython.h"

#include <stdexcept>

#include "../export.h"

namespace pyncpp
{

struct BaseExceptionPrivate;

class PYNCPP_EXPORT BaseException : public std::exception
{
public:
    static void registerThrower();

    static void raise(const char* message);

    BaseException(const BaseException& other);

    /// Temporarily creates a native Python exception and uses it to construct a
    /// C++ exception. This is intended for the wrapper code to throw exceptions
    /// as if they originated from the Python interpreter.
    ///
    BaseException(const char* message);

    /// For internal use.
    ///
    BaseException(PyObject* nativeException);

    ~BaseException() override;

    const char* what() const throw() override;

protected:
    static PyObject* createNativeException(PyObject* nativeClass, const char* message);

    void initializeFromNativeException(PyObject* nativeException);

private:
    BaseExceptionPrivate* const d;
};

} // namespace pyncpp


#endif // PYNCPP_BASE_EXCEPTION_H
