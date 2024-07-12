// Copyright (c) 2022 IHU Liryc, Université de Bordeaux, Inria.
// License: BSD-3-Clause

#include "exception_types.h"

namespace pyncpp::internal
{

void initializeExceptions()
{
   BaseException::registerThrower();
   Exception::registerThrower();
   GeneratorExit::registerThrower();
   KeyboardInterrupt::registerThrower();
   SystemExit::registerThrower();
   ArithmeticError::registerThrower();
   FloatingPointError::registerThrower();
   OverflowError::registerThrower();
   ZeroDivisionError::registerThrower();
   ImportError::registerThrower();
   ModuleNotFoundError::registerThrower();
   LookupError::registerThrower();
   IndexError::registerThrower();
   KeyError::registerThrower();
   NameError::registerThrower();
   UnboundLocalError::registerThrower();
   OSError::registerThrower();
   BlockingIOError::registerThrower();
   ChildProcessError::registerThrower();
   ConnectionError::registerThrower();
   BrokenPipeError::registerThrower();
   ConnectionAbortedError::registerThrower();
   ConnectionRefusedError::registerThrower();
   ConnectionResetError::registerThrower();
   FileExistsError::registerThrower();
   FileNotFoundError::registerThrower();
   InterruptedError::registerThrower();
   IsADirectoryError::registerThrower();
   NotADirectoryError::registerThrower();
   PermissionError::registerThrower();
   TimeoutError::registerThrower();
   RuntimeError::registerThrower();
   NotImplementedError::registerThrower();
   RecursionError::registerThrower();
   SyntaxError::registerThrower();
   IndentationError::registerThrower();
   TabError::registerThrower();
   ValueError::registerThrower();
   UnicodeError::registerThrower();
   UnicodeDecodeError::registerThrower();
   UnicodeEncodeError::registerThrower();
   UnicodeTranslateError::registerThrower();
   AssertionError::registerThrower();
   AttributeError::registerThrower();
   BufferError::registerThrower();
   EOFError::registerThrower();
   MemoryError::registerThrower();
   ReferenceError::registerThrower();
   StopAsyncIteration::registerThrower();
   StopIteration::registerThrower();
   SystemError::registerThrower();
   TypeError::registerThrower();
}

void finalizeExceptions()
{
    clearExceptionThrowers();
}

} // namespace pyncpp::internal
