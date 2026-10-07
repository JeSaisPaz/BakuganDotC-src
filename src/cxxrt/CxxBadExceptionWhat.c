// bdc 0x08a032ec CxxBadExceptionWhat
#include "bdc.h"

/* `std::bad_exception::what()` (vtable entry after `CxxBadExceptionDtor`, table `0x08af5a78`
   entry 6): returns the constant string pointer `0x08aa4434`. */

const char *CxxBadExceptionWhat(void *self)

{
  g_cxxEhFrameStack = g_cxxEhFrameStack;
  return "";
}

