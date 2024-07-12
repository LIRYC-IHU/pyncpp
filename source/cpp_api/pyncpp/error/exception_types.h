// Copyright (c) 2022, 2024 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#ifndef PYNCPP_EXCEPTION_TYPES_H
#define PYNCPP_EXCEPTION_TYPES_H

/// Each Python exception class has an associated C++ class.
/// (refer to the Python documentation for more information on the exception
/// types)
///

#include "../external/cpython.h"

#include "base_exception.h"
#include "error_handling.h"
#include "exception_thrower.h"

#define PYNCPP_EXCEPTION_CLASS(NAME, PARENT) \
    class PYNCPP_EXPORT NAME : public PARENT \
    { \
    public: \
        static void registerThrower() { internal::registerExceptionThrower<NAME>(PyExc_ ## NAME); } \
        static void raise(const char* message) { raiseError(PyExc_ ## NAME, message); } \
        NAME(const char* message) : PARENT(createNativeException(PyExc_ ## NAME, message)) {} \
        NAME(PyObject* nativeException) : PARENT(nativeException) {} \
    }

namespace pyncpp
{

// Base exceptions
PYNCPP_EXCEPTION_CLASS(Exception, BaseException);
PYNCPP_EXCEPTION_CLASS(GeneratorExit, BaseException);
PYNCPP_EXCEPTION_CLASS(KeyboardInterrupt, BaseException);
PYNCPP_EXCEPTION_CLASS(SystemExit, BaseException);

// Arithmetic errors
PYNCPP_EXCEPTION_CLASS(ArithmeticError, Exception);
PYNCPP_EXCEPTION_CLASS(FloatingPointError, ArithmeticError);
PYNCPP_EXCEPTION_CLASS(OverflowError, ArithmeticError);
PYNCPP_EXCEPTION_CLASS(ZeroDivisionError, ArithmeticError);

// Import errors
PYNCPP_EXCEPTION_CLASS(ImportError, Exception);
PYNCPP_EXCEPTION_CLASS(ModuleNotFoundError, ImportError);

// Lookup errors
PYNCPP_EXCEPTION_CLASS(LookupError, Exception);
PYNCPP_EXCEPTION_CLASS(IndexError, LookupError);
PYNCPP_EXCEPTION_CLASS(KeyError, LookupError);

// Name errors
PYNCPP_EXCEPTION_CLASS(NameError, Exception);
PYNCPP_EXCEPTION_CLASS(UnboundLocalError, NameError);

// OS errors
PYNCPP_EXCEPTION_CLASS(OSError, Exception);
PYNCPP_EXCEPTION_CLASS(BlockingIOError, OSError);
PYNCPP_EXCEPTION_CLASS(ChildProcessError, OSError);
PYNCPP_EXCEPTION_CLASS(ConnectionError, OSError);
PYNCPP_EXCEPTION_CLASS(BrokenPipeError, ConnectionError);
PYNCPP_EXCEPTION_CLASS(ConnectionAbortedError, ConnectionError);
PYNCPP_EXCEPTION_CLASS(ConnectionRefusedError, ConnectionError);
PYNCPP_EXCEPTION_CLASS(ConnectionResetError, OSError);
PYNCPP_EXCEPTION_CLASS(FileExistsError, OSError);
PYNCPP_EXCEPTION_CLASS(FileNotFoundError, OSError);
PYNCPP_EXCEPTION_CLASS(InterruptedError, OSError);
PYNCPP_EXCEPTION_CLASS(IsADirectoryError, OSError);
PYNCPP_EXCEPTION_CLASS(NotADirectoryError, OSError);
PYNCPP_EXCEPTION_CLASS(PermissionError, OSError);
PYNCPP_EXCEPTION_CLASS(ProcessLookupError, OSError);
PYNCPP_EXCEPTION_CLASS(TimeoutError, OSError);

// Runtime errors
PYNCPP_EXCEPTION_CLASS(RuntimeError, Exception);
PYNCPP_EXCEPTION_CLASS(NotImplementedError, RuntimeError);
PYNCPP_EXCEPTION_CLASS(RecursionError, RuntimeError);

// Syntax errors
PYNCPP_EXCEPTION_CLASS(SyntaxError, Exception);
PYNCPP_EXCEPTION_CLASS(IndentationError, SyntaxError);
PYNCPP_EXCEPTION_CLASS(TabError, IndentationError);

// Value errors
PYNCPP_EXCEPTION_CLASS(ValueError, Exception);
PYNCPP_EXCEPTION_CLASS(UnicodeError, ValueError);
PYNCPP_EXCEPTION_CLASS(UnicodeDecodeError, UnicodeError);
PYNCPP_EXCEPTION_CLASS(UnicodeEncodeError, UnicodeError);
PYNCPP_EXCEPTION_CLASS(UnicodeTranslateError, UnicodeError);

// Other errors
PYNCPP_EXCEPTION_CLASS(AssertionError, Exception);
PYNCPP_EXCEPTION_CLASS(AttributeError, Exception);
PYNCPP_EXCEPTION_CLASS(BufferError, Exception);
PYNCPP_EXCEPTION_CLASS(EOFError, Exception);
PYNCPP_EXCEPTION_CLASS(MemoryError, Exception);
PYNCPP_EXCEPTION_CLASS(ReferenceError, Exception);
PYNCPP_EXCEPTION_CLASS(StopAsyncIteration, Exception);
PYNCPP_EXCEPTION_CLASS(StopIteration, Exception);
PYNCPP_EXCEPTION_CLASS(SystemError, Exception);
PYNCPP_EXCEPTION_CLASS(TypeError, Exception);

namespace internal
{

void initializeExceptions();

void finalizeExceptions();

} // namespace internal

} // namespace pyncpp

#endif // PYNCPP_EXCEPTION_TYPES_H
