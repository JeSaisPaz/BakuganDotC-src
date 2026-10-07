// bdc 0x08a03140 CxxExceptionWhat
#include "bdc.h"

/* `std::exception::what()` (vtable `0x08af5a80` slot 2): returns the constant string pointer
   `0x08aa4434` (inside the runtime's merged type-name strings). The load/store of the EH frame head
   `0x08af120c` around it is the empty no-throw frame of the compiled body. */

const char *CxxExceptionWhat(void *self)
{
  g_cxxEhFrameStack = g_cxxEhFrameStack;
  return "";
}
